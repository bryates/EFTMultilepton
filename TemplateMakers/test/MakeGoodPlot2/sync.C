#include <iostream>
#include <fstream>
#include "includes.h"

void addTriggers(const std::vector<TString> &in, std::unordered_set<string> *trig) {
    for(auto &t : in) {
      trig->insert(t.Data());
      for(int i = 17; i >= 0; i--) {
        // Build HLT paths with version numbers (some in the hist files have versions)
        // Seems terrible, but hash tables are O(1) time complexity, so just a small penalty in memory
        // Faster than stipping off the versions per event O(n)
        trig->insert(TString::Format("%s_v%d", t.Data(), i).Data());
        /*
        std::cout << TString::Format("Adding: %s_v%d", t.Data(), i).Data() << "\t";
        std::cout << endl;
        */
      }
    }
}

template<class T>
void printQuantity(ofstream &myfile, T quant) {
  for(auto it : quant)
      myfile << it << "\t";
  myfile << std::endl;
}

template<class T>
void printHTML(ofstream &myhtml, TString &name, T quant) {
  for(auto it : quant)
      myhtml << "<tr>\n<th>" << name << "</th>\n<th>" << it << "</th>\n</tr>";
  myhtml << "</table>";
}

int findHLT(const std::vector<string> *trig, const std::unordered_set<string> &searchTrig, std::vector<bool> &goodtrig) {
  bool fired = false;
  for(auto &t : *trig) {
    if(searchTrig.find(t) != searchTrig.end()) {
      goodtrig.push_back(1);
      return 1; // Found a trigger, no need to keep looking
    }
  }
  if(!fired) goodtrig.push_back(0);
  return 0;
}

void sync(bool full=true, bool isData=false) {
    //std::vector<int> events = {4962184, 4962191, 4962189, 4962186, 4962204, 4962182, 4962200, 4962196, 4962199, 4962206, 4962201, 4962205, 4962214, 4962198, 4962216, 4962221, 4962239, 4962220, 4962215, 4962187, 4962208, 4962223, 4962228, 4962225, 4962217, 4962237, 4962212, 4962231, 4962233, 4962209, 4962188, 4962249, 4962238, 4962244, 4962222, 4962224, 4962236, 4962245, 4962264, 4962246, 4962243, 4962240, 4962248, 4962262, 4962265, 4962254, 4962255, 4962273, 4962293, 4962232, 4962298, 4962271, 4962270, 4962247, 4962290, 4962302, 4962283, 4962274, 4962299, 4962297, 4962295, 4962258, 4962312, 4962303, 4962325, 4962268, 4962300, 4962326, 4962322, 4962287, 4962319, 4962318, 4962314, 4962330, 4962327, 4962333, 4962323, 4962309, 4962332, 4962331, 4962343, 4962353, 4962338, 4962354, 4962308, 4962340, 4962339, 4962328, 4962360, 4962363, 4962365, 4962355, 4962366, 4962380, 4962358, 4962341, 4962369, 4962383, 4962362, 4962368};
    std::vector<int> events = {7453107,4673750,4675654,10727005};
    std::unordered_set<int> eventSet;
    std::unordered_set<string> eeTrig;
    std::unordered_set<string> emTrig;
    std::unordered_set<string> mmTrig;
    std::unordered_set<string> eeeTrig;
    std::unordered_set<string> mmmTrig;
    std::unordered_set<string> eemTrig;
    std::unordered_set<string> mmeTrig;
    std::unordered_set<string> eeeeTrig;
    std::unordered_set<string> mmmmTrig;
    std::unordered_set<string> eeemTrig;
    std::unordered_set<string> eemmTrig;
    std::unordered_set<string> mmmeTrig;
    std::map<TString, int> count;
    count["events"] = 0;
    count["ele0"] = 0;
    count["ele1"] = 0;
    count["ele2"] = 0;
    count["ele3"] = 0;
    count["ele4"] = 0;
    count["mu0"] = 0;
    count["mu1"] = 0;
    count["mu2"] = 0;
    count["mu3"] = 0;
    count["mu4"] = 0;
    count["tau0"] = 0;
    count["tau1"] = 0;
    count["tau2"] = 0;
    count["tau3"] = 0;
    count["tau4"] = 0;
    count["tau5"] = 0;
    count["tau6"] = 0;
    count["tau7"] = 0;
    count["tau8"] = 0;
    count["tau9"] = 0;
    count["jet0"] = 0;
    count["jet1"] = 0;
    count["jet2"] = 0;
    count["jet3"] = 0;
    count["jet4"] = 0;
    count["jet5"] = 0;
    count["jet6"] = 0;
    count["jet7"] = 0;
    count["jet8"] = 0;
    count["jet9"] = 0;
    count["jet10"] = 0;
    count["jet11"] = 0;
    count["jet12"] = 0;
    count["jet13"] = 0;
    count["jet14"] = 0;
    count["jet15"] = 0;
    count["jet16"] = 0;
    count["jet17"] = 0;
    count["bjet0"] = 0;
    count["bjet1"] = 0;
    count["bjet2"] = 0;
    count["bjet3"] = 0;
    count["bjet4"] = 0;
    count["bjet5"] = 0;
    count["bjet6"] = 0;
    count["eetrig"] = 0;
    count["emtrig"] = 0;
    count["mmtrig"] = 0;
    count["eeetrig"] = 0;
    count["mmmtrig"] = 0;
    count["eemtrig"] = 0;
    count["mmetrig"] = 0;
    count["eeeetrig"] = 0;
    count["mmmmtrig"] = 0;
    count["eeemtrig"] = 0;
    count["eemmtrig"] = 0;
    count["mmmetrig"] = 0;
    count["3l"] = 0;
    count["3lsfz"] = 0;
    std::cout << "Building unordered set of events" << std::endl;
    for(auto e : events)
        eventSet.insert(e);
    std::vector<TString> SingleMuonTriggers = {"HLT_IsoMu24", "HLT_IsoMu27"};
    std::vector<TString> SingleElecTriggers = {"HLT_Ele32_WPTight_Gsf", "HLT_Ele35_WPTight_Gsf"};
    //std::vector<TString> SingleElecTriggers = {"HLT_Ele32_WPTight_Gsf_L1DoubleEG", "HLT_Ele35_WPTight_Gsf"};
    std::vector<TString> DoubleMuonTrig = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8"};
    std::vector<TString> DoubleElecTrig = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ"};
    std::vector<TString> MuonEGTrig = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL", "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ"};
    std::vector<TString> TripleElecTrig = {"HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL"};
    std::vector<TString> TripleMuonTrig = {"HLT_TripleMu_12_10_5"};
    std::vector<TString> DoubleMuonElecTrig = {"HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ"};
    std::vector<TString> DoubleElecMuonTrig = {"HLT_Mu8_DiEle12_CaloIdL_TrackIdL"};
    std::vector<TString> patience = {
"Patience is not the ability to wait, but the ability to keep a good attitude while waiting.\n\t\t\t-Anonymous","Patience is bitter, but its fruit is sweet.\n\t\t\t-Aristotle","Trust the process. Your time is coming. Just do the work and the results will handle themselves.\n\t\t\t-Tony Gaskins","God has perfect timing; never early, never late. It takes a little patience and it takes a lot of faith but it's worth the wait.\n\t\t\t-Anonymous","Stay patient and trust the journey.\n\t\t\t-Anonymous","Have patience. All things are difficult before they become easy.\n\t\t\t-Saadi","Be patient. Everything is coming together.\n\t\t\t-Anonymous","Two things define you: your patience when you have nothing and your attitude when you have everything.\n\t\t\t-Anonymous","A man who masters patience masters everything else.\n\t\t\t-Anonymous","Patience is also a form of action.\n\t\t\t-Anonymous","Patience is the calm acceptance that things can happen in a different order than the one you have in your mind.\n\t\t\t-David G Allen","Our patience will achieve more than our force.\n\t\t\t-Edmund Burke","Patience is a conquering virtue.\n\t\t\t-Geoffrey Chaucer","Patience is when you're supposed to get mad, but you choose to understand.\n\t\t\t-Anonymous","Be patient, good things take time.\n\t\t\t-Anonymous","Tolerance and patience should not be read as signs of weakness, they are signs of strength.\n\t\t\t-Anonymous","Patience is the companion of wisdom.\n\t\t\t-Anonymous","The two most powerful warriors are patience and time.\n\t\t\t-Leo Tolstoy","The secret of patience is to do something else in the meantime.\n\t\t\t-Croft M Pentz","My dad said to me - Work hard and be patient. It was the best advice he ever gave me. You have to put the hours in.\n\t\t\t-Simon Cowell","It is easier to find men who will volunteer to die. Than to find those who are willing to endure pain with patience.\n\t\t\t-Julius Caesar","When you encounter various trials, big or small, be full of joy. They're opportunities to learn patience.\n\t\t\t-Scott Curran","Be strong enough to let go and wise enough to wait for what you deserve.\n\t\t\t-Anonymous","Patience and fortitude conquer all things.\n\t\t\t-Ralph Waldo Emerson","With love and patience, nothing is impossible.\n\t\t\t-Daisaku Ikeda","To lose patience is to lose the battle.\n\t\t\t-Mahatma Gandhi","Patience is the key to contentment.\n\t\t\t-The Prophet Muhammad (Peace Be Upon Him)","Patience has its limits. Take it too far, and it's cowardice.\n\t\t\t-George Jackson","I will not be distracted by noise, chatter, or setbacks. Patience, commitment, grace, and purpose will guide me.\n\t\t\t-Louise Hay","Patience is a key element of success.\n\t\t\t-Bill Gates","He that can have patience can have what he will.\n\t\t\t-Benjamin Franklin","Patience attracts happiness; it brings near that which is far.\n\t\t\t-Swahili Proverb","Patience is the road to wisdom.\n\t\t\t-Kao Kalia Yang","All great achievements require time.\n\t\t\t-Maya Angelou","Patience, persistence, and perspiration make an unbeatable combination for success.\n\t\t\t-Napoleon Hill"};
    /*
    for(auto &t : SingleElecTriggers) {
      eeTrig.insert(t.Data());
      for(int i = 17; i >= 0; i--) {
        // Build HLT paths with version numbers (some in the hist files have versions)
        // Seems terrible, but hash tables are O(1) time complexity, so just a small penalty in memory
        // Faster than stipping off the versions per event O(n)
        eeTrig.insert(TString::Format("%s_v%d", t.Data(), i).Data());
        std::cout << TString::Format("Adding: %s_v%d", t.Data(), i).Data() << "\t";
        std::cout << endl;
      }
    }
    for(auto &t : DoubleElecTrig) {
      eeTrig.insert(t.Data());
      for(int i = 17; i >= 0; i--)
        eeTrig.insert(TString::Format("%s_v%d", t.Data(), i).Data());
    }
    */

    std::cout << "Building unordered set of triggers" << std::endl;
    std::unordered_set<string> *Trig = &eeTrig;
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleElecTrig, Trig);

    Trig = &emTrig;
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(MuonEGTrig, Trig);

    Trig = &mmTrig;
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(DoubleMuonTrig, Trig);

    Trig=&eeeTrig;
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleElecTrig, Trig);
    addTriggers(TripleElecTrig, Trig);

    Trig=&mmmTrig;
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(DoubleMuonTrig, Trig);
    addTriggers(TripleMuonTrig, Trig);

    Trig=&eemTrig;
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleElecTrig, Trig);
    addTriggers(MuonEGTrig, Trig);
    addTriggers(DoubleElecMuonTrig, Trig);

    Trig=&mmeTrig;
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleMuonTrig, Trig);
    addTriggers(MuonEGTrig, Trig);
    addTriggers(DoubleMuonElecTrig, Trig);

    Trig=&eeeeTrig;
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleElecTrig, Trig);
    addTriggers(TripleElecTrig, Trig);

    Trig=&mmmmTrig;
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(DoubleMuonTrig, Trig);
    addTriggers(TripleMuonTrig, Trig);

    Trig=&eeemTrig;
    addTriggers(TripleElecTrig, Trig);
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleElecTrig, Trig);
    addTriggers(MuonEGTrig, Trig);
    addTriggers(DoubleElecMuonTrig, Trig);

    Trig=&eemmTrig;
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleElecTrig, Trig);
    addTriggers(DoubleMuonTrig, Trig);
    addTriggers(MuonEGTrig, Trig);
    addTriggers(DoubleElecMuonTrig, Trig);
    addTriggers(DoubleMuonElecTrig, Trig);

    Trig=&mmmeTrig;
    addTriggers(TripleMuonTrig, Trig);
    addTriggers(SingleMuonTriggers, Trig);
    addTriggers(SingleElecTriggers, Trig);
    addTriggers(DoubleMuonTrig, Trig);
    addTriggers(MuonEGTrig, Trig);
    addTriggers(DoubleMuonElecTrig, Trig);

    std::cout << "Sifthing through the data" << std::endl;
    auto r = new TRandom3(0);
    std::cout << patience.at(int(r->Uniform(0, patience.size()))) << std::endl;
    /*
    for(auto &ee : eeTrig) std::cout << ee << "\t";
    std::cout << std::endl;
    */
    ofstream myfile;
    myfile.open ("test.txt");
    ofstream myhtml;
    TString htmlname = "/afs/crc.nd.edu/user/b/byates2/www/sync.html";
    if(isData)
      htmlname = "/afs/crc.nd.edu/user/b/byates2/www/syncData.html";
    myhtml.open (htmlname);
    myhtml << "<table>\n";
    //TString path = "/hadoop/store/user/byates/lobster_trees__EFT_syn_2_11_20_central_noNdaud_noMult_noTauClean/ttH/";
    //TString path = "/hadoop/store/user/byates/lobster_trees__EFT_syn_24_11_20_central_noNdaud_noMult_noIso_noSuperCluster_noHoE_noEMuClean/ttH/";
    TString path = "/hadoop/store/user/byates/lobster_trees__EFT_syn_14_12_20_central_noNdaud_noMult_noIso_noSuperCluster_noHoE_noEMuClean_minTauIsoID_preselected/ttH/";
    // Good agreement https://indico.cern.ch/event/955306/attachments/2147494/3620217/Sync.%20Nov.20.pdf
    //TString path = "/hadoop/store/user/byates/lobster_trees__EFT_syn_17_11_20_central_noNdaud_noMult_noTauClean_noIso_noSuperCluster/ttH/";
    //TString path = "/hadoop/store/user/byates/lobster_trees__EFT_syn_8_1_21_central_noNdaud_noMult_noIso_noSuperCluster_noHoE_noEMuClean_minTauIsoID_preselected_emuClean_v1/ttH/";
    std::vector<TString> files = {"output_tree_1.root","output_tree_2.root","output_tree_3.root"};
    if(isData) {
      path = "/hadoop/store/user/byates/lobster_trees__EFT_syn_11_1_21_data_noNdaud_noMult_noIso_noSuperCluster_noHoE_noEMuClean_minTauIsoID_preselected_emuClean_v1/ttH/";
      files = {"output_tree_1.root","output_tree_2.root"};
    }
    //std::vector<TString> files = {"/hadoop/store/user/byates/lobster_trees__EFT_syn_29_10_20_central_noNdaud_noMult_rawJets/ttH/output_tree_3.root"};
    //std::vector<TString> files = {"/hadoop/store/user/byates/lobster_trees__EFT_syn_22_10_20_central/ttH/output_tree_3.root"};
    //std::vector<TString> files = {"/hadoop/store/user/byates/lobster_trees__EFT_syn_12_10_20_central_furong/ttH/output_tree_3.root"};
    //std::vector<TString> files = {"/hadoop/store/user/byates/lobster_trees__EFT_syn_19_10_20_central_elePreselection/ttH/output_tree_1.root", "/hadoop/store/user/byates/lobster_trees__EFT_syn_19_10_20_central_elePreselection/ttH/output_tree_2.root", "/hadoop/store/user/byates/lobster_trees__EFT_syn_19_10_20_central_elePreselection/ttH/output_tree_3.root"};
    //std::vector<TString> files = {"/hadoop/store/user/byates/lobster_trees__EFT_syn_12_10_20_central_furong/ttH/output_tree_1.root", "/hadoop/store/user/byates/lobster_trees__EFT_syn_12_10_20_central_furong/ttH/output_tree_2.root", "/hadoop/store/user/byates/lobster_trees__EFT_syn_12_10_20_central_furong/ttH/output_tree_3.root"};
    for(auto & f : files) {
        //auto fin = new TFile("/hadoop/store/user/byates/lobster_trees__EFT_syn_12_10_20_central_furong/ttH/output_tree_3.root");
        auto fin = new TFile(path+f);
        //auto fin = new TFile("/hadoop/store/user/byates/lobster_trees__EFT_syn_7_10_20_central_lepTight/ttH/output_tree_3.root");
        //auto fin = new TFile("/hadoop/store/user/byates/lobster_trees__EFT_syn_02_9_20_central/ttH/output_tree_3.root");
        auto summaryTree = (TTree*)fin->Get("OSTwoLepAna/summaryTree");
        vector<ttH::Electron> *ele = 0;
        //summaryTree->SetBranchAddress("preselected_electrons", &ele);
        summaryTree->SetBranchAddress("tight_electrons", &ele);
        vector<ttH::Muon> *mu = 0;
        //summaryTree->SetBranchAddress("preselected_muons", &mu);
        summaryTree->SetBranchAddress("tight_muons", &mu);
        vector<ttH::Lepton> *lep = 0;
        //summaryTree->SetBranchAddress("preselected_leptons", &lep);
        summaryTree->SetBranchAddress("tight_leptons", &lep);
        vector<ttH::Tau> *tau = 0;
        summaryTree->SetBranchAddress("preselected_taus", &tau);
        vector<ttH::Jet> *jet = 0;
        //summaryTree->SetBranchAddress("raw_jets", &jet);
        //summaryTree->SetBranchAddress("preselected_jets", &jet);
        summaryTree->SetBranchAddress("preselected_jets_noTauClean", &jet);
        int eventnum = 0;
        summaryTree->SetBranchAddress("eventnum", &eventnum);
        vector<string> *trig = 0;
        summaryTree->SetBranchAddress("passTrigger", &trig);
        auto *passTrigger_intree = trig;
        std::vector<int> goodevent;
        std::vector<int> goodele;
        std::vector<int> goodmu;
        std::vector<int> goodtau;
        std::vector<int> goodlep;
        std::vector<int> goodjet;
        std::vector<int> goodbjet;
        std::vector<string> goodtrig;
        std::vector<bool> goodeetrig;
        std::vector<bool> goodemtrig;
        std::vector<bool> goodmmtrig;
        std::vector<bool> goodeeetrig;
        std::vector<bool> goodmmmtrig;
        std::vector<bool> goodeemtrig;
        std::vector<bool> goodmmetrig;
        std::vector<bool> goodeeeetrig;
        std::vector<bool> goodmmmmtrig;
        std::vector<bool> goodeeemtrig;
        std::vector<bool> goodeemmtrig;
        std::vector<bool> goodmmmetrig;
        double tagcut = 0.1522;
        tagcut = 0.4941;
        for(int ievent = 0; ievent < summaryTree->GetEntries(); ievent++) {
            summaryTree->GetEntry(ievent);
            if(!full) if(eventSet.find(eventnum) == eventSet.end()) continue;
            *jet = simpleJetCut(*jet,"pt",30.0);
            *jet = simpleJetCut(*jet,"eta",2.4);
            auto taggedjetsmedium = keepTagged(*jet,"DM"); // DM = DeepCSV Medium WP
            auto taggedjetsloose = keepTagged(*jet,"DL");
            int numele = ele->size();
            int nummu = mu->size();
            int numtau = tau->size();
            int numlep = lep->size();
            int numjet = jet->size();
            goodevent.push_back(eventnum);
            goodele.push_back(numele);
            goodmu.push_back(nummu);
            goodtau.push_back(numtau);
            goodlep.push_back(numlep);
            //goodjet.push_back(taggedjetsmedium.size() + taggedjetsloose.size());
            goodjet.push_back(numjet);
            int bjet = 0;
            for(auto &j : *jet)
                if(j.DeepCSV > tagcut) bjet++;
            goodbjet.push_back(bjet);
            for(auto &t : *trig) goodtrig.push_back(t);
            goodtrig.push_back("\t");
            /*
            for(auto &t : *trig) {
              if(eeTrig.find(t) != eeTrig.end()) goodeetrig.push_back(1);
              else goodeetrig.push_back(0);
            }
            */
            count["events"]++;
            count["eetrig"] += findHLT(trig, eeTrig, goodeetrig);
            count["emtrig"] += findHLT(trig, emTrig, goodemtrig);
            count["mmtrig"] += findHLT(trig, mmTrig, goodmmtrig);
            count["eeetrig"] += findHLT(trig, eeeTrig, goodeeetrig);
            count["mmmtrig"] += findHLT(trig, mmmTrig, goodmmmtrig);
            count["eemtrig"] += findHLT(trig, eemTrig, goodeemtrig);
            count["mmetrig"] += findHLT(trig, mmeTrig, goodmmetrig);
            count["eeeetrig"] += findHLT(trig, eeeeTrig, goodeeeetrig);
            count["mmmmtrig"] += findHLT(trig, mmmmTrig, goodmmmmtrig);
            count["eeemtrig"] += findHLT(trig, eeemTrig, goodeeemtrig);
            count["eemmtrig"] += findHLT(trig, eemmTrig, goodeemmtrig);
            count["mmmetrig"] += findHLT(trig, mmmeTrig, goodmmmetrig);
            //std::cout << eventnum << "\t" << numele << "\t" << nummu << "\t" << numjet << "\t" << bjet << std::endl;
            TString cat = TString::Format("ele%d", numele > 4 ? 4 : numele);
            count[cat]++;
            cat = TString::Format("mu%d", nummu > 5 ? 5 : nummu);
            count[cat]++;
            cat = TString::Format("tau%d", numtau > 9 ? 9 : numtau);
            count[cat]++;
            cat = TString::Format("jet%d", numjet > 17 ? 17 : numjet);
            count[cat]++;
            cat = TString::Format("bjet%d", bjet > 6 ? 6 : bjet);
            count[cat]++;
            if(numlep == 3) {
              count["3l"]++;
              if(abs(lep->at(0).charge + lep->at(1).charge + lep->at(2).charge) == 1) {
                for(int l1 = 0; l1 < 3; l1++) {
                  for(int l2 = l1+1; l2 < 3; l2++) {
                    if(abs(lep->at(l1).pdgID) != abs(lep->at(l2).pdgID)) continue;
                    if(lep->at(l1).charge == lep->at(l2).charge) continue;
                    TLorentzVector p1, p2;
                    p1.SetPtEtaPhiM(lep->at(l1).tlv().Pt(), lep->at(l1).tlv().Eta(), lep->at(l1).tlv().Phi(), lep->at(l1).tlv().M());
                    p2.SetPtEtaPhiM(lep->at(l2).tlv().Pt(), lep->at(l2).tlv().Eta(), lep->at(l2).tlv().Phi(), lep->at(l2).tlv().M());
                    if(abs((p1+p2).M() - 91.2) < 10) count["3lsfz"]++;
                  }
                }
              }
            }
            if(eventnum == 4962182 || eventnum == 4962184) {
                /*
                for(size_t i = 0; i < lep->size(); i++) {
                    auto &l = lep->at(i);
                    std::cout << "Lepton " << i << "\tpdgId=" << l.pdgID << std::endl; 
                    std::cout << "Pt=" << l.tlv().Pt() 
                              << "\teta=" << l.tlv().Eta() 
                              << "\tphi=" << l.tlv().Phi()
                              << "\tlepMVA=" << l.lepMVA << std::endl << std::endl;
                }
                */
                std::cout << "Event " << eventnum << std::endl;
                std::cout << "nele=" << ele->size() << std::endl;
                for(size_t i = 0; i < ele->size(); i++) {
                    auto &e = ele->at(i);
                    std::cout << "Lepton " << i << "\tpdgId=" << e.pdgID << std::endl; 
                    std::cout << "Pt=" << e.tlv().Pt() 
                              << "\teta=" << e.tlv().Eta() 
                              << "\tphi=" << e.tlv().Phi()
                              << "\tmvaID=" << e.mvaID << std::endl << std::endl;
                }
                for(size_t i = 0; i < mu->size(); i++) {
                    auto &m = mu->at(i);
                    std::cout << "Lepton " << i << "\tpdgId=" << m.pdgID << std::endl; 
                    std::cout << "Pt=" << m.tlv().Pt() 
                              << "\teta=" << m.tlv().Eta() 
                              << "\tphi=" << m.tlv().Phi() << std::endl << std::endl;
                }
                for(size_t i = 0; i < tau->size(); i++) {
                    auto &t = tau->at(i);
                    std::cout << "Tau " << std::endl; 
                    std::cout << "Pt=" << t.tlv().Pt() 
                              << "\teta=" << t.tlv().Eta() 
                              << "\tphi=" << t.tlv().Phi() << std::endl << std::endl;
                    std::cout << "Tau Ids" << std::endl;
                    std::cout << "byLooseCombinedIsolationDeltaBetaCorr3Hits=" << t.byLooseCombinedIsolationDeltaBetaCorr3Hits << std::endl;
                    std::cout << "byVLooseIsolationMVArun2v1DBnewDMwLT=" << t.byVLooseIsolationMVArun2v1DBnewDMwLT << std::endl;
                    std::cout << "byVLooseIsolationMVArun2v1DBdR03oldDMwLT=" << t.byVLooseIsolationMVArun2v1DBdR03oldDMwLT << std::endl;
                }
                for(size_t i = 0; i < jet->size(); i++) {
                    auto &j = jet->at(i);
                    std::cout << "Jet " << i << "\tpdgId=" << j.pdgID << "\tgenPdgId=" << j.genPdgID << "\tDeepCSV=" << j.DeepCSV << std::endl; 
                    std::cout << "Pt=" << j.tlv().Pt() 
                              << "\teta=" << j.tlv().Eta() 
                              << "\tphi=" << j.tlv().Phi() << std::endl;
                    /*
                    std::cout << "neutral had frac=" << j.nHadFrac << "\tneutral EM frac=" << j.nEmFrac << std::endl;
                              << "\tn constituents=" << j.nMult << "\tcharged had frac=" << j.cHadFrac
                              << "\t charge mult=" << j.cMult << std::endl << std::endl;
                    */
                }
                std::cout << std::endl << "===========================================" << std::endl << std::endl;
            }
            //myfile << eventnum << "\t" << numele << "\t" << nummu << "\t" << numjet << std::endl;
        }
        for(auto it : goodevent)
            myfile << it << "\t";
        myfile << std::endl;
        for(auto it : goodele)
            myfile << it << "\t";
        myfile << std::endl;
        for(auto it : goodmu)
            myfile << it << "\t";
        myfile << std::endl;
        for(auto it : goodtau)
            myfile << it << "\t";
        myfile << std::endl;
        for(auto it : goodjet)
            myfile << it << "\t";
        myfile << std::endl;
        for(auto it : goodbjet)
            myfile << it << "\t";
        myfile << std::endl;
        /*
        for(auto it : goodeetrig)
            myfile << it << "\t";
        myfile << std::endl;
        for(auto &it : goodtrig)
            myfile << it << ",";
        myfile << std::endl;
        */
        printQuantity(myfile, goodevent);
        printQuantity(myfile, goodeetrig);
        printQuantity(myfile, goodemtrig);
        printQuantity(myfile, goodmmtrig);
        printQuantity(myfile, goodevent);
        printQuantity(myfile, goodeeetrig);
        printQuantity(myfile, goodmmmtrig);
        printQuantity(myfile, goodeemtrig);
        printQuantity(myfile, goodmmetrig);
        printQuantity(myfile, goodevent);
        printQuantity(myfile, goodeeeetrig);
        printQuantity(myfile, goodmmmmtrig);
        printQuantity(myfile, goodeeemtrig);
        printQuantity(myfile, goodeemmtrig);
        printQuantity(myfile, goodmmmetrig);

        fin->Close();
        delete fin;

    }

    for(auto & it : count) {
        std::cout << it.first << "=" << it.second << std::endl;
        if(it.first.Contains("trig")) continue;
        if(it.first.Contains("events")) continue;
        myhtml << "<tr>\n<th>" << it.first << "</th>\n<th>" << it.second << "</th>\n</tr>";
    }
    for(auto & it : count) {
        if(!it.first.Contains("trig")) continue;
        if(it.first.Contains("events")) continue;
        myhtml << "<tr>\n<th>" << it.first << "</th>\n<th>" << it.second << "</th>\n</tr>";
    }
 
    //printHTML(myhtml, "eetrig", goodeetrig);
    //printHTML(myhtml, "emtrig", goodemtrig);
    //printHTML(myhtml, "mmtrig", goodmmtrig);
    //printHTML(myhtml, "eeetrig", goodeeetrig);
    //printHTML(myhtml, "mmmtrig", goodmmmtrig);
    //printHTML(myhtml, "eemtrig", goodeemtrig);
    //printHTML(myhtml, "mmetrig", goodmmetrig);
    //printHTML(myhtml, "eeeetrig", goodeeeetrig);
    //printHTML(myhtml, "mmmmtrig", goodmmmmtrig);
    //printHTML(myhtml, "eeemtrig", goodeeemtrig);
    //printHTML(myhtml, "eemmtrig", goodeemmtrig);
    //printHTML(myhtml, "mmmetrig", goodmmmetrig);

    myhtml << "<tr>\n<th>events</th>\n<th>" << count["events"] << "</th>\n</tr>";
    myhtml << "</table>";
    myfile.close();
    myhtml.close();
}
