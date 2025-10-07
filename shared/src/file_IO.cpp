#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>


namespace fs = std::filesystem;
using json = nlohmann::json;

std::vector<std::string> load_run_directories(const std::string& json_path, const std::string& run_type) {
    std::ifstream f(json_path);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + json_path);
    }

    json data;
    f >> data;  // parse the JSON

    std::vector<std::string> directories;
    for (const auto& dir : data[run_type]) {
        directories.push_back(dir.get<std::string>());
    }

    return directories;
}

std::vector<std::string> search_nano(const std::string& path) {
    std::vector<std::string> results;

    // Ensure the path exists and is a directory
    if (!fs::exists(path) || !fs::is_directory(path)) {
        std::cerr << "Invalid path: " << path << std::endl;
        return results;
    }

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();

            // Match pattern "NANO*.root"
            if (filename.rfind("NANO", 0) == 0 && entry.path().extension() == ".root") {
                results.push_back(entry.path().string());
            }
        }
    }

    return results;
}
