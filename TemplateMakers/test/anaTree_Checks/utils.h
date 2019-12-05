#ifndef ANAUTILS_H_
#define ANAUTILS_H_

#include "EFTMultilepton/TemplateMakers/interface/split_string.h"

// Returns the sign of x
int sgn(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

// Reformats a TH1D histogram to have equal bin width on log scale
void binLogX(TH1D* hist) {
    TAxis* axis = hist->GetXaxis();
    int bins = axis->GetNbins();

    Double_t fro = axis->GetXmin();
    Double_t to  = axis->GetXmax();
    Double_t width = (to - fro) / bins;
    Axis_t *new_bins = new Axis_t[bins+1];

    for (int i = 0; i <= bins; i++) {
        new_bins[i] = TMath::Power(10,fro + i*width);
    }
    axis->Set(bins,new_bins);
    delete[] new_bins;
}

template <typename Lep, typename Jet>
void print_event(std::string str, std::vector<Lep> leptons, std::vector<Jet> loose_jets, std::vector<Jet> medium_jets) {
    std::cout << "Printing Event: " << str << std::endl;
    for (const Lep& lep: leptons) {
        std::cout << "Lepton: " << lep.pdgID << " (Q=" << lep.charge << ")" << std::endl;
    }
    std::cout << "btagLoose:  " << loose_jets.size() << std::endl;
    std::cout << "btagMedium: " << medium_jets.size() << std::endl;
    std::cout << std::endl;
}

template <typename T>
std::vector<T> kinematicCut(std::vector<T> input, std::string cut_name, double cut_val, double cut_type) {
    // cut_type = 0 --> cut is minimum allowed
    // cut_type = 1 --> cut is maximum allowed
    std::vector<T> kept;
    double obj_val;
    for (const auto & it: input) {
        if (cut_name == "pt") {
            obj_val = it.obj.Pt();
        } else if (cut_name == "eta") {
            obj_val = abs(it.obj.Eta());
        } else {
            std::cout << "Unknown cut variable" << std::endl;
            break;
        }
        if (cut_type == 0 && obj_val > cut_val) {
            kept.push_back(it);
        } else if (cut_type == 1 && obj_val < cut_val) {
            kept.push_back(it);
        }
    }
    return kept;
}

template <typename T>
std::vector<T> splitCollection(std::vector<T> input, std::set<int> pdgIDs_to_keep) {
    std::vector<T> kept;
    for (const auto & it: input) {
        if (pdgIDs_to_keep.count(it.pdgID)) {
            kept.push_back(it);
        }
    }
}

template <typename T>
std::vector<T> mergeCollection(std::vector<T> v1, std::vector<T> v2) {
    std::vector<T> merged(v1);
    merged.insert(merged.end(),v2.begin(),v2.end());
    return merged;
}

template <typename T>
double getInvMass(const T& p1, const T& p2) {
    return (p1.obj + p2.obj).M();
}

template <typename T>
int sumCharge(std::vector<T> & input) {
    int q = 0;
    for (const auto& it: input) q += it.charge;
    return q;
}

template <typename T>
int countParticles(std::vector<T> & input, int pdg_id) {
    int count = 0;
    for (const auto& it: input) {
        if (pdg_id == abs(it.pdgID)) {
            count += 1;
        }
    }
    return count;
}

template <typename T>
double getMinInvMass(std::vector<T> input) {
    double min_mass = -1;
    for (uint i = 0; i < input.size(); i++) {
        T p1 = input.at(i);
        for (uint j = i; j < input.size(); j++) {
            if (i == j) continue;
            T p2 = input.at(j);
            // double inv_mass = (p1.obj + p2.obj).M();
            double inv_mass = getInvMass(p1,p2);
            if (min_mass == -1) {
                min_mass = inv_mass;
            } else {
                min_mass = std::min(inv_mass,min_mass);
            }
        }
    }
    return min_mass;
}

template <typename T>
bool hasSFZ(std::vector<T> leptons, double cut_window) {
    for (uint i = 0; i < leptons.size(); i++) {
        T p1 = leptons.at(i);
        for (uint j = i; j < leptons.size(); j++) {
            if (i == j) continue;
            T p2 = leptons.at(j);
            if (fabs(p1.pdgID) != fabs(p2.pdgID)) continue;
            double inv_mass = getInvMass(p1,p2);
            if (fabs(inv_mass - 91.2) < cut_window) {
                return true;
            }
        }
    }
    return false;
}

template <typename T> vector<T> sortParticles(vector<T> & particles) {
    vector<T> sorted_particles(particles.begin(),particles.end());
    std::sort(sorted_particles.begin(),sorted_particles.end(), [] (T a, T b) {return a.obj.Pt() > b.obj.Pt();});
    return sorted_particles;
}

void printProgress(int current_index, int total_entries, int interval=20) {
    if (current_index % max(int(total_entries*interval/100.),interval) == 0) {
        float fraction = 100.*current_index/total_entries;
        std::cout << int(fraction) << " % processed " << std::endl;
    }
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

// Returns a new set that exists in both s1 and s2
std::set<TString> set_intersection(std::set<TString> s1, std::set<TString> s2) {
    std::set<TString> new_set;
    for (TString element: s1) {
        if (s2.count(element)) {
            new_set.insert(element);
        }
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
        if (words.size() == 3) {
            TString bin = words.at(0);
            TString syst = words.at(1);
            TString samp = words.at(2);
            ret.insert(samp);
        }
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
        if (words.size() == 3) {
            TString bin = words.at(0);
            TString syst = words.at(1);
            TString samp = words.at(2);   
            ret.insert(bin);
        } else if (words.size() == 2 && words.at(1).size() == 0) {
            std::vector<std::string> tmp_words;
            split_string(words.at(0),tmp_words,"__");
            // if (tmp_words.size() > 1) {
            //     continue;
            // }
            TString bin = words.at(0);
            ret.insert(bin);
        }
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
        if (words.size() == 3) {
            TString bin = words.at(0);
            TString syst = words.at(1);
            TString samp = words.at(2);   
            ret.insert(syst);
        }
    }
    return ret;
}

//TODO: This is a pretty big function to include in a utilities header file
bool compare_histogram(TH1EFT* h1, TH1EFT* h2, TString hist_name, bool skip_identical=false) {
    if (h1->GetNbinsX() != h2->GetNbinsX()) {
        std::cout << "ERROR compare_histogram(): nBins mismatch in " << hist_name << std::endl;
        std::cout << "\tf1 hist has " << h1->GetNbinsX() << " bins" << std::endl;
        std::cout << "\tf2 hist has " << h2->GetNbinsX() << " bins" << std::endl;
        return false;
    }

    if (h1->Integral() == 0 && h2->Integral() == 0) {
        return false;
    }

    std::stringstream ss;

    // TString delim = " ";
    TString delim = " | ";
    TString indent = "  ";

    ss << "Histogram: " << hist_name << std::endl;

    double bin_sum1 = 0.0;
    double bin_sum2 = 0.0;
    double bin1_err_sum = 0.0;
    double bin2_err_sum = 0.0;
    double abs_diff = 0.0;
    for (Int_t i = 1; i <= h1->GetNbinsX(); i++) {
        double bin1 = h1->GetBinContent(i);
        double bin2 = h2->GetBinContent(i);
        double diff = bin2 - bin1;

        double bin1_err = h1->GetBinError(i);
        double bin2_err = h2->GetBinError(i);

        double rel_err1 = 100*bin1_err / bin1;
        double rel_err2 = 100*bin2_err / bin2;

        double err_sqr1 = bin1_err*bin1_err;
        double err_sqr2 = bin2_err*bin2_err;

        // double sig_diff = diff / std::max(bin1_err,bin2_err);
        double sig_diff = diff / sqrt(err_sqr1 + err_sqr2);

        bin1_err_sum += err_sqr1;
        bin2_err_sum += err_sqr2;

        bin_sum1 += bin1;
        bin_sum2 += bin2;
        abs_diff += abs(diff);

        TString bin1_str = TString::Format("%+.2f +/- %+.2f(%.1f%%)",bin1,bin1_err,rel_err1);
        TString bin2_str = TString::Format("%+.2f +/- %+.2f(%.1f%%)",bin2,bin2_err,rel_err2);
        TString diff_str = TString::Format("(%+.2f) (%+.1f std)",diff,sig_diff);

        bool skip_bin = (abs(sig_diff) <= 2.0);

        skip_bin = false;
        if (!skip_bin) {        
            ss << indent << "Bin " << i << ": "
                      << std::setw(25) << std::right << bin1_str << delim
                      << std::setw(25) << std::right << bin2_str << delim
                      << std::setw(7) << std::right << diff_str << std::endl;
        }
    }
    bin1_err_sum = sqrt(bin1_err_sum);
    bin2_err_sum = sqrt(bin2_err_sum);

    double sum1_rel_err = 100*bin1_err_sum / bin_sum1;
    double sum2_rel_err = 100*bin2_err_sum / bin_sum2;

    double perc_diff = 0.0;
    if (bin_sum1) {
        perc_diff = 100*(bin_sum2 - bin_sum1) / bin_sum1;
    }
    double sum_diff = bin_sum2 - bin_sum1;
    // double sum_sig_diff = sum_diff / std::max(bin1_err_sum,bin2_err_sum);
    double sum_sig_diff = sum_diff / sqrt(bin1_err_sum*bin1_err_sum + bin2_err_sum*bin2_err_sum);
    TString sum1_str = TString::Format("%+.2f +/- %+.2f(%.1f%%)",bin_sum1,bin1_err_sum,sum1_rel_err);
    TString sum2_str = TString::Format("%+.2f +/- %+.2f(%.1f%%)",bin_sum2,bin2_err_sum,sum2_rel_err);
    TString abs_str  = TString::Format("%+.3f",abs_diff);
    TString perc_str = TString::Format("%+.2f",perc_diff);
    TString sum_diff_str = TString::Format("%+.3f",sum_diff);
    TString sig_diff_str = TString::Format("%+.1f",sum_sig_diff);
    ss << "h1 Sum: " << sum1_str << std::endl;
    ss << "h2 Sum: " << sum2_str << " (" << perc_str << "%)" << std::endl;
    ss << "Sum Diff: " << sum_diff_str << " (" << sig_diff_str << " std)" << std::endl;
    ss << "Abs Diff: " << abs_str << std::endl;

    bool within_error = abs(sum_sig_diff) < 1.0;

    if (!skip_identical || !within_error) {
        std::cout << ss.str();
    }

    return within_error;
}

//TODO: Unfinished, what was this to be used for? 
int check_histogram(TFile* f, TString hist_name) {
    TH1EFT* h = (TH1EFT*)f->Get(hist_name);

    if (!h) {
        std::cout << "ERROR check_histogram(): f does not have " << hist_name << std::endl;
        return -1;
    }

    if (h->Integral() == 0) {
        return -1;
    }

    double bin_sum = 0.0;
    double bin_err_sum = 0.0;
    for (Int_t i = 1; i <= h->GetNbinsX(); i++) {
    }

    return 0;
}

//ANAUTILS_H
#endif