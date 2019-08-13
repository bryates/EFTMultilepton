#include "includes.h"

void makeanatestfile() {
    std::vector<int> samples;

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

    std::cout << "Starting drawAllToFile" << std::endl;

    MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/tmpscratch/users/awightma/combinedHists/test/testing_2019_08_07/");
    //MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/tmpscratch/users/awightma/combinedHists/anatest23_2019_08_09_tllq4f-tchannel-plusJets/");
    //MakeGoodPlot *newplots = new MakeGoodPlot(samples,"/tmpscratch/users/awightma/combinedHists/anatest23_2019_08_09_tllq4f-tchannel-NoJets-NoEFT/");
    //newplots->drawAllToFile("anatest23","root");
    newplots->drawAllToFile("anatest23_new-ttll","root");

    //delete newplots;
    std::cout << "Finished!" << std::endl;
    return;
}
