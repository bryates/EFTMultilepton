#include "includes.h"

void wrapper(std::vector<int> samples, int mode=2)
{
      
    /////////////// histo making half /////////////
    
    // See https://root.cern.ch/doc/v608/mp102__readNtuplesFillHistosAndFit_8C.html
    Int_t njobs = 24;   // The ndpcs have 8 cores x 2 threads/core = max ~16 threads/machine.
                        // Remains to be seen what is the optimal number of threads. On earth 
                        // you have in principle max ~64 threads, but of course there you
                        // might as well be using condor or lobster, etc.
                        // EDIT: Please use condor, etc. if running at ND!
    
    ROOT::TTreeProcessorMP workers(njobs);              

    auto workItem = [](TTreeReader & reader)    // c++ lambda
    {      
        //instantiate class here!
        HistMaker *histmaker = new HistMaker();
        histmaker->setBranchAddresses(reader);
        histmaker->bookHistos();
        histmaker->run(reader); // contains the while loop
        histmaker->collectResults();
        return histmaker->objArray;
    };

    bool debug=false;
    
    if (mode==1 || mode==3)
    {    
        for (const auto sample : samples)
        {
            if (debug) cout << "inside sample loop" << endl;

            ofstream passSampNum;
            passSampNum.open("current_samp.txt");
            passSampNum << sample;
            passSampNum.close();
            if (debug) cout << "\"current_samp.txt\" written" << endl;
            cout << "Doing sample " << sample << endl;            
            TString thissample = loadsample(sample);
            if (debug) cout << "ran loadsample(" << sample << ")" << endl;
            TFileCollection fc("dum","","inputfiles__"+thissample+".txt");
            int numfiles = fc.GetNFiles();
            if (debug) cout << "about to run over " << numfiles << " files" << endl;
            int filesatatime = 2000; // trying to put bandaid on memory issue
            int numloops = (numfiles/filesatatime) + 1;

            for (int i=0; i<numloops; i++)
            {
                if (debug) cout << "doing " << i << " out of " << numloops << " chunks for this sample" << endl;                                
                
                TChain ch("OSTwoLepAna/summaryTree");
                ch.SetCacheSize(200000000);             // 100000000
                ch.SetCacheLearnEntries(100);
                
                int firstfile = i*filesatatime + 1;
                if (debug) cout << "firstfile: " << firstfile << endl;
                TFileCollection subfc("subdum","","inputfiles__"+thissample+".txt",filesatatime,firstfile);
                if (debug) cout << "inputfiles__" << thissample << ".txt loaded" << endl;
                ch.AddFileInfoList(subfc.GetList());
                if (debug) cout << "files added to TChain" << endl;
                
                double numgen = 1.;            
                if (sample<100) numgen = getNumInitialMCevents(sample,ch); // use this to get numgen          
                if (debug) cout << "ran getNumInitialMCevents" << endl;
                
                if (debug) cout << "launching jobs..." << endl;
                // This actually runs the hist maker and grabs the output in the form of a TObjArray:
                auto sumObjArray = workers.Process(ch, workItem, "OSTwoLepAna/summaryTree");                    
                if (debug) cout << "jobs for this chunk finished" << endl;
                
                TFile tempfile("temp_"+int2ss(sample)+"_"+int2ss(i)+".root","RECREATE");
                sumObjArray->Write();
                if (debug) cout << "hists dumped to file" << endl;           
                TH1D *scalehist = new TH1D("NumInitialWeightedMCevents","NumInitialWeightedMCevents",1,1,2);                    
                scalehist->SetBinContent(1,numgen);
                scalehist->Write();
                if (debug) cout << "wrote NumInitialWeightedMCevents" << endl;
                tempfile.Close();
                if (debug) cout << "hist file closed; done with this chunk" << endl;
            }
            if (debug) cout << "done with sample " << sample << endl;
        }  
    }
    
    //// This is an example of how you access the hists from objarray_vect:
    //auto trytogethist = (TH1D*)objarray_vect[0].FindObject("lepMVA sig1 barrel");    
    //trytogethist->Draw();
    

    if (mode==2 || mode==3)
    {  
        /////////////// drawing half ////////////////

        // Now do something with the hists (draw 'em, etc.)

        // This loads the hists from file and prepares to make the plots:
        MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/hadoop/store/user/gesmith/lobster_test_20180220_1204");

        // Then, picking one of these will run the "drawall" function (see drawall.h), and save resulting plots to
        // file/web area, or plot directly to screen (in x-windows):
    
        //newplots->drawAllToScreen();
        //newplots->drawAllToFile("plttest","pdf");
        newplots->drawAllToWebArea("EFT_all_plots_20_2_18_no_jet_btag_req__test_apply_lepSFs__explicit_idMediumPOG_muons__firstlobstertest","png"); // args: (name for this round of plots, image format -- png, pdf, etc.)
    }
}