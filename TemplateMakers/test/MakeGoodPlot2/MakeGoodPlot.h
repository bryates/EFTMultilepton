// MakeGoodPlot plot-making class.
// Created by Geoff Smith, 2017. 
//   Re-written from previous 2012-2017 version, 
//   with some functionality moved to HistMaker 
//   class and various helper functions/scripts.
// See github.com/cms-govner/EFTMultilepton/tree/EFT_code/TemplateMakers/test/MakeGoodPlot2.
// 

class MakeGoodPlot
{
    public:
    
        int numsamples = 1;
        int numdatasamples = 0;
        bool hasdata = false;
        bool groupsamples = false;
        int numtotalhists = 0;
        std::vector<int> samples;
        vector<TFile*> files;
        TString sample_names[200];
        TString sample_names_reg[200];
        string sample_names_std[200];
        Color_t color[200];
        double numgen[200];		
        vector<TString> categoryTS;
        vector<string> wilsoncoeffs;
        
        // CMS info stuff
        std::string cmsinfo;
        TLatex *CMSInfoLatex;
        TPaveText *pt[50];
        TPaveText *pt0;
        TPaveText *pt1; 
	    const double lumi2016 = 36814.; // 2.5%, pb^-1, https://twiki.cern.ch/twiki/bin/view/CMS/TWikiLUM
	    const double lumi2016up = (1.+0.025)*lumi2016;
	    const double lumi2016down = (1.-0.025)*lumi2016;
	    const double lumi2017 = 41530.; // 5% with normtag
	    const double lumi2017up = (1.+0.023)*lumi2017;
	    const double lumi2017down = (1.-0.023)*lumi2017;
        //const double lumi = lumi2016+lumi2017;
	    //const double lumiup = lumi2016up+lumi2017up; // note: for 2016+2017, this is not lumi*(1.+sqrt(0.025*0.025+0.05*0.05)).
	    //const double lumidown = lumi2016down+lumi2017down;
        //const double lumi = lumi2016;
	    //const double lumiup = lumi2016up;
	    //const double lumidown = lumi2016down;	    
        const double lumi = lumi2017;
	    const double lumiup = lumi2017up;
	    const double lumidown = lumi2017down;	 	    
	    
	    
        
        std::vector<TObjArray> hist;
        TObjArray canvas;
        
        double xsec[200];
        double q2up[200];
        double q2down[200];
        double pdfup[200];
        double pdfdown[200]; 
        const double extra_tllq_factor_2lss = 1.497; // see [1] in rateinfo()
        const double extra_tllq_factor_3lnonZ = 0.827; // see [1] in rateinfo()
        
        void get_rate_info();        
        void setup();        
        
        void lepeff_plots();
        void triggerstudies_plots();
        void jetcleaning_plots();
        void standard_plots();
        void standard_plots_normalized();
        void mc_validation_plots();
        void save_analysis_hists();
        void th1eft_test_plots();
        void fr_studies_plots();
        void pdf_studies_plots();
        void syst_sanity_check();
        void derive_njet_sfs();
        
        MakeGoodPlot() { cout << "Default constructor of MakeGoodPlot doesn't do anything. Use a different constructor." << endl; }
        MakeGoodPlot(std::vector<int> thesamps, TString histdir="");
        MakeGoodPlot(std::vector<int> thesamps, std::vector<TObjArray> exthists);

        ~MakeGoodPlot();
        
        void drawAll();
        void drawAllToScreen() { drawAll(); }
        void drawAllToFile(string plotfile, string plotoption="pdf");
        // void drawAllToWebArea(string plotfile, string plotoption="pdf");
        void drawAllToWebArea(string webarea,string plotfile, string plotoption="pdf");
    
};


MakeGoodPlot::MakeGoodPlot(std::vector<int> thesamps, TString histdir)
{    
    numsamples = thesamps.size();
    samples = thesamps;
    
    //cout << "In MakeGoodPlot constructor." << endl;
    
    for (int i=0; i<numsamples; i++)
    {
        // load the saved hists from file(s):
        // TString thishistfile = "temp_"+int2ss(samples[i])+".root";
        TString thishistfile = histdir+"temp_"+sample_int2TString(samples[i])+".root";
        
        // if (histdir!="")
        // {
        //     // in case the histograms are somewhere besides the current dir:
        //     thishistfile = histdir;
        //     // get subdir for this sample:
        //     TString sampTStemp = sample_int2TString(samples[i]);
        //     // the rest of this is due to the way lobster merges output files:
        //     thishistfile = thishistfile + "/" + sampTStemp + "/*.root";
        //     TString lsstring = "ls -d "+thishistfile+" > temp.txt";
        //     system(lsstring);
        //     ifstream passed_root_file;
        //     passed_root_file.open("temp.txt");
        //     passed_root_file >> thishistfile;
        //     passed_root_file.close();
        // }
        
        files.push_back( new TFile(thishistfile) ); 
        TIter next(files[i]->GetListOfKeys());
        TKey *key;
        TObjArray dummyArray;
        
        while ((key=(TKey*)next()))
        {
            // printf("key: %s points to an object of class: %s",
            // key->GetName(),
            // key->GetClassName());
            if (strncmp(key->GetName(),"blah",4)!=0)
            {
                dummyArray.Add(files[i]->Get(key->GetName()));
            }
        }
        
        numtotalhists = dummyArray.GetEntriesFast();
        hist.push_back(dummyArray);
    } 

    // Example usage of hist TObjArray vector:
    // auto thing = hist[0].FindObject("lepMVA sig1 endcap"); // will be a pointer
    // thing->Draw();
    
    setup();    
}

MakeGoodPlot::MakeGoodPlot(std::vector<int> thesamps, std::vector<TObjArray> exthists)
{
    // If for some reason you want to use this constructor, you need to have a 
    // std::vector<TObjArray> in the wrapper function that gets passed to
    // MakeGoodPlot.
    
    numsamples = thesamps.size();
    samples = thesamps;    
    
    // Get the hists directly instead of reading from file:    
    hist = exthists;
    
    setup();
}

//Note: This still does not fix the issue of root hanging when trying to end/close the anatest making script
MakeGoodPlot::~MakeGoodPlot()
{
    std::cout << "Cleaning up" << std::endl;
    for (uint i=0; i < this->files.size(); i++) {
        this->files.at(i)->Close();
        delete this->files.at(i);
    }
    std::cout << "Finished cleaning!" << std::endl;
}

