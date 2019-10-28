// IMPORTANT: If you do not explicitly include these interface headers, the event loop runs 3-4x slower!!
// #include "EFTMultilepton/TemplateMakers/interface/WCPoint.h"
// #include "EFTMultilepton/TemplateMakers/interface/WCFit.h"
// #include "EFTMultilepton/TemplateMakers/interface/TH1EFT.h"
#include "EFTMultilepton/TemplateMakers/interface/Stopwatch.h"
#include "utils.h"
#include "categories.h"
// NOTE: Cant include these header files like this, see note in 'categories.h'
// #include "EFTMultilepton/TemplateMakers/test/anaTree_Checks/utils.h"
// #include "EFTMultilepton/TemplateMakers/test/anaTree_Checks/categories.h"

void runit(TChain* ch, TString outf_name,int max_events) {
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

    std::vector<ttH::Lepton> *preselected_leptons_intree = 0;
    std::vector<ttH::Electron> *preselected_electrons_intree = 0;
    std::vector<ttH::Muon> *preselected_muons_intree = 0;

    std::vector<ttH::Jet> *preselected_jets_intree = 0;
    std::vector<ttH::Jet> *preselected_jets_JECup_intree = 0;
    std::vector<ttH::Jet> *preselected_jets_JECdown_intree = 0;

    int lumiBlock_intree = -1;
    int eventnum_intree = -1;
    int runNumber_intree = -1;

    // ch->SetBranchStatus("*",0);     //disable all branches
    // ch->SetBranchStatus("tight_leptons",1);
    // ch->SetBranchStatus("tight_electrons",1);
    // ch->SetBranchStatus("tight_muons",1);
    // ch->SetBranchStatus("preselected_jets_noTauClean",1);
    // ch->SetBranchStatus("lumiBlock",1);
    // ch->SetBranchStatus("eventnum",1);
    // ch->SetBranchStatus("runNumber",1);

    ch->SetBranchAddress("tight_leptons",&tight_leptons_intree);
    ch->SetBranchAddress("tight_electrons",&tight_electrons_intree);
    ch->SetBranchAddress("tight_muons",&tight_muons_intree);

    ch->SetBranchAddress("preselected_leptons",&preselected_leptons_intree);
    ch->SetBranchAddress("preselected_electrons",&preselected_electrons_intree);
    ch->SetBranchAddress("preselected_muons",&preselected_muons_intree);

    // ch->SetBranchAddress("selected_taus",&selected_taus_intree);

    ch->SetBranchAddress("preselected_jets_noTauClean",&preselected_jets_intree);
    // ch->SetBranchAddress("preselected_jets_JECup_noTauClean",&preselected_jets_JECup_intree);
    // ch->SetBranchAddress("preselected_jets_JECdown_noTauClean",&preselected_jets_JECdown_intree);

    ch->SetBranchAddress("lumiBlock",&lumiBlock_intree);
    ch->SetBranchAddress("eventnum",&eventnum_intree);
    ch->SetBranchAddress("runNumber",&runNumber_intree);


    int incl_counts = 0;
    std::set<int> ls_set;

    ////////////////////////////////////////////////////////////////////////////////////////////////

    ofstream fOut(outf_name.Data());

    for (int i = first_entry; i < last_entry; i++) {
        sw.startTimer("1 Event Loop");

        printProgress(i - first_entry,last_entry - first_entry,3);

        sw.startTimer("2 Get Entry");
        ch->GetEntry(i);
        sw.updateTimer("2 Get Entry");

        incl_counts += 1;

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

        // auto leptons = tight_leptons_intree;
        // auto eles    = tight_electrons_intree;
        // auto muons   = tight_muons_intree;

        auto leptons = preselected_leptons_intree;
        auto eles    = preselected_electrons_intree;
        auto muons   = preselected_muons_intree;

        sw.startTimer("3 Cut Checks");
        std::vector<ttH::Jet> cleaned_jets = kinematicCut(*preselected_jets_intree,"pt",30.0,0);
        cleaned_jets = kinematicCut(cleaned_jets,"eta",2.5,1);
        cleaned_jets = sortParticles(cleaned_jets);
        std::vector<ttH::Jet> loose_jets = BTag::applyCut(cleaned_jets,BTag::Tagger::DeepCSV,WorkingPoint::Loose);
        std::vector<ttH::Jet> medium_jets = BTag::applyCut(cleaned_jets,BTag::Tagger::DeepCSV,WorkingPoint::Medium);
        auto sorted_leps = sortParticles(*leptons);
        sw.updateTimer("3 Cut Checks");

        sw.startTimer("4 Get Category");
        Lepton::Category lepton_category = Lepton::getCategory(*leptons);
        BTag::Category btag_category = BTag::getCategory(loose_jets,medium_jets,lepton_category);
        Analysis::Category ana_cat = Analysis::getCategory(*leptons,cleaned_jets,lepton_category,btag_category);
        sw.updateTimer("4 Get Category");


        // int counter = 1;
        // for (auto lep: *leptons) {
        //     std::cout << "Lepton: " << counter << std::endl;
        //     std::cout << "\tpt:     " << lep.obj.Pt() << std::endl;
        //     std::cout << "\teta:    " << lep.obj.Eta() << std::endl;
        //     std::cout << "\tCharge: " << lep.charge << std::endl;
        //     std::cout << "\tlepMVA: " << lep.lepMVA << std::endl;
        //     counter++;
        // }

        // counter = 1;
        // for (auto jet: cleaned_jets) {
        //     std::cout << "Jet: " << counter << std::endl;
        //     std::cout << "\tpt:      " << jet.obj.Pt() << std::endl;
        //     std::cout << "\teta:     " << jet.obj.Eta() << std::endl;
        //     std::cout << "\tDeepCSV: " << jet.DeepCSV << std::endl;
        //     counter++;
        // }

        // if (sorted_leps.size() < 2 || cleaned_jets.size() < 2) {
        //     sw.updateTimer("1 Event Loop");
        //     continue;
        // }

        if (ana_cat == Analysis::None) {
           // Skips filling histograms for events that dont enter the event selection
           sw.updateTimer("1 Event Loop");
           continue;
        }

        // auto sorted_tight_leps = sortParticles(*leptons);

        sw.startTimer("5 Print Event");
        // std::cout << "Run:LS:Event: " << runNumber_intree << ":" << lumiBlock_intree << ":" << eventnum_intree << std::endl;
        // std::cout << "  Category: " << Analysis::getCategoryName(ana_cat) << std::endl;
        // std::cout << "  nTightLeps: " << leptons->size() << std::endl;
        // std::cout << "  nTightEles: " << eles->size() << std::endl;
        // std::cout << "  nTightMuons: " << muons->size() << std::endl;
        // std::cout << "  nCleanJets: " << cleaned_jets.size() << std::endl;
        // std::cout << "  Lep pt1: " << sorted_leps.at(0).obj.Pt() << std::endl;
        // std::cout << "  Lep pt2: " << sorted_leps.at(1).obj.Pt() << std::endl;
        // std::cout << "  Lep eta1: " << sorted_leps.at(0).obj.Eta() << std::endl;
        // std::cout << "  Lep eta2: " << sorted_leps.at(1).obj.Eta() << std::endl;
        // std::cout << "  Jet pt1: " << cleaned_jets.at(0).obj.Pt() << std::endl;
        // std::cout << "  Jet pt2: " << cleaned_jets.at(1).obj.Pt() << std::endl;
        // std::cout << "  Jet eta1: " << cleaned_jets.at(0).obj.Eta() << std::endl;
        // std::cout << "  Jet eta2: " << cleaned_jets.at(1).obj.Eta() << std::endl;
        // std::cout << "  Jet DeepCSV1: " << cleaned_jets.at(0).DeepCSV << std::endl;
        // std::cout << "  Jet DeepCSV2: " << cleaned_jets.at(1).DeepCSV << std::endl;

        TString sep = " ";
        fOut << runNumber_intree << ":" << runNumber_intree << ":" << lumiBlock_intree << ":" << eventnum_intree << sep
                  << ana_cat << sep
                  << leptons->size() << sep
                  << eles->size() << sep
                  << muons->size() << sep
                  << cleaned_jets.size() << sep
                  << sorted_leps.at(0).obj.Pt() << sep
                  << sorted_leps.at(0).obj.Eta() << sep
                  << sorted_leps.at(0).charge << sep
                  << sorted_leps.at(0).relIso << sep
                  << sorted_leps.at(0).miniIso << sep
                  << sorted_leps.at(0).lepMVA << sep
                  << sorted_leps.at(0).miniIsoCharged << sep
                  << sorted_leps.at(0).miniIsoNeutral << sep
                  << sorted_leps.at(1).obj.Pt() << sep
                  << sorted_leps.at(1).obj.Eta() << sep
                  << sorted_leps.at(1).charge << sep
                  << sorted_leps.at(1).relIso << sep
                  << sorted_leps.at(1).miniIso << sep
                  << sorted_leps.at(1).lepMVA << sep
                  << sorted_leps.at(1).miniIsoCharged << sep
                  << sorted_leps.at(1).miniIsoNeutral << sep
                  << cleaned_jets.at(0).obj.Pt() << sep
                  << cleaned_jets.at(0).obj.Eta() << sep
                  << cleaned_jets.at(0).DeepCSV << sep
                  << cleaned_jets.at(1).obj.Pt() << sep
                  << cleaned_jets.at(1).obj.Eta() << sep
                  << cleaned_jets.at(1).DeepCSV << sep
                  << "\n";
        sw.updateTimer("5 Print Event");

        sw.updateTimer("1 Event Loop");
    }
    std::cout << "Done!" << std::endl;
    std::cout << std::endl;
    std::cout << "Incl Entries: " << incl_counts << std::endl;

    std::cout << std::endl;
    sw.readAllTimers(1,"");
    std::cout << std::endl;
    sw.readAllTimers(0,"");
    std::cout << std::endl;
}


// void pick_events(TString inf_name, int max_events) {
void pick_events() {
    // TString inf_name = "infiles_a26_SingleMuonB.txt";
    // TString outf_name = "sync_a26_SingleMuon_EraB.dat";

    // TString inf_name = "infiles_a27_SingleMuonB.txt";
    // TString outf_name = "sync_a27_SingleMuon_EraB.dat";

    TString inf_name = "infiles_GTv6_SingleMuon_failed_events.txt";
    TString outf_name = "sync_GTv6_SingleMuon_failed_events_psleptons.dat";

    // TString inf_name = "infiles_GTv11_SingleMuon_failed_events.txt";
    // TString outf_name = "sync_GTv11_SingleMuon_failed_events_psleptons.dat";

    // TString inf_name = "infiles_test.txt";
    // TString outf_name = "sync_test.dat";

    // TString fpath_newGT_full = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/from_govner/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/output_tree_94X_dataRun2_v11.root";
    // TString fpath_newGT_skim = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/from_govner/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/skimmed_tree_94X_dataRun2_v11.root";
    // TString fpath_oldGT_full = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/from_govner/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/output_tree_94X_dataRun2_v6.root";
    // TString fpath_oldGT_skim = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/from_govner/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/skimmed_tree_94X_dataRun2_v6.root";

    // TString fpath = fpath_newGT_full;
    // TString outf_name = "sync_newGT_full.dat";

    // TString fpath = fpath_newGT_skim;
    // TString outf_name = "sync_newGT_skim.dat";

    // TString fpath = fpath_oldGT_full;
    // TString outf_name = "sync_oldGT_full.dat";

    // TString fpath = fpath_oldGT_skim;
    // TString outf_name = "sync_oldGT_skim.dat";


    int max_events = -1;

    TChain* ch = new TChain("OSTwoLepAna/summaryTree");

    // ch->AddFile(fpath);
    // std::cout << "Reading events from: " << fpath << std::endl;

    std::cout << "Reading files from: " << inf_name << std::endl;
    TString fpath;
    std::ifstream input_files(inf_name);
    while (input_files >> fpath) {
        ch->Add(fpath);
    }

    runit(ch,outf_name,max_events);
    delete ch;

    input_files.close();

    std::cout << "Finished!" << std::endl;
}