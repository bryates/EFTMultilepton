#include "includes.h"

// void makeanatestfile() {
void makeanatestfile(TString out_name,TString plot_option) {
    std::vector<int> samples;

    /* Note: all of the samples below need to be located in the same directory! This has the
        potential to make it very difficult to determine what samples were used to produce the
        output file, unless it is excessively named

       plot_option can be one of: root, png, or pdf
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
    // samples.push_back(31); // tHq

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
    newplots->drawAllToFile(out_name.Data(),plot_option.Data());

    // Will be saved under: ~awightma/www/eft_stuff/misc/geoff_plots
    // std::cout << "Starting drawAllToWebArea" << std::endl;
    // out_name += "/";
    // newplots->drawAllToWebArea(out_name.Data(),plot_option.Data());

    //delete newplots;
    std::cout << "Finished!" << std::endl;
    return;
}
