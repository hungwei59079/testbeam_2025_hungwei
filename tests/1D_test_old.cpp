#include <iostream>
#include "histograms.h"

int main(){
    draw_1D_histo("/eos/cms/store/group/dpg_hgcal/tb_hgcal/2025/SepTestBeam2025/Run112080/70b47280-a352-11f0-8f95-04d9f5f94829/prompt/NANO_112080_10.root", "Events", "HGCMetaData_trigTime",
              "Time_trig", "Time_trig",
              100, 0, 500,
              "Time_trig", "Counts",
              "test_output_old.root");
    return 0;
}
