#include <iostream>
#include <vector>

void draw_1D_histo(const std::string& input_root_file,
                   const std::string& tree_name,
                   const std::string& branch_name,
                   const std::string& histo_name,
                   const std::string& histo_title,
                   int bins,
                   double min,
                   double max,
                   const std::string& x_title = "ADC",
                   const std::string& y_title = "Events",
                   const std::string& output_root_file = "output.root");

void draw_1D_multi_histo(int length,
			 const std::string& input_root_file,
                         const std::string& tree_name,
                         const std::string& branch_name,
                         const std::vector<std::string>& histo_names,
                         const std::vector<std::string>& histo_titles,
                         int bins,
                         double min,
                         double max,
                         const std::string& x_title = "ADC",
                         const std::string& y_title = "Events",
                         const std::string& output_root_file = "output.root");


