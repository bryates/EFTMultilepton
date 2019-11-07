#include "EFTMultilepton/TemplateMakers/interface/WCPoint.h"
#include "EFTMultilepton/TemplateMakers/interface/WCFit.h"
#include "EFTMultilepton/TemplateMakers/interface/TH1EFT.h"
#include "EFTMultilepton/TemplateMakers/interface/split_string.h"
#include "utils.h"

void make_plot(TString category, TH1EFT* nom, TH1EFT* up, TH1EFT* down, bool save_it) {
    std::cout << "Category: " << category << std::endl;
    double bin_sum = 0.0;
    double sum_err = 0.0;
    for (Int_t i = 1; i <= nom->GetNbinsX(); i++) {
        double nom_content = nom->GetBinContent(i);
        double up_content = up->GetBinContent(i);
        double down_content = down->GetBinContent(i);

        double nom_err = nom->GetBinError(i);

        double up_ratio = 0.0;
        double down_ratio = 0.0;
        if (nom_content != 0 && up_content != 0 && down_content != 0) {
            up_ratio = 1 - nom_content/abs(up_content);
            down_ratio = 1 - nom_content/abs(down_content);
        }

        if (nom_content != 0) {
            bin_sum += nom_content;
            sum_err += nom_err*nom_err;
        }

        TString direction;
        if (sgn(up_ratio) == sgn(down_ratio)) {
            direction = "SAME";
        } else {
            direction = "OPP";
        }

        if (nom_content == 0) {
            direction = "";
        } else if (nom_content < 0) {
            direction = "NEG BIN";
        }

        TString nom_str     = TString::Format("%+.2f",nom_content);
        TString nom_err_str = TString::Format("%+.2f",nom_err);
        TString up_str      = TString::Format("%+.2f",up_content);
        TString down_str    = TString::Format("%+.2f",down_content);
        TString r_up_str    = TString::Format("%+.2f",up_ratio);
        TString r_down_str  = TString::Format("%+.2f",down_ratio);

        std::cout << "  Bin " << i << ": " << std::setw(7) << std::left << nom_str
                  << " +/- " << std::setw(7) << std::left << nom_err_str
                  << " (Up: " << std::setw(7) << std::left << up_str
                  << " " << std::setw(7) << std::left << r_up_str
                  << ", Down: " << std::setw(7) << std::left << down_str
                  << " " << std::setw(7) << std::left << r_down_str << ")"
                  << " " << direction << std::endl;
    }
    TString sum_str     = TString::Format("%+.2f",bin_sum);
    TString sum_err_str = TString::Format("%+.2f",sqrt(sum_err));
    std::cout << "Sum: " << sum_str << " +/- " << sum_err_str << std::endl;

    nom->SetLineColor(kBlack);
    up->SetLineColor(kRed);
    down->SetLineColor(kBlue);

    nom->SetTitle(category);

    TCanvas *c1 = new TCanvas("c1","",1280,720);
    c1->ToggleEventStatus();
    c1->cd();

    nom->Draw();
    up->Draw("same");
    down->Draw("same");

    double left         = 0.81;
    double right        = 0.95;
    double top          = 0.9;
    double scale_factor = 0.05;
    double minimum      = 0.1;
    double bottom = std::max(top - scale_factor*(3+1),minimum);
    TLegend *legend = new TLegend(left,top,right,bottom);

    legend->AddEntry(nom,"Nom","l");
    legend->AddEntry(up,"Up","l");
    legend->AddEntry(down,"Down","l");

    legend->Draw();

    TString save_name = category + "png";
    if (save_it) {
        c1->Print(save_name,"png");
    }

    delete legend;
    delete c1;
}

void plot_bin(TFile* f, TString sample, TString bin, TString syst, bool save_it) {
    TString up_var = syst + "UP";
    TString down_var = syst + "DOWN";

    TString nom_name  = bin;
    TString up_name   = bin + up_var;
    TString down_name = bin + down_var;
    if (sample.Length() != 0) {
        // This should be an 'anatest' style file
        nom_name  = bin + sample;
        up_name   = bin + up_var + "." + sample;
        down_name = bin + down_var + "." + sample;
    }

    TH1EFT* nom  = (TH1EFT*)f->Get(nom_name);
    TH1EFT* up   = (TH1EFT*)f->Get(up_name);
    TH1EFT* down = (TH1EFT*)f->Get(down_name);

    if (!nom) {
        std::cout << "plot_bin(): Unable to find nom hist with name " << nom_name << std::endl;
        return;
    }

    if (nom->Integral() == 0) {
        std::cout << "Skipping empty category: " << bin << std::endl;
        return;
    }

    make_plot(bin,nom,up,down,save_it);
}

// NOTE: This is exteremly dependent on the input histograms all having exactly same number of bins!
void merge_plot(TFile* f, TString sample, std::vector<TString> to_merge, TString syst, TString merged_name, bool save_it) {
    TString up_var = syst + "UP";
    TString down_var = syst + "DOWN";

    TString nom_name;
    TString up_name;
    TString down_name;

    nom_name  = to_merge.at(0);
    if (sample.Length() != 0) {
        // This should be an 'anatest' style file
        nom_name  = to_merge.at(0) + sample;
    }

    TH1EFT* tmp_hist = (TH1EFT*)f->Get(nom_name);
    if (!tmp_hist) {
        std::cout << "merge_plot(): Unable to find tmp_hist with name " << nom_name << std::endl;
        return;
    }

    Int_t nbins = tmp_hist->GetNbinsX();
    Double_t xlow = tmp_hist->GetBinLowEdge(1);
    Double_t xhigh = tmp_hist->GetBinLowEdge(nbins+1);
    TH1EFT* merged_nom = new TH1EFT(merged_name,merged_name,nbins,xlow,xhigh);
    TH1EFT* merged_up = new TH1EFT(merged_name+up_var,merged_name+up_var,nbins,xlow,xhigh);
    TH1EFT* merged_down = new TH1EFT(merged_name+down_var,merged_name+down_var,nbins,xlow,xhigh);


    for (TString bin: to_merge) {
        nom_name  = bin;
        up_name   = bin + up_var;
        down_name = bin + down_var;
        if (sample.Length() != 0) {
            // This should be an 'anatest' style file
            nom_name  = bin + sample;
            up_name   = bin + up_var + "." + sample;
            down_name = bin + down_var + "." + sample;
        }

        TH1EFT* nom  = (TH1EFT*)f->Get(nom_name);
        TH1EFT* up   = (TH1EFT*)f->Get(up_name);
        TH1EFT* down = (TH1EFT*)f->Get(down_name);


        if (!nom) {
            std::cout << "merge_plot(): Unable to find nom hist with name " << nom_name << std::endl;
            continue;
        }

        if (nom->Integral() == 0) {
            continue;
        }

        for (Int_t i = 1; i <= nom->GetNbinsX(); i++) {
            if (nom->GetBinContent(i) == 0) {
                nom->SetBinError(i,0.0);
            }
        }

        merged_nom->Add(nom,1.0);
        merged_up->Add(up,1.0);
        merged_down->Add(down,1.0);
    }

    make_plot(merged_name,merged_nom,merged_up,merged_down,save_it);
}

// What is this used for??
void print_err_band(TFile* f, TString sample, TString bin, std::vector<TString> systs) {
    double err_band_up = 0.0;
    double err_band_down = 0.0;

    TString nom_name = bin + sample;

    TH1EFT* nom  = (TH1EFT*)f->Get(nom_name);
    if (!nom) {
        std::cout << "print_err_band(): Unable to find nom hist with name " << nom_name << std::endl;
        return;
    }

    for (TString syst: systs) {
        TString up_var = syst + "UP";
        TString down_var = syst + "DOWN";

        TString up_name   = bin + up_var + "." + sample;
        TString down_name = bin + down_var + "." + sample;

        TH1EFT* up   = (TH1EFT*)f->Get(up_name);
        TH1EFT* down = (TH1EFT*)f->Get(down_name);

        for (Int_t i = 1; i <= nom->GetNbinsX(); i++) {
            double nom_content  = nom->GetBinContent(i);
            double up_content   = up->GetBinContent(i);
            double down_content = down->GetBinContent(i);

            double dUp = up_content - nom_content;
            double dDown = down_content - nom_content;

            if (i != 1) continue;

            if (dUp > 0) {
                err_band_up += dUp*dUp;
            } else {
                err_band_down += dDown*dDown;
            }

            if (dDown > 0) {
                err_band_up += dDown*dDown;
            } else {
                err_band_down += dDown*dDown;
            }
        }
    }

    std::cout << "Err Up: " << err_band_up << std::endl;
    std::cout << "Err Down: " << err_band_down << std::endl;
}

// Hastily thrown together root macro to extract nominal and up/down systematic variations from the 'mergedHists' step root files
// Note: The naming of the histograms is slightly different depending on if the file is at the 'mergedHists' stage or if its at the
//      full 'anatest' stage. Also the 'mergedHist' stage isn't fully normalized to expected data, while the 'anatest' stage is, so
//      EFT reweighting may need to be considered
//  'mergedHist' Format: 2lss_p_emu. && 2lss_p_emu.JESUP
//  'anatest' Format: 2lss_p_emu.tZq && 2lss_p_emu.JESUP.tZq
// Note2: This root macro sort of doesnt belong with the 'check_anaTree' code, but I moved it here since it was created
//      around the same time as that code and also didnt really belong in MakeGoodPlot2
void runit(TString fpath,TString sample,TString syst) {
    gStyle->SetPadRightMargin(0.2);
    gStyle->SetOptStat(0);

    std::cout << "Reading file: " << fpath << std::endl;
    std::cout << "Sample: " << sample << std::endl;
    std::cout << "Systematic: " << syst << std::endl;

    TFile* f = TFile::Open(fpath,"READ");

    // Note the '.' at the end of each category name!
    std::vector<TString> bins_2lss {
        // "2lss_p_ee_1b.",
        // "2lss_p_emu_1b.",
        // "2lss_p_mumu_1b.",
        // "2lss_m_ee_1b.",
        // "2lss_m_emu_1b.",
        // "2lss_m_mumu_1b.",

        // These bins are now merged by Tony for the combine fits
        "2lss_p_ee_2b.",
        "2lss_p_emu_2b.",
        "2lss_p_mumu_2b.",
        "2lss_m_ee_2b.",
        "2lss_m_emu_2b.",
        "2lss_m_mumu_2b.",
    };

    std::vector<TString> bins_3lep {
        "3l_mix_sfz_1b.",
        "3l_mix_m_1b.",
        "3l_mix_p_1b.",
        // "3l_ppp_1b.",
        // "3l_mmm_1b.",

        "3l_mix_sfz_2b.",
        "3l_mix_m_2b.",
        "3l_mix_p_2b.",
        // "3l_ppp_2b.",
        // "3l_mmm_2b."
    };

    std::vector<TString> all_bins;
    //all_bins.insert(all_bins.end(),bins_2lss.begin(),bins_2lss.end());    // The 2lss bins are merged by Tony, so don't plot these ones
    all_bins.insert(all_bins.end(),bins_3lep.begin(),bins_3lep.end());

    std::vector<TString> bins_2lss_p_2b {"2lss_p_ee_2b.","2lss_p_emu_2b.","2lss_p_mumu_2b."};
    std::vector<TString> bins_2lss_m_2b {"2lss_m_ee_2b.","2lss_m_emu_2b.","2lss_m_mumu_2b."};

    bool save_it = false;

    for (TString bin: all_bins) {
       plot_bin(f,sample,bin,syst,save_it);
    }
    std::cout << std::endl;

    TString merge_name;
    
    merge_name = "";

    merge_name = "merged_2lss_p_2b.";
    merge_plot(f,sample,bins_2lss_p_2b,syst,merge_name,save_it);
    std::cout << std::endl;

    merge_name = "merged_2lss_m_2b.";
    merge_plot(f,sample,bins_2lss_m_2b,syst,merge_name,save_it);
    std::cout << std::endl;

    merge_name = "merged_2lss.";
    merge_plot(f,sample,bins_2lss,syst,merge_name,save_it);
    std::cout << std::endl;

    merge_name = "merged_3lep.";
    merge_plot(f,sample,bins_3lep,syst,merge_name,save_it);
    std::cout << std::endl;

    f->Close();
}

void quick_plots(TString fpath, TString sample, TString syst) {
    runit(fpath,sample,syst);
}