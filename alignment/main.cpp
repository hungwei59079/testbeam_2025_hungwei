#include <iostream>
#include "file_IO.h"
#include "nlohmann/json.hpp"

int main() {
    try {
        // Load directories from JSON
        std::vector<std::string> dirs = load_run_directories("run_directories.json","muon_runs");

        // Search each one
        for (const auto& path : dirs) {
            std::cout << "Searching in: " << path << "\n";
            auto files = search_nano(path);

            std::cout << "Found " << files.size() << " files:\n";
            for (const auto& f : files)
                std::cout << "  " << f << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
