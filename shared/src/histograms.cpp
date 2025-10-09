#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TH2F.h"

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

    if (leaf_type == "Float_t") {
        Float_t value;
	t1->SetBranchAddress(branch_name.c_str(), &value);
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
	    h1->Fill(value);
        }
    }
    else if (leaf_type == "Double_t"){
	Double_t value;
        t1->SetBranchAddress(branch_name.c_str(), &value);
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            h1->Fill(value);
        }
    }
    else if (leaf_type == "UInt_t"){
	UInt_t value;
        t1->SetBranchAddress(branch_name.c_str(), &value);
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            h1->Fill(value);
        }
    }
    else if (leaf_type == "Int_t"){
	Int_t value;
        t1->SetBranchAddress(branch_name.c_str(), &value);
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            h1->Fill(value);
        }
    }
    else if (leaf_type == "UShort_t"){
	UShort_t value;
        t1->SetBranchAddress(branch_name.c_str(), &value);
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            h1->Fill(value);
        }
    }
    else if (leaf_type == "Short_t"){
	Short_t value;
        t1->SetBranchAddress(branch_name.c_str(), &value);
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            h1->Fill(value);
        }
    }
    else {
        std::cerr << "Unsupported type: " << leaf_type << std::endl;
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

    // --- Type handling ---
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
    else if (leaf_type == "UInt_t") {
        std::vector<UInt_t> value(L);
        t1->SetBranchAddress(branch_name.c_str(), value.data());
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            for (int j = 0; j < L; ++j)
                histos[j]->Fill(value[j]);
        }
    }
    else if (leaf_type == "UShort_t") {
        std::vector<UShort_t> value(L);
        t1->SetBranchAddress(branch_name.c_str(), value.data());
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            for (int j = 0; j < L; ++j)
                histos[j]->Fill(value[j]);
        }
    }
    else if (leaf_type == "Short_t") {
        std::vector<Short_t> value(L);
        t1->SetBranchAddress(branch_name.c_str(), value.data());
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            for (int j = 0; j < L; ++j)
                histos[j]->Fill(value[j]);
        }
    }
    else if (leaf_type == "Int_t") {
        std::vector<Int_t> value(L);
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

void draw_2D_histo(const std::string& input_root_file,
                   const std::string& tree_name,
                   const std::string& branch_for_x,
		   const std::string& branch_for_y,
                   const std::string& histo_name,
                   const std::string& histo_title,
                   int bins_x, int bins_y,
                   double min_x, double min_y,
                   double max_x, double max_y,
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

    // Get branches
    TBranch *branch_x = t1->GetBranch(branch_for_x.c_str());
    TBranch *branch_y = t1->GetBranch(branch_for_y.c_str());
    if (!branch_x || !branch_y) {
        std::cerr << "Error: could not find both branches " 
                  << branch_for_x << " and " << branch_for_y << std::endl;
        f_in->Close();
        return;
    }

    // Get leaves
    TLeaf *leaf_x = branch_x->GetLeaf(branch_for_x.c_str());
    TLeaf *leaf_y = branch_y->GetLeaf(branch_for_y.c_str());
    if (!leaf_x || !leaf_y) {
        std::cerr << "Error: could not get leaves for branches." << std::endl;
        f_in->Close();
        return;
    }

    std::string type_x = leaf_x->GetTypeName();
    std::string type_y = leaf_y->GetTypeName();

    std::cout << "Detected X branch type: " << type_x << std::endl;
    std::cout << "Detected Y branch type: " << type_y << std::endl;

    // Create 2D histogram
    TH2F *h2 = new TH2F(histo_name.c_str(), histo_title.c_str(),
                        bins_x, min_x, max_x,
                        bins_y, min_y, max_y);
    h2->SetDirectory(nullptr);
    h2->GetXaxis()->SetTitle(x_title.c_str());
    h2->GetYaxis()->SetTitle(y_title.c_str());

    Long64_t n_entries = t1->GetEntries();
    std::cout << "Entries in tree: " << n_entries << std::endl;

    // Helper lambda to fill the histogram for all combinations
    auto fill_histo = [&](auto &val_x, auto &val_y) {
        t1->SetBranchAddress(branch_for_x.c_str(), &val_x);
        t1->SetBranchAddress(branch_for_y.c_str(), &val_y);
        for (Long64_t i = 0; i < n_entries; ++i) {
            t1->GetEntry(i);
            h2->Fill(val_x, val_y);
        }
    };

    // Dispatch by type
    if (type_x == "Float_t") {
        Float_t x_val;
        if (type_y == "Float_t") { Float_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Double_t") { Double_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UInt_t") { UInt_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Int_t") { Int_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UShort_t") { UShort_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Short_t") { Short_t y_val; fill_histo(x_val, y_val); }
        else { std::cerr << "Unsupported Y type: " << type_y << std::endl; }
    }
    else if (type_x == "Double_t") {
        Double_t x_val;
        if (type_y == "Float_t") { Float_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Double_t") { Double_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UInt_t") { UInt_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Int_t") { Int_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UShort_t") { UShort_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Short_t") { Short_t y_val; fill_histo(x_val, y_val); }
        else { std::cerr << "Unsupported Y type: " << type_y << std::endl; }
    }
    else if (type_x == "UInt_t") {
        UInt_t x_val;
        if (type_y == "Float_t") { Float_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Double_t") { Double_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UInt_t") { UInt_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Int_t") { Int_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UShort_t") { UShort_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Short_t") { Short_t y_val; fill_histo(x_val, y_val); }
        else { std::cerr << "Unsupported Y type: " << type_y << std::endl; }
    }
    else if (type_x == "Int_t") {
        Int_t x_val;
        if (type_y == "Float_t") { Float_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Double_t") { Double_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UInt_t") { UInt_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Int_t") { Int_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UShort_t") { UShort_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Short_t") { Short_t y_val; fill_histo(x_val, y_val); }
        else { std::cerr << "Unsupported Y type: " << type_y << std::endl; }
    }
    else if (type_x == "UShort_t") {
        UShort_t x_val;
        if (type_y == "Float_t") { Float_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Double_t") { Double_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UInt_t") { UInt_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Int_t") { Int_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UShort_t") { UShort_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Short_t") { Short_t y_val; fill_histo(x_val, y_val); }
        else { std::cerr << "Unsupported Y type: " << type_y << std::endl; }
    }
    else if (type_x == "Short_t") {
        Short_t x_val;
        if (type_y == "Float_t") { Float_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Double_t") { Double_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UInt_t") { UInt_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Int_t") { Int_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "UShort_t") { UShort_t y_val; fill_histo(x_val, y_val); }
        else if (type_y == "Short_t") { Short_t y_val; fill_histo(x_val, y_val); }
        else { std::cerr << "Unsupported Y type: " << type_y << std::endl; }
    }
    else {
        std::cerr << "Unsupported X type: " << type_x << std::endl;
    }

    std::cout << "Histogram filling complete." << std::endl;

    // Write output histogram
    TFile *f_out = TFile::Open(output_root_file.c_str(), "RECREATE");
    h2->Write();
    f_out->Close();

    f_in->Close();
    delete h2;

    std::cout << "2D histogram " << histo_name << " saved to " << output_root_file << std::endl;
}
