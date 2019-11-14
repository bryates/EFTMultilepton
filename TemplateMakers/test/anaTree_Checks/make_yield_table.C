#include "EFTMultilepton/TemplateMakers/interface/WCPoint.h"
#include "EFTMultilepton/TemplateMakers/interface/WCFit.h"
#include "EFTMultilepton/TemplateMakers/interface/TH1EFT.h"
#include "EFTMultilepton/TemplateMakers/interface/split_string.h"
#include "utils.h"

// Run with: make_yield_table.C("path/to/anatest.root","ex_name")

struct YieldInfo {
    double yield;
    double error;   //Note: This is really the sum of the err^2
};

struct YieldCategory {
    TString name;
    std::map<TString,YieldInfo> yields;
};

// Returns a 2 element struct, the first is the yield for a given histogram and the second is the associated error
YieldInfo get_yield(TFile* f, TString hist_name) {
    YieldInfo info;
    TH1EFT* h = (TH1EFT*)f->Get(hist_name);
    if (!h) {
        TString err_str = TString::Format("get_yield(): file missing histogram %s",hist_name.Data());
        // std::cout << err_str << std::endl;
        info.yield = -1;
        info.error = -1;
        return info;
    }

    double bin_sum = 0.0;
    double sum_err = 0.0;
    for (Int_t i = 1; i <= h->GetNbinsX(); i++) {
        double bin = h->GetBinContent(i);
        double err = h->GetBinError(i);
        bin_sum += bin;
        sum_err += err*err;
    }
    info.yield = bin_sum;
    info.error = sum_err;
    return info;
}

YieldCategory get_category(TFile* f, TString cat_name, std::vector<TString> & samples) {
    YieldCategory cat;
    cat.name = cat_name;
    for (TString s: samples) {
        if (cat.yields.find(s) != cat.yields.end()) {
            TString err_str = TString::Format("get_category(): duplicate sample found for category - %s %s",s.Data(),cat_name.Data());
            std::cout << err_str << std::endl;
            raise;
        }
        TString hist_name = cat_name + "." + s;
        YieldInfo info = get_yield(f,hist_name);
        cat.yields[s] = info;
    }
    return cat;
}

YieldCategory merge_cats(TString new_cat, std::vector<YieldCategory> & cats) {
    YieldCategory merged_cat;
    merged_cat.name = new_cat;
    for (YieldCategory cat: cats) {
        // for (auto it = cat.yields.begin(); it != cat.yields.end(); it++) {
        for (auto& kv: cat.yields) {
            TString s = kv.first;
            YieldInfo info = kv.second;
            if (merged_cat.yields.find(s) == merged_cat.yields.end()) {
                YieldInfo new_yield;
                new_yield.yield = 0.0;
                new_yield.error = 0.0;
                merged_cat.yields[s] = new_yield;
            }
            // YieldInfo mrg_info = merged_cat.yields[s];
            // mrg_info.yield += info.yield;
            // mrg_info.error += info.error;
            merged_cat.yields[s].yield += info.yield;
            merged_cat.yields[s].error += info.error;
        }
    }
    return merged_cat;
}

// Returns a sum over a list of samples for a given category
// NOTE: This modifies the YieldCategory object passed to it
void sum_samples(TString new_samp, YieldCategory & cat, std::vector<TString> & samples) {
    if (cat.yields.find(new_samp) != cat.yields.end()) {
        TString err_str = TString::Format("sum_samples(): Invalid name for sample sum - %s",new_samp.Data());
        std::cout << err_str << std::endl;
        raise;
    }
    YieldInfo sum;
    sum.yield = 0.0;
    sum.error = 0.0;
    for (TString s: samples) {
        if (cat.yields.find(s) == cat.yields.end()) continue;
        YieldInfo info = cat.yields[s];
        if (info.error < 0) continue;
        sum.yield += info.yield;
        sum.error += info.error;
    }
    cat.yields[new_samp] = sum;
}

void print_category(YieldCategory & cat, std::vector<TString> & samples) {
    std::cout << "Category: " << cat.name << std::endl;
    if (samples.size()) {
        for (TString s: samples) {
            if (cat.yields.find(s) == cat.yields.end()) {
                TString str = TString::Format("\t%s: N/A",s.Data());
                std::cout << str << std::endl;
                continue;
            }
            YieldInfo info = cat.yields[s];
            TString str = TString::Format("\t%s: %.2f +/- %.2f",s.Data(),info.yield,sqrt(info.error));
            std::cout << str << std::endl;
        }
    } else {
        for (auto& kv : cat.yields) {
            TString s = kv.first;
            YieldInfo info = kv.second;
            TString str = TString::Format("\t%s: %.2f +/- %.2f",s.Data(),info.yield,sqrt(info.error));
            std::cout << str << std::endl;
        }
    }
}

TString get_line_break(std::vector<YieldCategory> & cats) {
    std::stringstream ss;
    ss << std::setw(12) << "";
    for (YieldCategory cat: cats) {
        ss << "---------------" << "---";   // 15 + 3
    }
    TString str = ss.str();
    return str;
}

TString get_header(std::vector<YieldCategory> & cats, TString table_name) {
    TString delim = " & ";
    std::stringstream header;
    header << std::setw(12) << std::right << table_name << " & ";
    for (YieldCategory cat: cats) {
        header << std::setw(15) << std::right << cat.name << delim;
    }

    TString str = header.str();
    return str;
}

TString get_row(std::vector<YieldCategory> & cats, TString sample) {
    TString delim = " & ";
    std::stringstream row;
    row << std::setw(12) << std::right << sample << " & ";
    for (YieldCategory cat: cats) {
        if (cat.yields.find(sample) == cat.yields.end()) {
            TString err_str = TString::Format("get_row(): Invalid sample - %s %s",cat.name.Data(),sample.Data());
            std::cout << err_str << std::endl;
            raise;
        }
        double yield = cat.yields[sample].yield;
        double error = sqrt(cat.yields[sample].error);
        TString col = TString::Format("%.2f +/- %.2f",yield,error);

        row << std::setw(15) << std::right << col << delim;
    }
    TString str = row.str();
    return str;
}

// void make_yield_table(TString fpath, std::vector<TString> samples) {
void make_yield_table(TString fpath,TString table_name) {
    // std::stringstream ss;
    std::cout << "Reading file: " << fpath << std::endl;

    TFile* f = TFile::Open(fpath,"READ");

    if (!f) {
        std::cout << "[ERROR] Unable to read file!" << std::endl;
        return;
    }

    // Note: Since we plan to merge the 2lss bins, we comment them out here!!
    std::vector<TString> bins {
        // "2lss_p_ee_2b",
        // "2lss_p_emu_2b",
        // "2lss_p_mumu_2b",
        // "2lss_m_ee_2b",
        // "2lss_m_emu_2b",
        // "2lss_m_mumu_2b",

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

    // list of categories we plan to merge
    std::vector<TString> mrg_2lss_p {"2lss_p_ee_2b","2lss_p_emu_2b","2lss_p_mumu_2b"};
    std::vector<TString> mrg_2lss_m {"2lss_m_ee_2b","2lss_m_emu_2b","2lss_m_mumu_2b"};

    // Sub-list of private signals to include in the table
    std::vector<TString> private_sgnl {
        "tllq_16D",
        "ttH_16D",
        "ttll_16D",
        "ttlnu_16D",
        // "tHq_16D",
    };

    // Sub-list of central signals to include in the table
    std::vector<TString> central_sgnl {
        // "tZq",
        // "ttH",
        // "ttZ",
        // "ttW",
    };

    // Sub-list of MC backgrounds to include in the table
    std::vector<TString> mc_bkgds {
        // "ttGJets",
        // "WZ",
        // "WWW",
    };

    // Sub-list of data-driven backgrounds to include in the table
    std::vector<TString> dd_bkgds {
        // "fakes",
        // "charge_flips",
    };

    std::vector<TString> all_bkgds;
    all_bkgds.insert(all_bkgds.end(),mc_bkgds.begin(),mc_bkgds.end());
    all_bkgds.insert(all_bkgds.end(),dd_bkgds.begin(),dd_bkgds.end());

    std::vector<TString> all_samples;
    all_samples.insert(all_samples.end(),private_sgnl.begin(),private_sgnl.end());
    all_samples.insert(all_samples.end(),central_sgnl.begin(),central_sgnl.end());
    all_samples.insert(all_samples.end(),all_bkgds.begin(),all_bkgds.end());

    // These represent the columns of the table
    std::vector<YieldCategory> cats;
    std::vector<YieldCategory> cats_2lss_p;
    std::vector<YieldCategory> cats_2lss_m;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Note: This is just to merge the 2lss lepton flavor categories,
    //  without including the unmerged categories in the table. I do it
    //  this way b/c I didn't implement an easy way to 'remove' columns
    //  from the table. The 'Sgnl Sum' and 'Bkgd Sum' need to be calculated
    //  here, since we plan to use them later on in the full yield table.
    for (TString bin: mrg_2lss_p) {
        YieldCategory cat = get_category(f,bin,all_samples);
        sum_samples("Sgnl Sum",cat,private_sgnl);
        sum_samples("Bkgd Sum",cat,all_bkgds);
        cats_2lss_p.push_back(cat);
    }
    cats.push_back(merge_cats("2lss_p",cats_2lss_p));

    for (TString bin: mrg_2lss_m) {
        YieldCategory cat = get_category(f,bin,all_samples);
        sum_samples("Sgnl Sum",cat,private_sgnl);
        sum_samples("Bkgd Sum",cat,all_bkgds);
        cats_2lss_m.push_back(cat);
    }
    cats.push_back(merge_cats("2lss_m",cats_2lss_m));
    ////////////////////////////////////////////////////////////////////////////////////////////////

    for (TString bin: bins) {
        YieldCategory cat = get_category(f,bin,all_samples);
        sum_samples("Sgnl Sum",cat,private_sgnl);
        sum_samples("Bkgd Sum",cat,all_bkgds);
        cats.push_back(cat);
    }

    std::cout << get_header(cats,table_name) << std::endl;
    // std::cout << get_header(cats,"") << std::endl;
    for (TString s: private_sgnl) std::cout << get_row(cats,s) << std::endl;
    std::cout << get_line_break(cats) << std::endl;
    // for (TString s: all_bkgds) std::cout << get_row(cats,s) << std::endl;
    // std::cout << line_break << std::endl;
    std::cout << get_row(cats,"Bkgd Sum") << std::endl;
    std::cout << get_row(cats,"Sgnl Sum") << std::endl;
}