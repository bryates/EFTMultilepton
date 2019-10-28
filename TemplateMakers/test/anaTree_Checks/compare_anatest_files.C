#include "utils.h"

void print_diff(TString header,std::set<TString> s) {
    std::cout << header << std::endl;
    if (s.size()) {
        for (TString str: s) std::cout << "\t" << str << std::endl;
    } else {
        std::cout << "\t*NONE*" << std::endl;
    }
}

void compare_histogram(TFile* f1, TFile* f2, TString hist_name) {
    TH1EFT* h1 = (TH1EFT*)f1->Get(hist_name);
    TH1EFT* h2 = (TH1EFT*)f2->Get(hist_name);

    if (!h1) {
        std::cout << "ERROR compare_histogram(): f1 does not have " << hist_name << std::endl;
        return;
    } else if (!h2) {
        std::cout << "ERROR compare_histogram(): f2 does not have " << hist_name << std::endl;
    }

    if (h1->GetNbinsX() != h2->GetNbinsX()) {
        std::cout << "ERROR compare_histogram(): nBins mismatch in " << hist_name << std::endl;
        std::cout << "\tf1 hist has " << h1->GetNbinsX() << " bins" << std::endl;
        std::cout << "\tf2 hist has " << h2->GetNbinsX() << " bins" << std::endl;
        return;
    }

    if (h1->Integral() == 0 && h2->Integral() == 0) {
        return;
    }

    std::cout << "Histogram: " << hist_name << std::endl;

    TString delim = " ";

    double bin_sum1 = 0.0;
    double bin_sum2 = 0.0;
    double abs_diff = 0.0;
    for (Int_t i = 1; i <= h1->GetNbinsX(); i++) {
        double bin1 = h1->GetBinContent(i);
        double bin2 = h2->GetBinContent(i);
        double diff = bin2 - bin1;

        bin_sum1 += bin1;
        bin_sum2 += bin2;
        abs_diff += abs(diff);

        TString bin1_str = TString::Format("%+.2f",bin1);
        TString bin2_str = TString::Format("%+.2f",bin2);
        TString diff_str = TString::Format("%+.2f",diff);

        std::cout << "  Bin " << i << ": "
                  << std::setw(7) << std::right << bin1_str << delim
                  << std::setw(7) << std::right << bin2_str << delim
                  << std::setw(7) << std::right << diff_str << std::endl;
    }
    double perc_diff = 0.0;
    if (bin_sum1) {
        perc_diff = 100*(bin_sum2 - bin_sum1) / bin_sum1;
    }
    TString sum1_str = TString::Format("%+.2f",bin_sum1);
    TString sum2_str = TString::Format("%+.2f",bin_sum2);
    TString abs_str  = TString::Format("%+.2f",abs_diff);
    TString perc_str = TString::Format("%+.2f",perc_diff);
    std::cout << "h1 Sum: " << sum1_str << std::endl;
    std::cout << "h2 Sum: " << sum2_str << " (" << perc_str << "%)" << std::endl;
    std::cout << "Abs Diff: " << abs_str << std::endl;
    std::cout << std::endl;

    return;
}

void compare_anatest_files(TString fpath1, TString fpath2) {
    TFile* f1 = TFile::Open(fpath1,"READ");
    TFile* f2 = TFile::Open(fpath2,"READ");

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
        "",     // For non anatest files
        // "data",
        // "charge_flips",
        // "fakes",
        // "tZq",
        // "ttH",
        // "ttW",
        // "ttZ",
        // "WZ",
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
        "3l_ppp_1b",
        "3l_mmm_1b",

        "3l_mix_sfz_2b",
        "3l_mix_m_2b",
        "3l_mix_p_2b",
        "3l_ppp_2b",
        "3l_mmm_2b",

        "4l_2b",
    };

    std::vector<TString> systs {
        "",
    };

    TString sep = ".";
    for (TString samp: samples) {
        for (TString syst: systs) {
            for (TString bin: bins) {
                if (samp.Length()) {
                    // This is likely an anatest file
                    TString hist_name = bin + sep;
                    if (samp != "data" && syst.Length()) {
                        hist_name += syst + sep;
                    }
                    hist_name += samp;
                    compare_histogram(f1,f2,hist_name);
                } else {
                    // This is likely a hist file that goes into anatest
                    TString hist_name = bin + sep;
                    // std::cout << hist_name << std::endl;
                    compare_histogram(f1,f2,hist_name);
                }
            }
        }
        std::cout << "------------------------------------------------------------" << std::endl;
    }

    f1->Close();
    f2->Close();
}