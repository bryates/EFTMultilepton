#ifndef _TwoJetVariables_h
#define _TwoJetVariables_h

#include  "EFTMultilepton/TemplateMakers/interface/KinematicVariable.h"
#include "EFTMultilepton/TemplateMakers/interface/BranchInfo.h"

//Takes two jet collections; may be the same, or different
class TwoJetVariables: public KinematicVariable<double> {

public:
  //Store branch values so they are accessible to other classes
  vector<BranchInfo<double>> myVars;
  
  //An "abs" prefix on a variable indicates the absolute value of a
  //"pair" variable (e.g. |mass|, |deltaR|), or the absolute value
  //of each element of a non-pair variable (e.g. |eta1| + |eta2|)
  //A "vect" prefix indicates the sum of vectors (vect1 + vect2)
  string variable; //mass, mass, MT, deltaR, deltaEta, deltaPhi, absDeltaEta, absDeltaPhi,
                   //pt, pz, energy, eta, phi, absPz, absEta, absPhi,
                   //vectPt, vectPz, vectEnergy, vectEta, vectPhi
                   //absVectPt, absVectPz, absVectEnergy, absVectEta, absVectPhi,
                   //CSV
  //An "abs" suffix indicates the absolute value of each variable as a whole,
  //e.g. sum ( |a1+a2| + |b1+b2| + |c1+c2| ...)
  //An "abs" prefix indicates the absolute value of the final output,
  //e.g. | sum ( (a1+a2) + (b1+b2) + (c1+c2) ...)
  string which_pair; //all_pairs, max, min, avg, sum, closest_to, second_closest_to,
                     //all_pairs_abs, maxAbs, minAbs, avgAbs, sumAbs,
                     //absMax, absMin, absAvg, absSum, vector_sum
                     //num_within
  string new_name; //Replacement name if desired, otherwise set to ""
  BNjetCollection **selCollection1; //first selected collection
  string branch_name_1; //first selected collection name
  unsigned int min1; //first object in the first collection
  unsigned int max1; //last object in the first collection
  BNjetCollection **selCollection2; //second selected collection
  string branch_name_2; //second collected collection name
  unsigned int min2; //first object in the second collection 
  unsigned int max2; //last object in the second collection 
  double target_value; //only matters for closest_to, second_closest_to, and num_within; otherwise, set to -99
  string pair_req_1;
  string pair_req_2;
  double within_value;
  
  TwoJetVariables(string _variable, string _which_pair, string _new_name,
                  BNjetCollection **_selCollection1, string _branch_name_1, int _min1, int _max1,
                  BNjetCollection **_selCollection2, string _branch_name_2, int _min2, int _max2,
                  double _target_value = -99, string _pair_req_1 = "none", string _pair_req_2 = "none", double _within_value = -99);
  
  void evaluate();
  
};

TwoJetVariables::TwoJetVariables(string _variable, string _which_pair, string _new_name,
                                 BNjetCollection **_selCollection1, string _branch_name_1, int _min1, int _max1,
                                 BNjetCollection **_selCollection2, string _branch_name_2, int _min2, int _max2,
                                 double _target_value, string _pair_req_1, string _pair_req_2, double _within_value):
  variable(_variable), which_pair(_which_pair), new_name(_new_name),
  selCollection1(_selCollection1), branch_name_1(_branch_name_1), min1(_min1), max1(_max1),
  selCollection2(_selCollection2), branch_name_2(_branch_name_2), min2(_min2), max2(_max2),
  target_value(_target_value), pair_req_1(_pair_req_1), pair_req_2(_pair_req_2), within_value(_within_value)
{

  //Convert target and within value to integers for branch name
  int target_value_whole = floor(target_value);
  int target_value_fraction = static_cast<int>(target_value*10)%10;
  int within_value_whole = floor(within_value);
  int within_value_fraction = static_cast<int>(within_value*10)%10;
  
  // --Create the branches--
  //Creates branches for each pair of objects
  if (which_pair == "all_pairs" || which_pair == "all_pairs_abs") { 
    for (unsigned int i=0; i<max1; i++) {
      for (unsigned int j=0; j<max2; j++) {
        if (i >= min1-1 && j >= min2-1) { //Start branches at min value 
          //Eliminates pairs of the same object (e.g. jet1_jet1) and redundant pairs (jet1_jet2 and jet2_jet1) 
          if ( typeid(*selCollection1).name() != typeid(*selCollection2).name() || i < j ) {
            TString bName = Form("%s_%d_%s_%d_%s", branch_name_1.c_str(), i+1, branch_name_2.c_str(), j+1, variable.c_str());
            if (which_pair == "all_pairs_abs") bName = Form("%s_%d_%s_%d_abs_%s", branch_name_1.c_str(), i+1, branch_name_2.c_str(), j+1, variable.c_str());
            if (new_name.length() > 0) bName = Form("%s_%d_%d", new_name.c_str(), i+1, j+1);
            branches[bName] = BranchInfo<double>(bName);
          }
        }
      }
    }
  }
  //If you gave it a new name, use that name
  else if (new_name.length() > 0) {
    TString bName = Form("%s", new_name.c_str());
    branches[bName] = BranchInfo<double>(bName);
  }    
  //Creates a branch for the pair of objects
  else if (which_pair == "closest_to" || which_pair == "second_closest_to") { 
    TString bName = Form("%s_%s_%s_%dp%d_%s", branch_name_1.c_str(), branch_name_2.c_str(), which_pair.c_str(),
                         target_value_whole, target_value_fraction, variable.c_str()); 
    branches[bName] = BranchInfo<double>(bName);
  }
  else if (which_pair == "num_within") {
    TString bName = Form("%s_%s_%s_%dp%d_of_%dp%d_%s", branch_name_1.c_str(), branch_name_2.c_str(), which_pair.c_str(),
                         within_value_whole, within_value_fraction, target_value_whole, target_value_fraction, variable.c_str());
    branches[bName] = BranchInfo<double>(bName);
  }
  //Creates a branch for the pair of objects
  else {
    TString bName = Form("%s_%s_%s_%s", branch_name_1.c_str(), branch_name_2.c_str(), which_pair.c_str(), variable.c_str()); 
    branches[bName] = BranchInfo<double>(bName);
  }
  //Initializes branch values to default
  this->resetVal = KinematicVariableConstants::DOUBLE_INIT;

}

void TwoJetVariables::evaluate() {
  if (this->evaluatedThisEvent) return;
  evaluatedThisEvent = true;

  //double thisValue = KinematicVariableConstants::DOUBLE_INIT; //Values of each individual object
  double thisValueSum = 0.0; //Sum over each object value
  //double thisValueSumAbs = 0.0; //Sum over absolute value of each object value
  double thisValueIterator = 0.0; //Counting the total number of individual objects
  double thisValueCounter = 0.0; //Counting the total number of individual objects passing some criteria

  double thisPairValue = KinematicVariableConstants::DOUBLE_INIT; //Values of each pair of objects
  double thisPairValueSum = 0.0; //Sum over value of each object pair
  double thisPairValueSumAbs = 0.0; //Sum over absolute value of each object pair
  double thisPairValueIterator = 0.0; //Conting the total number of distinct pairs
  double thisPairValueCounter = 0.0; //Conting the total number of distinct pairs passing some criteria
  
  double thisPairValue2 = KinematicVariableConstants::DOUBLE_INIT; //For saving pair values from previous iterations
  
  TLorentzVector thisVectorSum(0.,0.,0.,0.); //Sum of vectors of individual objects
  TLorentzVector thisVectorTransverse(0.,0.,0.,0.); //Transverse vectors of individual objects
  TLorentzVector thisVectorTransverseSum(0.,0.,0.,0.); //Sum of transverse vectors of individual objects
  
  TLorentzVector vect1; //Vector for first object
  TLorentzVector vect2; //Vector for second object
  TLorentzVector vect12; //Combined vector of two objects

  TLorentzVector vect1_transverse; //Transverse vector for first object
  TLorentzVector vect2_transverse; //Transverse vector for second object
  TLorentzVector vect12_transverse; //Combined transverse vector of two
  
  //Convert target and within value to integers for branch name
  int target_value_whole = floor(target_value);
  int target_value_fraction = static_cast<int>(target_value*10)%10;
  int within_value_whole = floor(within_value);
  int within_value_fraction = static_cast<int>(within_value*10)%10;

  bool useValue = 0; //Use individual values for sum and average calculations
  if ( variable == "pt" || variable == "pz" || variable == "energy" || variable == "eta" || variable == "phi" ||
       variable == "absPz" || variable == "absEta" || variable == "absPhi" ||
       variable == "CSV") {
    useValue = 1;
  }
  bool usePairValue = 0; //Use pair values for sum and average calculations
  if ( variable == "mass" || variable == "MT" || variable == "deltaR" || variable == "deltaEta" || variable == "deltaPhi" ||
       variable == "absDeltaEta" || variable == "absDeltaPhi" || variable == "vectPt" || variable == "vectPz" ||
       variable == "vectEnergy" || variable == "vectEta" || variable == "vectPhi" || variable == "absVectPhi" ||
       variable == "absVectPz" || variable == "absVectEnergy" || variable == "absVectEta" || variable == "absVectPhi" ) {
    usePairValue = 1;
  }


  //Use individual values
  if ( useValue || which_pair == "vector_sum" ) {
    //Loop over first set of objects alone
    for (unsigned int iObj1 = 0; iObj1 < (*selCollection1)->size(); iObj1++) {

      //Sets min and max number of objects
      if ( iObj1 >= min1-1 && iObj1 < max1 ) {

        thisValueIterator += 1.0;
        vect1.SetPtEtaPhiE(ptr((*selCollection1)->at(iObj1))->pt,ptr((*selCollection1)->at(iObj1))->eta,ptr((*selCollection1)->at(iObj1))->phi,
                           max(ptr((*selCollection1)->at(iObj1))->energy,ptr((*selCollection1)->at(iObj1))->pt)); //Hack for "energy" of MET
        vect1_transverse.SetPtEtaPhiE(ptr((*selCollection1)->at(iObj1))->pt,0.0,ptr((*selCollection1)->at(iObj1))->phi,ptr((*selCollection1)->at(iObj1))->pt);
        thisVectorSum += vect1; thisVectorTransverseSum += vect1_transverse;
        
        if ( variable == "pt" ) { thisValueSum += vect1.Pt(); thisValueCounter += 1.0*( abs(vect1.Pt() - target_value) < within_value ); }
        else if ( variable == "pz" ) { thisValueSum += vect1.Pz(); thisValueCounter += 1.0*( abs(vect1.Pz() - target_value) < within_value ); }
        else if ( variable == "energy" ) { thisValueSum += vect1.E(); thisValueCounter += 1.0*( abs(vect1.E() - target_value) < within_value ); }
        else if ( variable == "eta" ) { thisValueSum += vect1.Eta(); thisValueCounter += 1.0*( abs(vect1.Eta() - target_value) < within_value ); }
        else if ( variable == "phi" ) { thisValueSum += vect1.Phi(); thisValueCounter += 1.0*( abs(vect1.Phi() - target_value) < within_value ); }
        else if ( variable == "absPz" ) { thisValueSum += abs(vect1.Pz()); thisValueCounter += 1.0*( abs(vect1.Pz() - target_value) < within_value ); }
        else if ( variable == "absEta" ) { thisValueSum += abs(vect1.Eta()); thisValueCounter += 1.0*( abs(vect1.Eta() - target_value) < within_value ); }
        else if ( variable == "absPhi" ) { thisValueSum += abs(vect1.Phi()); thisValueCounter += 1.0*( abs(vect1.Phi() - target_value) < within_value ); }
        else if ( variable == "CSV" ) { thisValueSum += max(ptr((*selCollection1)->at(iObj1))->btagCombinedSecVertex,0.0); thisValueCounter += 1.0*( abs(max(ptr((*selCollection1)->at(iObj1))->btagCombinedSecVertex,0.0) - target_value) < within_value ); }
        else if ( which_pair != "vector_sum" ) { std::cerr << " No valid entry for variable: " << variable << std::endl; continue; }
        else continue;
      }      
    }//End loop over iObj1
    
    //Loop over second set of objects alone
    for (unsigned int iObj2 = 0; iObj2 < (*selCollection2)->size(); iObj2++) {

      //Sets min and max number of objects, eliminates use of the same object twice 
      if ( iObj2 >= min2-1 && iObj2 < max2 && (typeid(*selCollection1).name() != typeid(*selCollection2).name()
                            || iObj2 >= (*selCollection1)->size() || iObj2 >= max1 ) ) {
        thisValueIterator += 1.0;
        vect2.SetPtEtaPhiE(ptr((*selCollection2)->at(iObj2))->pt,ptr((*selCollection2)->at(iObj2))->eta,ptr((*selCollection2)->at(iObj2))->phi,
                           max(ptr((*selCollection2)->at(iObj2))->energy,ptr((*selCollection2)->at(iObj2))->pt)); //Hack for "energy" of MET
        vect2_transverse.SetPtEtaPhiE(ptr((*selCollection2)->at(iObj2))->pt,0.0,ptr((*selCollection2)->at(iObj2))->phi,ptr((*selCollection2)->at(iObj2))->pt);
        thisVectorSum += vect2; thisVectorTransverseSum += vect2_transverse;
        
        if ( variable == "pt" ) { thisValueSum += vect2.Pt(); thisValueCounter += 1.0*( abs(vect2.Pt() - target_value) < within_value ); }
        else if ( variable == "pz" ) { thisValueSum += vect2.Pz(); thisValueCounter += 1.0*( abs(vect2.Pz() - target_value) < within_value ); }
        else if ( variable == "energy" ) { thisValueSum += vect2.E(); thisValueCounter += 1.0*( abs(vect2.E() - target_value) < within_value ); }
        else if ( variable == "eta" ) { thisValueSum += vect2.Eta(); thisValueCounter += 1.0*( abs(vect2.Eta() - target_value) < within_value ); }
        else if ( variable == "phi" ) { thisValueSum += vect2.Phi(); thisValueCounter += 1.0*( abs(vect2.Phi() - target_value) < within_value ); }
        else if ( variable == "absPz" ) { thisValueSum += abs(vect2.Pz()); thisValueCounter += 1.0*( abs(vect2.Pz() - target_value) < within_value ); }
        else if ( variable == "absEta" ) { thisValueSum += abs(vect2.Eta()); thisValueCounter += 1.0*( abs(vect2.Eta() - target_value) < within_value ); }
        else if ( variable == "absPhi" ) { thisValueSum += abs(vect2.Phi()); thisValueCounter += 1.0*( abs(vect2.Phi() - target_value) < within_value ); }
        else if ( variable == "CSV" ) { thisValueSum += max(ptr((*selCollection2)->at(iObj2))->btagCombinedSecVertex,0.0); thisValueCounter += 1.0*( abs(max(ptr((*selCollection2)->at(iObj2))->btagCombinedSecVertex,0.0) - target_value) < within_value ); }
        else if ( which_pair != "vector_sum" ) { std::cerr << " No valid entry for variable: " << variable << std::endl; continue; }
        else continue;
      }
    }  

    //Fill the branches
    TString bName = Form("%s_%s_%s_%s", branch_name_1.c_str(), branch_name_2.c_str(), which_pair.c_str(), variable.c_str());
    if (new_name.length() > 0) bName = Form("%s", new_name.c_str());

    if ( which_pair == "avg" ) branches[bName].branchVal = thisValueSum / thisValueIterator;
    else if ( which_pair == "sum" ) branches[bName].branchVal = thisValueSum;
    //else if ( which_pair == "avgAbs" ) branches[bName].branchVal = thisValueSumAbs / thisValueIterator;
    //else if ( which_pair == "sumAbs" ) branches[bName].branchVal = thisValueSumAbs;
    else if ( which_pair == "absAvg" ) branches[bName].branchVal = abs(thisValueSum) / thisValueIterator;
    else if ( which_pair == "absSum" ) branches[bName].branchVal = abs(thisValueSum);
    else if ( which_pair == "vector_sum" ) {
      if ( variable == "mass" ) branches[bName].branchVal = thisVectorSum.M();
      else if ( variable == "MT" ) branches[bName].branchVal = thisVectorTransverseSum.M();
      else if ( variable == "pt" ) branches[bName].branchVal = thisVectorSum.Pt();
      else if ( variable == "pz" ) branches[bName].branchVal = thisVectorSum.Pz();
      else if ( variable == "energy" ) branches[bName].branchVal = thisVectorSum.Energy();
      else if ( variable == "eta" ) branches[bName].branchVal = thisVectorSum.Eta();
      else if ( variable == "phi" ) branches[bName].branchVal = thisVectorSum.Phi();
      else if ( variable == "absPz" ) branches[bName].branchVal = abs(thisVectorSum.Pz());
      else if ( variable == "absEta" ) branches[bName].branchVal = abs(thisVectorSum.Eta());
      else if ( variable == "absPhi" ) branches[bName].branchVal = abs(thisVectorSum.Phi());
      else std::cerr << " No valid entry for variable in vector_sum" << std::endl;
    }
    else if ( which_pair == "num_within" && useValue ) branches[bName].branchVal = thisValueCounter;
    else std::cerr << " No valid entry for which_pair: " << which_pair << std::endl; 
  } //End if ( useValue || which_pair == "vector_sum" )
  
  //Loop over pairs of objects
  //for (typename BNjetCollection::const_iterator object1 = (*selCollection1)->begin(); object1 != (*selCollection1)->end(); ++object1) {
  for (unsigned int iObj1 = 0; iObj1 < (*selCollection1)->size(); iObj1++) {

    //for (typename BNjetCollection::const_iterator object2 = (*selCollection2)->begin(); object2 != (*selCollection2)->end(); ++object2) {
    for (unsigned int iObj2 = 0; iObj2 < (*selCollection2)->size(); iObj2++) {

      //Sets min and max number of objects, eliminates pairs of the same object (e.g. jet1_jet1) and redundant pairs (jet1_jet2 and jet2_jet1) 
      if (iObj1 >= min1-1 && iObj2 >= min2-1 && iObj1<max1 && iObj2<max2 &&
          (typeid(*selCollection1).name() != typeid(*selCollection2).name() || iObj1 < iObj2) ) {

        //Adds requirements to eliminate some pairs of objects
        if ( pair_req_1 == "same_flavour" || pair_req_2 == "same_flavour" ) {          
          BNlepton* lepton_1 = (BNlepton*)ptr((*selCollection1)->at(iObj1));
          BNlepton* lepton_2 = (BNlepton*)ptr((*selCollection2)->at(iObj2));

          if ( ( lepton_1->isMuon != 1 && lepton_1->isElectron != 1 ) || ( lepton_2->isMuon != 1 && lepton_2->isElectron != 1 ) ) {
            std::cout << "Why are we requiring same flavour on non-leptons?" << std::endl;
          }
          else if ( lepton_1->isMuon != lepton_2->isMuon ) continue;          
        }
        
        if ( pair_req_1 == "opposite_sign" || pair_req_2 == "opposite_sign" ) {
          BNlepton* lepton_1 = (BNlepton*)ptr((*selCollection1)->at(iObj1));
          BNlepton* lepton_2 = (BNlepton*)ptr((*selCollection2)->at(iObj2));
          
          if ( ( abs(lepton_1->tkCharge) != 1 && lepton_1->tkCharge != -99 ) || ( abs(lepton_2->tkCharge) != 1 && lepton_2->tkCharge != -99 ) ) {
            std::cout << "Why are we requiring opposite sign on non-leptons?" << std::endl;
            continue;
          }
          else if ( lepton_1->tkCharge + lepton_2->tkCharge != 0 ) continue;          
        }
        
        thisPairValueIterator += 1.0;

        vect1.SetPtEtaPhiE(ptr((*selCollection1)->at(iObj1))->pt,ptr((*selCollection1)->at(iObj1))->eta,ptr((*selCollection1)->at(iObj1))->phi,
                           max(ptr((*selCollection1)->at(iObj1))->energy,ptr((*selCollection1)->at(iObj1))->pt)); //Hack for "energy" of MET
        vect2.SetPtEtaPhiE(ptr((*selCollection2)->at(iObj2))->pt,ptr((*selCollection2)->at(iObj2))->eta,ptr((*selCollection2)->at(iObj2))->phi,
                           max(ptr((*selCollection2)->at(iObj2))->energy,ptr((*selCollection2)->at(iObj2))->pt)); //Hack for "energy" of MET
        vect12 = vect1 + vect2;
        vect1_transverse.SetPtEtaPhiE(ptr((*selCollection1)->at(iObj1))->pt,0.0,ptr((*selCollection1)->at(iObj1))->phi,ptr((*selCollection1)->at(iObj1))->pt);
        vect2_transverse.SetPtEtaPhiE(ptr((*selCollection2)->at(iObj2))->pt,0.0,ptr((*selCollection2)->at(iObj2))->phi,ptr((*selCollection2)->at(iObj2))->pt);
        vect12_transverse = vect1 + vect2;

        if ( variable == "mass" ) thisPairValue = vect12.M();
        else if ( variable == "MT" ) thisPairValue = vect12_transverse.M();
        else if ( variable == "deltaR" ) thisPairValue = vect1.DeltaR(vect2);
        else if ( variable == "deltaEta" ) thisPairValue = vect1.Eta() - vect2.Eta();
        else if ( variable == "deltaPhi" ) thisPairValue = vect1.Phi() - vect2.Phi();
        else if ( variable == "absDeltaEta" ) thisPairValue = abs(vect1.Eta() - vect2.Eta());
        else if ( variable == "absDeltaPhi" ) thisPairValue = abs(vect1.Phi() - vect2.Phi());
        else if ( variable == "pt" ) thisPairValue = vect1.Pt() + vect2.Pt();
        else if ( variable == "pz" ) thisPairValue = vect1.Pz() + vect2.Pz();
        else if ( variable == "energy" ) thisPairValue = vect1.Energy() + vect2.Energy();
        else if ( variable == "eta" ) thisPairValue = vect1.Eta() + vect2.Eta();
        else if ( variable == "phi" ) thisPairValue = vect1.Phi() + vect2.Phi();
        else if ( variable == "absPz" ) thisPairValue = abs(vect1.Pz()) + abs(vect2.Pz());
        else if ( variable == "absEta" ) thisPairValue = abs(vect1.Eta()) + abs(vect2.Eta());
        else if ( variable == "absPhi" ) thisPairValue = abs(vect1.Phi()) + abs(vect2.Phi());
        else if ( variable == "vectPt" ) thisPairValue = vect12.Pt(); 
        else if ( variable == "vectPz" ) thisPairValue = vect12.Pz(); 
        else if ( variable == "vectEnergy" ) thisPairValue = vect12.Energy(); 
        else if ( variable == "vectEta" ) thisPairValue = vect12.Eta();
        else if ( variable == "vectPhi" ) thisPairValue = vect12.Phi();
        else if ( variable == "absVectPz" ) thisPairValue = abs(vect12.Pz());
        else if ( variable == "absVectEta" ) thisPairValue = abs(vect12.Eta());
        else if ( variable == "absVectPhi" ) thisPairValue = abs(vect12.Phi());
        else if ( variable == "CSV" ) thisPairValue = max(ptr((*selCollection1)->at(iObj1))->btagCombinedSecVertex,0.0) + max(ptr((*selCollection2)->at(iObj2))->btagCombinedSecVertex,0.0);
        else { std::cerr << " No valid entry for variable: " << variable << std::endl; continue; }

        thisPairValueSum += thisPairValue;
        thisPairValueSumAbs += abs(thisPairValue);

        // --Fill the branches--
        //Fills branches for each pair of objects
        if ( which_pair == "all_pairs" || which_pair == "all_pairs_abs") { 
          if ( which_pair == "all_pairs_abs" ) {
            TString bName = Form("%s_%d_%s_%d_abs_%s", branch_name_1.c_str(), iObj1+1, branch_name_2.c_str(), iObj2+1, variable.c_str());
            if (new_name.length() > 0) bName = Form("%s_%d_%d", new_name.c_str(), iObj1+1, iObj2+1);
            branches[bName].branchVal = abs(thisPairValue);
          }
          else {
            TString bName = Form("%s_%d_%s_%d_%s", branch_name_1.c_str(), iObj1+1, branch_name_2.c_str(), iObj2+1, variable.c_str());
            if (new_name.length() > 0) bName = Form("%s_%d_%d", new_name.c_str(), iObj1+1, iObj2+1);
            branches[bName].branchVal = thisPairValue;
          }
        }
        //Fills a branch for the pair of objects
        else if ( which_pair == "closest_to" || which_pair == "second_closest_to" ) {
          TString bName = Form("%s_%s_%s_%dp%d_%s", branch_name_1.c_str(), branch_name_2.c_str(), which_pair.c_str(), target_value_whole, target_value_fraction, variable.c_str());
          if (new_name.length() > 0) bName = Form("%s", new_name.c_str());

          if ( which_pair == "closest_to" ) {
            if ( abs(thisPairValue - target_value) < abs(branches[bName].branchVal - target_value) ) {
              branches[bName].branchVal = thisPairValue;
            }
          }
          else if ( which_pair == "second_closest_to" ) {
            //If thisPairValue is closer than thisPairValue2
            if ( abs(thisPairValue - target_value) < abs(thisPairValue2 - target_value) ) {
              branches[bName].branchVal = thisPairValue2; //thisPairValue2 is the second closest
              thisPairValue2 = thisPairValue; //Reset thisPairValue2 to be closest so far
            }
            //If thisPairValue is not closer than thisPairValue2, but is closer than branchVal
            else if ( abs(thisPairValue - target_value) < abs(branches[bName].branchVal - target_value) ) {
              branches[bName].branchVal = thisPairValue; //thisPairValue is the second closest
            }
            //End with thisPairValue2 as closest so far, branchVal is second closest so far
          }
        }
        else if ( which_pair == "num_within" && usePairValue ) {
          TString bName = Form("%s_%s_%s_%dp%d_of_%dp%d_%s", branch_name_1.c_str(), branch_name_2.c_str(), which_pair.c_str(),
                               within_value_whole, within_value_fraction, target_value_whole, target_value_fraction, variable.c_str());
          if (new_name.length() > 0) bName = Form("%s", new_name.c_str());
          
          if ( abs(thisPairValue - target_value) < within_value ) thisPairValueCounter += 1.0;
          branches[bName].branchVal = thisPairValueCounter;
        }
        //Fills a branch for the pair of objects
        else {
          TString bName = Form("%s_%s_%s_%s", branch_name_1.c_str(), branch_name_2.c_str(), which_pair.c_str(), variable.c_str());
          if (new_name.length() > 0) bName = Form("%s", new_name.c_str());
          
          if ( which_pair == "max" ) {
            if ( thisPairValue > branches[bName].branchVal || branches[bName].branchVal == KinematicVariableConstants::DOUBLE_INIT ) {
              branches[bName].branchVal = thisPairValue;
            }
          }
          else if ( which_pair == "min" ) {
            if ( thisPairValue < branches[bName].branchVal || branches[bName].branchVal == KinematicVariableConstants::DOUBLE_INIT ) {
              branches[bName].branchVal = thisPairValue;
            }
          }
          else if ( which_pair == "maxAbs" ) { 
            if ( abs(thisPairValue) > branches[bName].branchVal || branches[bName].branchVal == KinematicVariableConstants::DOUBLE_INIT ) {
              branches[bName].branchVal = abs(thisPairValue);
            }
          }
          else if ( which_pair == "minAbs" ) {
            if ( abs(thisPairValue) < branches[bName].branchVal || branches[bName].branchVal == KinematicVariableConstants::DOUBLE_INIT )  {
              branches[bName].branchVal = abs(thisPairValue);
            }
          }
          else if ( which_pair == "absMax" ) {
            if ( thisPairValue > branches[bName].branchVal || branches[bName].branchVal == KinematicVariableConstants::DOUBLE_INIT ) {
              branches[bName].branchVal = abs(thisPairValue);
            }
          }
          else if ( which_pair == "absMin" ) {
            if ( thisPairValue < branches[bName].branchVal || branches[bName].branchVal == KinematicVariableConstants::DOUBLE_INIT ) {
              branches[bName].branchVal = abs(thisPairValue);
            }
          }
          else if ( which_pair == "avg" && usePairValue ) branches[bName].branchVal = thisPairValueSum / thisPairValueIterator;                                                                                                                      
          else if ( which_pair == "sum" && usePairValue ) branches[bName].branchVal = thisPairValueSum;
          else if ( which_pair == "avgAbs" && usePairValue ) branches[bName].branchVal = thisPairValueSumAbs / thisPairValueIterator;
          else if ( which_pair == "sumAbs" && usePairValue ) branches[bName].branchVal = thisPairValueSumAbs;
          else if ( which_pair == "absAvg" && usePairValue ) branches[bName].branchVal = abs(thisPairValueSum) / thisPairValueIterator;
          else if ( which_pair == "absSum" && usePairValue ) branches[bName].branchVal = abs(thisPairValueSum);
          else if ( which_pair != "vector_sum" && usePairValue ) { std::cerr << " No valid entry for variable " << variable << " with which_pair " << which_pair << std::endl; continue; }
          else continue;
        }
      } // end if (iObj1>=min1-1 && iObj2>=min2-1 && iObj1<max1 && iObj2<max2)
    } //End loop over object2
  } //End loop over object1

  //Clean out values from last event 
 myVars.clear();

  for ( typename map<TString, BranchInfo<double>>::iterator iBranch = branches.begin();
        iBranch != branches.end(); iBranch++ ) {
    myVars.push_back(iBranch->second);
  }

}

#endif 
