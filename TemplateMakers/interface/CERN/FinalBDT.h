#ifndef _FinalBDT_h
#define _FinalBDT_h

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

#include "EFTMultilepton/TemplateMakers/interface/KinematicVariable.h"
#include "EFTMultilepton/TemplateMakers/interface/BranchInfo.h"
#include <typeinfo>

class FinalBDT: public KinematicVariable<double> {

public:

  double FinalBDT_TwoMuon;
  double FinalBDT_MuonEle;
  double FinalBDT_TwoEle;
  double FinalBDT_TwoLepton;

  //Input variables for SS dilepton
  Float_t varlep2AbsEta;
  Float_t varlep2Pt;
  Float_t varMHT;
  Float_t varmindr_lep2_jet;
  Float_t varMT_met_lep1;
  Float_t varsum_pt;
  
  vector<TMVA::Reader *> reader;

  GenericCollectionMember<double, BNleptonCollection> * allLeptonEta;
  GenericCollectionMember<double, BNleptonCollection> * allLeptonPt;
  TwoObjectKinematic<BNleptonCollection,BNjetCollection> * myMHT;
  TwoObjectKinematic<BNleptonCollection,BNjetCollection> *myMinDeltaRLep2Jet;
  TwoObjectKinematic<BNmetCollection,BNleptonCollection> *myMtMetLep;
  TwoObjectKinematic<BNleptonCollection,BNjetCollection> *mySumPt;

  FinalBDT(GenericCollectionMember<double, BNleptonCollection> * _allLeptonEta,
           GenericCollectionMember<double, BNleptonCollection> * _allLeptonPt,
           TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _myMHT,
           TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _myMinDeltaRLep2Jet,
           TwoObjectKinematic<BNmetCollection,BNleptonCollection> * _myMtMetLep,
           TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _mySumPt);

  void evaluate();

};

FinalBDT::FinalBDT(GenericCollectionMember<double, BNleptonCollection> * _allLeptonEta,
                   GenericCollectionMember<double, BNleptonCollection> * _allLeptonPt,
                   TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _myMHT,
                   TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _myMinDeltaRLep2Jet,
                   TwoObjectKinematic<BNmetCollection,BNleptonCollection> * _myMtMetLep,
                   TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _mySumPt):
  allLeptonEta(_allLeptonEta), allLeptonPt(_allLeptonPt), myMHT(_myMHT),
  myMinDeltaRLep2Jet(_myMinDeltaRLep2Jet), myMtMetLep(_myMtMetLep), mySumPt(_mySumPt) {
  
  branches["FinalBDT_TwoMuon"] = BranchInfo<double>("FinalBDT_TwoMuon");
  branches["FinalBDT_MuonElectron"] = BranchInfo<double>("FinalBDT_MuonElectron");
  branches["FinalBDT_TwoElectron"] = BranchInfo<double>("FinalBDT_TwoElectron");
  branches["FinalBDT_TwoLepton"] = BranchInfo<double>("FinalBDT_TwoLepton");
  
  std::vector< TString >catList;
  catList.push_back("SS_ge4jge1t_useSide_2_6var_TwoMuon");
  catList.push_back("SS_ge4jge1t_useSide_2_6var_MuonEle");
  catList.push_back("SS_ge4jge1t_useSide_2_6var_TwoEle");
  catList.push_back("SS_ge4jge1t_useSide_2_6var_test");

  for( unsigned int jj = 0 ; jj < 4 ; ++jj ) { //4 readers for 4 BDTs

    reader.push_back( new TMVA::Reader( "!Color:!Silent" ));
    
    reader[jj]->AddVariable( "lep2AbsEta", &varlep2AbsEta );
    reader[jj]->AddVariable( "lep2Pt", &varlep2Pt );
    reader[jj]->AddVariable( "MHT", &varMHT );
    reader[jj]->AddVariable( "mindr_lep2_jet", &varmindr_lep2_jet );
    reader[jj]->AddVariable( "MT_met_lep1", &varMT_met_lep1 );
    reader[jj]->AddVariable( "sum_pt", &varsum_pt );

    TString dir = (string(getenv("CMSSW_BASE"))+"/src/EFTMultilepton/TemplateMakers/data/CERN/BDT_weights/").c_str();
    TString label = catList[jj];
    TString file_name = "TMVAClassification_BDTG.weights.xml";
    //TString file_name = "TMVAClassification_CFMlpANN.weights.xml";
    TString weight_file_name = dir + label + "/" + file_name;

    reader[jj]->BookMVA( "BDTG method", weight_file_name );
    //reader[jj]->BookMVA( "CFMlpANN method", weight_file_name );

    std::cout << "Loading weight file " << weight_file_name << std::endl;
  }

}

void FinalBDT::evaluate() {
  if (this->evaluatedThisEvent) return;
  evaluatedThisEvent = true;
  
  //--------
  
  allLeptonEta->evaluate();
  allLeptonPt->evaluate();
  myMHT->evaluate();
  myMinDeltaRLep2Jet->evaluate();
  myMtMetLep->evaluate();
  mySumPt->evaluate();
  
  varlep2AbsEta = abs((*allLeptonEta).myVars[1].branchVal); //Get the branch for the second lepton
  varlep2Pt = (*allLeptonPt).myVars[1].branchVal; //Get the branch for the second lepton
  varMHT = (*myMHT).myVars[0].branchVal;
  varmindr_lep2_jet = (*myMinDeltaRLep2Jet).myVars[0].branchVal;
  varMT_met_lep1 = (*myMtMetLep).myVars[0].branchVal;
  varsum_pt = (*mySumPt).myVars[0].branchVal;

  for( unsigned int jj = 0 ; jj < 4 ; ++jj ) {
    
    TMVA::Reader  *tmpReader = reader[jj];
    TString mvaName = "BDTG";
    //TString mvaName = "CFMlpANN";

    //TString bName = mvaName + TString("_") + catList[jj];
    TString bName = "";
    if (jj==0) bName = "FinalBDT_TwoMuon";
    if (jj==1) bName = "FinalBDT_MuonElectron";
    if (jj==2) bName = "FinalBDT_TwoElectron";
    if (jj==3) bName = "FinalBDT_TwoLepton";
    
    TString methodName = mvaName + TString(" method");
    Float_t annOut  = tmpReader->EvaluateMVA( methodName );
    
    branches[bName].branchVal = annOut;
  }
  
}
  

#endif 
