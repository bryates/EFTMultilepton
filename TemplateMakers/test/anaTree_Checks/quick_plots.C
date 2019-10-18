// Returns the sign of x
int sgn(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

// Returns a new set that contains all the elements from s1 that are missing from s2
std::set<TString> set_diff(std::set<TString> s1, std::set<TString> s2) {
    std::set<TString> new_set;
    for (TString element: s1) {
        if (s2.count(element)) {
            continue;
        }
        new_set.insert(element);
    }
    return new_set;
}

std::set<TString> find_all_samples(TFile* f) {
    std::set<TString> ret;

    TIter next(f->GetListOfKeys());
    TKey *key;
    while ((key=(TKey*)next())) {
        TString key_name = key->GetName();
        std::vector<std::string> words;
        split_string(key_name.Data(),words,".");
        if (words.size() != 3) {
            continue;
        }
        TString bin = words.at(0);
        TString syst = words.at(1);
        TString samp = words.at(2);
        
        ret.insert(samp);
    }

    return ret;
}

std::set<TString> find_all_bins(TFile* f) {
    std::set<TString> ret;

    TIter next(f->GetListOfKeys());
    TKey *key;
    while ((key=(TKey*)next())) {
        TString key_name = key->GetName();
        std::vector<std::string> words;
        split_string(key_name.Data(),words,".");
        if (words.size() != 3) {
            continue;
        }
        TString bin = words.at(0);
        TString syst = words.at(1);
        TString samp = words.at(2);
        
        ret.insert(bin);
    }

    return ret;
}

std::set<TString> find_all_systs(TFile* f) {
    std::set<TString> ret;

    TIter next(f->GetListOfKeys());
    TKey *key;
    while ((key=(TKey*)next())) {
        TString key_name = key->GetName();
        std::vector<std::string> words;
        split_string(key_name.Data(),words,".");
        if (words.size() != 3) {
            continue;
        }
        TString bin = words.at(0);
        TString syst = words.at(1);
        TString samp = words.at(2);
        
        ret.insert(syst);
    }

    return ret;
}

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

void compare_anatest_systs(TString fpath1, TString fpath2) {
    TFile* f1 = TFile::Open(fpath1,"READ");
    TFile* f2 = TFile::Open(fpath2,"READ");

    if (!f1 || !f2) {
        std::cout << "compare_anatest_systs(): Missing at least one input file!" << std::endl;
        return;
    }
    std::cout << "File1: " << f1->GetName() << std::endl;
    std::cout << "File2: " << f2->GetName() << std::endl;

    std::set<TString> systs1 = find_all_systs(f1);
    std::set<TString> systs2 = find_all_systs(f2);

    // std::cout << "Systs1: " << systs1.size() << std::endl;
    // std::cout << "Systs2: " << systs2.size() << std::endl;

    std::set<TString> diff1 = set_diff(systs1,systs2);
    std::set<TString> diff2 = set_diff(systs2,systs1);

    std::cout << "In f1 but not in f2" << std::endl;
    if (diff1.size()) {
        for (TString syst: diff1) std::cout << "\t" << syst << std::endl;
    } else {
        std::cout << "\t*NONE*" << std::endl;
    }

    std::cout << "In f2 but not in f1" << std::endl;
    if (diff2.size()) {
        for (TString syst: diff2) std::cout << "\t" << syst << std::endl;
    } else {
        std::cout << "\t*NONE*" << std::endl;
    }

    f1->Close();
    f2->Close();
}

// Hastily thrown together root macro to extract nominal and up/down systematic variations from the 'mergedHists' step root files
// Note: The naming of the histograms is slightly different depending on if the file is at the 'mergedHists' stage or if its at the
//      full 'anatest' stage. Also the 'mergedHist' stage isn't fully normalized to expected data, while the 'anatest' stage is
//  'mergedHist' Format: 2lss_p_emu. && 2lss_p_emu.JESUP
//  'anatest' Format: 2lss_p_emu.tZq && 2lss_p_emu.JESUP.tZq
// Note2: This root macro sort of doesnt belong with the 'check_anaTree' code, but I moved it here since it was created
//      around the same time as that code and also didnt really belong in MakeGoodPlot2
void runit(TString fpath, TString sample,TString syst) {
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
    all_bins.insert(all_bins.end(),bins_2lss.begin(),bins_2lss.end());
    all_bins.insert(all_bins.end(),bins_3lep.begin(),bins_3lep.end());

    std::vector<TString> bins_2lss_p_1b {"2lss_p_ee_1b","2lss_p_emu_1b","2lss_p_mumu_1b"};
    std::vector<TString> bins_2lss_m_1b {"2lss_m_ee_1b","2lss_m_emu_1b","2lss_m_mumu_1b"};

    for (TString bin: all_bins) {
       plot_bin(f,sample,bin,syst,false);
    }
    std::cout << std::endl;

    TString merge_name;
    
    merge_name = "";

    merge_name = "merged_2lss.";
    merge_plot(f,sample,bins_2lss,syst,merge_name,false);
    std::cout << std::endl;

    merge_name = "merged_3lep.";
    merge_plot(f,sample,bins_3lep,syst,merge_name,false);
    std::cout << std::endl;

    f->Close();
}

void quick_plots() {
    TString geoff_dir = "/afs/crc.nd.edu/user/g/gsmith15/Public/for_Tony/";
    TString tony_dir  = "/afs/crc.nd.edu/user/a/awightma/Public/for_tony/";
    TString hist_dir  = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/CMSSW_8_1_0/src/CombineHarvester/TopEFT/hist_files/";

    TString fname_a15   = "TOP-19-001_unblinded_v1.root";
    TString fname_a16   = "anatest16.root";
    TString fname_a17   = "anatest17.root";
    TString fname_a18   = "anatest18.root";
    TString fname_a19   = "anatest19.root";
    TString fname_a20   = "anatest20.root";
    TString fname_a21   = "anatest21.root";
    TString fname_a22   = "anatest22.root";
    TString fname_a23v3 = "anatest23_v3.root";
    TString fname_a24   = "anatest24.root";
    TString fname_a25   = "anatest25.root";

    TString fpath_a15 = hist_dir + fname_a15;
    TString fpath_a16 = hist_dir + fname_a16;
    TString fpath_a17 = hist_dir + fname_a17;
    TString fpath_a18 = hist_dir + fname_a18;
    TString fpath_a19 = hist_dir + fname_a19;
    TString fpath_a20 = hist_dir + fname_a20;
    TString fpath_a22 = hist_dir + fname_a22;

    TString fpath_a21 = geoff_dir + fname_a21;

    TString fpath_a23 = tony_dir + fname_a23v3;
    TString fpath_a24 = tony_dir + fname_a24;
    TString fpath_a25 = tony_dir + fname_a25;

    TString fpath = fpath_a25;

    std::vector<TString> private_signal {"tllq_16D","ttH_16D","ttll_16D","ttlnu_16D","tHq_16D"};
    std::vector<TString> central_signal {"tZq","ttH","ttZ","ttW"};
    std::vector<TString> central_bkgd {"ttGJets","WZ","WWW"};

    TString syst = "PDF";
    //TString syst = "MUR";
    //TString syst = "MUF";
    //TString syst = "MURMUF";

    // for (TString sample: private_signal) {
    //     runit(fpath,sample,syst);
    // }

    // for (TString sample: central_signal) {
    //     runit(fpath,sample,syst);
    // }

    // for (TString sample: central_bkgd) {
    //     runit(fpath,sample,syst);
    // }

    compare_anatest_systs(fpath_a15,fpath_a16);
    std::cout << endl;
    compare_anatest_systs(fpath_a16,fpath_a17);
    std::cout << endl;
    compare_anatest_systs(fpath_a17,fpath_a18);
    std::cout << endl;
    compare_anatest_systs(fpath_a18,fpath_a19);
    std::cout << endl;
    compare_anatest_systs(fpath_a19,fpath_a20);
    std::cout << endl;
    compare_anatest_systs(fpath_a20,fpath_a21);
    std::cout << endl;
    compare_anatest_systs(fpath_a21,fpath_a22);
    std::cout << endl;
    compare_anatest_systs(fpath_a22,fpath_a23);
    std::cout << endl;
    compare_anatest_systs(fpath_a23,fpath_a24);
    std::cout << endl;
    compare_anatest_systs(fpath_a24,fpath_a25);
}