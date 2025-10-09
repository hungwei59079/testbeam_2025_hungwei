#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TBranch.h"
#include "TLeaf.h"

void draw_1D_multi_histo(
    int length,
    const std::string& input_root_file,
    const std::string& tree_name,
    const std::string& branch_name,
    const std::vector<std::string>& histo_names,
    const std::vector<std::string>& histo_titles,
    int bins,
    double min,
    double max,
    const std::string& x_title,
    const std::string& y_title,
    const std::string& output_root_file)
{
    // --- Open input file ---
    TFile* f_in = TFile::Open(input_root_file.c_str(), "READ");
    if (!f_in || f_in->IsZombie()) {
        std::cerr << "Error: cannot open input file " << input_root_file << std::endl;
        return;
    }

    // --- Get tree ---
    TTree* t1 = nullptr;
    f_in->GetObject(tree_name.c_str(), t1);
    if (!t1) {
        std::cerr << "Error: cannot find tree " << tree_name << std::endl;
        f_in->Close();
        return;
    }

    // --- Get branch ---
    TBranch* branch = t1->GetBranch(branch_name.c_str());
    if (!branch) {
        std::cerr << "Error: branch " << branch_name << " not found in tree " << tree_name << std::endl;
        f_in->Close();
        return;
    }

    // --- Get leaf and detect type ---
    TLeaf* leaf = branch->GetLeaf(branch_name.c_str());
    if (!leaf) {
        std::cerr << "Error: could not get leaf for branch " << branch_name << std::endl;
        f_in->Close();
        return;
    }

    std::string leaf_type = leaf->GetTypeName();
    std::cout << "Detected branch type: " << leaf_type << std::endl;

    Long64_t n_entries = t1->GetEntries();
    std::cout << "Entries in tree: " << n_entries << std::endl;

    int L = length; // user-specified multiplicity

    if ((int)histo_names.size() < L || (int)histo_titles.size() < L) {
        std::cerr << "Error: need at least " << L << " histogram names/titles." << std::endl;
        f_in->Close();
        return;
    }

    // --- Create histograms ---
    std::vector<TH1F*> histos(L);
    for (int i = 0; i < L; ++i) {
        histos[i] = new TH1F(histo_names[i].c_str(), histo_titles[i].c_str(), bins, min, max);
        histos[i]->SetDirectory(nullptr);
        histos[i]->GetXaxis()->SetTitle(x_title.c_str());
        histos[i]->GetYaxis()->SetTitle(y_title.c_str());
    }

    // --- Dynamic array allocation depending on type ---
    if (leaf_type == "Float_t") {
        std::vector<Float_t> value(L);
        t1->SetBranchAddress(branch_name.c_str(), value.data());
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            for (int j = 0; j < L; ++j)
                histos[j]->Fill(value[j]);
        }
    }
    else if (leaf_type == "Double_t") {
        std::vector<Double_t> value(L);
        t1->SetBranchAddress(branch_name.c_str(), value.data());
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            for (int j = 0; j < L; ++j)
                histos[j]->Fill(value[j]);
        }
    }
    else if (leaf_type == "UInt_t" || leaf_type == "UShort_t" ||
             leaf_type == "Short_t" || leaf_type == "Int_t") {
        std::vector<UShort_t> value(L);
        t1->SetBranchAddress(branch_name.c_str(), value.data());
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            for (int j = 0; j < L; ++j)
                histos[j]->Fill(value[j]);
        }
    }
    else {
        std::cerr << "Unsupported type: " << leaf_type << std::endl;
    }

    std::cout << "Histogram filling complete." << std::endl;

    // --- Write output file ---
    TFile* f_out = TFile::Open(output_root_file.c_str(), "RECREATE");
    for (auto h : histos) h->Write();
    f_out->Close();

    f_in->Close();
    for (auto h : histos) delete h;

    std::cout << "All histograms saved to " << output_root_file << std::endl;
}
