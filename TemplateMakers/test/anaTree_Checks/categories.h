#ifndef ANACATS_H_
#define ANACATS_H_
#include <map>

#include "EFTMultilepton/TemplateMakers/interface/TH1EFT.h"
#include "utils.h"
// NOTE: Cant do this type of include as it will break when we try to run with lobster, b/c
//      when lobster copies the 'extra_inputs' files it just puts them in a top level directory.
//      We might be able to get around this by putting these header files in the interface directory
//      or by telling the lobster sandbox to include a directory containing these files
//#include "EFTMultilepton/TemplateMakers/test/anaTree_Checks/utils.h"

namespace WorkingPoint {
    enum WP {Loose,Medium,Tight};
}

namespace Lepton {
    enum Category {None,TwoLepOS,TwoLepSS,ThreeLep,FourLep};

    // Returns the Lepton::Category based on the lepton collection passed to it
    // NOTE: Checks the min invariant dilepton mass cut
    template <typename T>
    Lepton::Category getCategory(std::vector<T> leptons) {
        double min_inv_mass = getMinInvMass(leptons);
        int charge = sumCharge(leptons);
        if (leptons.size() == 2 && min_inv_mass > 12.0) {
            return (charge != 0) ? Lepton::TwoLepSS : Lepton::TwoLepOS;
        } else if (leptons.size() == 3) {
            return Lepton::ThreeLep;
        } else if (leptons.size() >= 4) {
            return Lepton::FourLep;
        }
        return Lepton::None;
    }
}

namespace BTag {
    enum Tagger {CSV,DeepCSV};
    enum Category {None,OneBTag,TwoBTag};

    // Returns the BTag::Category based on the jet collection+Lepton::Category passed to it
    // NOTE: This depends on Lepton::Category, so it implicitly includes any cuts that were used to get the lepton category
    template <typename T>
    BTag::Category getCategory(std::vector<T> loose_jets, std::vector<T> medium_jets, Lepton::Category lep_cat) {
        if (lep_cat == Lepton::TwoLepSS || lep_cat == Lepton::TwoLepOS) {
            if (medium_jets.size() >= 2) {
                return BTag::TwoBTag;
            } else if (medium_jets.size() >= 1 && loose_jets.size() >= 2) {
                return BTag::TwoBTag;
            }
        } else if (lep_cat == Lepton::ThreeLep) {
            if (medium_jets.size() >= 2) {
                return BTag::TwoBTag;
            } else if (medium_jets.size() == 1) {
                return BTag::OneBTag;
            }
        } else if (lep_cat == Lepton::FourLep) {
            // Currently the same as the TwoLepSS category
            if (medium_jets.size() >= 2) {
                return BTag::TwoBTag;
            } else if (medium_jets.size() >= 1 && loose_jets.size() >= 2) {
                return BTag::TwoBTag;
            }
        }
        return BTag::None;  // This can be possible
    }

    // See: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
    double getWP(Tagger tagger, WorkingPoint::WP wp) {
        if (tagger == CSV) {
            if (wp == WorkingPoint::Loose) {
                return 0.5426;
            } else if (wp == WorkingPoint::Medium) {
                return 0.8484;
            } else if (wp == WorkingPoint::Tight) {
                return 0.941;
            } else {
                std::cout << "Invalid WorkingPoint for CSV tagger" << std::endl;
                throw;
            }
        } else if (tagger == DeepCSV) {
            if (wp == WorkingPoint::Loose) {
                return 0.1522;
            } else if (wp == WorkingPoint::Medium) {
                return 0.4941;
            } else if (wp == WorkingPoint::Tight) {
                return 0.8001;
            } else {
                std::cout << "Invalid WorkingPoint for DeepCSV tagger" << std::endl;
                throw;
            }
        } else {
            std::cout << "Invalid tagger" << std::endl;
            throw;
        }
    }

    // Jet passes a particular btag req.
    template <typename T>
    bool passesWP(const T& jet, Tagger tagger, WorkingPoint::WP wp) {
        double cut = getWP(tagger,wp);
        if (tagger == CSV) {
            return (jet.csv > cut);
        } else if (tagger == DeepCSV) {
            return (jet.DeepCSV > cut);
        } else {
            return false;
        }
    }

    // Return a list of jets that pass a given btag req.
    template <typename T>
    std::vector<T> applyCut(const std::vector<T> jets, Tagger tagger, WorkingPoint::WP wp) {
        std::vector<T> kept;
        for (const T& jet: jets) {
            if (passesWP(jet,tagger,wp)) {
                kept.push_back(jet);
            }
        }
        return kept;
    }
}

namespace Analysis {
    enum Category {
       None,
       DiEleOSSF,DiMuonOSSF,
       DiEleSSPlus,DiEleSSMinus,
       DiMixSSPlus,DiMixSSMinus,
       DiMuonSSPlus,DiMuonSSMinus,
       ThreeLepOneBTagPlus,ThreeLepOneBTagMinus,ThreeLepOneBTagSFOSZ,
       ThreeLepTwoBTagPlus,ThreeLepTwoBTagMinus,ThreeLepTwoBTagSFOSZ
    };

    // Maps a std::string to a specific Analysis::Category, should be 1-to-1 with kCategoryToString
    const std::map<std::string,Analysis::Category> kStringToCategory = {
        {"" ,Analysis::None},
        {"e+e-" ,Analysis::DiEleOSSF},
        {"mu+mu-" ,Analysis::DiMuonOSSF},
        {"e+e+" ,Analysis::DiEleSSPlus},
        {"e-e-" ,Analysis::DiEleSSMinus},
        {"e+mu+" ,Analysis::DiMixSSPlus},
        {"e-mu-" ,Analysis::DiMixSSMinus},
        {"mu+mu+" ,Analysis::DiMuonSSPlus},
        {"mu-mu-" ,Analysis::DiMuonSSMinus},
        {"3l1b+" ,Analysis::ThreeLepOneBTagPlus},
        {"3l1b-" ,Analysis::ThreeLepOneBTagMinus},
        {"sfz1b" ,Analysis::ThreeLepOneBTagSFOSZ},
        {"3l2b+" ,Analysis::ThreeLepTwoBTagPlus},
        {"3l2b-" ,Analysis::ThreeLepTwoBTagMinus},
        {"sfz2b" ,Analysis::ThreeLepTwoBTagSFOSZ}
    };

    // Maps an Analysis::Category to a specific string, should be in 1-to-1 correspondance with kStringToCategory
    const std::map<Analysis::Category,std::string> kCategoryToString = {
        {Analysis::None,""},
        {Analysis::DiEleOSSF,"e+e-"},
        {Analysis::DiMuonOSSF,"mu+mu-"},
        {Analysis::DiEleSSPlus,"e+e+"},
        {Analysis::DiEleSSMinus,"e-e-"},
        {Analysis::DiMixSSPlus,"e+mu+"},
        {Analysis::DiMixSSMinus,"e-mu-"},
        {Analysis::DiMuonSSPlus,"mu+mu+"},
        {Analysis::DiMuonSSMinus,"mu-mu-"},
        {Analysis::ThreeLepOneBTagPlus,"3l1b+"},
        {Analysis::ThreeLepOneBTagMinus,"3l1b-"},
        {Analysis::ThreeLepOneBTagSFOSZ,"sfz1b"},
        {Analysis::ThreeLepTwoBTagPlus,"3l2b+"},
        {Analysis::ThreeLepTwoBTagMinus,"3l2b-"},
        {Analysis::ThreeLepTwoBTagSFOSZ,"sfz2b"}
    };

    // Returns the string name of Analysis::Category
    std::string getCategoryName(Analysis::Category cat) {
        return (kCategoryToString.count(cat)) ? kCategoryToString.at(cat) : "";
    }

    Analysis::Category getCategoryName(std::string s) {
        return (kStringToCategory.count(s)) ? kStringToCategory.at(s) : Analysis::None;
    }

    // Returns the Lepton::Category that the Analysis::Category is a subset of
    Lepton::Category getParentLeptonCategory(Analysis::Category cat) {
        if (cat == DiEleOSSF || cat == DiMuonOSSF) {
            return Lepton::TwoLepOS;
        } else if (cat == DiEleSSPlus || cat == DiEleSSMinus) {
            return Lepton::TwoLepSS;
        } else if (cat == DiMixSSPlus || cat == DiMixSSMinus) {
            return Lepton::TwoLepSS;
        } else if (cat == DiMuonSSPlus || cat == DiMuonSSMinus) {
            return Lepton::TwoLepSS;
        } else if (cat == ThreeLepOneBTagPlus || cat == ThreeLepOneBTagMinus || cat == ThreeLepOneBTagSFOSZ) {
            return Lepton::ThreeLep;
        } else if (cat == ThreeLepTwoBTagPlus || cat == ThreeLepTwoBTagMinus || cat == ThreeLepTwoBTagSFOSZ) {
            return Lepton::ThreeLep;
        }
        return Lepton::None;
    }

    // Returns a vector of Analysis::Category that are a subset of the Lepton::Category
    // NOTE: This is what determines the bin order in the histograms!
    std::vector<Analysis::Category> getLeptonChildCategories(Lepton::Category cat) {
        std::vector<Analysis::Category> vec;
        // group order: n-leptons,lep-flavor,btag,charge,issfz
        /*if (cat == Lepton::TwoLepSS) {
            vec = {
               DiEleSSPlus,DiEleSSMinus,
               DiMixSSPlus,DiMixSSMinus,
               DiMuonSSPlus,DiMuonSSMinus
            };
        } else if (cat == Lepton::ThreeLep) {
            vec = {
               ThreeLepOneBTagPlus,ThreeLepOneBTagMinus,ThreeLepOneBTagSFOSZ,
               ThreeLepTwoBTagPlus,ThreeLepTwoBTagMinus,ThreeLepTwoBTagSFOSZ
            };
        }*/

        // (alternate) group order: n-leptons,issfz,btag,charge
        if (cat == Lepton::TwoLepOS) {
            vec = {
                DiEleOSSF,DiMuonOSSF
            };
        } else if (cat == Lepton::TwoLepSS) {
            vec = {
                DiEleSSPlus,DiMixSSPlus,DiMuonSSPlus,
                DiEleSSMinus,DiMixSSMinus,DiMuonSSMinus
            };
        } else if (cat == Lepton::ThreeLep) {
            vec = {
                ThreeLepOneBTagPlus,ThreeLepOneBTagMinus,
                ThreeLepTwoBTagPlus,ThreeLepTwoBTagMinus,
                ThreeLepOneBTagSFOSZ,ThreeLepTwoBTagSFOSZ
            };
        }
        return vec;
    }

    // Returns the Analysis::Category, should only be usable after all the relevant input quantities are calculated
    // NOTE: Uses Z-mass window cut and njet req.
    template <typename Lep, typename Jet>
    Analysis::Category getCategory(std::vector<Lep> & leptons, std::vector<Jet> & jets, Lepton::Category lep_cat, BTag::Category btag_cat) {
        if (lep_cat == Lepton::None || btag_cat == BTag::None) {
            return Analysis::None;
        }
        int n_eles = countParticles(leptons,11);
        int n_muons = countParticles(leptons,13);

        bool is_sfz = hasSFZ(leptons,10.0);
        int charge = sumCharge(leptons);

        if (lep_cat == Lepton::TwoLepOS && is_sfz && jets.size() >= 4) {
            if (n_eles == 2) {
                return Analysis::DiEleOSSF;
            } else if (n_muons == 2) {
                return Analysis::DiMuonOSSF;
            } else {
                return Analysis::None;
            }
        } else if (lep_cat == Lepton::TwoLepSS && jets.size() >= 4) {
            if (n_eles == 2) {
                return (charge > 0) ? Analysis::DiEleSSPlus : Analysis::DiEleSSMinus;
            } else if (n_muons == 2) {
                return (charge > 0) ? Analysis::DiMuonSSPlus : Analysis::DiMuonSSMinus;
            } else if (n_eles == 1 && n_muons == 1) {
                return (charge > 0) ? Analysis::DiMixSSPlus : Analysis::DiMixSSMinus;
            } else {
                std::cout << "[Error] Analysis::getCategory() - Invalid number of leptons for Lepton::TwoLepSS category!" << std::endl;
                throw;
            }
        } else if (lep_cat == Lepton::ThreeLep && jets.size() >= 2) {
            if (is_sfz) {
                //NOTE: If we had more btag bins, this will need to be expanded into a proper 'if' block
                return (btag_cat == BTag::OneBTag) ? Analysis::ThreeLepOneBTagSFOSZ : Analysis::ThreeLepTwoBTagSFOSZ;
            } else if (btag_cat == BTag::OneBTag) {
                return (charge > 0) ? Analysis::ThreeLepOneBTagPlus : Analysis::ThreeLepOneBTagMinus;
            } else if (btag_cat == BTag::TwoBTag) {
                return (charge > 0) ? Analysis::ThreeLepTwoBTagPlus : Analysis::ThreeLepTwoBTagMinus;
            } else {
                std::cout << "[Error] Analysis::getCategory() - Invalid btag category for Lepton::ThreeLep category!" << std::endl;
                throw;
            }
        }
        return Analysis::None;
    }
}

// TODO: These two functions dont really belong here, should figure out a better place to put them
// NOTE: Both this and 'print_table' require that the input histogram has the x-axis set with bin labels
void print_bins(TH1EFT* hist, std::vector<int> bin_entries, double norm=0.0) {
    WCPoint* sm_pt = new WCPoint("sm",0.0);
    if (norm > 0) {
        std::cout << "Using norm: " << norm << std::endl;
    }
    for (Int_t bin_idx = 1; bin_idx <= hist->GetNbinsX(); bin_idx++) {
        std::string label = hist->GetXaxis()->GetBinLabel(bin_idx);
        Analysis::Category cat = Analysis::getCategoryName(label);
        double entires = bin_entries.at(bin_idx-1);
        
        WCFit bin_fit = hist->GetBinFit(bin_idx);
        double bin_content = hist->GetBinContent(bin_idx);
        double bin_val = bin_fit.evalPoint(sm_pt);

        double bin_content_err = hist->GetBinError(bin_idx);

        if (norm > 0) {
            bin_content = bin_content*norm;
            bin_content_err = bin_content_err*norm;
            bin_val = bin_val*norm;
        }

        std::string cat_name = Analysis::getCategoryName(cat);

        std::cout << "Bin " << cat_name << std::endl;
        std::cout << " Entries: " << entires << std::endl;
        std::cout << " Content: " << bin_content << std::endl;
        std::cout << " Content Err: " << bin_content_err << std::endl;
        std::cout << " SM Eval: " << bin_val << std::endl;
    }
    delete sm_pt;
}

// Prints the bin content and associated statistical errors from a single histogram in a table row form
void print_table(TH1EFT* hist,TString row_name,double norm,bool incl_header) {
    WCPoint* sm_pt = new WCPoint("sm",0.0);
    //if (norm > 0) {
    //    std::cout << "Using norm: " << norm << std::endl;
    //}

    int width = 0;
    for (Int_t bin_idx = 1; bin_idx <= hist->GetNbinsX(); bin_idx++) {
        std::string label = hist->GetXaxis()->GetBinLabel(bin_idx);
        Analysis::Category cat = Analysis::getCategoryName(label);
        double bin_val = hist->GetBinFit(bin_idx).evalPoint(sm_pt);
        double bin_err = hist->GetBinError(bin_idx);
        if (norm > 0) {
            bin_val = bin_val*norm;
            bin_err = bin_err*norm;
        }
        TString cat_name = Analysis::getCategoryName(cat);  // A bit unnecessary, but trying to be explicit
        TString val_str = TString::Format("%.2f +/- %.2f",bin_val,bin_err);

        width = std::max(width,(int)cat_name.Length());
        width = std::max(width,(int)val_str.Length());
    }

    std::stringstream header;
    std::stringstream row;
    header << std::setw(20) << "";
    row << std::setw(20) << row_name;
    if (norm > 0) {
        header << " & " << std::setw(5) << "norm";
        row << " & " << std::setw(5) << std::right << TString::Format("%.2f",norm);
    }
    for (Int_t bin_idx = 1; bin_idx <= hist->GetNbinsX(); bin_idx++) {
        std::string label = hist->GetXaxis()->GetBinLabel(bin_idx);
        Analysis::Category cat = Analysis::getCategoryName(label);
        double bin_val = hist->GetBinFit(bin_idx).evalPoint(sm_pt);
        double bin_err = hist->GetBinError(bin_idx);
        if (norm > 0) {
            bin_val = bin_val*norm;
            bin_err = bin_err*norm;
        }
        TString cat_name = Analysis::getCategoryName(cat);  // A bit unnecessary, but trying to be explicit
        TString val_str = TString::Format("%.2f +/- %.2f",bin_val,bin_err);

        header << " & " << std::setw(width) << std::left << cat_name;
        row << " & " << std::setw(width) << std::right << val_str;
    }

    if (incl_header) std::cout << header.str() << std::endl;
    std::cout << row.str() << std::endl;

    delete sm_pt;
}

//ANACATS_H
#endif