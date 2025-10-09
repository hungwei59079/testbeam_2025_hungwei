#include <iostream>
#include "histograms.h"
#include <vector>
#include <string>

/*
int main(){
    draw_1D_histo("/eos/cms/store/group/dpg_hgcal/tb_hgcal/2025/SepTestBeam2025/Run112080/70b47280-a352-11f0-8f95-04d9f5f94829/prompt/NANO_112080_10.root", "Events", "HGCMetaData_trigTime",
              "Time_trig", "Time_trig",
              100, 0, 500,
              "Time_trig", "Counts",
              "test_output.root");
    return 0;
}
*/

int main() {
    std::vector<std::string> name;
    std::vector<std::string> title;
    char temp_name[100];
    char temp_title[100];
    for (int i=0; i<10000; i++){
	sprintf(temp_name,"ADC_Histo_no_%i",i);
	sprintf(temp_title,"ADC_Histo_no_%i",i);
	name.push_back(temp_name);
	title.push_back(temp_title);
    }
    draw_1D_multi_histo(
        2220,
        "/eos/cms/store/group/dpg_hgcal/tb_hgcal/2025/SepTestBeam2025/Run112080/70b47280-a352-11f0-8f95-04d9f5f94829/prompt/NANO_112080_10.root",
        "Events",
        "HGCDigi_adc",
        name,
        title,
        100,
        50,
        150,
        "ADC",
        "Events",
        "test_output.root"
    );

    return 0;
}
