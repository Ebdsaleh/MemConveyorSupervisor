// src/supervisor.h
#ifndef SUPERVISOR_H
#define SUPERVISOR_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

struct PageTableEntry {
    size_t page_index; // Which 16MB lump this is
    bool is_present; // Is it currently in the 16MB window?
};

class MemConveyorSupervisor {
    public:
        MemConveyorSupervisor(const std::string path);
        ~MemConveyorSupervisor();
        // --- CONSTANTS ---
        // We lock this to 16MB to match your 486DX limit
        const size_t WINDOW_SIZE = 16 * 1024 * 1024;
        
        unsigned char* get_window_pointer();
        size_t get_file_size() const;
        // Automates the swap based on a page request
        bool handle_page_fault(size_t requested_page);
        
    private:
        struct Pimpl;
        std::unique_ptr<Pimpl>pimpl;
        void initialize_page_table();
        std::vector<PageTableEntry> page_table;
        int active_page_index = -1; // Which page is currently "In the Slot"
};
#endif