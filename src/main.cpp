// src/main.cpp
#include "supervisor.h"
#include <iostream>
#include <fstream>
#include <thread> // For sleeping the loop
#include <chrono>

int main() {
    std::string model_file = "model_data/ibm-granite_granite-3.3-2b-instruct-IQ2_M.gguf";
    std::string request_file = "REQ.BIN";
    std::string ack_file = "ACK.BIN";

    MemConveyorSupervisor supervisor(model_file);

    std::cout << "--- SUPERVISOR ACTIVE: Waiting for Guest Request ---" << std::endl;

    while (true) {
        // 3. Check if the 486 Guest has dropped a request on the "conveyor"
        if (fs::exists(request_file)) {
            std::ifstream req(request_file, std::ios::binary);
            int requested_lump = 0;
            
            // Read the binary index sent by the Guest (e.g., 0, 1, 2...)
            req.read(reinterpret_cast<char*>(&requested_lump), sizeof(int));
            req.close();

            std::cout << "[GUEST] Request received for Page: " << requested_lump << std::endl;

            // 4. Use the NEW handle_page_fault method
            // This replaces the old manual "load_lump" call.
            // It automatically checks if the page is already in the 16MB window.
            if (supervisor.handle_page_fault(requested_lump)) {
                
                // 5. Complete the Handshake
                fs::remove(request_file);      // Clear the request
                std::ofstream ack(ack_file);  // Signal the Guest that data is ready
                ack << "OK";
                ack.close();
                
            }
        }

        // 6. Polling interval (100ms is plenty for a 486 simulation)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}