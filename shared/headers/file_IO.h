#pragma once
#include <string>
#include <vector>

std::vector<std::string> load_run_directories(const std::string& json_path, const std::string& run_type);
std::vector<std::string> search_nano(const std::string& path);
