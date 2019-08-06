# Notre Dame EFT multi-lepton framework (currently based on ttH framework)

The locations of the latest trees are [here](https://twiki.cern.ch/twiki/bin/view/CMS/NotreDameTrees).

## Installation

Setup CMSSW and clone repositories by running:        

	curl https://raw.githubusercontent.com/cms-govner/EFTMultilepton/EFT_code/setup.sh|sh -

NOTE: Due to concurrent `scram` usage, these may fail with errors. In this case, run `scram b -j8` again. 

Then try running over some miniAOD:

	voms-proxy-init -voms cms
	cd EFTMultilepton/TemplateMakers/test/
	cmsRun osTwoLep_cfg.py

To view our custom object classes' TLorentzVectors in the TBrowser, add the following to your [rootlogon.C](https://github.com/cms-govner/EFTMultilepton/blob/master/doc/rootlogon.C) file (assuming you've setup cmssw):
   	
	gSystem->Load("libEFTMultileptonTemplateMakers.so");
