// IMPORTANT: If you do not explicitly include these interface headers, the event loop runs 3-4x slower!!
#include "EFTMultilepton/TemplateMakers/interface/WCPoint.h"
#include "EFTMultilepton/TemplateMakers/interface/WCFit.h"
#include "EFTMultilepton/TemplateMakers/interface/TH1EFT.h"
#include "EFTMultilepton/TemplateMakers/interface/Stopwatch.h"
#include "utils.h"
#include "categories.h"
// NOTE: Cant include these header files like this, see note in 'categories.h'
// #include "EFTMultilepton/TemplateMakers/test/anaTree_Checks/utils.h"
// #include "EFTMultilepton/TemplateMakers/test/anaTree_Checks/categories.h"

// NOTE: Using skim=1 will mess up the overall normalization, so avoid using it if you want to compare different samples
void runit(TChain* ch, TString outf_name, int max_events, double sm_xsec, int skim) {
    Stopwatch sw;
    int chain_entries = ch->GetEntries();
    int last_entry  = chain_entries;
    if (max_events != -1) {
        last_entry = std::min(max_events,chain_entries);
    }
    int first_entry = 0;

    std::cout << "Chain Entries: " << chain_entries << std::endl;
    std::cout << "To Process: " << last_entry - first_entry << std::endl;

    std::vector<ttH::Lepton> *tight_leptons_intree = 0;
    std::vector<ttH::Electron> *tight_electrons_intree = 0;
    std::vector<ttH::Muon> *tight_muons_intree = 0;
    std::vector<ttH::Tau> *selected_taus_intree = 0;

    std::vector<ttH::Jet> *preselected_jets_intree = 0;
    std::vector<ttH::Jet> *preselected_jets_JECup_intree = 0;
    std::vector<ttH::Jet> *preselected_jets_JECdown_intree = 0;

    std::unordered_map<std::string,double> *eftwgts_intree = 0;
    double originalXWGTUP_intree = -1.;
    double nnpdfWeightUp_intree = -1.;
    double nnpdfWeightDown_intree = -1.;

    double muRWeightUp_intree = -1.;
    double muRWeightDown_intree = -1.;
    double muFWeightUp_intree = -1.;
    double muFWeightDown_intree = -1.;
    double muRmuFWeightUp_intree = -1.;
    double muRmuFWeightDown_intree = -1.;

    int lumiBlock_intree = -1;

    ch->SetBranchAddress("tight_leptons",&tight_leptons_intree);
    ch->SetBranchAddress("tight_electrons",&tight_electrons_intree);
    ch->SetBranchAddress("tight_muons",&tight_muons_intree);

    ch->SetBranchAddress("selected_taus",&selected_taus_intree);

    ch->SetBranchAddress("preselected_jets_noTauClean",&preselected_jets_intree);
    // ch->SetBranchAddress("preselected_jets_JECup_noTauClean",&preselected_jets_JECup_intree);
    // ch->SetBranchAddress("preselected_jets_JECdown_noTauClean",&preselected_jets_JECdown_intree);

    ch->SetBranchAddress("eftwgts",&eftwgts_intree);
    ch->SetBranchAddress("originalXWGTUP",&originalXWGTUP_intree);

    ch->SetBranchAddress("nnpdfWeightUp",&nnpdfWeightUp_intree);
    ch->SetBranchAddress("nnpdfWeightDown",&nnpdfWeightDown_intree);

    ch->SetBranchAddress("muRWeightUp",&muRWeightUp_intree);
    ch->SetBranchAddress("muRWeightDown",&muRWeightDown_intree);

    ch->SetBranchAddress("muFWeightUp",&muFWeightUp_intree);
    ch->SetBranchAddress("muFWeightDown",&muFWeightDown_intree);

    ch->SetBranchAddress("muRmuFWeightUp",&muRmuFWeightUp_intree);
    ch->SetBranchAddress("muRmuFWeightDown",&muRmuFWeightDown_intree);

    ch->SetBranchAddress("lumiBlock",&lumiBlock_intree);

    double incl_wgts = 0.0;
    double incl_sm = 0.0;
    double incl_orig_wgt = 0.0;
    double sm_wgt = 0.0;

    bool is_eft = true;

    int incl_counts = 0;
    std::set<int> ls_set;

    WCPoint* sm_pt = new WCPoint("sm",0.0);
    WCFit* incl_fit = new WCFit();
    TH1EFT* h_mcwgts_sign_incl = new TH1EFT("h_mcwgts_sign_incl","h_mcwgts_sign_incl",4,-2,2);
    TH1EFT* h_sumSM_incl = new TH1EFT("h_sumSM_incl","h_sumSM_incl",1,0,1);
    TH1EFT* h_sumSM_ee_p = new TH1EFT("h_sumSM_ee_p","h_sumSM_ee_p",1,0,1);
    TH1EFT* h_sumSM_ee_m = new TH1EFT("h_sumSM_ee_m","h_sumSM_ee_m",1,0,1);
    TH1EFT* h_sumSM_emu_p = new TH1EFT("h_sumSM_emu_p","h_sumSM_emu_p",1,0,1);
    TH1EFT* h_sumSM_emu_m = new TH1EFT("h_sumSM_emu_m","h_sumSM_emu_m",1,0,1);
    TH1EFT* h_sumSM_mumu_p = new TH1EFT("h_sumSM_mumu_p","h_sumSM_mumu_p",1,0,1);
    TH1EFT* h_sumSM_mumu_m = new TH1EFT("h_sumSM_mumu_m","h_sumSM_mumu_m",1,0,1);
    TH1EFT* h_njets_incl = new TH1EFT("h_njets_incl","h_njets_incl",10,0,10);
    TH1EFT* h_njets_sr = new TH1EFT("h_njets_sr","h_njets_sr",10,0,10);
    TH1EFT* h_njets_sfz1b = new TH1EFT("h_njets_sfz1b","h_njets_sfz1b",10,0,10);
    TH1EFT* h_nleps_incl = new TH1EFT("h_nleps_incl","h_nleps_incl",6,0,6);
    TH1EFT* h_neles_incl = new TH1EFT("h_neles_incl","h_neles_incl",6,0,6);
    TH1EFT* h_nmuons_incl = new TH1EFT("h_nmuons_incl","h_nmuons_incl",6,0,6);
    TH1EFT* h_ntaus_incl = new TH1EFT("h_ntaus_incl","h_ntaus_incl",6,0,6);
    ////////////////////////////////////////////////////////////////////////////////////////////////
    TH1EFT* h_lep1_pt_incl  = new TH1EFT("h_lep1_pt_incl" ,"h_lep1_pt_incl",50,0,250);
    TH1EFT* h_lep1_eta_incl = new TH1EFT("h_lep1_eta_incl","h_lep1_eta_incl",50,-5,5);
    TH1EFT* h_lep1_mva_incl = new TH1EFT("h_lep1_mva_incl","h_lep1_mva_incl",50,-2,1);
    TH1EFT* h_lep2_pt_incl  = new TH1EFT("h_lep2_pt_incl" ,"h_lep2_pt_incl",50,0,250);
    TH1EFT* h_lep2_eta_incl = new TH1EFT("h_lep2_eta_incl","h_lep2_eta_incl",50,-5,5);
    TH1EFT* h_lep2_mva_incl = new TH1EFT("h_lep2_mva_incl","h_lep2_mva_incl",50,-2,1);
    TH1EFT* h_jet1_pt_incl  = new TH1EFT("h_jet1_pt_incl" ,"h_jet1_pt_incl",50,0,250);
    TH1EFT* h_jet1_eta_incl = new TH1EFT("h_jet1_eta_incl","h_jet1_eta_incl",50,-5,5);
    TH1EFT* h_jet2_pt_incl  = new TH1EFT("h_jet2_pt_incl" ,"h_jet2_pt_incl",50,0,250);
    TH1EFT* h_jet2_eta_incl = new TH1EFT("h_jet2_eta_incl","h_jet2_eta_incl",50,-5,5);
    TH1EFT* h_invmass_dilep_incl = new TH1EFT("h_invmass_dilep_incl","h_invmass_dilep_incl",50,0,200);
    TH1EFT* h_invmass_diele_incl = new TH1EFT("h_invmass_diele_incl","h_invmass_diele_incl",50,0,200);
    TH1EFT* h_invmass_dimuon_incl = new TH1EFT("h_invmass_dimuon_incl","h_invmass_dimuon_incl",50,0,200);
    TH1EFT* h_invmass_ditau_incl = new TH1EFT("h_invmass_ditau_incl","h_invmass_ditau_incl",50,0,200);
    ////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<Analysis::Category> cats_2los = Analysis::getLeptonChildCategories(Lepton::TwoLepOS);
    std::vector<Analysis::Category> cats_2lss = Analysis::getLeptonChildCategories(Lepton::TwoLepSS);
    std::vector<Analysis::Category> cats_3lep = Analysis::getLeptonChildCategories(Lepton::ThreeLep);
    std::vector<Analysis::Category> cats_all;
    cats_all.insert(cats_all.end(),cats_2los.begin(),cats_2los.end());
    cats_all.insert(cats_all.end(),cats_2lss.begin(),cats_2lss.end());
    cats_all.insert(cats_all.end(),cats_3lep.begin(),cats_3lep.end());
    std::vector<int> bin_entries_all(cats_all.size(),0);
    TH1EFT* h_all_cats = new TH1EFT("h_all_cats","h_all_cats",cats_all.size(),0,cats_all.size());
    for (uint i = 0; i < cats_all.size(); i++) {
        Int_t bin_idx = i+1;
        Analysis::Category cat = cats_all.at(i);
        std::string cat_name = Analysis::getCategoryName(cat);
        h_all_cats->GetXaxis()->SetBinLabel(bin_idx,cat_name.data());
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    TH1D* h_smwgts_incl = new TH1D("h_smwgts_incl","h_smwgts_incl",100,-9,0.5);
    TH1D* h_smwgts_ee_p = new TH1D("h_smwgts_ee_p","h_smwgts_ee_p",100,-9,0.5);
    TH1D* h_smwgts_ee_m = new TH1D("h_smwgts_ee_m","h_smwgts_ee_m",100,-9,0.5);
    TH1D* h_smwgts_emu_p = new TH1D("h_smwgts_emu_p","h_smwgts_emu_p",100,-9,0.5);
    TH1D* h_smwgts_emu_m = new TH1D("h_smwgts_emu_m","h_smwgts_emu_m",100,-9,0.5);
    TH1D* h_smwgts_mumu_p = new TH1D("h_smwgts_mumu_p","h_smwgts_mumu_p",100,-9,0.5);
    TH1D* h_smwgts_mumu_m = new TH1D("h_smwgts_mumu_m","h_smwgts_mumu_m",100,-9,0.5);
    binLogX(h_smwgts_incl);
    binLogX(h_smwgts_ee_p);
    binLogX(h_smwgts_ee_m);
    binLogX(h_smwgts_emu_p);
    binLogX(h_smwgts_emu_m);
    binLogX(h_smwgts_mumu_p);
    binLogX(h_smwgts_mumu_m);

    TH1D* h_muRUp      = new TH1D("h_muRUp","h_muRUp",100,-1.0,2.5);
    TH1D* h_muRDown    = new TH1D("h_muRDown","h_muRDown",100,-1.0,2.5);
    TH1D* h_muFUp      = new TH1D("h_muFUp","h_muFUp",100,-1.0,2.5);
    TH1D* h_muFDown    = new TH1D("h_muFDown","h_muFDown",100,-1.0,2.5);
    TH1D* h_muRmuFUp   = new TH1D("h_muRmuFUp","h_muRmuFUp",100,-1.0,2.5);
    TH1D* h_muRmuFDown = new TH1D("h_muRmuFDown","h_mmuFuRDown",100,-1.0,2.5);
    TH1D* h_nnpdfUp    = new TH1D("h_nnpdfUp","h_nnpdfUp",100,-1.0,2.5);
    TH1D* h_nnpdfDown  = new TH1D("h_nnpdfDown","h_nnpdfDown",100,-1.0,2.5);

    std::vector<TH1EFT*> th1eft_hists {
        h_sumSM_incl,
        h_sumSM_ee_p,
        h_sumSM_ee_m,
        h_sumSM_emu_p,
        h_sumSM_emu_m,
        h_sumSM_mumu_p,
        h_sumSM_mumu_m,
        h_all_cats,
        h_njets_incl,
        h_njets_sr,
        h_njets_sfz1b,
        h_nleps_incl,
        h_neles_incl,
        h_nmuons_incl,
        h_ntaus_incl,
        h_lep1_pt_incl,
        h_lep1_eta_incl,
        h_lep1_mva_incl,
        h_lep2_pt_incl,
        h_lep2_eta_incl,
        h_lep2_mva_incl,
        h_jet1_pt_incl,
        h_jet1_eta_incl,
        h_jet2_pt_incl,
        h_jet2_eta_incl,
        h_invmass_dilep_incl,
        h_invmass_diele_incl,
        h_invmass_dimuon_incl,
        h_invmass_ditau_incl,
        h_mcwgts_sign_incl
    };

    std::vector<TH1D*> th1d_hists {
        h_smwgts_incl,
        h_smwgts_ee_p,h_smwgts_ee_m,
        h_smwgts_emu_p,h_smwgts_emu_m,
        h_smwgts_mumu_p,h_smwgts_mumu_m,
        h_muRUp,h_muRDown,
        h_muFUp,h_muFDown,
        h_muRmuFUp,h_muRmuFDown,
        h_nnpdfUp,h_nnpdfDown
    };

    WCFit empty_fit({},"");

    for (int i = first_entry; i < last_entry; i++) {
        sw.startTimer("1 Event Loop");

        printProgress(i - first_entry,last_entry - first_entry,3);

        sw.startTimer("2 Get Entry");
        ch->GetEntry(i);
        sw.updateTimer("2 Get Entry");

        double muR_up_dir = sgn(1.0 - muRWeightUp_intree);
        double muF_up_dir = sgn(1.0 - muFWeightUp_intree);
        double muRmuF_up_dir = sgn(1.0 - muRWeightUp_intree);

        double muR_down_dir = sgn(1.0 - muRWeightDown_intree);
        double muF_down_dir = sgn(1.0 - muFWeightDown_intree);
        double muRmuF_down_dir = sgn(1.0 - muRWeightDown_intree);

        bool is_same_dir = true;
        is_same_dir = is_same_dir && (muR_up_dir == muF_up_dir && muR_up_dir == muRmuF_up_dir);
        is_same_dir = is_same_dir && (muR_down_dir == muF_down_dir && muR_down_dir == muRmuF_down_dir);

        // std::cout << "Nominal: " << originalXWGTUP_intree << std::endl;
        // std::cout << "-------------------------------------------" << std::endl;
        // std::cout << "muR Up: " << muRWeightUp_intree << std::endl;
        // std::cout << "muF Up: " << muFWeightUp_intree << std::endl;
        // std::cout << "muR+muF Up: " << muRmuFWeightUp_intree << std::endl;
        // if (muR_up_dir == muF_up_dir && muR_up_dir == muRmuF_up_dir) {
        //     std::cout << "Direction: SAME" << std::endl;
        // } else {
        //     std::cout << "Direction: DIFF" << std::endl;
        // }
        // std::cout << "-------------------------------------------" << std::endl;
        // std::cout << "muR Down: " << muRWeightDown_intree << std::endl;
        // std::cout << "muF Down: " << muFWeightDown_intree << std::endl;
        // std::cout << "muR+muF Down: " << muRmuFWeightDown_intree << std::endl;
        // if (muR_down_dir == muF_down_dir && muR_down_dir == muRmuF_down_dir) {
        //     std::cout << "Direction: SAME" << std::endl;
        // } else {
        //     std::cout << "Direction: DIFF" << std::endl;
        // }
        // std::cout << std::endl;

        ls_set.insert(lumiBlock_intree);
        incl_orig_wgt += originalXWGTUP_intree;

        sw.startTimer("3 Cut Checks");
        std::vector<ttH::Jet> cleaned_jets = kinematicCut(*preselected_jets_intree,"pt",30.0,0);
        cleaned_jets = kinematicCut(cleaned_jets,"eta",2.5,1);
        cleaned_jets = sortParticles(cleaned_jets);
        std::vector<ttH::Jet> loose_jets = BTag::applyCut(cleaned_jets,BTag::Tagger::DeepCSV,WorkingPoint::Loose);
        std::vector<ttH::Jet> medium_jets = BTag::applyCut(cleaned_jets,BTag::Tagger::DeepCSV,WorkingPoint::Medium);
        auto sorted_leps = sortParticles(*tight_leptons_intree);
        sw.updateTimer("3 Cut Checks");

        sw.startTimer("4 Get Category");
        Lepton::Category lepton_category = Lepton::getCategory(*tight_leptons_intree);
        BTag::Category btag_category = BTag::getCategory(loose_jets,medium_jets,lepton_category);
        Analysis::Category ana_cat = Analysis::getCategory(*tight_leptons_intree,cleaned_jets,lepton_category,btag_category);
        sw.updateTimer("4 Get Category");

        if (skim && ana_cat == Analysis::None) {
            // Skips filling histograms for events that dont enter the event selection
            sw.updateTimer("1 Event Loop");
            continue;
        }

        sw.startTimer("5 EFT Wgts Loop");
        sm_wgt = 0.0;
        incl_wgts += originalXWGTUP_intree;
        std::vector<WCPoint> pts;
        if (eftwgts_intree->size()) {
            for (auto& kv: *eftwgts_intree) {
                WCPoint wcpt(kv.first,kv.second);
                pts.push_back(wcpt);
                if (wcpt.isSMPoint()) {
                    sm_wgt = wcpt.wgt;
                }
            }
        } else {
            is_eft = false;
            sm_wgt = originalXWGTUP_intree;
            WCPoint wcpt("smpt",sm_wgt);
            pts.push_back(wcpt);
        }
        sw.updateTimer("5 EFT Wgts Loop");

        sw.startTimer("6 Make Fit");
        WCFit wc_fit(pts,"");

        incl_fit->addFit(wc_fit);
        incl_sm += sm_wgt;
        incl_counts += 1;
        sw.updateTimer("6 Make Fit");

        sw.startTimer("7 Fill Hist");

        double mcwgt_sign = (originalXWGTUP_intree > 0) ? 1 : -1;
        h_mcwgts_sign_incl->Fill(mcwgt_sign,1); // Note: This will not have SumW2 auto-enabled, b/c of the unit weight
        double pt1,pt2 = -1;
        double eta1,eta2 = -99;
        double mva1,mva2 = -99;
        for (uint i1 = 0; i1 < sorted_leps.size(); i1++) {
            auto p1 = sorted_leps.at(i1);
            if (i1 == 0) {
                pt1 = p1.obj.Pt();
                eta1 = p1.obj.Eta();
                mva1 = p1.lepMVA;
            } else if (i1 == 1) {
                pt2 = p1.obj.Pt();
                eta2 = p1.obj.Eta();
                mva2 = p1.lepMVA;
            }
            for (uint i2 = i1; i2 < sorted_leps.size(); i2++) {
                if (i1 == i2) continue;
                auto p2 = sorted_leps.at(i2);
                double inv_mass = getInvMass(p1,p2);
                h_invmass_dilep_incl->Fill(inv_mass,sm_wgt,wc_fit);
                if (abs(p1.pdgID) == 11 && abs(p2.pdgID) == 11) {
                    h_invmass_diele_incl->Fill(inv_mass,sm_wgt,wc_fit);
                } else if (abs(p1.pdgID) == 13 && abs(p2.pdgID) == 13) {
                    h_invmass_dimuon_incl->Fill(inv_mass,sm_wgt,wc_fit);
                }
            }
        }
        h_lep1_pt_incl->Fill(pt1,sm_wgt,wc_fit);
        h_lep1_eta_incl->Fill(eta1,sm_wgt,wc_fit);
        h_lep1_mva_incl->Fill(mva1,sm_wgt,wc_fit);

        h_lep2_pt_incl->Fill(pt2,sm_wgt,wc_fit);
        h_lep2_eta_incl->Fill(eta2,sm_wgt,wc_fit);
        h_lep2_mva_incl->Fill(mva2,sm_wgt,wc_fit);

        for (uint i1 = 0; i1 < selected_taus_intree->size(); i1++) {
            auto p1 = selected_taus_intree->at(i1);
            for (uint i2 = i1; i2 < selected_taus_intree->size(); i2++) {
                if (i1 == i2) continue;
                auto p2 = selected_taus_intree->at(i2);
                double inv_mass = getInvMass(p1,p2);
                h_invmass_dilep_incl->Fill(inv_mass,sm_wgt,wc_fit);
                h_invmass_ditau_incl->Fill(inv_mass,sm_wgt,wc_fit);
            }
        }

        // Reset the pt/eta variables
        pt1 = -1; pt2 = -1;
        eta1 = -99; eta2 = -99;
        // The jets are already sorted by pt
        for (uint i1 = 0; i1 < cleaned_jets.size(); i1++) {
            auto p1 = cleaned_jets.at(i1);
            if (i1 == 0) {
                pt1 = p1.obj.Pt();
                eta1 = p1.obj.Eta();
            } else if (i1 == 1) {
                pt2 = p1.obj.Pt();
                eta2 = p1.obj.Eta();
            }
        }
        h_jet1_pt_incl->Fill(pt1,sm_wgt,wc_fit);
        h_jet1_eta_incl->Fill(eta1,sm_wgt,wc_fit);

        h_jet2_pt_incl->Fill(pt2,sm_wgt,wc_fit);
        h_jet2_eta_incl->Fill(eta2,sm_wgt,wc_fit);

        h_njets_incl->Fill(cleaned_jets.size(),sm_wgt,wc_fit);
        h_nleps_incl->Fill(tight_leptons_intree->size(),sm_wgt,wc_fit);
        h_neles_incl->Fill(tight_electrons_intree->size(),sm_wgt,wc_fit);
        h_nmuons_incl->Fill(tight_muons_intree->size(),sm_wgt,wc_fit);
        h_ntaus_incl->Fill(selected_taus_intree->size(),sm_wgt,wc_fit);
        if (ana_cat != Analysis::None) {
            h_njets_sr->Fill(cleaned_jets.size(),sm_wgt,wc_fit);
        }
        if (ana_cat == Analysis::ThreeLepOneBTagSFOSZ) {
            h_njets_sfz1b->Fill(cleaned_jets.size(),sm_wgt,wc_fit);
        }

        h_muRUp->Fill(muRWeightUp_intree);
        h_muRDown->Fill(muRWeightDown_intree);

        h_muFUp->Fill(muFWeightUp_intree);
        h_muFDown->Fill(muFWeightDown_intree);

        h_muRmuFUp->Fill(muRmuFWeightUp_intree);
        h_muRmuFDown->Fill(muRmuFWeightDown_intree);

        h_nnpdfUp->Fill(nnpdfWeightUp_intree);
        h_nnpdfDown->Fill(nnpdfWeightDown_intree);

        h_sumSM_incl->Fill(0,sm_wgt,wc_fit);
        h_smwgts_incl->Fill(fabs(sm_wgt));
        if (ana_cat == Analysis::DiEleSSPlus) {
            h_sumSM_ee_p->Fill(0,sm_wgt,wc_fit);
            h_smwgts_ee_p->Fill(fabs(sm_wgt));
        } else if (ana_cat == Analysis::DiEleSSMinus) {
            h_sumSM_ee_m->Fill(0,sm_wgt,wc_fit);
            h_smwgts_ee_m->Fill(fabs(sm_wgt));
        } else if (ana_cat == Analysis::DiMixSSPlus) {
            h_sumSM_emu_p->Fill(0,sm_wgt,wc_fit);
            h_smwgts_emu_p->Fill(fabs(sm_wgt));
        } else if (ana_cat == Analysis::DiMixSSMinus) {
            h_sumSM_emu_m->Fill(0,sm_wgt,wc_fit);
            h_smwgts_emu_m->Fill(fabs(sm_wgt));
        } else if (ana_cat == Analysis::DiMuonSSPlus) {
            h_sumSM_mumu_p->Fill(0,sm_wgt,wc_fit);
            h_smwgts_mumu_p->Fill(fabs(sm_wgt));
        } else if (ana_cat == Analysis::DiMuonSSMinus) {
            h_sumSM_mumu_m->Fill(0,sm_wgt,wc_fit);
            h_smwgts_mumu_m->Fill(fabs(sm_wgt));
        }

        for (uint j = 0; j < cats_all.size(); j++) {
            Analysis::Category cat = cats_all.at(j);
            if (cat == ana_cat) {
                bin_entries_all.at(j) += 1;
                h_all_cats->Fill(j+0.5,sm_wgt,wc_fit);
                break;
            }
        }
        sw.updateTimer("7 Fill Hist");

        sw.updateTimer("1 Event Loop");
    }
    std::cout << "Done!" << std::endl;
    std::cout << std::endl;
    std::cout << "Incl Entries: " << incl_counts << std::endl;
    std::cout << "Incl SM Xsec: " << incl_sm << std::endl;
    std::cout << "Incl SM Eval: " << incl_fit->evalPoint(sm_pt) << std::endl;
    std::cout << "Incl Orig Wgt: " << incl_orig_wgt << std::endl;

    double lumi2017 = 41530.;

    std::cout << "Lumi2017: " << lumi2017 << std::endl;
    std::cout << "SM xsec: " << sm_xsec << std::endl;

    // Normalization based on sum of the weights at SM
    // Note: We don't actually modify/scale the histograms here, this is just for printout purposes
    double event_norm = incl_orig_wgt;
    if (is_eft) {
        event_norm = incl_fit->evalPoint(sm_pt);
        // event_norm = incl_sm;   // Should be basically the same
    }
    double norm = lumi2017*sm_xsec / event_norm;
    // double norm = 1.0;

    TString row_name = h_all_cats->GetName();

    // print_bins(h_all_cats,cats_all,bin_entries_all,norm);
    print_bins(h_all_cats,bin_entries_all,norm);
    std::cout << std::endl;
    // print_table(h_all_cats,cats_all,norm);
    print_table(h_all_cats,row_name,norm,true);

    std::cout << std::endl;
    sw.readAllTimers(1,"");
    std::cout << std::endl;
    sw.readAllTimers(0,"");
    std::cout << std::endl;

    if (outf_name.Length()) {
        std::cout << "Saving to " << outf_name << std::endl;
        TFile outf(outf_name,"RECREATE");
        for (TH1EFT* ptr: th1eft_hists) {
            ptr->Write();
        }
        for (TH1D* ptr: th1d_hists) {
            ptr->Write();
        }
        outf.Close();
    }

    for (TH1EFT* ptr: th1eft_hists) {
        delete ptr;
    }
    for (TH1D* ptr: th1d_hists) {
        delete ptr;
    }
}


void check_anaTrees(TString outf_name,TString inf_name, int events, double sm_xsec, int skim) {
    TChain* ch = new TChain("OSTwoLepAna/summaryTree");

    std::cout << "Reading files from: " << inf_name << std::endl;

    TString fpath;
    std::ifstream input_files(inf_name);
    while (input_files >> fpath) {
        ch->Add(fpath);
    }

    // double sm_xsec = 0.0942;   // tZq xsec from rateinfo.h

    runit(ch,outf_name,events,sm_xsec,skim);
    delete ch;

    input_files.close();

    std::cout << "Finished!" << std::endl;
}