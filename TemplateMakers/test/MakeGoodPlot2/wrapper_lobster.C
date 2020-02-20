#include "includes.h"

void wrapper_lobster(TString sample, TString input_filenames, TString output_name, bool doQFs=false, bool dofakes=false)
{
    // input_filenames -> just "files.txt" provided by lobster. open and read files, add to chain as doing already below.

    bool debug = false;
    
    if (debug) cout << " " << endl;
    if (debug) cout << "At the top of wrapper function" << endl;    
    
    //////////////////////////////////////////////////
    // Set up tree reader    
    
    TChain *ch = new TChain("OSTwoLepAna/summaryTree");
    //ch->SetCacheSize(200000000);             // 100000000 // 200000000
    ch->SetCacheLearnEntries(100);

    if (debug) cout << "Doing sample " << sample << endl;            

    TFileCollection fc("dum","",input_filenames);
    int numfiles = fc.GetNFiles();
    if (debug) cout << "About to run over " << numfiles << " files" << endl;
                
    ch->AddFileInfoList(fc.GetList());
    //if (debug) cout << ch->GetEntries() << endl;

    
    //bool split = (sample_TString2int(sample)>=84 && sample_TString2int(sample)<=88) ? true : false;
    bool split = false;
    
    if (!split) {
        TTreeReader reader(ch);
        // Fill histograms
        if (debug) cout << "Before Histmaker" << endl;
        unique_ptr<HistMaker> histmaker(new HistMaker(sample,doQFs,dofakes));
        if (debug) cout << "Before setBranchAddresses" << endl;
        histmaker->setBranchAddresses(reader);
        if (debug) cout << "Before bookHistos" << endl;
        histmaker->bookHistos();
        if (debug) cout << "Before run" << endl;
        histmaker->run(reader); // contains the while loop
        if (debug) cout << "Before collectResults" << endl;
        histmaker->collectResults();
        auto sumObjArray = histmaker->objArray;
        if (debug) cout << "Done with HistMaker" << endl;
    
        //////////////////////////////////////////////////
        // Dump histos to file
    
        // double numgen = 1.;            
        // numgen = getNumInitialMCevents(0,*ch); // use this to get numgen (To do -> should really remove the first arg as it's just a dummy int now)
        double numgen = getNumSystMCevents(*ch,"");
        double pdfUp_gen      = getNumSystMCevents(*ch,"PDFUP");
        double pdfDown_gen    = getNumSystMCevents(*ch,"PDFDOWN");
        double muRUp_gen      = getNumSystMCevents(*ch,"MURUP");
        double muRDown_gen    = getNumSystMCevents(*ch,"MURDOWN");
        double muFUp_gen      = getNumSystMCevents(*ch,"MUFUP");
        double muFDown_gen    = getNumSystMCevents(*ch,"MUFDOWN");
        double muRmuFUp_gen   = getNumSystMCevents(*ch,"MURMUFUP");
        double muRmuFDown_gen = getNumSystMCevents(*ch,"MURMUFDOWN");
        if (debug) cout << "Ran getNumInitialMCevents: " << numgen << endl;
    
        //TString supl="_Nom";
        //if (doQFs) supl="_ChargeFlips";
        //if (dofakes) supl="_Fakes";
    
        //TFile tempfile("temp_"+int2ss(sample)+"_"+int2ss(i)+".root","RECREATE");
        TFile tempfile(output_name,"RECREATE");
        sumObjArray->Write();
        if (debug) cout << "Hists dumped to file" << endl;           
        TH1D* scalehist = new TH1D("NumInitialWeightedMCevents","NumInitialWeightedMCevents",1,1,2);
        TH1D* pdfUp_hist      = new TH1D("numSummedWeights_pdfUp"     ,"numSummedWeights_pdfUp"     ,1,1,2);
        TH1D* pdfDown_hist    = new TH1D("numSummedWeights_pdfDown"   ,"numSummedWeights_pdfDown"   ,1,1,2);
        TH1D* muRUp_hist      = new TH1D("numSummedWeights_muRUp"     ,"numSummedWeights_muRUp"     ,1,1,2);
        TH1D* muRDown_hist    = new TH1D("numSummedWeights_muRDown"   ,"numSummedWeights_muRDown"   ,1,1,2);
        TH1D* muFUp_hist      = new TH1D("numSummedWeights_muFUp"     ,"numSummedWeights_muFUp"     ,1,1,2);
        TH1D* muFDown_hist    = new TH1D("numSummedWeights_muFDown"   ,"numSummedWeights_muFDown"   ,1,1,2);
        TH1D* muRmuFUp_hist   = new TH1D("numSummedWeights_muRmuFUp"  ,"numSummedWeights_muRmuFUp"  ,1,1,2);
        TH1D* muRmuFDown_hist = new TH1D("numSummedWeights_muRmuFDown","numSummedWeights_muRmuFDown",1,1,2);
        
        scalehist->SetBinContent(1,numgen);
        pdfUp_hist->SetBinContent(1,pdfUp_gen);
        pdfDown_hist->SetBinContent(1,pdfDown_gen);
        muRUp_hist->SetBinContent(1,muRUp_gen);
        muRDown_hist->SetBinContent(1,muRDown_gen);
        muFUp_hist->SetBinContent(1,muFUp_gen);
        muFDown_hist->SetBinContent(1,muFDown_gen);
        muRmuFUp_hist->SetBinContent(1,muRmuFUp_gen);
        muRmuFDown_hist->SetBinContent(1,muRmuFDown_gen);
        
        scalehist->Write();
        pdfUp_hist->Write();
        pdfDown_hist->Write();
        muRUp_hist->Write();
        muRDown_hist->Write();
        muFUp_hist->Write();
        muFDown_hist->Write();
        muRmuFUp_hist->Write();
        muRmuFDown_hist->Write();
        if (debug) cout << "Wrote NumInitialWeightedMCevents" << endl;
        tempfile.Close();
    } else {
        int entries = ch->GetEntries();
        //int entries = 2000;
        int eventsperloop = 5000; // adjust as needed
        int maxloops = ceil((double)entries/(double)eventsperloop);
        //////////////////////////////////////////////////
        
        for (int i=0; i<maxloops; i++) {
            int firstevent = i*eventsperloop;
            int lastevent = min(entries,firstevent+eventsperloop);
            // Fill histograms
            if (debug) cout << "Before Histmaker" << endl;
            TTreeReader reader(ch);
            //HistMaker *histmaker = new HistMaker(sample,doQFs,dofakes);
            unique_ptr<HistMaker> histmaker(new HistMaker(sample,doQFs,dofakes));
            if (debug) cout << "Before setBranchAddresses" << endl;
            histmaker->setBranchAddresses(reader);
            if (debug) cout << "Before bookHistos" << endl;
            histmaker->bookHistos();
            if (debug) cout << "Before run" << endl;
            histmaker->run(reader,firstevent,lastevent); // actually only goes to lastevent-1
            if (debug) cout << "Before collectResults" << endl;
            histmaker->collectResults();
            auto sumObjArray = histmaker->objArray;
            if (debug) cout << "Done with HistMaker" << endl;
    
            //////////////////////////////////////////////////
            // Dump histos to file
    
            TFile tempfile("output_temp"+int2ss(i)+".root","RECREATE"); // the naming of this should now be handled by lobster
            sumObjArray->Write();
            if (debug) cout << "Hists dumped to file" << endl;           
            tempfile.Close();
            //delete sumObjArray;
            //delete histmaker;
            //system("hadd -a -k output.root output_temp.root");
        }
        
        system("hadd output.root output_temp*.root");
        double numgen = 1.;            
        numgen = getNumInitialMCevents(0,*ch); // use this to get numgen (To do -> should really remove the first arg as it's just a dummy int now)
        if (debug) cout << "Ran getNumInitialMCevents" << endl;
        TFile tempfile("output.root","UPDATE");
        TH1D *scalehist = new TH1D("NumInitialWeightedMCevents","NumInitialWeightedMCevents",1,1,2);                    
        scalehist->SetBinContent(1,numgen);
        scalehist->Write();
        if (debug) cout << "Wrote NumInitialWeightedMCevents" << endl;
        tempfile.Close();
        TRandom3 rand;
        rand.SetSeed(0);
        int secs = ceil(45.*60.*rand.Rndm());
        system("sleep "+int2ss(secs)); // spread out transfers to solve issue with deepthought server overloading
    }

    //////////////////////////////////////////////////
    
    // garbage collection (if it becomes necessary):
    // delete scalehist;
    // delete sumObjArray;
    // delete histmaker;
    // // reader.SetTree(0); // ?
    // delete ch; // might have to do something with reader first..

    // feb 2019: it did become necessary. See Histmaker dtor.
}