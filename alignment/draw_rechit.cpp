#include <iostream>
#include "histograms.h"

int main(){
    std::vector<std::string> names;
    std::vector<std::string> titles;
    char name[100]; char title[100];
    for (int i = 1; i < 13; i++){
	sprintf(name,"Layer %i",i);
	sprintf(title,"Layer %i",i);
	names.push_back(name);
	titles.push_back(title);
    }

    draw_2D_histo(12,
	      "merged_output.root",
              "ProcessedEvents",
              "HGCMetaData_trigTime","RecHitEnergy",
              names,
              titles,
              50,200,
              50,-100,
              150,100,
              "Time_trig", "RecHitEnergy",
              "RecHit.root");
    return 0;
}

