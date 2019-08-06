#ifndef _TightCharges_h
#define _TightCharges_h

#include  "EFTMultilepton/TemplateMakers/interface/KinematicVariable.h"

class TightCharges: public KinematicVariable<int> {

public:
  BNleptonCollection **selCollection;
  string mem;
  string storePrefix;
  unsigned int max_leptons;
  string CERNTightChargeCut;
  unsigned int loopMax;

  TightCharges(BNleptonCollection **_selCollection, string _mem, string _storePrefix, unsigned int _max_leptons);
  void evaluate ();
  void setCut (string cut);
  bool passCut ();
};

TightCharges::TightCharges (BNleptonCollection **_selCollection, string _mem, string _storePrefix, unsigned int _max_leptons):
  selCollection(_selCollection),
  mem(_mem),
  storePrefix(_storePrefix),
  max_leptons(_max_leptons)
{
    this->resetVal = KinematicVariableConstants::INT_INIT;

    for (unsigned int i=0; i<max_leptons; i++) {
      TString branchName = Form("%s_%d_%s", storePrefix.c_str(), i+1, mem.c_str());
      branches[branchName] = BranchInfo<int>(branchName);
      branches[branchName].branchVal = this->resetVal;
    }
}

void TightCharges::evaluate () {
  if (this->evaluatedThisEvent) return;
  evaluatedThisEvent = true;

  //--------
  BNleptonCollection *selectedLeptons = (*selCollection);
  TString branchName;
  loopMax = (unsigned (max_leptons) < selectedLeptons->size()) ? unsigned(max_leptons) : selectedLeptons->size();

  for (unsigned int i = 0; i < loopMax; i++) {
      BNlepton *iLepton = selectedLeptons->at(i);
      branchName = Form("%s_%d_%s", storePrefix.c_str(), i+1, mem.c_str());
      if (iLepton->isMuon) {
//        std::cout << "tkPtErr/tkPt: " << ((BNmuon*) iLepton)->innerTrackPtError / ((BNmuon*) iLepton)->innerTrackPt << std::endl;
        branches[branchName].branchVal = ( ((BNmuon*) iLepton)->innerTrackPtError / max( ((BNmuon*) iLepton)->innerTrackPt, 1.0 ) < 0.2 );
      }
      else {
//         std::cout << "isGsfCtfScPixChargeConsistent: " << ((BNelectron*) iLepton)->isGsfCtfScPixChargeConsistent << std::endl;
//         std::cout << "numberOfExpectedInnerHits: " << ((BNelectron*) iLepton)->numberOfExpectedInnerHits << std::endl;
//         std::cout << "passConvVeto: " << ((BNelectron*) iLepton)->passConvVeto << std::endl;
          branches[branchName].branchVal = ( ((BNelectron*) iLepton)->isGsfCtfScPixChargeConsistent);
      }
  }
}

void TightCharges::setCut(string cut) {
    CERNTightChargeCut = cut;
}

bool TightCharges::passCut() {
    evaluate();
    if (CERNTightChargeCut == "pass") {
      for (unsigned int i = 0; i < loopMax; i++) {
        if (branches[Form("%s_%d_%s", storePrefix.c_str(), i+1, mem.c_str())].branchVal == 0) return false;
      }
      return true;
    }
    else {
      for (unsigned int i = 0; i < loopMax; i++) {
        if (branches[Form("%s_%d_%s", storePrefix.c_str(), i+1, mem.c_str())].branchVal == 0) return true;
      }
      return false;
    }
}

#endif
