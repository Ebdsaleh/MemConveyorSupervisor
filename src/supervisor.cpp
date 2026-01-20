// src/supervisor.cpp
#include "supervisor.h"
#include <memory>

struct MemConveyorSupervisor::Pimpl {
    std::string model_path;
    size_t file_size = 0;
    // This is the "Rubber band" buffer in the Host RAM
    unsigned char* conveyorWindow = nullptr;

    ~Pimpl() {
        if (conveyorWindow) {
            delete[] conveyorWindow;
        }
    }
};

MemConveyorSupervisor::MemConveyorSupervisor(const std::string path) : pimpl(std::make_unique<Pimpl>()) {
    pimpl->model_path = path;
    // 1. Check if the model exists
    if (fs::exists(pimpl->model_path)){
        pimpl->file_size = fs::file_size(pimpl->model_path);
        // 2. Allocate the 16MB window
        pimpl->conveyorWindow = new unsigned char[WINDOW_SIZE];
        initialize_page_table();
        std::cout << "[SUCCESS] Supervisor Attached to: " << pimpl->model_path << std::endl;
        std::cout << "[INFO] Total Model Size: " << (pimpl->file_size / 1024 / 1024) << " MB" << std::endl;
    } else {
        std::cerr << "[ERROR] Could not find model file!" << std::endl;

    }
}


void MemConveyorSupervisor::initialize_page_table() {
    size_t total_pages = pimpl->file_size / WINDOW_SIZE;
    if (pimpl->file_size % WINDOW_SIZE != 0) total_pages++;

    for (size_t i = 0; i <total_pages; i++) {
        page_table.push_back({i, false});
    }
    std::cout << "[MMU] Page Table Map: " << total_pages << " entries." << std::endl;
    }


bool MemConveyorSupervisor::handle_page_fault(size_t requested_page) {
    // 1. Safety Check: Is the page index valid?
    if (requested_page >= page_table.size()) return false;

    // 2. Intelligence: Is the page already in the 16MB window? (A "Hit")
    if (active_page_index == (int)requested_page) {
        return true; // No disk work needed!
    }

    // 3. Action: The "Sucker" logic happens here ONLY during a "Fault"
    std::ifstream file(pimpl->model_path, std::ios::binary);
    // (requested_page * 16MB) replaces the old manual offset calculation
    file.seekg(requested_page * WINDOW_SIZE, std::ios::beg);
    file.read(reinterpret_cast<char*>(pimpl->conveyorWindow), WINDOW_SIZE);
    file.close();

    // 4. Update the Map: Mark the old page absent and new page present
    if (active_page_index != -1) page_table[active_page_index].is_present = false;
    page_table[requested_page].is_present = true;
    active_page_index = (int)requested_page;

    return true;
}


// --- THE BRIDGE ---
// Returns a pointer to the current 16MB data chunk
unsigned char* MemConveyorSupervisor::get_window_pointer() {
    return pimpl->conveyorWindow;
}

size_t MemConveyorSupervisor::get_file_size() const { return pimpl->file_size; }

MemConveyorSupervisor::~MemConveyorSupervisor() = default;
