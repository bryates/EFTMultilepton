void HistMaker::bookHistos()
{
    // this function is ONLY for booking the histo/tgraph arrays!
    
    //cout << "Booking histograms..." << endl;
               
    //TH1D *th1ds[arrsize]; 
    //TH2D *th2ds[arrsize];
    
    //TGraph *tgraphs[arrsize];
    
    bool debug = false;
    
    if (debug) cout << "inside bookHistos()" << endl;
    //for (int i=0; i<(int)th1arrsize; i++) th1ds[i] = new TH1D("blah"+int2ss(i),"",10,0,1);
    for (int i=0; i<(int)th1arrsize; i++) th1ds[i] = new TH1EFT("blah"+int2ss(i),"",10,0,1);
    if (debug) cout << "initialized th1ds" << endl;
    for (int i=0; i<(int)th1eftarrsize; i++) th1efts[i] = new TH1EFT("blahEFT"+int2ss(i),"",15,0,15);
    if (debug) cout << "initialized th1efts" << endl;
    for (int i=0; i<(int)th2arrsize; i++) th2ds[i] = new TH2D("blah2D"+int2ss(i),"",10,0,1,10,0,1);
    if (debug) cout << "initialized th2ds" << endl;
    /////////////////////////////////////////////////////////////////////////
    // Actually book them here. The histos each need to be uniquely named
    // because they are inserted into a dictionary and looked up later.
            
    unsigned int i=0;
    unsigned int j=0;
    unsigned int ii=0;
    
    // electrons
    
    // 1,2,3 are different pt ranges
    th1ds[i]->SetName("eleMVA sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;  
    if (debug) cout << "bookhistos 2" << endl;                         
    th1ds[i]->SetName("eleMVA sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    if (debug) cout << "bookhistos 2.1" << endl;
    th1ds[i]->SetName("electron lepMVA sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("electron lepMVA sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("electron lepMVA sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    if (debug) cout << "bookhistos 2.2" << endl;
    th1ds[i]->SetName("eleMVA bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    if (debug) cout << "bookhistos 2.3" << endl;
    th1ds[i]->SetName("electron lepMVA bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("electron lepMVA bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    th1ds[i]->SetName("electron lepMVA bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    if (debug) cout << "bookhistos 2.4" << endl;
    th1ds[i]->SetName("eleMVA GP WP90 sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    if (debug) cout << "bookhistos 2.5" << endl;
    th1ds[i]->SetName("eleMVA GP WP90 bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    if (debug) cout << "bookhistos 2.6" << endl;
    th1ds[i]->SetName("eleMVA sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                          
    th1ds[i]->SetName("eleMVA sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    if (debug) cout << "bookhistos 2.7" << endl;
    th1ds[i]->SetName("electron lepMVA sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("electron lepMVA sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("electron lepMVA sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    if (debug) cout << "bookhistos 2.8" << endl;
    th1ds[i]->SetName("eleMVA bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    if (debug) cout << "bookhistos 2.9" << endl;
    th1ds[i]->SetName("electron lepMVA bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("electron lepMVA bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    th1ds[i]->SetName("electron lepMVA bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    if (debug) cout << "bookhistos 2.10" << endl;
    th1ds[i]->SetName("eleMVA GP WP90 sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    if (debug) cout << "bookhistos 2.11" << endl;
    th1ds[i]->SetName("eleMVA GP WP90 bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;  
    if (debug) cout << "bookhistos 2.12" << endl;

    // inclusive in pt
    th1ds[i]->SetName("eleMVA sig barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("eleMVA bkd barrel");             th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("electron lepMVA sig barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("electron lepMVA bkd barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
            
    th1ds[i]->SetName("eleMVA GP WP90 sig barrel");     th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("eleMVA GP WP90 bkd barrel");     th1ds[i]->SetBins(500,-1,1);    i++;

    th1ds[i]->SetName("eleMVA sig endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("eleMVA bkd endcap");             th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("electron lepMVA sig endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("electron lepMVA bkd endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
            
    th1ds[i]->SetName("eleMVA GP WP90 sig endcap");     th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("eleMVA GP WP90 bkd endcap");     th1ds[i]->SetBins(500,-1,1);    i++;


    // muons

    // 1,2,3 are different pt ranges                            
    th1ds[i]->SetName("muon lepMVA sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("muon lepMVA sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("muon lepMVA sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                                                         

    th1ds[i]->SetName("muon lepMVA bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("muon lepMVA bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    th1ds[i]->SetName("muon lepMVA bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                            

    th1ds[i]->SetName("muon POG loose ID sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            

    th1ds[i]->SetName("muon POG loose ID bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                                       

    th1ds[i]->SetName("muon lepMVA sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("muon lepMVA sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("muon lepMVA sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                                                       

    th1ds[i]->SetName("muon lepMVA bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("muon lepMVA bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    th1ds[i]->SetName("muon lepMVA bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                            

    th1ds[i]->SetName("muon POG loose ID sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            

    th1ds[i]->SetName("muon POG loose ID bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("muon POG loose ID bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;  


    // inclusive in pt
    th1ds[i]->SetName("muon lepMVA sig barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("muon lepMVA bkd barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
            
    th1ds[i]->SetName("muon POG loose ID sig barrel");     th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("muon POG loose ID bkd barrel");     th1ds[i]->SetBins(500,-1,1);    i++;

    th1ds[i]->SetName("muon lepMVA sig endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("muon lepMVA bkd endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
            
    th1ds[i]->SetName("muon POG loose ID sig endcap");     th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("muon POG loose ID bkd endcap");     th1ds[i]->SetBins(500,-1,1);    i++;

    if (debug) cout << "bookhistos 3" << endl;

    /// jet cleaning studies ///
    
    th2ds[j]->SetName("muon_numjets_vs_dR");                    th2ds[j]->SetBins(50,0.,2.,10,0,10);    j++;
    th2ds[j]->SetName("electron_numjets_vs_dR");                th2ds[j]->SetBins(50,0.,2.,10,0,10);    j++;
    th2ds[j]->SetName("muon_numjets_vs_dR_prompt");             th2ds[j]->SetBins(50,0.,2.,10,0,10);    j++;
    th2ds[j]->SetName("muon_numjets_vs_dR_nonprompt");          th2ds[j]->SetBins(50,0.,2.,10,0,10);    j++;
    th2ds[j]->SetName("electron_numjets_vs_dR_prompt");         th2ds[j]->SetBins(50,0.,2.,10,0,10);    j++;
    th2ds[j]->SetName("electron_numjets_vs_dR_nonprompt");      th2ds[j]->SetBins(50,0.,2.,10,0,10);    j++;    
    
    th2ds[j]->SetName("muon_numjets_vs_dR_zoomin");                    th2ds[j]->SetBins(50,0.,0.2,10,0,10);    j++;
    th2ds[j]->SetName("electron_numjets_vs_dR_zoomin");                th2ds[j]->SetBins(50,0.,0.2,10,0,10);    j++;
    th2ds[j]->SetName("muon_numjets_vs_dR_prompt_zoomin");             th2ds[j]->SetBins(50,0.,0.2,10,0,10);    j++;
    th2ds[j]->SetName("muon_numjets_vs_dR_nonprompt_zoomin");          th2ds[j]->SetBins(50,0.,0.2,10,0,10);    j++;
    th2ds[j]->SetName("electron_numjets_vs_dR_prompt_zoomin");         th2ds[j]->SetBins(50,0.,0.2,10,0,10);    j++;
    th2ds[j]->SetName("electron_numjets_vs_dR_nonprompt_zoomin");      th2ds[j]->SetBins(50,0.,0.2,10,0,10);    j++;  

    th1ds[i]->SetNameTitle("jet_pt_minus_electron_pt_one_jet_in_dR0p4_prompt",";(#vec{p}_{jet}-#vec{p}_{ele})_{T}");      th1ds[i]->SetBins(200,-100,200);    i++;
    th1ds[i]->SetNameTitle("jet_pt_minus_electron_pt_one_jet_in_dR0p4_nonprompt",";(#vec{p}_{jet}-#vec{p}_{ele})_{T}");   th1ds[i]->SetBins(200,-100,200);    i++;
    th1ds[i]->SetNameTitle("jet_pt_minus_muon_pt_one_jet_in_dR0p4_prompt",";(#vec{p}_{jet}-#vec{p}_{mu})_{T}");          th1ds[i]->SetBins(200,-100,200);    i++;
    th1ds[i]->SetNameTitle("jet_pt_minus_muon_pt_one_jet_in_dR0p4_nonprompt",";(#vec{p}_{jet}-#vec{p}_{mu})_{T}");       th1ds[i]->SetBins(200,-100,200);    i++;
    

    th2ds[j]->SetNameTitle("jet_pt_minus_electron_pt_one_jet_in_dR0p4_prompt_vs_lep_pt",";ele p_{T};(#vec{p}_{jet}-#vec{p}_{ele})_{T}");      th2ds[j]->SetBins(200,0,200,200,-100,200);    j++;
    th2ds[j]->SetNameTitle("jet_pt_minus_electron_pt_one_jet_in_dR0p4_nonprompt_vs_lep_pt",";ele p_{T};(#vec{p}_{jet}-#vec{p}_{ele})_{T}");   th2ds[j]->SetBins(200,0,200,200,-100,200);    j++;
    th2ds[j]->SetNameTitle("jet_pt_minus_muon_pt_one_jet_in_dR0p4_prompt_vs_lep_pt",";mu p_{T};(#vec{p}_{jet}-#vec{p}_{mu})_{T}");          th2ds[j]->SetBins(200,0,200,200,-100,200);    j++;
    th2ds[j]->SetNameTitle("jet_pt_minus_muon_pt_one_jet_in_dR0p4_nonprompt_vs_lep_pt",";mu p_{T};(#vec{p}_{jet}-#vec{p}_{mu})_{T}");       th2ds[j]->SetBins(200,0,200,200,-100,200);    j++;
    
    th2ds[j]->SetNameTitle("jet_pt_minus_electron_pt_one_jet_in_dR0p4_prompt_vs_lep_jet_pt_ratio",";ele p_{T} / jet p_{T};(#vec{p}_{jet}-#vec{p}_{ele})_{T}");      th2ds[j]->SetBins(200,0.,1.5,200,-100,200);    j++;
    th2ds[j]->SetNameTitle("jet_pt_minus_electron_pt_one_jet_in_dR0p4_nonprompt_vs_lep_jet_pt_ratio",";ele p_{T} / jet p_{T};(#vec{p}_{jet}-#vec{p}_{ele})_{T}");   th2ds[j]->SetBins(200,0.,1.5,200,-100,200);    j++;
    th2ds[j]->SetNameTitle("jet_pt_minus_muon_pt_one_jet_in_dR0p4_prompt_vs_lep_jet_pt_ratio",";mu p_{T} / jet p_{T};(#vec{p}_{jet}-#vec{p}_{mu})_{T}");          th2ds[j]->SetBins(200,0.,1.5,200,-100,200);    j++;
    th2ds[j]->SetNameTitle("jet_pt_minus_muon_pt_one_jet_in_dR0p4_nonprompt_vs_lep_jet_pt_ratio",";mu p_{T} / jet p_{T};(#vec{p}_{jet}-#vec{p}_{mu})_{T}");       th2ds[j]->SetBins(200,0.,1.5,200,-100,200);    j++;    
    
    if (debug) cout << "bookhistos 4" << endl;

    // th1ds[i]->SetNameTitle("2los_ee__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2los_emu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2los_mumu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2los_sfz_ee__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2los_sfz_mumu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2lss_p_ee__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2lss_p_emu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2lss_p_mumu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2lss_m_ee__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2lss_m_emu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("2lss_m_mumu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("3l_ppp__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("3l_mmm__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // //th1ds[i]->SetNameTitle("3l_mix__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("3l_mix_p__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("3l_mix_m__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("3l_mix_sfz__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("4l__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("ge5l__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;    
    // th1ds[i]->SetNameTitle("1l_mu__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("1l_e__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    // th1ds[i]->SetNameTitle("all__njets",";njets");  th1ds[i]->SetBins(15,0,15);    i++;

    th1ds[i]->SetNameTitle("category_yields",""); th1ds[i]->SetBins(20,1,21);
    th1ds[i]->GetXaxis()->SetBinLabel(1,"2los_ee");
    th1ds[i]->GetXaxis()->SetBinLabel(2,"2los_sfz_ee");
    th1ds[i]->GetXaxis()->SetBinLabel(3,"2los_emu");
    th1ds[i]->GetXaxis()->SetBinLabel(4,"2los_mumu");
    th1ds[i]->GetXaxis()->SetBinLabel(5,"2los_sfz_mumu");
    th1ds[i]->GetXaxis()->SetBinLabel(6,"2lss_p_ee");
    th1ds[i]->GetXaxis()->SetBinLabel(7,"2lss_p_emu");
    th1ds[i]->GetXaxis()->SetBinLabel(8,"2lss_p_mumu");
    th1ds[i]->GetXaxis()->SetBinLabel(9,"2lss_m_ee");
    th1ds[i]->GetXaxis()->SetBinLabel(10,"2lss_m_emu");
    th1ds[i]->GetXaxis()->SetBinLabel(11,"2lss_m_mumu");
    th1ds[i]->GetXaxis()->SetBinLabel(12,"3l_ppp");
    th1ds[i]->GetXaxis()->SetBinLabel(13,"3l_mmm");
    //th1ds[i]->GetXaxis()->SetBinLabel(14,"3l_mix");
    th1ds[i]->GetXaxis()->SetBinLabel(14,"3l_mix_p");
    th1ds[i]->GetXaxis()->SetBinLabel(15,"3l_mix_m");
    th1ds[i]->GetXaxis()->SetBinLabel(16,"3l_mix_sfz");
    th1ds[i]->GetXaxis()->SetBinLabel(17,"4l");     
    th1ds[i]->GetXaxis()->SetBinLabel(18,"ge5l");         
    th1ds[i]->GetXaxis()->SetBinLabel(19,"1l_mu");                           
    th1ds[i]->GetXaxis()->SetBinLabel(20,"1l_e");                           i++;
    
    
    // todo: implement a 1b version of these if actually going to use 1b cats
    
    // are these actually used??
    th1ds[i]->SetNameTitle("2lss_p_ee_2b_QFevents.",";njets");  th1ds[i]->SetBins(15,0,15);    i++;       
    th1ds[i]->SetNameTitle("2lss_p_emu_2b_QFevents.",";njets");  th1ds[i]->SetBins(15,0,15);    i++;      
    th1ds[i]->SetNameTitle("2lss_m_ee_2b_QFevents.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;      
    th1ds[i]->SetNameTitle("2lss_m_emu_2b_QFevents.",";njets");    th1ds[i]->SetBins(15,0,15);    i++;    
    th1ds[i]->SetNameTitle("2lss_p_ee_2b_QFevents.JESUP",";njets");   th1ds[i]->SetBins(15,0,15);    i++; 
    th1ds[i]->SetNameTitle("2lss_p_emu_2b_QFevents.JESUP",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_ee_2b_QFevents.JESUP",";njets");   th1ds[i]->SetBins(15,0,15);    i++; 
    th1ds[i]->SetNameTitle("2lss_m_emu_2b_QFevents.JESUP",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_ee_2b_QFevents.JESDOWN",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_emu_2b_QFevents.JESDOWN",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_ee_2b_QFevents.JESDOWN",";njets");  th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_emu_2b_QFevents.JESDOWN",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_ee_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_ee_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_sfz_ee_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_sfz_ee_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_emu_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_emu_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_mumu_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_mumu_2b_Fakes.",";njets");      th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_sfz_mumu_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2los_sfz_mumu_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_ee_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_ee_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_emu_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_emu_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_mumu_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_p_mumu_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_ee_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_ee_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_emu_1b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_emu_2b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_mumu_1b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("2lss_m_mumu_2b_Fakes.",";njets"); th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_ppp_1b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_ppp_2b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_mmm_1b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_mmm_2b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_mix_1b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_mix_2b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_mix_sfz_1b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("3l_mix_sfz_2b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("4l_1b_Fakes.",";njets");      th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("4l_2b_Fakes.",";njets");   th1ds[i]->SetBins(15,0,15);    i++;
    th1ds[i]->SetNameTitle("ge5l_1b_Fakes.",";njets");    th1ds[i]->SetBins(15,0,15);    i++;    
    
    
    //////////////////////////////////////////////////////////////////
    // actual analysis hists
    
    // sr range
    int tlssnbins_sr = 4; int tlssmin_sr = 4; int tlssmax_sr = 8;               // actual   
    ////int tlssnbins_sr = 5; int tlssmin_sr = 3; int tlssmax_sr = 8;               // test
    int tlnbins_sr = 4; int tlmin_sr = 2; int tlmax_sr = 6;                     // actual
    
    //int tlssnbins_sr = 2; int tlssmin_sr = 4; int tlssmax_sr = 6;               // test
    //int tlnbins_sr = 2; int tlmin_sr = 2; int tlmax_sr = 4;                     // test
    
    // cr range
    int tlssnbins_cr = 3; int tlssmin_cr = 1; int tlssmax_cr = 4;               // actual
    int tlnbins_cr = 5; int tlmin_cr = 1; int tlmax_cr = 6;                     // actual
     
    // pick: sr or cr HAS TO MATCH WHAT YOU'RE DOING IN EVENSELECTION! 
    // Edit 1-25-19: now handled in HistMaker ctr

    int tlssnbins = tlssnbins_cr; 
    int tlssmin = tlssmin_cr; 
    int tlssmax = tlssmax_cr;
    int tlnbins = tlnbins_cr; 
    int tlmin = tlmin_cr; 
    int tlmax = tlmax_cr;

    if (isSR)
    {
        tlssnbins = tlssnbins_sr; 
        tlssmin = tlssmin_sr; 
        tlssmax = tlssmax_sr;
        tlnbins = tlnbins_sr; 
        tlmin = tlmin_sr; 
        tlmax = tlmax_sr;
    }

    // temp!!!!
    // tlssnbins = 15; 
    // tlssmin = 0; 
    // tlssmax = 15;
    // tlnbins = 15; 
    // tlmin = 0; 
    // tlmax = 15;

    std::vector<TString> cat_strings = {
        "2los_ee",
        "2los_emu",
        "2los_mumu",
        "2los_sfz_ee",
        "2los_sfz_mumu",
        "2lss_p_ee",
        "2lss_p_emu",
        "2lss_p_mumu",
        "2lss_m_ee",
        "2lss_m_emu",
        "2lss_m_mumu",
        "3l_ppp",
        "3l_mmm",
        // "3l_mix",
        "3l_mix_p",
        "3l_mix_m",
        "3l_mix_sfz",
        "4l",
        "ge5l",
        "1l_mu",
        "1l_e",
        "all",
    };

    if (debug) cout << "bookhistos 5" << endl;
    
    for (int isys=0; isys<numberOfSysts; isys++) {// see utils.h
        if (debug) cout << isys << endl;
        if (debug) cout << i << endl;
        if (debug) cout << ii << endl;
        if (debug) cout << "bookhistos 5.1" << endl;
        
        for (TString s: cat_strings) {
            th1efts[ii]->SetNameTitle(s+"_1b."+systint2str(isys),";njets");
            if (s.BeginsWith("2lss_")) {
                th1efts[ii]->SetBins(tlssnbins,tlssmin,tlssmax);
            } else if (s.BeginsWith("3l_")) {
                th1efts[ii]->SetBins(tlnbins,tlmin,tlmax);
            } else if (s.BeginsWith("4l_") || s.BeginsWith("ge5l_")) {
                th1efts[ii]->SetBins(4,1,5);
            }
            ii++;
            th1efts[ii]->SetNameTitle(s+"_2b."+systint2str(isys),";njets");
            if (s.BeginsWith("2lss_")) {
                th1efts[ii]->SetBins(tlssnbins,tlssmin,tlssmax);
            } else if (s.BeginsWith("3l_")) {
                th1efts[ii]->SetBins(tlnbins,tlmin,tlmax);
            } else if (s.BeginsWith("4l_") || s.BeginsWith("ge5l_")) {
                th1efts[ii]->SetBins(4,1,5);
            }
            ii++;
        }
        
        if (debug) cout << "bookhistos 5.2" << endl;
        
        // usual ("CR") range:
        // int other_tlssnbins = 15; 
        // int other_tlssmin = 0; 
        // int other_tlssmax = 15;
        // int other_tlnbins = 15; 
        // int other_tlmin = 0; 
        // int other_tlmax = 15;
        // int other_flnbins = 15;
        // int other_flmin = 0; 
        // int other_flmax = 15;
        
        // unusual (if doing the njets sfs with the whole range):
        int other_tlssnbins = 8; 
        int other_tlssmin = 0; 
        int other_tlssmax = 8;
        int other_tlnbins = 6; 
        int other_tlmin = 0; 
        int other_tlmax = 6;
        int other_flnbins = 5;
        int other_flmin = 0; 
        int other_flmax = 5;

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__njets."+systint2str(isys),";njets");
            if (s.EqualTo("2lss_p_ee")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("2lss_p_emu")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("2lss_p_mumu")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("2lss_m_ee")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("2lss_m_emu")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("2lss_m_mumu")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("3l_ppp")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("3l_mmm")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("3l_mix")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("3l_mix_p")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("3l_mix_m")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("3l_mix_sfz")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else if (s.EqualTo("4l")) {
                th1ds[i]->SetBins(other_tlssnbins,other_tlssmin,other_tlssmax);
            } else {
                th1ds[i]->SetBins(15,0,15);
            }
            i++;
        }
        
        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__nbjets."+systint2str(isys),";nbjets");  th1ds[i]->SetBins(15,0,15);    i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__jetpt."+systint2str(isys),";jet p_{T}");  th1ds[i]->SetBins(50,0,200);    i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__jeteta."+systint2str(isys),";jet #eta");  th1ds[i]->SetBins(50,-3.0,3.0);    i++;    
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__jetDeepCSV."+systint2str(isys),";jet DeepCSV");  th1ds[i]->SetBins(50,0,1);    i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__lep1pt."+systint2str(isys),";lep 1 p_{T}");  th1ds[i]->SetBins(50,0,200);i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__lep2pt."+systint2str(isys),";lep 2 p_{T}");  th1ds[i]->SetBins(50,0,200);    i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__lepeta."+systint2str(isys),";lep #eta");  th1ds[i]->SetBins(50,-3.0,3.0);    i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__llmass."+systint2str(isys),";Inv. Mass (l,l)");  th1ds[i]->SetBins(100,0,150);    i++;
        }
        
        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__met."+systint2str(isys),";MET p_{T}");  th1ds[i]->SetBins(50,0,300);    i++;
        }       
        
        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__MHT."+systint2str(isys),";MHT p_{T}");  th1ds[i]->SetBins(100,0,300);    i++;
        }       

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__HT."+systint2str(isys),";HT");  th1ds[i]->SetBins(200,0,1000);    i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__PUMVA."+systint2str(isys),";jet PU MVA");  th1ds[i]->SetBins(200,0,1);    i++;
        }

        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__ntaus."+systint2str(isys),";n#tau_{had}");  th1ds[i]->SetBins(10,0,10);    i++;
        }
        
        for (TString s: cat_strings) {
            th1ds[i]->SetNameTitle(s+"__hadtopmass."+systint2str(isys),";top_{had} inv. mass");  th1ds[i]->SetBins(200,0,400);    i++;
        }
    }
    
    //////////////////////////////////////////////////////////////////
    
    if (debug) cout << "bookhistos 6" << endl;
    
    th1ds[i]->SetNameTitle("category_yields_njets_nbjets",""); th1ds[i]->SetBins(149,1,150);

    std::vector<TString> jet_bins;
    jet_bins = {"2j_1b","2j_2b","3j_1b","3j_2b","4j_1b","4j_2b","5j_1b","5j_2b","6j_1b","6j_2b"};
    unsigned int k=1;
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_ee_"+s); k++; // 10
    }

    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_sfz_ee"+s); k++; // 20
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_emu_"+s); k++; // 30
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_mumu_"+s); k++; // 40
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_sfz_mumu_"+s); k++; // 50
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_ee_"+s); k++; // 60
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_emu_"+s); k++; // 70
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_mumu_"+s); k++; // 80
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_ee_"+s); k++; // 70
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_emu_"+s); k++; // 80
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_mumu_"+s); k++; // 90
    }
    
    jet_bins = {"1j_1b","2j_1b","2j_2b","3j_1b","3j_2b","4j_1b","4j_2b"};
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_ppp_"+s); k++; // 117
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mmm_"+s); k++; // 124
    }
    
    // for (TString s: jet_bins) {
    //     th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_"+s); k++; // 131
    // }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_p_"+s); k++; // 131
    }

    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_m_"+s); k++; // 138
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_sfz_"+s); k++; // 145
    }

    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_1j_1b"); k++;    
    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_2j_1b"); k++;
    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_2j_2b"); k++; // 148
    
    th1ds[i]->GetXaxis()->SetBinLabel(k,"ge5l_1j_1b"); i++; // 149
    
    th1ds[i]->SetNameTitle("category_yields_njets_nbjets_JESUP",""); th1ds[i]->SetBins(149,1,150); k=1;
    jet_bins = {"2j_1b","2j_2b","3j_1b","3j_2b","4j_1b","4j_2b","5j_1b","5j_2b","6j_1b","6j_2b"};
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_ee_"+s); k++; // 10
    }

    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_sfz_ee"+s); k++; // 20
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_emu_"+s); k++; // 30
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_mumu_"+s); k++; // 40
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_sfz_mumu_"+s); k++; // 50
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_ee_"+s); k++; // 60
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_emu_"+s); k++; // 70
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_mumu_"+s); k++; // 80
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_ee_"+s); k++; // 70
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_emu_"+s); k++; // 80
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_mumu_"+s); k++; // 90
    }
    
    jet_bins = {"1j_1b","2j_1b","2j_2b","3j_1b","3j_2b","4j_1b","4j_2b"};
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_ppp_"+s); k++; // 117
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mmm_"+s); k++; // 124
    }
    
    // for (TString s: jet_bins) {
    //     th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_"+s); k++; // 131
    // }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_p_"+s); k++; // 131
    }

    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_m_"+s); k++; // 138
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_sfz_"+s); k++; // 145
    }

    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_1j_1b"); k++;    
    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_2j_1b"); k++;
    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_2j_2b"); k++; // 148
    
    th1ds[i]->GetXaxis()->SetBinLabel(k,"ge5l_1j_1b"); i++; // 149
    
    th1ds[i]->SetNameTitle("category_yields_njets_nbjets_JESDOWN",""); th1ds[i]->SetBins(149,1,150); k=1;
    jet_bins = {"2j_1b","2j_2b","3j_1b","3j_2b","4j_1b","4j_2b","5j_1b","5j_2b","6j_1b","6j_2b"};
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_ee_"+s); k++; // 10
    }

    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_sfz_ee"+s); k++; // 20
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_emu_"+s); k++; // 30
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_mumu_"+s); k++; // 40
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2los_sfz_mumu_"+s); k++; // 50
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_ee_"+s); k++; // 60
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_emu_"+s); k++; // 70
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_p_mumu_"+s); k++; // 80
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_ee_"+s); k++; // 70
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_emu_"+s); k++; // 80
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"2lss_m_mumu_"+s); k++; // 90
    }
    
    jet_bins = {"1j_1b","2j_1b","2j_2b","3j_1b","3j_2b","4j_1b","4j_2b"};
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_ppp_"+s); k++; // 117
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mmm_"+s); k++; // 124
    }
    
    // for (TString s: jet_bins) {
    //     th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_"+s); k++; // 131
    // }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_p_"+s); k++; // 131
    }

    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_m_"+s); k++; // 138
    }
    
    for (TString s: jet_bins) {
        th1ds[i]->GetXaxis()->SetBinLabel(k,"3l_mix_sfz_"+s); k++; // 145
    }

    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_1j_1b"); k++;    
    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_2j_1b"); k++;
    th1ds[i]->GetXaxis()->SetBinLabel(k,"4l_2j_2b"); k++; // 148
    
    th1ds[i]->GetXaxis()->SetBinLabel(k,"ge5l_1j_1b"); i++; // 149 

    th1ds[i]->SetNameTitle("category_yields_noOS",""); th1ds[i]->SetBins(13,1,14); k=1;
    for (TString s: cat_strings) {
        if (s.EqualTo("1l_mu")       || s.EqualTo("1l_e")     || s.EqualTo("all")) continue;
        if (s.EqualTo("2los_ee")     || s.EqualTo("2los_emu") || s.EqualTo("2los_mumu")) continue;
        if (s.EqualTo("2los_sfz_ee") || s.EqualTo("2los_sfz_mumu")) continue;
        th1ds[i]->GetXaxis()->SetBinLabel(k,s); k++;
    }
    i++;

    for (TString s: cat_strings) {
        if (s.EqualTo("all")) continue; // Wasn't included for some reason...
        th2ds[j]->SetNameTitle(s+"__nbjets_vs_njets",";njets;nbjets");       th2ds[j]->SetBins(15,0,15,10,0,10);    j++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jetcsv",";jet CSV");  th1ds[i]->SetBins(50,0,1);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jetDeepCSVprobb",";jet DeepCSV (prob b)");  th1ds[i]->SetBins(50,0,1);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jetDeepCSVprobbb",";jet DeepCSV (prob bb)");  th1ds[i]->SetBins(50,0,1);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jetDeepCSVprobc",";jet DeepCSV (prob c)");  th1ds[i]->SetBins(50,0,1);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jetDeepCSVprobudsg",";jet DeepCSV (prob lf)");  th1ds[i]->SetBins(50,0,1);    i++;
    }
    
    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__leppt",";lep p_{T}");  th1ds[i]->SetBins(50,0,200);    i++;
    }

    // for (TString s: cat_strings) {
    //     th1ds[i]->SetNameTitle(s+"__met",";MET p_{T}");  th1ds[i]->SetBins(50,0,200);    i++;
    // } 
    
    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__metphi",";MET #phi");  th1ds[i]->SetBins(50,-3.3,3.3);    i++;
    }
    
    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__numPVs",";Number of PVs");  th1ds[i]->SetBins(80,0,80);    i++;
    }
    
    th1ds[i]->SetNameTitle("numTruePVs",";True Number of Interactions");  th1ds[i]->SetBins(200,0,200);    i++;
    
    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__numTruePVs",";True Number of Interactions");  th1ds[i]->SetBins(80,0,80);    i++;
    }
 
    // for (TString s: cat_strings) {
    //     th1ds[i]->SetNameTitle(s+"__MHT",";MHT p_{T}");  th1ds[i]->SetBins(100,0,150);    i++;
    // }
    
    // for (TString s: cat_strings) {
    //     th1ds[i]->SetNameTitle(s+"__HT",";H_{T}");  th1ds[i]->SetBins(200,0,1000);    i++;
    // }   
    
    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__lepMVA",";lepMVA");  th1ds[i]->SetBins(200,-1,1);    i++;
    }

    //matchedJetdR; //?

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__dxy",";lepton dxy");  th1ds[i]->SetBins(200,-0.05,0.05);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__dz",";lepton dz");  th1ds[i]->SetBins(200,-0.1,0.1);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__miniIso",";lepton miniIso");  th1ds[i]->SetBins(200,0,1);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__miniIsoCharged",";lepton miniIsoCharged");  th1ds[i]->SetBins(200,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__miniIsoNeutral",";lepton miniIsoNeutral");  th1ds[i]->SetBins(200,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jetPtRatio",";lepton jetPtRatio");  th1ds[i]->SetBins(200,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jetPtRel",";lepton jetPtRel");  th1ds[i]->SetBins(200,0,400);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__nearestJetCSV",";lepton nearestJetCSV");  th1ds[i]->SetBins(200,0,1);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__sip3D",";lepton sip3D");  th1ds[i]->SetBins(200,0,8);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__jet_nCharged_tracks",";lepton jet_nCharged_tracks");  th1ds[i]->SetBins(20,0,20);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__correctedPt",";lepton correctedPt");  th1ds[i]->SetBins(200,0,200);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__rho",";lepton rho");  th1ds[i]->SetBins(200,0,15);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__effArea",";lepton effArea");  th1ds[i]->SetBins(200,0,0.2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__miniIsoR",";lepton miniIsoR");  th1ds[i]->SetBins(200,0,0.3);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__nnpdfWeightUp",";nnpdfWeightUp");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__nnpdfWeightDown",";nnpdfWeightDown");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__muRWeightUp",";muRWeightUp");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__muRWeightDown",";muRWeightDown");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__muFWeightUp",";muFWeightUp");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__muFWeightDown",";muFWeightDown");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__muRmuFWeightUp",";muRmuFWeightUp");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__muRmuFWeightDown",";muRmuFWeightDown");  th1ds[i]->SetBins(100,0,2);    i++;
    }

    //todo
    // miniAbsIsoCharged; 0-25
    // miniAbsIsoNeutral; 0-25
    // miniAbsIsoNeutralcorr;   0-25?

    // for (TString s: cat_strings) {
    //     th1ds[i]->SetNameTitle(s+"__PUMVA",";jet PU MVA");  th1ds[i]->SetBins(200,0,1);    i++;
    // }
    
    for (TString s: cat_strings) {
        th1ds[i]->SetNameTitle(s+"__qgid",";jet qgID");  th1ds[i]->SetBins(200,0,1);    i++;
    }    
    
    // some trigger stuff
    th1ds[i]->SetNameTitle("single_ele_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    th1ds[i]->SetNameTitle("double_ele_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    th1ds[i]->SetNameTitle("single_mu_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    th1ds[i]->SetNameTitle("double_mu_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    th1ds[i]->SetNameTitle("mu_ele_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    th1ds[i]->SetNameTitle("all_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    
    for (TString s: cat_strings) {
        if (s.EqualTo("all")) continue; // Wasn't included for some reason...
        th1ds[i]->SetNameTitle(s+"__all_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    }

    for (TString s: cat_strings) {
        if (s.EqualTo("all")) continue; // Wasn't included for some reason...
        th1ds[i]->SetNameTitle(s+"__single_ele_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    }

    for (TString s: cat_strings) {
        if (s.EqualTo("all")) continue; // Wasn't included for some reason...
        th1ds[i]->SetNameTitle(s+"__double_ele_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    }

    for (TString s: cat_strings) {
        if (s.EqualTo("all")) continue; // Wasn't included for some reason...
        th1ds[i]->SetNameTitle(s+"__single_mu_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    }

    for (TString s: cat_strings) {
        if (s.EqualTo("all")) continue; // Wasn't included for some reason...
        th1ds[i]->SetNameTitle(s+"__double_mu_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    }

    for (TString s: cat_strings) {
        if (s.EqualTo("all")) continue; // Wasn't included for some reason...
        th1ds[i]->SetNameTitle(s+"__mu_ele_trigs",";passed trigger");  th1ds[i]->SetBins(2,0,2);    i++;
    }

    th2ds[j]->SetNameTitle("status_vs_pdgID",";pdgID;status");            th2ds[j]->SetBins(100,-50,50,100,0,100);    j++;
    th1ds[i]->SetNameTitle("particle_pdgID",";pdgID");                    th1ds[i]->SetBins(100,-50,50);    i++;
    th1ds[i]->SetNameTitle("mom_pdgID",";pdgID (mother)");                th1ds[i]->SetBins(100,-50,50);    i++;
    th1ds[i]->SetNameTitle("gmom_pdgID",";pdgID (grandmother)");          th1ds[i]->SetBins(100,-50,50);    i++;
    th2ds[j]->SetNameTitle("mom_vs_pdgID",";pdgID;pdgID (mother)");       th2ds[j]->SetBins(100,-50,50,100,-50,50);    j++;
    th2ds[j]->SetNameTitle("gmom_vs_pdgID",";pdgID;pdgID (grandmother)"); th2ds[j]->SetBins(100,-50,50,100,-50,50);    j++;
    
    th1ds[i]->SetNameTitle("gen_jet_energy",";gen jet energy"); th1ds[i]->SetBins(100,0,1000);  i++;
    th1ds[i]->SetNameTitle("gen_jet_eta",";gen jet #eta");      th1ds[i]->SetBins(50,-5.0,5.0); i++;
    th1ds[i]->SetNameTitle("gen_jet_phi",";gen jet #phi");      th1ds[i]->SetBins(50,-3.3,3.3); i++;
    th1ds[i]->SetNameTitle("ngenjets",";number of gen jets");   th1ds[i]->SetBins(25,0,25);     i++;

    th1ds[i]->SetNameTitle("all__jet_pdgID",";jet pdgID");                    th1ds[i]->SetBins(100,-50,50);    i++;
    th1ds[i]->SetNameTitle("all__jet_mom_pdgID",";jet pdgID (mother)");       th1ds[i]->SetBins(100,-50,50);    i++;
    th1ds[i]->SetNameTitle("all__jet_gmom_pdgID",";jet pdgID (grandmother)"); th1ds[i]->SetBins(100,-50,50);    i++;

    ///////////////////////////////////////////////////////////////////////////////////
    // regular trigger eff hists:
    // general / reasonable bins:
    Double_t bins[14];
    int numbins = 13; // bins size-1
    
    for (int ibin=0; ibin<4; ibin++) bins[ibin] = (Double_t)5.0*ibin;
    
    bins[4] = 20; // first "real" bin (?)
    //bins[5] = 40;
    //bins[5] = 1000; // 

    bins[5] = 25; // comment this out and change bin size to 13 when making 3l plots...
    bins[6] = 30;
    bins[7] = 35;
    bins[8] = 40;
    bins[9] = 45;
    bins[10] = 50;
    bins[11] = 60;
    bins[12] = 80;
    bins[13] = 100;
    
    // category-specific bins:
    // Double_t bins_mumu[4];
    // int numbins_mumu = 3; // bins size-1
    // bins_mumu[0] = 0;
    // bins_mumu[1] = 20;
    // bins_mumu[2] = 35;
    // bins_mumu[3] = 100;
      
    // Double_t bins_emu[5];
    // int numbins_emu = 4; // bins size-1
    // bins_emu[0] = 0;
    // bins_emu[1] = 25;
    // bins_emu[2] = 35;
    // bins_emu[3] = 50;    
    // bins_emu[4] = 100;
    
    // Double_t bins_ee[4];
    // int numbins_ee = 3; // bins size-1
    // bins_ee[0] = 0;
    // bins_ee[1] = 25;
    // bins_ee[2] = 30;   
    // bins_ee[3] = 100;    
    
    // ..or to just use the same everywhere:
    Double_t bins_mumu[14];
    Double_t bins_emu[14];
    Double_t bins_ee[14];
    int numbins_mumu = numbins;
    int numbins_emu = numbins;
    int numbins_ee = numbins;
    std::copy(std::begin(bins), std::end(bins), std::begin(bins_mumu));
    std::copy(std::begin(bins), std::end(bins), std::begin(bins_emu));
    std::copy(std::begin(bins), std::end(bins), std::begin(bins_ee));
    
    th1ds[i]->SetName("SSeePt_lep1_denom");     th1ds[i]->TH1::SetBins(numbins_ee,bins_ee);    i++;
    th1ds[i]->SetName("SSeePt_lep1_numer");     th1ds[i]->TH1::SetBins(numbins_ee,bins_ee);    i++;
    th1ds[i]->SetName("SSeePt_lep1_effic");     th1ds[i]->TH1::SetBins(numbins_ee,bins_ee);    i++;
    th1ds[i]->SetName("SSeuPt_lep1_denom");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSeuPt_lep1_numer");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSeuPt_lep1_effic");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuePt_lep1_denom");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuePt_lep1_numer");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuePt_lep1_effic");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuuPt_lep1_denom");     th1ds[i]->TH1::SetBins(numbins_mumu,bins_mumu);    i++;
    th1ds[i]->SetName("SSuuPt_lep1_numer");     th1ds[i]->TH1::SetBins(numbins_mumu,bins_mumu);    i++;
    th1ds[i]->SetName("SSuuPt_lep1_effic");     th1ds[i]->TH1::SetBins(numbins_mumu,bins_mumu);    i++;
    th1ds[i]->SetName("SS3lPt_lep1_denom");     th1ds[i]->TH1::SetBins(numbins,bins);    i++;
    th1ds[i]->SetName("SS3lPt_lep1_numer");     th1ds[i]->TH1::SetBins(numbins,bins);    i++;
    th1ds[i]->SetName("SS3lPt_lep1_effic");     th1ds[i]->TH1::SetBins(numbins,bins);    i++;

    th1ds[i]->SetName("SSeePt_lep2_denom");     th1ds[i]->TH1::SetBins(numbins_ee,bins_ee);    i++;
    th1ds[i]->SetName("SSeePt_lep2_numer");     th1ds[i]->TH1::SetBins(numbins_ee,bins_ee);    i++;
    th1ds[i]->SetName("SSeePt_lep2_effic");     th1ds[i]->TH1::SetBins(numbins_ee,bins_ee);    i++;
    th1ds[i]->SetName("SSeuPt_lep2_denom");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSeuPt_lep2_numer");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSeuPt_lep2_effic");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuePt_lep2_denom");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuePt_lep2_numer");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuePt_lep2_effic");     th1ds[i]->TH1::SetBins(numbins_emu,bins_emu);    i++;
    th1ds[i]->SetName("SSuuPt_lep2_denom");     th1ds[i]->TH1::SetBins(numbins_mumu,bins_mumu);    i++;
    th1ds[i]->SetName("SSuuPt_lep2_numer");     th1ds[i]->TH1::SetBins(numbins_mumu,bins_mumu);    i++;
    th1ds[i]->SetName("SSuuPt_lep2_effic");     th1ds[i]->TH1::SetBins(numbins_mumu,bins_mumu);    i++;
    th1ds[i]->SetName("SS3lPt_lep2_denom");     th1ds[i]->TH1::SetBins(numbins,bins);    i++;
    th1ds[i]->SetName("SS3lPt_lep2_numer");     th1ds[i]->TH1::SetBins(numbins,bins);    i++;
    th1ds[i]->SetName("SS3lPt_lep2_effic");     th1ds[i]->TH1::SetBins(numbins,bins);    i++;

    // th1ds[i]->SetName("SSee2DEta_denom");     // maybe add these later
    // th1ds[i]->SetName("SSee2DEta_numer");     
    // th1ds[i]->SetName("SSee2DEta_effic");     
    // th1ds[i]->SetName("SSeu2DEta_denom");     
    // th1ds[i]->SetName("SSeu2DEta_numer");     
    // th1ds[i]->SetName("SSeu2DEta_effic");     
    // th1ds[i]->SetName("SSue2DEta_denom");     
    // th1ds[i]->SetName("SSue2DEta_numer");     
    // th1ds[i]->SetName("SSue2DEta_effic");     
    // th1ds[i]->SetName("SSuu2DEta_denom");     
    // th1ds[i]->SetName("SSuu2DEta_numer");     
    // th1ds[i]->SetName("SSuu2DEta_effic");     
    // th1ds[i]->SetName("SS3l2DEta_denom");     
    // th1ds[i]->SetName("SS3l2DEta_numer");     
    // th1ds[i]->SetName("SS3l2DEta_effic");     

    th1ds[i]->SetName("SSeeEta_lep1_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeeEta_lep1_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeeEta_lep1_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeuEta_lep1_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeuEta_lep1_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeuEta_lep1_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSueEta_lep1_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSueEta_lep1_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSueEta_lep1_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSuuEta_lep1_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSuuEta_lep1_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSuuEta_lep1_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SS3lEta_lep1_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SS3lEta_lep1_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SS3lEta_lep1_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;

    th1ds[i]->SetName("SSeeEta_lep2_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeeEta_lep2_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeeEta_lep2_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeuEta_lep2_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeuEta_lep2_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSeuEta_lep2_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSueEta_lep2_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSueEta_lep2_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSueEta_lep2_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSuuEta_lep2_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSuuEta_lep2_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SSuuEta_lep2_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SS3lEta_lep2_denom");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SS3lEta_lep2_numer");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;
    th1ds[i]->SetName("SS3lEta_lep2_effic");     th1ds[i]->SetBins(12,-3.0,3.0);    i++;

    th1ds[i]->SetName("SS3l_total_effic");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SS3l_total_denom");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SS3l_total_numer");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSuu_total_effic");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSuu_total_denom");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSuu_total_numer");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSeu_total_effic");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSeu_total_denom");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSeu_total_numer");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSee_total_effic");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSee_total_denom");     th1ds[i]->SetBins(1,0,10000);    i++;
    th1ds[i]->SetName("SSee_total_numer");     th1ds[i]->SetBins(1,0,10000);    i++;

    ///////////////////////////////////////////////////////////////////////////////////

    if (debug) cout << "bookhistos oo" << endl;

    // th1ds[12]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var1 sig","",100,0,1);  //0.04
    // th1ds[13]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var2 sig","",100,0,1);  //0.04
    // th1ds[14]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var3 sig","",100,0,1);  //0.04
    // th1ds[15]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var1 bkd","",100,0,1);  //0.04                            
    // th1ds[16]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var2 bkd","",100,0,1);  //0.04
    // th1ds[17]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var3 bkd","",100,0,1);  //0.04
    
    // th1ds[12]->SetNameTitle("test","test");                         th1ds[12]->SetBins(100,0,400);    

    /////////////////////////////////////////////////////////////////////////    
    // Done booking. Populate the dictionaries:
    
    // WHY WAS IT DONE THIS WAY????????????
    for (int k=0; k<(int)th1arrsize; k++) th1d[th1ds[k]->GetName()] = th1ds[k];
    for (int k=0; k<(int)th2arrsize; k++) th2d[th2ds[k]->GetName()] = th2ds[k];
    for (int k=0; k<(int)th1eftarrsize; k++) th1eft[th1efts[k]->GetName()] = th1efts[k];
    
    if (debug) cout << "end of bookhistos" << endl;   
}