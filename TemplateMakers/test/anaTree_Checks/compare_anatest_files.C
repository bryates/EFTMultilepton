#include "utils.h"

void print_diff(TString header,std::set<TString> s) {
    std::cout << header << std::endl;
    if (s.size()) {
        for (TString str: s) std::cout << "\t" << str << std::endl;
    } else {
        std::cout << "\t*NONE*" << std::endl;
    }
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
        // "",     // For non anatest files
        // "data",
        // "charge_flips",
        // "fakes",
        // "tZq",
        // "ttH",
        // "ttW",
        // "ttZ",
        // "WZ",
        // "WWW",
        // "ttGJets",
        // "ttll_16D",
        // "ttlnu_16D",
        "tllq_16D",
        // "ttH_16D",
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

    bool skip_identical = true;

    std::cout << "------------------------------------------------------------" << std::endl;
    
    TString sep = ".";
    for (TString samp: samples) {
        bool cmp_res;
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
                    cmp_res = compare_histogram(f1,f2,hist_name,skip_identical);
                } else {
                    // This is likely a hist file that goes into anatest
                    cmp_res = compare_histogram(f1,f2,hist_name,skip_identical);
                }
                total_compares += 1;
                good_compares += cmp_res;
            }
        }
        std::cout << std::endl;
        std::cout << "Systs Checked: " << systs_var.size() << std::endl;
        std::cout << "Good Histograms: " << good_compares << std::endl;
        std::cout << "Total Histograms: " << total_compares << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
    }

    f1->Close();
    f2->Close();
}