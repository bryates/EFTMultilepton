void MakeGoodPlot::save_analysis_hists()
{
    bool debug = false;
    
    vector<TString> allAnaHists;
    vector<TString> cats;
    
    //allAnaHists.push_back("2los_ee_1b.");
    //allAnaHists.push_back("2los_ee_2b.");
    //allAnaHists.push_back("2los_sfz_ee_1b.");
    //allAnaHists.push_back("2los_sfz_ee_2b.");
    //allAnaHists.push_back("2los_emu_1b.");
    //allAnaHists.push_back("2los_emu_2b.");
    //allAnaHists.push_back("2los_mumu_1b.");
    //allAnaHists.push_back("2los_mumu_2b.");
    //allAnaHists.push_back("2los_sfz_mumu_1b.");
    //allAnaHists.push_back("2los_sfz_mumu_2b.");
    //allAnaHists.push_back("2lss_p_ee_1b.");
    allAnaHists.push_back("2lss_p_ee_2b.");
    //allAnaHists.push_back("2lss_p_emu_1b.");
    allAnaHists.push_back("2lss_p_emu_2b.");
    //allAnaHists.push_back("2lss_p_mumu_1b.");
    allAnaHists.push_back("2lss_p_mumu_2b.");
    //allAnaHists.push_back("2lss_m_ee_1b.");
    allAnaHists.push_back("2lss_m_ee_2b.");
    //allAnaHists.push_back("2lss_m_emu_1b.");
    allAnaHists.push_back("2lss_m_emu_2b.");
    //allAnaHists.push_back("2lss_m_mumu_1b.");
    allAnaHists.push_back("2lss_m_mumu_2b.");
    //allAnaHists.push_back("3l_ppp_1b.");
    //allAnaHists.push_back("3l_ppp_2b.");
    //allAnaHists.push_back("3l_mmm_1b.");
    //allAnaHists.push_back("3l_mmm_2b.");
    allAnaHists.push_back("3l_mix_p_1b.");
    allAnaHists.push_back("3l_mix_m_1b.");
    allAnaHists.push_back("3l_mix_p_2b.");
    allAnaHists.push_back("3l_mix_m_2b.");
    allAnaHists.push_back("3l_mix_sfz_1b.");
    allAnaHists.push_back("3l_mix_sfz_2b.");
    //allAnaHists.push_back("4l_1b.");
    allAnaHists.push_back("4l_2b.");
    //allAnaHists.push_back("ge5l_1b.");
    
    // ?
    //cats.push_back("2lss.");   
    //cats.push_back("3l.");
    //cats.push_back("2lss_p_ee_1b.");   
    //cats.push_back("3l_ppp_1b.");    
    
    vector<TString> allSysts;
    allSysts.push_back("");     // This being first VERY important to ensure the nominal histograms
                                // are normalized BEFORE being used to normalize the shape-only systs
    allSysts.push_back("JESUP");
    allSysts.push_back("JESDOWN");
    allSysts.push_back("FRUP");
    allSysts.push_back("FRDOWN");
    //allSysts.push_back("FRQCD");
    //allSysts.push_back("FRTTBAR");
    allSysts.push_back("CERR1UP");
    allSysts.push_back("CERR1DOWN");
    allSysts.push_back("CERR2UP");
    allSysts.push_back("CERR2DOWN");
    allSysts.push_back("HFUP");
    allSysts.push_back("HFDOWN");
    allSysts.push_back("HFSTATS1UP");
    allSysts.push_back("HFSTATS1DOWN");
    allSysts.push_back("HFSTATS2UP");
    allSysts.push_back("HFSTATS2DOWN");
    allSysts.push_back("LFUP");
    allSysts.push_back("LFDOWN");
    allSysts.push_back("LFSTATS1UP");
    allSysts.push_back("LFSTATS1DOWN");
    allSysts.push_back("LFSTATS2UP");
    allSysts.push_back("LFSTATS2DOWN");
    allSysts.push_back("PDFUP");
    allSysts.push_back("PDFDOWN");
    allSysts.push_back("MURUP");
    allSysts.push_back("MURDOWN");
    allSysts.push_back("MUFUP");
    allSysts.push_back("MUFDOWN");
    allSysts.push_back("MURMUFUP");
    allSysts.push_back("MURMUFDOWN");
    allSysts.push_back("LEPIDUP");
    allSysts.push_back("LEPIDDOWN");
    allSysts.push_back("PSISRUP");
    allSysts.push_back("PSISRDOWN");
    allSysts.push_back("TRGUP");
    allSysts.push_back("TRGDOWN");
    allSysts.push_back("PUUP");
    allSysts.push_back("PUDOWN");  
    //allSysts.push_back("ADHOCNJUP");
    //allSysts.push_back("ADHOCNJDOWN");
    
    // for the other systs, need some way of picking MC-only, etc.

    // construct hists for combined 2lss + 3l categories
    // Note: this modifies existing histograms, so comment it out if not using.
    
    cout << "here1" << endl;
    
    for (int i=0; i<numsamples; i++)
    {     
        for (const auto syst : allSysts)
        {
	        int thisSamp = samples[i];
	        cout << i << ", " << thisSamp << ", " << syst << endl;

            //auto combohist1 = (TH1EFT*)hist[i].FindObject("2lss_p_ee_1b."+syst); //->Clone("2lss."+syst);
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_p_emu_1b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_p_mumu_1b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_m_ee_1b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_m_emu_1b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_m_mumu_1b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_p_ee_2b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_p_emu_2b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_p_mumu_2b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_m_ee_2b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_m_emu_2b."+syst));
            //combohist1->Add((TH1EFT*)hist[i].FindObject("2lss_m_mumu_2b."+syst));            
            //if (debug) combohist1->DumpFits();
            //if (debug) cout << "Should be same as: " << endl;
            //if (debug) ((TH1EFT*)hist[i].FindObject("2lss_p_ee_1b."+syst))->DumpFits();
            //hist[i].Add(combohist1);
            //
            //auto combohist2 = (TH1EFT*)hist[i].FindObject("3l_ppp_1b."+syst); //->Clone("3l."+syst);
            //combohist2->Add((TH1EFT*)hist[i].FindObject("3l_mmm_1b."+syst));
            //combohist2->Add((TH1EFT*)hist[i].FindObject("3l_mix_1b."+syst));
            //combohist2->Add((TH1EFT*)hist[i].FindObject("3l_mix_sfz_1b."+syst));
            //combohist2->Add((TH1EFT*)hist[i].FindObject("3l_ppp_2b."+syst));
            //combohist2->Add((TH1EFT*)hist[i].FindObject("3l_mmm_2b."+syst));
            //combohist2->Add((TH1EFT*)hist[i].FindObject("3l_mix_2b."+syst));
            //combohist2->Add((TH1EFT*)hist[i].FindObject("3l_mix_sfz_2b."+syst));            
            //if (debug) combohist2->DumpFits();
            //if (debug) cout << "Should be same as: " << endl;
            //if (debug) ((TH1EFT*)hist[i].FindObject("3l_ppp_1b."+syst))->DumpFits();
            //hist[i].Add(combohist2);            

	        if (!(thisSamp>=84 && thisSamp<=88) && (syst=="ADHOCNJUP" || syst=="ADHOCNJDOWN")) continue;
            if (thisSamp > 90 && (syst=="MURMUFUP" || syst=="MURMUFDOWN")) {
                // The data-based samples haven't been re-made and so are missing these histograms
                continue;
            }
            auto combohist1 = (TH1EFT*)hist[i].FindObject("3l_mix_p_1b."+syst); //->Clone("3l."+syst);
            combohist1->Add((TH1EFT*)hist[i].FindObject("3l_ppp_1b."+syst));
            
            auto combohist2 = (TH1EFT*)hist[i].FindObject("3l_mix_m_1b."+syst);
            combohist2->Add((TH1EFT*)hist[i].FindObject("3l_mmm_1b."+syst));
            
            auto combohist3 = (TH1EFT*)hist[i].FindObject("3l_mix_p_2b."+syst); //->Clone("3l."+syst);
            combohist3->Add((TH1EFT*)hist[i].FindObject("3l_ppp_2b."+syst));            
            
            auto combohist4 = (TH1EFT*)hist[i].FindObject("3l_mix_m_2b."+syst); //->Clone("3l."+syst);
            combohist4->Add((TH1EFT*)hist[i].FindObject("3l_mmm_2b."+syst));
        }
    } 

    // cout << "\\hline" << endl;
    // cout << " & 2lss ($e^{+}e^{+}$)";
    // cout << " & 2lss ($e^{+}\\mu^{+}$)";
    // cout << " & 2lss ($\\mu^{+}\\mu^{+}$)";
    // cout << " & 2lss ($e^{-}e^{-}$)";
    // cout << " & 2lss ($e^{-}\\mu^{-}$)";
    // cout << " & 2lss ($\\mu^{-}\\mu^{-}$)";
    // cout << " & 3l (1b \"p\")";
    // cout << " & 3l (1b \"m\")";
    // cout << " & 3l ($\\geq$2b \"p\")";
    // cout << " & 3l ($\\geq$2b \"m\")";
    // cout << " & 3l (SFOS Z, 1b)";
    // cout << " & 3l (SFOS Z, $\\geq$2b)";
    // cout << " & $\\geq$4l";
    // cout << " \\\\ " << endl;
    // cout << "\\hline" << endl;

    //TH1EFT *data;

    for (int i=0; i<numsamples; i++)
    {    
        int thisSamp = samples[i];
        
        //cout << " " << endl;
        //cout << " " << endl;
        //cout << "Doing " << sample_names[thisSamp] << endl;
        //cout << " " << endl; 
        
        string strtochg = TString2string(sample_names_reg[thisSamp]);
        //std::replace( strtochg.begin(), strtochg.begin()+1, '_', '');
        strtochg.erase(0,1);
        sample_names_reg[thisSamp] = strtochg.c_str();
        
  
        //cout << "\\hline" << endl;
        //cout << " & 2lss ($e^{+}e^{+}$)";
        //cout << " & 2lss ($e^{+}\\mu^{+}$)";
        //cout << " & 2lss ($\\mu^{+}\\mu^{+}$)";
        //cout << " & 2lss ($e^{-}e^{-}$)";
        //cout << " & 2lss ($e^{-}\\mu^{-}$)";
        //cout << " & 2lss ($\\mu^{-}\\mu^{-}$)";
        //cout << " & 3l (1b)";
        //cout << " & 3l ($\\geq$2b)";
        //cout << " & 3l (SFOS Z, 1b)";
        //cout << " & 3l (SFOS Z, $\\geq$2b)";
        //cout << " & $\\geq$4l";
        //cout << " \\\\ " << endl;
        //cout << "\\hline" << endl;
  
        vector<double> nomylds;
        for (const TString thissyst : allSysts)
        {
            if (thissyst=="") cout << sample_names[thisSamp] << ": ";
            else cout << sample_names[thisSamp] << ", " << thissyst << ": ";
            
            int cnt=0;            
            
            // bool is_shape_syst = (thissyst=="PDFUP" || thissyst=="PDFDOWN" || thissyst=="MURUP" || thissyst=="MURDOWN" || thissyst=="MUFUP" || thissyst=="MUFDOWN" || thissyst=="MURMUFUP" || thissyst=="MURMUFDOWN");
            bool is_shape_syst = (thissyst == "PDFUP" || thissyst == "PDFDOWN" || thissyst == "PSISRUP" || thissyst == "PSISRDOWN");
            
            for (const TString thiscat : allAnaHists)
            {   
                if (debug) cout << thiscat+thissyst << endl;
                
                
                if (thisSamp!=95 && (thissyst=="FRUP" || thissyst=="FRDOWN")) continue;
                if (thisSamp==95 && (thissyst!="FRUP" && thissyst!="FRDOWN" && thissyst!="")) continue;
                if (thisSamp==94 && thissyst!="") continue;
                if (thisSamp>99 && thissyst!="") continue;
                if (!(thisSamp>=84 && thisSamp<=88) && (thissyst=="ADHOCNJUP" || thissyst=="ADHOCNJDOWN")) continue;

                auto thishist = (TH1EFT*)hist[i].FindObject(thiscat+thissyst);
                
                if (thissyst=="") thishist->SetName(thishist->GetName()+sample_names_reg[thisSamp]);
                else thishist->SetName(thishist->GetName()+string(".")+sample_names_reg[thisSamp]);
                
                
                if (thisSamp<40)
                {
                    thishist->Scale(lumi*xsec[thisSamp]/numgen[thisSamp]);

                    if (is_shape_syst) {
                        auto nomhist = (TH1EFT*)hist[i].FindObject(thiscat+sample_names_reg[thisSamp]); // should have already been scaled
                                                                                                        // refers to the fact that the nominal
                                                                                                        //   hists are the 'first' systematic in
                                                                                                        //   the list!
                        double normamnt = (nomhist->GetEntries() != 0 && thishist->Integral() != 0.) ? nomhist->Integral()/thishist->Integral() : 1.;
                        thishist->Scale(normamnt);
                    }

                    // Dibosons
                    if (thisSamp==11)
                    {
                        if (thissyst=="") ((TH1EFT*)hist[i-1].FindObject(thiscat+thissyst+sample_names_reg[10]))->Add(thishist); // assumes i-1 is sample 10 (WZ)!!!
                        else ((TH1EFT*)hist[i-1].FindObject(thiscat+thissyst+string(".")+sample_names_reg[10]))->Add(thishist); // assumes i-1 is sample 10 (WZ)!!!
                    }
                    if (thisSamp==12)
                    {
                        if (thissyst=="") ((TH1EFT*)hist[i-2].FindObject(thiscat+thissyst+sample_names_reg[10]))->Add(thishist); // assumes i-2 is sample 10 (WZ)!!!
                        else ((TH1EFT*)hist[i-2].FindObject(thiscat+thissyst+string(".")+sample_names_reg[10]))->Add(thishist); // assumes i-2 is sample 10 (WZ)!!!
                    }
                    // Tribosons
                    if (thisSamp==23)
                    {
                        if (thissyst=="") ((TH1EFT*)hist[i-1].FindObject(thiscat+thissyst+sample_names_reg[22]))->Add(thishist); // assumes i-1 is sample 22 (WWW)!!!
                        else ((TH1EFT*)hist[i-1].FindObject(thiscat+thissyst+string(".")+sample_names_reg[22]))->Add(thishist); // assumes i-1 is sample 22 (WWW)!!!
                    }
                    if (thisSamp==24)
                    {
                        if (thissyst=="") ((TH1EFT*)hist[i-2].FindObject(thiscat+thissyst+sample_names_reg[22]))->Add(thishist); // assumes i-2 is sample 22 (WWW)!!!
                        else ((TH1EFT*)hist[i-2].FindObject(thiscat+thissyst+string(".")+sample_names_reg[22]))->Add(thishist); // assumes i-2 is sample 22 (WWW)!!!
                    }
                    if (thisSamp==25)
                    {
                        if (thissyst=="") ((TH1EFT*)hist[i-3].FindObject(thiscat+thissyst+sample_names_reg[22]))->Add(thishist); // assumes i-3 is sample 22 (WWW)!!!
                        else ((TH1EFT*)hist[i-3].FindObject(thiscat+thissyst+string(".")+sample_names_reg[22]))->Add(thishist); // assumes i-3 is sample 22 (WWW)!!!
                    }                    
                }
                else if(thisSamp>=40 && thisSamp<84) 
                {
                    thishist->ScaleFits(lumi);
                    thishist->Scale(WCPoint());
                    // if (thissyst=="PDFUP" || thissyst=="PDFDOWN" || thissyst=="MURUP" || thissyst=="MURDOWN" || thissyst=="MUFUP" || thissyst=="MUFDOWN" || thissyst=="MURMUFUP" || thissyst=="MURMUFDOWN")
                    if (is_shape_syst)
                    {
                        auto nomhist = (TH1EFT*)hist[i].FindObject(thiscat+sample_names_reg[thisSamp]); // should have already been scaled
                        double normamnt = (nomhist->GetEntries()!=0 && thishist->Integral()!=0.) ? nomhist->Integral()/thishist->Integral() : 1.;
                        thishist->ScaleFits(normamnt);
                        thishist->Scale(WCPoint());
                    }
                }
                else if(thisSamp>=84 && thisSamp<90) 
                {
                    double addlfactor = 1.;
                    //if (thisSamp==87 && (thiscat=="3l_mix_p_1b." || thiscat=="3l_mix_m_1b." || thiscat=="3l_mix_p_2b." || thiscat=="3l_mix_m_2b."))
                    //{
                    //    addlfactor = extra_tllq_factor_3lnonZ;
                    //}
                    //else if (thisSamp==87 && 
                    //            (thiscat=="2lss_p_ee_2b." || thiscat=="2lss_p_emu_2b." || thiscat=="2lss_p_mumu_2b." ||
                    //             thiscat=="2lss_m_ee_2b." || thiscat=="2lss_m_emu_2b." || thiscat=="2lss_m_mumu_2b.")
                    //        )
                    //{
                    //    addlfactor = extra_tllq_factor_2lss;
                    //}
                    
                    //thishist->ScaleFits(addlfactor*lumi*xsec[thisSamp]); // see rateinfo.h
                    thishist->ScaleFits(lumi*xsec[thisSamp]/numgen[thisSamp]); // using updated norm method for EFT samps
                    thishist->Scale(WCPoint()); // SM
                    
                    //WCPoint pt;
                    //pt.setStrength("ctZ",10.);
                    //thishist->Scale(pt);
                    
                    // systematics that are shape-only variations:
                    // if (thissyst=="PDFUP" || thissyst=="PDFDOWN" || thissyst=="MURUP" || thissyst=="MURDOWN" || thissyst=="MUFUP" || thissyst=="MUFDOWN" || thissyst=="MURMUFUP" || thissyst=="MURMUFDOWN")
                    if (is_shape_syst)
                    {
                        auto nomhist = (TH1EFT*)hist[i].FindObject(thiscat+sample_names_reg[thisSamp]); // should have already been scaled
                        double normamnt = (nomhist->GetEntries()!=0 && thishist->Integral()!=0.) ? nomhist->Integral()/thishist->Integral() : 1.;
                        thishist->ScaleFits(normamnt);
                        thishist->Scale(WCPoint()); // SM
                    }
                }
                //else if (thisSamp>99)
                else if (thisSamp==104)
                {
                    //cout << "hey" << endl;
                    //if (thisSamp==100) data = (TH1EFT*)hist[i].FindObject(thiscat+thissyst+sample_names_reg[thisSamp])->Clone();
                    //else data->Add(thishist);
                    
                    // The following assumes that you're including all the data samples (100-104), and that 104 was the last sample included.
                    auto data = (TH1EFT*)hist[i].FindObject(thiscat+sample_names_reg[thisSamp]);
                    data->Add((TH1EFT*)hist[i-1].FindObject(thiscat+sample_names_reg[103]));
                    data->Add((TH1EFT*)hist[i-2].FindObject(thiscat+sample_names_reg[102]));
                    data->Add((TH1EFT*)hist[i-3].FindObject(thiscat+sample_names_reg[101]));
                    data->Add((TH1EFT*)hist[i-4].FindObject(thiscat+sample_names_reg[100]));                    
                }
                
                TString cltxt = "\\textcolor{black}{";
                
                if (thissyst=="")
                {
                    nomylds.push_back(thishist->Integral());
                }
                else
                {
                    double offamnt = abs((thishist->Integral() - nomylds[cnt])/nomylds[cnt]);
                    if (offamnt>0.05) cltxt = "\\textcolor{red}{";
                
                }

                // cout << " & " << cltxt << std::fixed << std::setprecision(2) << thishist->Integral() << "}";
                //cout << " & " << cltxt << std::fixed << std::setprecision(2) << abs((thishist->Integral() - nomylds[cnt])/nomylds[cnt]) << "}";
                
                if (!(thisSamp>=10 && thisSamp<=12) && !(thisSamp>=22 && thisSamp<=25) && thisSamp<100) canvas.Add(thishist); //<-- last step 
                if (thisSamp==12) 
                {
                    if (thissyst=="") canvas.Add( (TH1EFT*)hist[i-2].FindObject(thiscat+thissyst+sample_names_reg[10]) );
                    else canvas.Add( (TH1EFT*)hist[i-2].FindObject(thiscat+thissyst+string(".")+sample_names_reg[10]) );
                }
                if (thisSamp==25) 
                {
                    if (thissyst=="") canvas.Add( (TH1EFT*)hist[i-3].FindObject(thiscat+thissyst+sample_names_reg[22]) );
                    else canvas.Add( (TH1EFT*)hist[i-3].FindObject(thiscat+thissyst+string(".")+sample_names_reg[22]) );
                }     
                if (thisSamp==104)
                {
                    auto data = (TH1EFT*)hist[i].FindObject(thiscat+sample_names_reg[thisSamp]);
                    data->SetName(thiscat+thissyst+"data");
                    //cout << "blah" << endl;
                    canvas.Add(data);
                }           
                cnt++;
            }
            
            // cout << " \\\\ " << endl;   
            
            //for (const TString thiscat : cats)
            //{   
            //    if (debug) cout << thiscat+thissyst << endl;
            //    auto thishist = (TH1EFT*)hist[i].FindObject(thiscat+thissyst);
            //    
            //    if (thissyst=="") thishist->SetName(thishist->GetName()+sample_names_reg[thisSamp]);
            //    else thishist->SetName(thishist->GetName()+string(".")+sample_names_reg[thisSamp]);
            //    
            //    if (thisSamp<40) thishist->Scale(lumi*xsec[thisSamp]/numgen[thisSamp]);
            //    else if(thisSamp>=40 && thisSamp<94) 
            //    {
            //        thishist->ScaleFits(lumi);
            //    }
            //    
            //    canvas.Add(thishist); //<-- last step 
            //}
        }



        // Need to run over muR/muF/muRmuF systematics again to compute the envelope and then normalize the result to get proper shape-only variations
        // NOTE: At this point all of the histograms should have already been properly (re-)scaled, we just want to compute the envelope and normalize
        // TODO: Clean this code up to be more compact/concise
        if (thisSamp > 0 && thisSamp < 90) { // The muR/muF envelope is only for MC based samples
            // This is so we don't end up including unnecessary samples that were already merged into other samples
            bool is_merged_sample = false;
            is_merged_sample = (is_merged_sample || thisSamp == 11 || thisSamp == 12);                      // Merged Dibosons
            is_merged_sample = (is_merged_sample || thisSamp == 23 || thisSamp == 24 || thisSamp == 25);    // Merged Tribosons
            if (is_merged_sample) continue;
            for (const TString thiscat : allAnaHists) {
                //Note1: thiscat+sample_names_reg[...] should be something like '2lss_p_ee_2b.ttH' or '2lss_p_ee_2b.ttll_16D'
                //Note2: hist is a vector of TObjArrays, one TObjArray per input histogram
                //Note3: the systematic histograms have already been renamed above, so the 'FindObject' needs to search for the new name
                TString nom_name = thiscat + sample_names_reg[thisSamp];

                // std::cout << "thisSamp: " << thisSamp << std::endl;
                // std::cout << "thiscat: " << thiscat << std::endl;
                // std::cout << "sample_names: " << sample_names_reg[thisSamp] << std::endl;
                // std::cout << nom_name << std::endl;

                auto nomhist = (TH1EFT*)hist[i].FindObject(nom_name);
                
                TString mur_up_name    = thiscat + "MURUP" + "." + sample_names_reg[thisSamp];
                TString muf_up_name    = thiscat + "MUFUP" + "." + sample_names_reg[thisSamp];
                TString murmuf_up_name = thiscat + "MURMUFUP" + "." + sample_names_reg[thisSamp];

                auto murhist_up    = (TH1EFT*)hist[i].FindObject(mur_up_name);
                auto mufhist_up    = (TH1EFT*)hist[i].FindObject(muf_up_name);
                auto murmufhist_up = (TH1EFT*)hist[i].FindObject(murmuf_up_name);

                bool is_bad = false;
                if (!murhist_up) {
                    std::cout << "[ERROR] Missing histogram " << mur_up_name << std::endl;
                    is_bad = true;
                }

                if (!mufhist_up) {
                    std::cout << "[ERROR] Missing histogram " << muf_up_name << std::endl;
                    is_bad = true;
                }

                if (!murmufhist_up) {
                    std::cout << "[ERROR] Missing histogram " << murmuf_up_name << std::endl;
                    is_bad = true;
                }

                TString mur_down_name    = thiscat + "MURDOWN" + "." + sample_names_reg[thisSamp];
                TString muf_down_name    = thiscat + "MUFDOWN" + "." + sample_names_reg[thisSamp];
                TString murmuf_down_name = thiscat + "MURMUFDOWN" + "." + sample_names_reg[thisSamp];

                auto murhist_down    = (TH1EFT*)hist[i].FindObject(mur_down_name);
                auto mufhist_down    = (TH1EFT*)hist[i].FindObject(muf_down_name);
                auto murmufhist_down = (TH1EFT*)hist[i].FindObject(murmuf_down_name);

                if (!murhist_down) {
                    std::cout << "[ERROR] Missing histogram " << mur_down_name << std::endl;
                    is_bad = true;
                }

                if (!mufhist_down) {
                    std::cout << "[ERROR] Missing histogram " << muf_down_name << std::endl;
                    is_bad = true;
                }

                if (!murmufhist_down) {
                    std::cout << "[ERROR] Missing histogram " << murmuf_down_name << std::endl;
                    is_bad = true;
                }

                if (is_bad) continue;

                // The name of the new systematic computed from the envelope of muR/muF/muR+muF
                TString env_up_name   = thiscat + "Q2RFUP" + "." + sample_names_reg[thisSamp];
                TString env_down_name = thiscat + "Q2RFDOWN" + "." + sample_names_reg[thisSamp];

                std::vector<TH1EFT*> up_variants {murhist_up,mufhist_up,murmufhist_up};
                std::vector<TH1EFT*> down_variants {murhist_down,mufhist_down,murmufhist_down};

                TH1EFT* envhist_up   = calculateEnvelope(env_up_name,nomhist,up_variants);
                TH1EFT* envhist_down = calculateEnvelope(env_down_name,nomhist,down_variants);

                /*
                Int_t nbins = nomhist->GetNbinsX();
                Double_t xlo = nomhist->GetBinLowEdge(1);
                Double_t xhi = nomhist->GetBinLowEdge(nbins+1);
                TH1EFT* envhist_up   = new TH1EFT(env_up_name,"",nbins,xlo,xhi);
                TH1EFT* envhist_down = new TH1EFT(env_down_name,"",nbins,xlo,xhi);

                std::cout << std::fixed << std::setprecision(3);
                // std::cout << "Sample: " << sample_names_reg[thisSamp] << std::endl;
                // std::cout << "Category: " << thiscat << std::endl;
                // Note: This ignores the underflow and overflow bins!
                for (Int_t j = 1; j <= nbins; j++) {
                    double nom_content = nomhist->GetBinContent(j);

                    double mur_up_content    = murhist_up->GetBinContent(j);
                    double muf_up_content    = mufhist_up->GetBinContent(j);
                    double murmuf_up_content = murmufhist_up->GetBinContent(j);

                    double mur_down_content    = murhist_down->GetBinContent(j);
                    double muf_down_content    = mufhist_down->GetBinContent(j);
                    double murmuf_down_content = murmufhist_down->GetBinContent(j);

                    double extreme_value = 0.0;
                    
                    if (abs(nom_content - mur_up_content) > extreme_value) {
                        extreme_value = abs(nom_content - mur_up_content);
                        envhist_up->SetBinContent(j,murhist_up->GetBinContent(j));
                        envhist_up->SetBinError(j,murhist_up->GetBinError(j));
                        envhist_up->hist_fits.at(j-1) = murhist_up->GetBinFit(j);    // Note: We use j-1 here b/c of the off-by-one index
                    }

                    if (abs(nom_content - muf_up_content) > extreme_value) {
                        extreme_value = abs(nom_content - muf_up_content);
                        envhist_up->SetBinContent(j,mufhist_up->GetBinContent(j));
                        envhist_up->SetBinError(j,mufhist_up->GetBinError(j));
                        envhist_up->hist_fits.at(j-1) = mufhist_up->GetBinFit(j);    // Note: We use j-1 here b/c of the off-by-one index
                    }

                    if (abs(nom_content - murmuf_up_content) > extreme_value) {
                        extreme_value = abs(nom_content - murmuf_up_content);
                        envhist_up->SetBinContent(j,murmufhist_up->GetBinContent(j));
                        envhist_up->SetBinError(j,murmufhist_up->GetBinError(j));
                        envhist_up->hist_fits.at(j-1) = murmufhist_up->GetBinFit(j);    // Note: We use j-1 here b/c of the off-by-one index
                    }

                    extreme_value = 0.0;

                    if (abs(nom_content - mur_down_content) > extreme_value) {
                        extreme_value = abs(nom_content - mur_down_content);
                        envhist_down->SetBinContent(j,murhist_down->GetBinContent(j));
                        envhist_down->SetBinError(j,murhist_down->GetBinError(j));
                        envhist_down->hist_fits.at(j-1) = murhist_down->GetBinFit(j);    // Note: We use j-1 here b/c of the off-by-one index
                    }
                    if (abs(nom_content - muf_down_content) > extreme_value) {
                        extreme_value = abs(nom_content - muf_down_content);
                        envhist_down->SetBinContent(j,mufhist_down->GetBinContent(j));
                        envhist_down->SetBinError(j,mufhist_down->GetBinError(j));
                        envhist_down->hist_fits.at(j-1) = mufhist_down->GetBinFit(j);    // Note: We use j-1 here b/c of the off-by-one index
                    }
                    if (abs(nom_content - murmuf_down_content) > extreme_value) {
                        extreme_value = abs(nom_content - murmuf_down_content);
                        envhist_down->SetBinContent(j,murmufhist_down->GetBinContent(j));
                        envhist_down->SetBinError(j,murmufhist_down->GetBinError(j));
                        envhist_down->hist_fits.at(j-1) = murmufhist_down->GetBinFit(j);    // Note: We use j-1 here b/c of the off-by-one index
                    }
                }
                // std::cout << "Nominal Integral:  " << nomhist->Integral() << std::endl;
                // std::cout << "Env Up Integral:   " << envhist_up->Integral() << std::endl;
                // std::cout << "Env Down Integral: " << envhist_down->Integral() << std::endl;
                */


                // NOTE: This is not the correct thing to do. To do the proper 'shape' normalization
                //      we need to divide the histogram by Ngen, where Ngen is the sum of all events
                //      in the sample reweighted to to the corresponding UP/DOWN variant of the systematic
                //      being considered. Additionally, this normalization should be done BEFORE calculating
                //      the envelope.
                // Make them into shape-only systematics
                double normamnt = 0.0;
                if (thisSamp < 40) {
                    normamnt = (nomhist->GetEntries()!=0 && envhist_up->Integral()!=0.) ? nomhist->Integral()/envhist_up->Integral() : 1.;
                    envhist_up->Scale(normamnt);
                    normamnt = (nomhist->GetEntries()!=0 && envhist_down->Integral()!=0.) ? nomhist->Integral()/envhist_down->Integral() : 1.;
                    envhist_down->Scale(normamnt);
                } else if (thisSamp >= 40 && thisSamp < 90) {            
                    normamnt = (nomhist->GetEntries()!=0 && envhist_up->Integral()!=0.) ? nomhist->Integral()/envhist_up->Integral() : 1.;
                    envhist_up->ScaleFits(normamnt);
                    envhist_up->Scale(WCPoint());
                    normamnt = (nomhist->GetEntries()!=0 && envhist_down->Integral()!=0.) ? nomhist->Integral()/envhist_down->Integral() : 1.;
                    envhist_down->ScaleFits(normamnt);
                    envhist_down->Scale(WCPoint());
                }

                canvas.Add(envhist_up);
                canvas.Add(envhist_down);
            }
        }


        cout << "  " << endl;
        cout << "  " << endl;        
        
    }
    
    // That's it! Now just pick SaveAllToFile draw option
    
}
