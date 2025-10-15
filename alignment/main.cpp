#include <iostream>
#include "file_IO.h"
#include "nlohmann/json.hpp"
#include "TFile.h"
#include "TTree.h"
 
void to_be_named(const std::string& input_file, const std::string& output_file = "merged_output.root") {
    // Open input ROOT file
    TFile* inFile = TFile::Open(input_file.c_str(), "READ");
    if (!inFile || inFile->IsZombie()) {
        std::cerr << "Error: cannot open input file " << input_file << std::endl;
        return;
    }

    // Access tree
    TTree* tree = dynamic_cast<TTree*>(inFile->Get("Events")); // change "Events" if your tree name differs
    if (!tree) {
        std::cerr << "Error: cannot find TTree 'Events' in " << input_file << std::endl;
        inFile->Close();
        return;
    }

    // --- Input branches ---
// --- Input branches ---
    const int MAXHITS = 3000;  // safe upper bound

    Float_t HGCHit_energy[MAXHITS];
    Int_t   HGCHit_layer[MAXHITS];
    UInt_t  HGCMetaData_trigTime;
    Int_t  nHGCHit;  // size counter branch name; adjust to match your file

    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("HGCHit_layer", 1);
    tree->SetBranchStatus("HGCHit_energy", 1);
    tree->SetBranchStatus("HGCMetaData_trigTime", 1);
    tree->SetBranchStatus("nHGCHit", 1);

    tree->SetBranchAddress("HGCHit_energy", HGCHit_energy);
    tree->SetBranchAddress("HGCHit_layer", HGCHit_layer);
    tree->SetBranchAddress("HGCMetaData_trigTime", &HGCMetaData_trigTime);
    tree->SetBranchAddress("nHGCHit", &nHGCHit);
    // --- Output file ---
    // "UPDATE" lets us append to the same file if it already exists
    TFile* outFile = TFile::Open(output_file.c_str(), "UPDATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Error: cannot open output file " << output_file << std::endl;
        inFile->Close();
        return;
    }

    // Create (or get existing) tree
    TTree* outTree = static_cast<TTree*>(outFile->Get("ProcessedEvents"));
    bool newTree = false;
    if (!outTree) {
        newTree = true;
        outTree = new TTree("ProcessedEvents", "Processed data with summed RecHitEnergy");
    }

    // --- Output branches ---
    UInt_t out_trigTime;
    double out_RecHitEnergy[12];

    if (newTree) {
        outTree->Branch("HGCMetaData_trigTime", &out_trigTime, "HGCMetaData_trigTime/i");
        outTree->Branch("RecHitEnergy", out_RecHitEnergy, "RecHitEnergy[12]/D");
    } else {
        outTree->SetBranchAddress("HGCMetaData_trigTime", &out_trigTime);
        outTree->SetBranchAddress("RecHitEnergy", out_RecHitEnergy);
    }

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        out_trigTime = HGCMetaData_trigTime;
        for (int j = 0; j < 12; ++j) out_RecHitEnergy[j] = 0.0;
        for (UInt_t idx = 0; idx < nHGCHit; ++idx) {
            int layer = HGCHit_layer[idx];
            if (layer >= 1 && layer <= 12)
                out_RecHitEnergy[layer - 1] += HGCHit_energy[idx];
        }
	//std::cout<<"out_RecHitEnergy[0] = "<<out_RecHitEnergy[0]<<std::endl;
        outTree->Fill();
    }

    // Save and clean up
    outFile->cd();
    outTree->Write("", TObject::kOverwrite);
    outFile->Close();
    inFile->Close();

    std::cout << "Processed " << nEntries << " events from " << input_file << std::endl;
}

int main() {
    std::vector<std::string> dirs;
    std::vector<std::string> NANO_files;
    try {
        // Load directories from JSON
	dirs = load_run_directories("run_directories_new.json","muon_runs");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    // Search each one
    for (const auto& path : dirs) {
        auto files = search_nano(path);
        for (const auto& f : files){
            // std::cout << "  " << f << '\n';
	    NANO_files.push_back(f);
	}
    }
    for (const auto& f: NANO_files){
        std::cout<<"Found file:"<<f<<std::endl;
    }

    for (const auto& f: NANO_files){
        to_be_named(f);
    }
    return 0;
}
