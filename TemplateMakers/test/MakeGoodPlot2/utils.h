const int numberOfSysts = 49; // change this when changing below ////// 45 // 35 // 23???   note: min # is 1

TString systint2str(int syst)
{
    // (non-trivial) systematics for this analysis
    
    if (syst==0) return "";
    if (syst==1) return "JESUP";
    if (syst==2) return "JESDOWN";
    if (syst==3) return "FRUP";
    if (syst==4) return "FRDOWN";
    if (syst==5) return "FRQCD"; // not considered
    if (syst==6) return "FRTTBAR"; // not considered
    if (syst==7) return "CERR1UP";
    if (syst==8) return "CERR1DOWN";
    if (syst==9) return "CERR2UP";
    if (syst==10) return "CERR2DOWN";
    if (syst==11) return "HFUP";
    if (syst==12) return "HFDOWN";
    if (syst==13) return "HFSTATS1UP";
    if (syst==14) return "HFSTATS1DOWN";
    if (syst==15) return "HFSTATS2UP";
    if (syst==16) return "HFSTATS2DOWN";
    if (syst==17) return "LFUP";
    if (syst==18) return "LFDOWN";
    if (syst==19) return "LFSTATS1UP";
    if (syst==20) return "LFSTATS1DOWN";
    if (syst==21) return "LFSTATS2UP";
    if (syst==22) return "LFSTATS2DOWN";
    if (syst==23) return "PDFUP";                               // <--- shape-only
    if (syst==24) return "PDFDOWN";                             // <--- shape-only
    if (syst==25) return "MURUP";                               // <--- shape-only
    if (syst==26) return "MURDOWN";                             // <--- shape-only
    if (syst==27) return "MUFUP";                               // <--- shape-only
    if (syst==28) return "MUFDOWN";                             // <--- shape-only
    if (syst==29) return "LEPIDUP";
    if (syst==30) return "LEPIDDOWN";
    if (syst==31) return "FRPT1"; // not considered
    if (syst==32) return "FRPT2"; // not considered
    if (syst==33) return "FRETA1"; // not considered
    if (syst==34) return "FRETA2"; // not considered
    if (syst==35) return "PFUP"; // not considered
    if (syst==36) return "PFDOWN"; // not considered
    if (syst==37) return "PSISRUP";     // Now calculated from actual event weights
    if (syst==38) return "PSISRDOWN";   // Now calculated from actual event weights
    if (syst==39) return "PSFSRUP";     // Now calculated from actual event weights
    if (syst==40) return "PSFSRDOWN";   // Now calculated from actual event weights
    if (syst==41) return "TRGUP";
    if (syst==42) return "TRGDOWN";
    if (syst==43) return "PUUP";
    if (syst==44) return "PUDOWN";        
    if (syst==45) return "ADHOCNJUP";
    if (syst==46) return "ADHOCNJDOWN";

    if (syst==47) return "MURMUFUP";
    if (syst==48) return "MURMUFDOWN";

    cout << "Unspecified systematic!" << endl;
    return "";
}

int getRobinBtagSyst(int syst)
{

    //// Robin's btag syst mapping (see csvSF_treeReader_13TeV.C):
    // CMS_ttHl_btag_cErr1 - up : 29
    // CMS_ttHl_btag_cErr2 - up : 31
    // CMS_ttHl_btag_HF - up : 17
    // CMS_ttHl_btag_LF - up : 19
    // CMS_ttHl16_btag_HFStats1 - up : 21
    // CMS_ttHl16_btag_HFStats2 - up : 25
    // CMS_ttHl16_btag_LFStats1 - up : 23
    // CMS_ttHl16_btag_LFStats2 - up : 27
    //     
    // CMS_ttHl_btag_cErr1 - down : 30
    // CMS_ttHl_btag_cErr2 - down : 32
    // CMS_ttHl_btag_HF - down : 18
    // CMS_ttHl_btag_LF - down : 20
    // CMS_ttHl16_btag_HFStats1 - down : 22
    // CMS_ttHl16_btag_HFStats2 - down : 26
    // CMS_ttHl16_btag_LFStats1 - down : 24
    // CMS_ttHl16_btag_LFStats2 - down : 28

    
    if (syst<1 || syst>22) return 0;
    if (syst==1) return 11;
    if (syst==2) return 12;
    if (syst>2 && syst<7) return 0;
    if (syst==7) return 29;
    if (syst==8) return 30;
    if (syst==9) return 31;
    if (syst==10) return 32;
    if (syst==11) return 17;
    if (syst==12) return 18;
    if (syst==13) return 21;
    if (syst==14) return 22;
    if (syst==15) return 25;
    if (syst==16) return 26;
    if (syst==17) return 19;
    if (syst==18) return 20;
    if (syst==19) return 23;
    if (syst==20) return 24;
    if (syst==21) return 27;
    if (syst==22) return 28;
    
    cout << "should not be logically possible to get here (see utils.h)" << endl;
    return -1;
}


TString int2ss(int theint)
{
	std::ostringstream thess;
	thess << theint;
	TString string_out = thess.str();
	return string_out;
}
string d2ss(double dbl,int prec=-1)
{
    std::ostringstream thess;
    if (prec>0) thess << fixed << setprecision(prec) << dbl;
    else thess << dbl;
    std::string thestr = thess.str();
    return thestr;
}
std::string TString2string(TString the_abomination)
{
        std::ostringstream thess;
        thess << the_abomination;
        std::string string_out = thess.str();
        return string_out;
}
TLegend getleg(string format)
{
    if (format=="bl") 
    {                       
        TLegend leg(0.12,0.12,0.3,0.4);
        leg.SetLineColor(kWhite);
        leg.SetShadowColor(kWhite);            
        leg.SetTextFont(42);                        
        return leg;
    }
    else if (format=="ul" || format=="tl")
    {        
        TLegend leg(0.3,0.77,0.7,0.88); // not sure about this one..
        leg.SetLineColor(kWhite);
        leg.SetShadowColor(kWhite);            
        leg.SetTextFont(42);                        
        return leg;      
    }
    else if (format=="br")     
    {
        TLegend leg(0.55,0.12,0.89,0.4);
        leg.SetLineColor(kWhite);
        leg.SetShadowColor(kWhite);            
        leg.SetTextFont(42);
        return leg;
    }          
    else if (format=="ur" || format=="tr")
    {     
        TLegend leg(0.55,0.7,0.89,0.89);
        leg.SetLineColor(kWhite);
        leg.SetShadowColor(kWhite);            
        leg.SetTextFont(42);                        
        return leg;        
    }   
    else if (format=="above")
    {
        TLegend leg(0.11,0.91,0.89,0.99);
        leg.SetLineColor(kWhite);
        leg.SetShadowColor(kWhite);            
        leg.SetTextFont(42);
        leg.SetNColumns(3);
        return leg;
    }  
    else if (format=="darren")
    {        
        TLegend leg(0.14,0.75,0.89,0.89);
        leg.SetLineColor(kWhite);
        leg.SetShadowColor(kWhite);            
        leg.SetTextFont(42);
        leg.SetNColumns(5);
        return leg;
    }  
    else
    {    
        TLegend leg(0.55,0.7,0.9,0.9);
        leg.SetLineColor(kWhite);
        leg.SetShadowColor(kWhite);            
        leg.SetTextFont(42);                        
        return leg;
    }
}

string getjettagcategory(string lepcat, int jets, int tags)
{
    
    string jetstr = "";
    string tagstr = "";
    
    if (tags>=2) tagstr = "_2b";
    else if (tags==1) tagstr = "_1b";
    else return "null";
    
    std::size_t foundstr = lepcat.find("2l");
    
    if ( foundstr!=std::string::npos )
    {
        if (jets>=6) jetstr = "_6j";
        else if (jets==5) jetstr = "_5j";
        else if (jets==4) jetstr = "_4j";
        else if (jets==3) jetstr = "_3j";
        else if (jets==2) jetstr = "_2j";
        else return "null";
    }
    
    foundstr = lepcat.find("3l");
    if ( foundstr!=std::string::npos )
    {
        if (jets>=4) jetstr = "_4j";
        else if (jets==3) jetstr = "_3j";
        else if (jets==2) jetstr = "_2j";
        else if (jets==1) jetstr = "_1j";
        else return "null";
    }
    
    foundstr = lepcat.find("4l");
    if ( foundstr!=std::string::npos )
    {
        if (jets>=2) jetstr = "_2j";
        else if (jets==1) jetstr = "_1j";
        else return "null";
    }
    
    foundstr = lepcat.find("5l");
    if ( foundstr!=std::string::npos )
    {
        if (jets>=1) jetstr = "_1j";
        else return "null";
        if (tags>=1) tagstr = "_1b";
        else return "null";
    }

    //if (jets<0) return lepcat+tagstr;    
    return lepcat+jetstr+tagstr;

}
string gettagcategory(string lepcat, int tagsM, int tagsL=-1)
{
    string tagstr = "";
    if (tagsL<0) // old nominal case (ignore loose)
    {
        //if (lepcat=="ge5l" && tags>=1) tagstr = "_1b";
        if (tagsM>=2) tagstr = "_2b";
        else if (tagsM==1) tagstr = "_1b";
        else return "null";
        return lepcat+tagstr;
    }
    else // doing some crazy $#*! with the loose btags
    {
        // uncomment if "group 1"
        // if (tagsM>=2) tagstr = "_2b";
        // else if (tagsM==1 && tagsL>=1 && lepcat.substr(0,2)=="3l") tagstr = "_1b";
        // else return "null";
        // return lepcat+tagstr;
        
        // uncomment if "group 2"
        // if (tagsM==1 && tagsL>=1) tagstr = "_2b";
        // else if (tagsM==1 && tagsL==0 && lepcat.substr(0,2)=="3l") tagstr = "_1b";
        // else return "null";
        // return lepcat+tagstr;
        
        // // uncomment if "group 3"
        // if (tagsM==0 && tagsL>=2) tagstr = "_2b";
        // else if (tagsM==0 && tagsL==1 && lepcat.substr(0,2)=="3l") tagstr = "_1b";
        // else return "null";
        // return lepcat+tagstr;
        
        // // uncomment if "group 4"
        // if (tagsM==1 && tagsL==0 && lepcat.substr(0,4)=="2lss") tagstr = "_2b"; // only 2lss
        // else return "null";
        // return lepcat+tagstr;
        
        // what we're going with:
        if (lepcat.substr(0,4)=="2lss" && (tagsM>=2 || (tagsM>=1 && tagsL>=2))) tagstr = "_2b";
        else if (tagsM>=2 && lepcat.substr(0,2)=="3l") tagstr = "_2b";
        else if (tagsM==1 && lepcat.substr(0,2)=="3l") tagstr = "_1b";
        else if (lepcat=="4l" && (tagsM>=2 || (tagsM>=1 && tagsL>=2))) tagstr = "_2b";
        else return "null";
        return lepcat+tagstr;
        
    }
    
}


void clean_neg_bins(TH1 &negBinHist)
{
    bool debug = false;
    
    //cout << "here a" << endl;
    if (debug) cout << negBinHist.GetName() << endl;
    
    // this works for 1D and 2D hists
    int numbinsx = negBinHist.GetNbinsX();
    if (debug) cout << "numbinsx " << numbinsx << endl;
    int numbinsy = negBinHist.GetNbinsY();
    if (debug) cout << "numbinsy " << numbinsy << endl;
    int numbins = numbinsx*numbinsy;
    
    for (int i=1; i<=numbinsx; i++)
    {
        for (int j=1; j<=numbinsy; j++)
        {                  
            int bin = negBinHist.GetBin(i,j);
            double bincont = negBinHist.GetBinContent(bin);
            
            if (bincont<0.) 
            {
                if (debug) cout << "bin " << bin << endl;
                negBinHist.SetBinContent(bin,0.);
                negBinHist.SetBinError(bin,0.);
            }
        } 
    }
    //cout << "here b" << endl;
    
}

// normally you shouldn't need this (if objects were cleaned correctly when filling the trees):
template <typename coll1type, typename coll2type> coll1type cleanObjs(coll1type colltobecleaned, coll2type colltocleanwith, double mindR) // could add other options besides dR
{
    coll1type returnedCleanColl; // meant to be ttH::X objects
    
    for (const auto mostlikelyajet : colltobecleaned)
    {
        bool toadd = true;
        
        for (const auto mostlikelyalep : colltocleanwith)
        {
            if ( getdR(mostlikelyajet,mostlikelyalep)<mindR ) toadd = false;
        }
        
        if (toadd) returnedCleanColl.push_back(mostlikelyajet);
    }
    
    return returnedCleanColl;
}
    

int getComboSampInt(int sample)
{
    //Vjets
    if (sample==6 || sample==16 || sample==7)
    {
        return 90;
    }
    //Diboson 
    else if (sample==10 || sample==11 || sample==12)
    {
        return 91;
    }        
    //Triboson
    else if (sample==22 || sample==23 || sample==24 || sample==25)
    {
        return 92;
    }        
    //SingleTop 
    else if (sample==17 || sample==18 || sample==19 || sample==20 || sample==21)
    {
        return 93;
    }        
    //TTBar .. not sure if we want this, but leaving for now
    else if (sample==13 || sample==14 || sample==15) // || sample==5)
    {
        return 5;
    }
    //data
    else if (sample==100 || sample==101 || sample==102 || sample==103 || sample==104)
    {
        return 0;
    }    
    else return sample;
}
    
double fillHistOverflowAware(TH1* h, double loc)
{
    // last bin with upper-edge xup EXCLUDED
    double range_max = h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
    if (loc<range_max) return loc;
    else
    {
        double lowedge = h->GetXaxis()->GetBinLowEdge(h->GetNbinsX());
        return (range_max+lowedge)/2.; // should work, except in case of very finely-grained hist or hist with very small range starting at a non-0 value
    }
    cout << "Something is very wrong. See utils.h." << endl;
    return loc;
}

/*
Generic function to find an element in vector and also its position.
It returns a pair of bool & int i.e.
 
bool : Represents if element is present in vector or not.
int : Represents the index of element in vector if its found else -1
 
*/
template < typename T>
std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
	std::pair<bool, int > result;
 
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
 
	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}
 
	return result;
}

// Computes the envelope from the list of variants bin-by-bin by finding the variant furthest
//  away from the nominal histogram
TH1EFT* calculateEnvelope(TString env_name, TH1EFT* h_nom, std::vector<TH1EFT*> variants) {
    Int_t nbins = h_nom->GetNbinsX();
    Double_t xlo = h_nom->GetBinLowEdge(1);
    Double_t xhi = h_nom->GetBinLowEdge(nbins+1);

    TH1EFT* h_env = new TH1EFT(env_name,"",nbins,xlo,xhi);

    // Note: This ignores the underflow and overflow bins!
    for (Int_t i = 1; i <= nbins; i++) {
        double nom_content = h_nom->GetBinContent(i);
        double max_content = 0.0;
        for (TH1EFT* h_var: variants) {
            double var_content = h_var->GetBinContent(i);
            double var_err = h_var->GetBinError(i);
            double diff = abs(nom_content - var_content);
            if (diff > max_content) {
                max_content = diff;
                h_env->SetBinContent(i,var_content);
                h_env->SetBinError(i,var_err);
                h_env->hist_fits.at(i-1) = h_var->GetBinFit(i);     // Note: we use i=1 here b/c of the off-by-one index
            }
        }
    }

    return h_env;
}