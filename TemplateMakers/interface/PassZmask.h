#ifndef _PassZmask_h
#define _PassZmask_h

#include  "EFTMultilepton/TemplateMakers/interface/KinematicVariable.h"
#include  "EFTMultilepton/TemplateMakers/interface/TwoObjectKinematic.h"

class PassZmask: public KinematicVariable<int> {
  
public:

  string PassZmaskCut;
  
  PassZmask(TwoObjectKinematic<BNleptonCollection,BNleptonCollection> * _myZmass,
            TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _myMHT,
        GenericCollectionMember<double, BNmetCollection> * _myMet);
  void evaluate();
  void setCut (string cut);
  bool passCut ();
  
  TwoObjectKinematic<BNleptonCollection,BNleptonCollection> * myZmass;
  TwoObjectKinematic<BNleptonCollection,BNjetCollection> * myMHT;
  GenericCollectionMember<double, BNmetCollection> * myMet;
};

PassZmask::PassZmask(TwoObjectKinematic<BNleptonCollection,BNleptonCollection> * _myZmass,
                     TwoObjectKinematic<BNleptonCollection,BNjetCollection> * _myMHT,
                     GenericCollectionMember<double, BNmetCollection> * _myMet):
  myZmass(_myZmass), myMHT(_myMHT), myMet(_myMet) {
  
    branches["PassZmask_mht"] = BranchInfo<int>("PassZmask_mht");
    branches["PassZmask_met"] = BranchInfo<int>("PassZmask_met");
    
}

void PassZmask::evaluate() {
  if (this->evaluatedThisEvent) return;
  evaluatedThisEvent = true;

  //--------

  myZmass->evaluate();
  myMHT->evaluate();
  myMet->evaluate();

  float Zmass = (*myZmass).myVars[0].branchVal;
  float mht = (*myMHT).myVars[0].branchVal;
  float met = (*myMet).myVars[0].branchVal;

  int PassZmask_mht = ( (Zmass < (65.5 + 3*mht/8)) || (Zmass > (108 - mht/4)) || (Zmass < (79 - 3*mht/4)) || (Zmass > (99 + mht/2)) );
  int PassZmask_met = ( (Zmass < (65.5 + 3*met/8)) || (Zmass > (108 - met/4)) || (Zmass < (79 - 3*met/4)) || (Zmass > (99 + met/2)) );

  branches["PassZmask_mht"].branchVal = PassZmask_mht;
  branches["PassZmask_met"].branchVal = PassZmask_met;

}

void PassZmask::setCut(string cut) {
  PassZmaskCut = cut;
}

bool PassZmask::passCut() {
  evaluate();
  if (PassZmaskCut == "PassZmask_mht") {
    if (branches["PassZmask_mht"].branchVal == 0) return false;
    else return true;
  }
  else if (PassZmaskCut == "PassZmask_met") {
    if (branches["PassZmask_met"].branchVal == 0) return false;
    else return true;
  }
  else return true;
}


#endif 
