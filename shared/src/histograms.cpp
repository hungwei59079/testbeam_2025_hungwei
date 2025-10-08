#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TBranch.h"
#include "TLeaf.h"

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
                   const std::string& output_root_file = "output.root")
{
    // Open input file
    TFile *f_in = TFile::Open(input_root_file.c_str(), "READ");
    if (!f_in || f_in->IsZombie()) {
        std::cerr << "Error: cannot open input file " << input_root_file << std::endl;
        return;
    }

    // Get tree
    TTree *t1 = nullptr;
    f_in->GetObject(tree_name.c_str(), t1);
    if (!t1) {
        std::cerr << "Error: cannot find tree " << tree_name << std::endl;
        f_in->Close();
        return;
    }

    // Get branch and check existence
    TBranch *branch = t1->GetBranch(branch_name.c_str());
    if (!branch) {
        std::cerr << "Error: branch " << branch_name << " not found in tree " << tree_name << std::endl;
        f_in->Close();
        return;
    }

    // Get the leaf (type info)
    TLeaf *leaf = branch->GetLeaf(branch_name.c_str());
    if (!leaf) {
        std::cerr << "Error: could not get leaf for branch " << branch_name << std::endl;
        f_in->Close();
        return;
    }

    std::string leaf_type = leaf->GetTypeName();
    std::cout << "Detected branch type: " << leaf_type << std::endl;

    // Create histogram
    TH1F *h1 = new TH1F(histo_name.c_str(), histo_title.c_str(), bins, min, max);
    h1->SetDirectory(nullptr);
    h1->GetXaxis()->SetTitle(x_title.c_str());
    h1->GetYaxis()->SetTitle(y_title.c_str());

    Long64_t n_entries = t1->GetEntries();
    std::cout << "Entries in tree: " << n_entries << std::endl;

    // Prepare a variable for each possible type
    Int_t    value_int;
    UInt_t   value_uint;
    Float_t  value_float;
    Double_t value_double;

    // Assign branch address dynamically
    if (leaf_type == "Int_t") {
        t1->SetBranchAddress(branch_name.c_str(), &value_int);
        for (Long64_t i = 0; i < n_entries; ++i) { t1->GetEntry(i); h1->Fill(value_int); }

    } else if (leaf_type == "UInt_t") {
        t1->SetBranchAddress(branch_name.c_str(), &value_uint);
        for (Long64_t i = 0; i < n_entries; ++i) { t1->GetEntry(i); h1->Fill(value_uint); }

    } else if (leaf_type == "Float_t") {
        t1->SetBranchAddress(branch_name.c_str(), &value_float);
        for (Long64_t i = 0; i < n_entries; ++i) { t1->GetEntry(i); h1->Fill(value_float); }

    } else if (leaf_type == "Double_t") {
        t1->SetBranchAddress(branch_name.c_str(), &value_double);
        for (Long64_t i = 0; i < n_entries; ++i) { t1->GetEntry(i); h1->Fill(value_double); }

    } else {
        std::cerr << "Unsupported branch type: " << leaf_type
                  << " (skipping histogram fill)" << std::endl;
        f_in->Close();
        delete h1;
        return;
    }
    std::cout<<"Histogram filling complete."<<std::endl;
    // Write output histogram
    TFile *f_out = TFile::Open(output_root_file.c_str(), "RECREATE");
    h1->Write();
    f_out->Close();

    f_in->Close();
    delete h1;

    std::cout << "Histogram " << histo_name << " saved to " << output_root_file << std::endl;
}

