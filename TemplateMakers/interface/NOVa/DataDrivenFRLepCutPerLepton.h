#ifndef _DataDrivenFRLepCutPerLepton_h
#define _DataDrivenFRLepCutPerLepton_h

#include "EFTMultilepton/TemplateMakers/interface/KinematicVariable.h"
#include "EFTMultilepton/TemplateMakers/interface/BranchInfo.h"
#include <typeinfo>

template <class collectionType>
class DataDrivenFRLepCutPerLepton: public KinematicVariable<double> {

public:

  //Store branch values so they are accessible to other classes
  vector<BranchInfo<double>> myVars;

  BEANhelper * beanHelper;
  collectionType **selCollection;
  unsigned int number_of_leptons;
  int working_point;
  string file_name_NP;
  string file_name_QF;
  string branchNameNP[6];
  string branchNameQF_sel_fail[6];
  string branchNameQF_sel[6];
  string label;

  TFile * weight_file_NP;
  TFile * weight_file_QF;
  //TH2 * FR_NP_loose_mu; //FR for < 2 b-jets
  //TH2 * FR_NP_loose2_mu; //FR for >= 2 b-jets
  //TH2 * FR_NP_loose_el; //FR for < 2 b-jets
  //TH2 * FR_NP_loose2_el; //FR for >= 2 b-jets
  //TH2 * FR_NP_tight_mu; //FR for < 2 b-jets
  //TH2 * FR_NP_tight2_mu; //FR for >= 2 b-jets
  //TH2 * FR_NP_tight_el; //FR for < 2 b-jets
  //TH2 * FR_NP_tight2_el; //FR for >= 2 b-jets
  TH1 * FR_QF_el_sel_fail; //Charge flip FR
  TH1 * FR_QF_el_sel; //Charge flip FR

  DataDrivenFRLepCutPerLepton(BEANhelper * _beanHelper, collectionType **_selCollection, int _number_of_leptons,
                        int _working_point, string _file_name_NP, string _file_name_QF, string _label);
  ~DataDrivenFRLepCutPerLepton();
  void evaluate();

};

template <class collectionType>
DataDrivenFRLepCutPerLepton<collectionType>::DataDrivenFRLepCutPerLepton(BEANhelper * _beanHelper, collectionType **_selCollection, int _number_of_leptons,
                                                             int _working_point, string _file_name_NP, string _file_name_QF, string _label = ""):
  selCollection(_selCollection), number_of_leptons(_number_of_leptons),
  working_point(_working_point), file_name_NP(_file_name_NP), file_name_QF(_file_name_QF),label(_label) {

  this->resetVal = 1.0;

  for (unsigned int i=0; i<number_of_leptons; i++) {
    branchNameNP[i] = Form("DataDrivenFRLepCut_NP%s_Lep%d", label.c_str(), i+1);
    branchNameQF_sel_fail[i] = Form("DataDrivenFRLepCut_QF_sel_fail%s_Lep%d", label.c_str(), i+1);
    branchNameQF_sel[i] = Form("DataDrivenFRLepCut_QF_sel%s_Lep%d", label.c_str(), i+1);
    branches[branchNameNP[i]] = BranchInfo<double>(branchNameNP[i]);
    branches[branchNameQF_sel_fail[i]] = BranchInfo<double>(branchNameQF_sel_fail[i]);
    branches[branchNameQF_sel[i]] = BranchInfo<double>(branchNameQF_sel[i]);
    branches[branchNameNP[i]].branchVal = 1.0;
    branches[branchNameQF_sel_fail[i]].branchVal = 1.0;
    branches[branchNameQF_sel[i]].branchVal = 1.0;
  }

  string directory = (string(getenv("CMSSW_BASE"))+"/src/EFTMultilepton/TemplateMakers/data/NOVa/lepCut_weights/").c_str();
//   TString weight_file_name_NP = Form("%s%s.root", directory.c_str(), file_name_NP.c_str());
//   weight_file_NP = TFile::Open(weight_file_name_NP);

//   FR_NP_loose_mu = (TH2*)weight_file_NP->Get("FR_loose_mu")->Clone();
//   FR_NP_loose2_mu = (TH2*)weight_file_NP->Get("FR_loose2_mu")->Clone();
//   FR_NP_loose_el = (TH2*)weight_file_NP->Get("FR_loose_el")->Clone();
//   FR_NP_loose2_el = (TH2*)weight_file_NP->Get("FR_loose2_el")->Clone();
//   FR_NP_tight_mu = (TH2*)weight_file_NP->Get("FR_tight_mu")->Clone();
//   FR_NP_tight2_mu = (TH2*)weight_file_NP->Get("FR_tight2_mu")->Clone();
//   FR_NP_tight_el = (TH2*)weight_file_NP->Get("FR_tight_el")->Clone();
//   FR_NP_tight2_el = (TH2*)weight_file_NP->Get("FR_tight2_el")->Clone();

  TString weight_file_name_QF = Form("%s%s.root", directory.c_str(), file_name_QF.c_str());
  weight_file_QF = TFile::Open(weight_file_name_QF);
  FR_QF_el_sel_fail = (TH1*)weight_file_QF->Get("QF_data_el_SS_lepCut_fail")->Clone();
  FR_QF_el_sel = (TH1*)weight_file_QF->Get("QF_data_el_SS_lepCut")->Clone();

}

template <class collectionType>
void DataDrivenFRLepCutPerLepton<collectionType>::evaluate() {

  if (this->evaluatedThisEvent) return;
  evaluatedThisEvent = true;

  for (unsigned int iObj = 0; iObj < (*selCollection)->size(); iObj++) {
    if (iObj < number_of_leptons) {

      double lep_pt = ptr((*selCollection)->at(iObj))->pt;
      double lep_eta = abs(ptr((*selCollection)->at(iObj))->eta);
      int isElectron = 0;
      int lepCut = -2;

      if (ptr((*selCollection)->at(iObj))->isElectron) {
        lepCut = beanHelper->GetElectronLepCut(*(BNelectron*)ptr((*selCollection)->at(iObj))); }
      else {
        lepCut = beanHelper->GetMuonLepCut(*(BNmuon*)ptr((*selCollection)->at(iObj))); }
      
      //TH2 * FR_NP_histo = 0;

      if (lepCut < working_point) {
        double FR = 0.0;
        if (ptr((*selCollection)->at(iObj))->isElectron) {
          if (working_point == 2) {
            if (lep_pt > 40) FR = 0.15; //0.12;
            else if (lep_pt > 20) FR = 0.15; //0.16;
            else FR = 0.15; } //0.16; }
          else if (working_point == 1) FR = 0.69;
          else std::cout << "Error: invalid lepCut FR working point = " << working_point << std::endl;
        }
        else if (ptr((*selCollection)->at(iObj))->isMuon) {
          if (working_point == 2) {
            if (lep_pt > 30) FR = 0.36; //0.40;
            else if (lep_pt > 20) FR = 0.21; //0.34;
            else if (lep_pt > 15) FR = 0.20; //0.26;
            else FR = 0.17; } //0.18; }
          else if (working_point == 1) FR = 0.51;
          else std::cout << "Error: invalid lepCut FR working point = " << working_point << std::endl;
        }
        else std::cout << "Error: lepton is neither muon nor electron" << std::endl;

        if (FR == 1 || FR == 0) std::cout << "Error: DataDrivenFRLepCutPerLepton FR = " << FR << std::endl;
        branches[branchNameNP[iObj]].branchVal = FR/(1.0-FR);
      }
      else {
        if (ptr((*selCollection)->at(iObj))->isElectron == 1) {
          //int pt_bin  = std::max(1, std::min(FR_QF_el->GetNbinsX(), FR_QF_el->GetXaxis()->FindBin(lep_pt)));
          int eta_bin  = std::max(1, std::min(FR_QF_el_sel_fail->GetNbinsX(), FR_QF_el_sel_fail->GetXaxis()->FindBin(lep_eta)));
          branches[branchNameQF_sel_fail[iObj]].branchVal = FR_QF_el_sel_fail->GetBinContent(eta_bin);
          branches[branchNameQF_sel[iObj]].branchVal = FR_QF_el_sel->GetBinContent(eta_bin);
        }
      }
    } //end if ( iObj < number_of_leptons )
  } //end loop over iObj

  //Clean out values from last event
  myVars.clear();
  for ( typename map<TString, BranchInfo<double>>::iterator iBranch = branches.begin();
        iBranch != branches.end(); iBranch++ ) {
    myVars.push_back(iBranch->second);
  }
  
}

template <class collectionType>
DataDrivenFRLepCutPerLepton<collectionType>::~DataDrivenFRLepCutPerLepton() {

  //Delete histograms BEFORE closing file
  //delete FR_NP_loose_mu;
  //delete FR_NP_loose2_mu;
  //delete FR_NP_loose_el;
  //delete FR_NP_loose2_el;
  //delete FR_NP_tight_mu;
  //delete FR_NP_tight2_mu;
  //delete FR_NP_tight_el;
  //delete FR_NP_tight2_el;
  delete FR_QF_el_sel_fail;
  delete FR_QF_el_sel;
  //weight_file_NP->Close();
  weight_file_QF->Close();

}

#endif
