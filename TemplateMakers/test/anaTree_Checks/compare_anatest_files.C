#include "utils.h"

//TODO: Put this in utils?
void print_diff(TString header,std::set<TString> s) {
    std::cout << header << std::endl;
    if (s.size()) {
        for (TString str: s) std::cout << "\t" << str << std::endl;
    } else {
        std::cout << "\t*NONE*" << std::endl;
    }
}

double fill_stat_dist(TH1EFT* h1, TH1EFT* h2, TString hist_name, TH1D* stat_hist) {
    if (h1->GetNbinsX() != h2->GetNbinsX()) {
        std::cout << "ERROR fill_stat_dist(): nBins mismatch in " << hist_name << std::endl;
        std::cout << "\tf1 hist has " << h1->GetNbinsX() << " bins" << std::endl;
        std::cout << "\tf2 hist has " << h2->GetNbinsX() << " bins" << std::endl;
        return 0.0;
    }

    if (h1->Integral() == 0 && h2->Integral() == 0) {
        return 0.0;
    }

    double chi_sqr = 0.0;
    for (Int_t i = 1; i <= h1->GetNbinsX(); i++) {
        double bin1 = h1->GetBinContent(i);
        double bin2 = h2->GetBinContent(i);
        double diff = bin2 - bin1;

        double bin1_err = h1->GetBinError(i);
        double bin2_err = h2->GetBinError(i);

        double err_sqr1 = bin1_err*bin1_err;
        double err_sqr2 = bin2_err*bin2_err;

        if (err_sqr1 == 0 || err_sqr2 == 0) {
            // std::cout << "Skipping bin: " << i << std::endl;
            continue;
        }

        // double sig_diff = diff / std::max(bin1_err,bin2_err);
        double sig_diff = diff / sqrt(err_sqr1 + err_sqr2);

        chi_sqr += sig_diff*sig_diff;

        stat_hist->Fill(sig_diff);
    }
    return chi_sqr;
}

// template<typename T>
// void makePlot(T* h,TString name) {
void makePlot(TH1D* h, TString name) {
    std::cout << "Making plot: " << name << std::endl;
    std::vector<int> clrs {
        kBlack,
        kBlue,
        kRed+1,
        kGreen+1,
        kMagenta,
        kCyan,
        kPink+1,
        kSpring+1,
        kAzure-9,
    };

    double ymax = 0.0;
    ymax = std::max(ymax,h->GetMaximum());

    TCanvas* canv = new TCanvas(name,name,1300,1000);
    Float_t small = 1.e-5;
    const float padding = 1e-5;
    canv->Divide(1,1,small,small);
    //canv->GetPad(1)->SetPad(xlow,ylow,xup,yup);
    canv->GetPad(1)->SetPad(padding,padding,1-padding,1-padding);
    canv->GetPad(1)->SetLeftMargin(.11);
    canv->GetPad(1)->SetRightMargin(.05);
    //canv->GetPad(1)->SetBottomMargin(.3);
    canv->GetPad(1)->SetBottomMargin(.1);
    canv->GetPad(1)->Modified();

    canv->cd(1);
    gPad->Modified();

    // Coordinates are: (x1,y1,x2,y2)
    // TLegend *leg = new TLegend(0.14,0.75,0.94,0.89);
    TLegend *leg = new TLegend(0.14,0.75,0.50,0.85);
    leg->SetFillColor(kWhite);
    leg->SetLineColor(kWhite);
    leg->SetShadowColor(kWhite);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    leg->SetNColumns(1);

    int clr_idx = (0 % clrs.size());
    h->SetLineColor(clrs.at(clr_idx));
    h->GetYaxis()->SetRangeUser(0.0,1.25*ymax);

    leg->AddEntry(h,h->GetTitle(),"l");

    canv->cd(1);
    // h->SetTitle(name);
    h->SetTitle("");
    h->Draw("hist");
    canv->GetPad(1)->RedrawAxis();

    canv->cd(1);
    leg->Draw();

    TString outf = name + ".png";
    canv->Print(outf,"png");

    delete leg;
    delete canv;
}

template <typename T>
bool is_missing(T* h1, T* h2, TString hname) {
    bool missing = false;
    if (!h1) {
        std::cout << "ERROR: f1 does not have " << hname << std::endl;
        missing = true;
    }
    if (!h2) {
        std::cout << "ERROR: f2 does not have " << hname << std::endl;
        missing = true;
    }
    return missing;
}

void compare_anatest_files(TString fp1, TString fp2, TString f1_title, TString f2_title, TString out_name) {
    // gStyle->SetOptStat(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);

    bool compare_to_central = false;
    if (fp1 == fp2) {
        compare_to_central = true;
    }

    TFile* f1 = TFile::Open(fp1,"READ");
    TFile* f2 = TFile::Open(fp2,"READ");

    if (!f1 || !f2) {
        std::cout << "compare_anatest_files(): Missing at least one input file!" << std::endl;
        return;
    }
    std::cout << "File1: " << f1->GetName() << std::endl;
    std::cout << "File2: " << f2->GetName() << std::endl;

    std::set<TString> diff1;
    std::set<TString> diff2;

    std::set<TString> bins1 = find_all_bins(f1);
    std::set<TString> bins2 = find_all_bins(f2);

    std::set<TString> systs1 = find_all_systs(f1);
    std::set<TString> systs2 = find_all_systs(f2);

    std::set<TString> procs1 = find_all_samples(f1);
    std::set<TString> procs2 = find_all_samples(f2);

    diff1 = set_diff(bins1,bins2);
    diff2 = set_diff(bins2,bins1);
    print_diff("Bins in f1 but not in f2",diff1);
    print_diff("Bins in f2 but not in f1",diff2);

    diff1 = set_diff(procs1,procs2);
    diff2 = set_diff(procs2,procs1);
    print_diff("Procs in f1 but not in f2",diff1);
    print_diff("Procs in f2 but not in f1",diff2);

    diff1 = set_diff(systs1,systs2);
    diff2 = set_diff(systs2,systs1);
    print_diff("Systematics in f1 but not in f2",diff1);
    print_diff("Systematics in f2 but not in f1",diff2);

    std::vector<TString> samples {
        // "",     // For non anatest files
        // "data",
        // "charge_flips",
        // "fakes",
        //"tZq",
        //"ttH",
        //"ttW",
        //"ttZ",
        // "WZ",
        // "WWW",
        // "ttGJets",
        "ttll_16D",
        "ttlnu_16D",
        // "tllq_16D",
        "ttH_16D",
        // "tHq_16D",
    };

    // std::set<TString> bins = set_intersection(bins1,bins2);

    std::vector<TString> bins {
        "2lss_p_ee_2b",
        "2lss_p_emu_2b",
        "2lss_p_mumu_2b",
        "2lss_m_ee_2b",
        "2lss_m_emu_2b",
        "2lss_m_mumu_2b",

        "3l_mix_sfz_1b",
        "3l_mix_m_1b",
        "3l_mix_p_1b",
        /* "3l_ppp_1b", */
        /* "3l_mmm_1b", */

        "3l_mix_sfz_2b",
        "3l_mix_m_2b",
        "3l_mix_p_2b",
        /* "3l_ppp_2b", */
        /* "3l_mmm_2b", */

        "4l_2b",
    };

    std::vector<TString> skip_systs {
        // "JES",      // Expected to change a23 -> a24
        // "MUR",      // Expected to change a24 -> a25 and a25 -> a26 (for tllq4f/tZq)
        // "MUF",      // Expected to change a24 -> a25 and a25 -> a26 (for tllq4f/tZq)
        // "MURMUF",   // Expected to change a24 -> a25 and a25 -> a26 (for tllq4f/tZq) 
        // "PSISR",    // Expected to change a25 -> a26
        // "PDF",      // Expected to change a24 -> a25 (for central samples)
        "FR"        // Only applies to fakes
    };

    std::map<TString,TString> central_map {
        {"ttll_16D","ttZ"},
        {"ttlnu_16D","ttW"},
        {"ttH_16D","ttH"},
        {"tllq_16D","tZq"},
    };

    std::set<TString> systs_to_skip;
    for (TString s: skip_systs) {
        TString up = s + "UP";
        TString down = s + "DOWN";
        systs_to_skip.insert(up);
        systs_to_skip.insert(down);
    }

    // std::set<TString> systs_var = set_intersection(systs1,systs2);
    std::set<TString> systs_var {};
    systs_var = set_diff(systs_var,systs_to_skip);

    systs_var.insert("");   // This is for the nominal histogram

    // std::vector<TString> systs_var {""};
    // for (TString s: systs) {
    //     TString up = s + "UP";
    //     TString down = s + "DOWN";
    //     systs_var.push_back(up);
    //     systs_var.push_back(down);
    // }

    bool skip_identical = false;
    TH1D* stat_hist;
    TH1D* central_stat_hist1;
    TH1D* central_stat_hist2;

    TString cmp_title = f1_title + " vs. " + f2_title;
    if (compare_to_central) {
        cmp_title = f1_title + " vs. " + "Central";
        stat_hist = new TH1D(cmp_title,cmp_title,40,-10,10);
    } else {
        stat_hist = new TH1D(cmp_title,cmp_title,20,-5,5);

        TString central_cmp_title1 = f1_title + " vs. " + "Central";
        TString central_cmp_title2 = f2_title + " vs. " + "Central";
        central_stat_hist1 = new TH1D(central_cmp_title1,central_cmp_title1,40,-8,8);
        central_stat_hist2 = new TH1D(central_cmp_title2,central_cmp_title2,40,-8,8);
    }


    std::cout << "------------------------------------------------------------" << std::endl;
    
    double chi_sqr = 0.0;
    double chi_sqr_central1 = 0.0;
    double chi_sqr_central2 = 0.0;

    TString sep = ".";
    for (TString samp: samples) {
        int total_compares = 0;
        int good_compares = 0;
        std::cout << "Sample: " << samp << std::endl;
        for (TString syst: systs_var) {
            for (TString bin: bins) {
                TString hist_name = bin + sep;
                if (samp.Length()) {
                    // This is likely an anatest file
                    if (samp != "data" && syst.Length()) {
                        hist_name += syst + sep;
                    }
                    hist_name += samp;
                }
                TH1EFT* h1 = (TH1EFT*)f1->Get(hist_name);
                TH1EFT* h2 = (TH1EFT*)f2->Get(hist_name);
                if (!compare_to_central && !is_missing(h1,h2,hist_name)) {
                    good_compares += compare_histogram(h1,h2,hist_name,skip_identical);
                    if (samp != "data") {
                        chi_sqr += fill_stat_dist(h1,h2,hist_name,stat_hist);
                    }
                    total_compares += 1;
                }
                auto k = central_map.find(samp);
                if (k != central_map.end()) {
                    // Found a match!
                    TString central_hist = bin + sep + k->second;
                    TH1EFT* h3 = (TH1EFT*)f1->Get(central_hist);
                    if (compare_to_central) {
                        if (!is_missing(h1,h3,hist_name)) {                        
                            good_compares += compare_histogram(h1,h3,hist_name,skip_identical);
                            if (samp != "data") {
                                chi_sqr += fill_stat_dist(h1,h3,hist_name,stat_hist);
                            }
                            total_compares += 1;
                        }
                    } else {                    
                        if (!is_missing(h1,h3,central_hist)) {
                            chi_sqr_central1 += fill_stat_dist(h1,h3,central_hist,central_stat_hist1);
                        }
                        if (!is_missing(h2,h3,central_hist)) {
                            chi_sqr_central2 += fill_stat_dist(h2,h3,central_hist,central_stat_hist2);
                        }
                    }
                }
            }
        }
        std::cout << std::endl;
        std::cout << "Systs Checked: " << systs_var.size() << std::endl;
        std::cout << "Good Histograms: " << good_compares << std::endl;
        std::cout << "Total Histograms: " << total_compares << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
    }

    if (compare_to_central) {
        std::cout << "Central-f1 Chi-Sqr: " << chi_sqr << std::endl;
    } else {    
        std::cout << "f1-f2 Chi-Sqr: " << chi_sqr << std::endl;
        std::cout << "Central-f1 Chi-Sqr: " << chi_sqr_central1 << std::endl;
        std::cout << "Central-f2 Chi-Sqr: " << chi_sqr_central2 << std::endl;
    }

    if (out_name.Length()) {
        TString outf_name = out_name + "_stdev_dist";
        makePlot(stat_hist,outf_name);
        if (!compare_to_central) {        
            // Note: For now dont produce central plots, just make them via the 'compare_to_central' switch
            // outf_name = out_name + "_central-f1_stdev_dist";
            // makePlot(central_stat_hist1,outf_name);
            // outf_name = out_name + "_central-f2_stdev_dist";
            // makePlot(central_stat_hist2,outf_name);
        }
    }

    std::cout << "Cleaning up..." << std::endl;

    f1->Close();
    f2->Close();

    // delete stat_hist;

    std::cout << "Finished!" << std::endl;
}