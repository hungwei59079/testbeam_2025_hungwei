#include <iostream>
#include "file_IO.h"
#include "nlohmann/json.hpp"
#include "TFile.h"
#include "TTree.h"
#include "ROOT/RDataFrame.hxx"
/*
void to_be_named(std::vector<std::string> file){
    TFile *f1;
    TTree *t1;
    int HGCHit_layer[3000];
    int TrigTime;
    float energy[3000];
    int N_events;
    float RecHit[12];
    
    f1 = TFile::Open(file.c_str());
    f1->GetObject("Events",t1);
    t1->SetBranchAddress("HGCHit_layer",HGCHit_layer);	
    t1->SetBranchAddress("HGCHit_energy",energy);
    t1->SetBranchAddress("HGCMetaData_trigTime",TrigTime);
    t1->SetBranchStatus ("*", 0);
    t1->SetBranchStatus ("HGCHit_layer",1);
    t1->SetBranchStatus ("HGCHit_energy",1);
    t1->SetBranchStatus ("HGCMetaData_trigTime",1);
    N_events = t1->GetEntries();
    for (event=0; event<N_events; event++){
	t1->GetEntry();
	for (layer=0;layer<12;layer++){
	    RecHit[layer]=0;
	    for (i=0; i<3000; i++){
		if (HGCHit_layer[i]==layer){
		    RecHit[layer]+=energy[i];
		}
	    }
	}
    }
}
*/
int main() {
    std::vector<std::string> dirs;
    try {
        // Load directories from JSON
	dirs = load_run_directories("run_directories.json","muon_runs");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    // Search each one
    for (const auto& path : dirs) {
	std::cout << "Searching in: " << path << "\n";
        auto files = search_nano(path);

        std::cout << "Found " << files.size() << " files:\n";
        for (const auto& f : files){
            std::cout << "  " << f << '\n';
	}
    }

    return 0;
}
