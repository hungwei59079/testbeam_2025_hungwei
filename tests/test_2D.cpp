#include <iostream>
#include "histograms.h"

int main(){
    draw_2D_histo("/eos/cms/store/group/dpg_hgcal/tb_hgcal/2025/SepTestBeam2025/Run112080/70b47280-a352-11f0-8f95-04d9f5f94829/prompt/NANO_112080_10.root",
	      "Events",
	      "HGCMetaData_trigTime","HGCMetaData_trigWidth",
              "Time_trig", 
	      "Time_trig",
              50,100, 
	      100,0,
	      150,100,
              "Time_trig", "trig_width",
              "test_output_2D.root");
    return 0;
}
