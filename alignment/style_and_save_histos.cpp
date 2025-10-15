#include <TFile.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TColor.h>
#include <iostream>
#include <TSystem.h>

int main() {
    const char* input_file = "RecHit.root";
    // 1. Open the ROOT file
    TFile* f = TFile::Open(input_file, "READ");
    if (!f || f->IsZombie()) {
        std::cerr << "Error: cannot open file " << input_file << std::endl;
        return 1;
    }

    // 2. Use inverted kSunset palette
    gStyle->SetPalette(kSunset);
    TColor::InvertPalette();

    gStyle->SetNumberContours(255);
    gStyle->SetOptStat(0);  // Hide stats box

    // 3. Prepare a canvas
    TCanvas* c = new TCanvas("c", "RecHit 2D histograms", 1200, 600);
    c->SetRightMargin(0.15); // leave space for color bar

    // 4. Loop through Layer 1–12
    for (int i = 1; i <= 12; ++i) {
        TString hname = Form("Layer %d", i);
        TH2F* hist = dynamic_cast<TH2F*>(f->Get(hname));
        if (!hist) {
            std::cerr << "Warning: histogram " << hname << " not found." << std::endl;
            continue;
        }

        // 5. Apply z-axis range and draw
        hist->GetZaxis()->SetRangeUser(0, 5000);
        hist->SetTitle(hname);
        c->cd();
        hist->Draw("COLZ");

        // 6. Save as PNG
        TString outname = Form("RecHit_%02d.png", i);
        c->SaveAs(outname);
        std::cout << "Saved " << outname << std::endl;
    }

    f->Close();
    return 0;
}
