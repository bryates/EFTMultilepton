#include "includes.h"

void makeanatestfile() {
    std::vector<int> samples;

    /* Note: all of the samples below need to be located in the same directory! This has the
        potential to make it very difficult to determine what samples were used to produce the
        output file, unless it is excessively named
    */

    // Diboson
    samples.push_back(10);
    samples.push_back(11);
    samples.push_back(12);
    
    // Triboson
    samples.push_back(22);
    samples.push_back(23);
    samples.push_back(24);
    samples.push_back(25); 

    // data-driven backgrounds
    samples.push_back(94); // QFs
    samples.push_back(95); // Fakes

    // convs
    samples.push_back(30);

    // private sgnl
    samples.push_back(85); // ttlnu_multidim
    samples.push_back(86); // ttll_multidim
    samples.push_back(84); // ttH_multidim
    samples.push_back(87); // tllq_multidim
    samples.push_back(88); // tHq_multidim

    // central sgnl
    samples.push_back(8);  // ttW   
    samples.push_back(9);  // ttZ    
    samples.push_back(1);  // ttH  
    samples.push_back(26); // tZq
    samples.push_back(31); // tHq

    // data samples
    samples.push_back(100); // single mu data
    samples.push_back(101); // single ele data
    samples.push_back(102); // double mu data
    samples.push_back(103); // double ele data
    samples.push_back(104); // mu+EG data


    std::cout << "Instantiating MakeGoodPlot" << std::endl;
    //MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/tmpscratch/users/awightma/combinedHists/test/testing_2019_08_07/");
    //MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/tmpscratch/users/awightma/combinedHists/anatest23_2019_08_09_tllq4f-tchannel-plusJets/");
    //MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/tmpscratch/users/awightma/combinedHists/anatest23_2019_08_09_tllq4f-tchannel-NoJets-NoEFT/");
    MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/tmpscratch/users/awightma/analysisWorkflow/mergedHists/mixed_samples/");
    
    std::cout << "Starting drawAllToFile" << std::endl;
    //newplots->drawAllToFile("anatest23","root");
    //newplots->drawAllToFile("dump_name","root");

    /*
        --- anatest24 ---
        Uses data files from '.../mergedHists/2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps',
        as well as the central_tHq sample (which shouldn't be used in the combine fit)
        this includes:
            temp_data, temp_SingleMuon, temp_SingleElectron, temp_QFs, temp_MuonEG, temp_Fakes,
            temp_DoubleMuon, and temp_DoubleEG, temp_tHq
        The MC samples are from '.../mergedHists/2019_10_09_full_MC'
        this includes:
            temp_WWZ, temp_WWW, temp_ZZZ, temp_WZZ, temp_WW, temp_ttGJets, temp_ttH, temp_ttH_multidim
            temp_ttlnu_multidim, temp_WZ, temp_ZZ, temp_ttW, temp_tHq_multidim, temp_tZq, temp_ttZ,
            temp_ttll_multidim, and temp_tllq_multidim
        All of the bkgd and central signal samples were simply reprocessed, the private signal samples
        are all from Round5/Batch{1,2} except for the tllq_multidim sample which came from
        'tllq4f-tch-NoHiggs_0partons_xqcut0' and 'tllq4f-tch-NoHiggs_0partons_xqcut0_extra'. As a result
        of this, the private tllq sample only has ~2.5M total events
        Changes:
            * The tllq sample differs from the previous one in that it has no intermediate Higgses,
            has +0partons, and is t-ch only
            * Includes the JEC AK4PFchs fix (commit/61c772a87d9f0ab1c9d89f9305b4a2da46c3cf9e)
        
        --- anatest25 ---
        Uses data files from '.../mergedHists/2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps',
        as well as the central_tHq sample (which shouldn't be used in the combine fit)
        this includes:
            temp_data, temp_SingleMuon, temp_SingleElectron, temp_QFs, temp_MuonEG, temp_Fakes,
            temp_DoubleMuon, and temp_DoubleEG
        The MC samples are from '.../mergedHists/2019_10_13_full_MC'
        this includes:
            temp_WWZ, temp_WWW, temp_ZZZ, temp_WZZ, temp_WW, temp_ttGJets, temp_ttH, temp_ttH_multidim
            temp_ttlnu_multidim, temp_WZ, temp_ZZ, temp_ttW, temp_tHq_multidim, temp_tZq, temp_ttZ,
            temp_ttll_multidim, and temp_tllq_multidim
        All non-data related samples were remade to include the muR+muF up/down variations to allow
        calculation of bin-by-bin envelope
        Changes:
            * Fixed muR/muF scalefactor systematics to use correct ME weight id
            * Add muR+muF up/down variation (commit/6201381699ee864ccd70c1dcd32302e8330a19e7)
            * Fix to properly identify PDF weight id for non-private samples 
    */
    newplots->drawAllToFile("anatest25","root");

    //delete newplots;
    std::cout << "Finished!" << std::endl;
    return;
}
