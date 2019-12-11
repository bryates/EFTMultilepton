#include "EFTMultilepton/TemplateMakers/interface/WCPoint.h"
#include "EFTMultilepton/TemplateMakers/interface/WCFit.h"
#include "EFTMultilepton/TemplateMakers/interface/TH1EFT.h"
#include "EFTMultilepton/TemplateMakers/interface/Stopwatch.h"

std::vector<double> COEFFS {
    1.0,    // c0 --> (sm,sm)
    6.0,    // c1 --> ( x, x)
    3.0,    // c2 --> ( y, y)
    5.0,    // c3 --> ( x, y)
    4.0,    // c4 --> ( x,sm)
    2.0,    // c5 --> ( y,sm)
};

std::vector<std::pair<std::string,std::string>> PAIRS {
    {"sm","sm"},
    {"x","x"},
    {"y","y"},
    {"x","y"},
    {"x","sm"},
    {"y","sm"},
};

std::string getWgtStr(double x, double y) {
    TString rwgt_str = TString::Format("rwgt_%s_%.9f_%s_%.9f","x",x,"y",y);
    return rwgt_str.Data();
}

double getWgt(double x, double y) {
    double c0 = COEFFS.at(0);
    double c1 = COEFFS.at(1);
    double c2 = COEFFS.at(2);
    double c3 = COEFFS.at(3);
    double c4 = COEFFS.at(4);
    double c5 = COEFFS.at(5);

    return c0 + c1*x*x + c2*y*y + c3*x*y + c4*x + c5*y;
}

WCPoint getPoint(double x, double y) {
    std::string str = getWgtStr(x,y);
    double wgt = getWgt(x,y);
    WCPoint wcpt(str,wgt);
    return wcpt;
}

WCFit getFit(uint npts) {
    std::vector<WCPoint> pts;
    for (uint j = 0; j < npts; j++) {
        double x = gRandom->Uniform(-3.0,3.0);
        double y = gRandom->Uniform(-3.0,3.0);
        WCPoint wcpt = getPoint(x,y);
        pts.push_back(wcpt);
    }
    WCFit fit(pts,"");
    return fit;
}

double getAvgCoeffError(WCFit & fit) {
    double avg_err = 0.0;
    for (uint i = 0; i < COEFFS.size(); i++) {
        double real_coeff = COEFFS.at(i);
        std::string n1 = PAIRS.at(i).first;
        std::string n2 = PAIRS.at(i).second;
        double fit_coeff = fit.getCoefficient(n1,n2);
        avg_err += fabs(real_coeff - fit_coeff);
    }
    avg_err *= 1.0 / COEFFS.size();
    return avg_err;
}

double getAvgResidue(WCFit & fit) {
    uint npts = 500;
    double avg_res = 0.0;
    for (uint i = 0; i < npts; i++) {
        double x = gRandom->Uniform(-3.0,3.0);
        double y = gRandom->Uniform(-3.0,3.0);
        WCPoint wcpt = getPoint(x,y);
        avg_res += fabs(wcpt.wgt - fit.evalPoint(&wcpt));
    }
    avg_res *= 1.0 / npts;
    return avg_res;
}

std::string getCoeffErrorStr(WCFit & fit) {
    double avg_err = getAvgCoeffError(fit);
    TString s = TString::Format("Avg err=%.5f",avg_err);
    return s.Data();
}

std::string getResidueStr(WCFit & fit) {
    double avg_res = getAvgResidue(fit);
    TString s = TString::Format("Avg res=%.5f",avg_res);
    return s.Data();
}

bool test_pts(uint npts) {
    std::vector<WCPoint> pts;
    for (uint i = 0; i < npts; i++) {
        double x = gRandom->Uniform(-2.0,2.0);
        double y = gRandom->Uniform(-2.0,2.0);
        WCPoint wcpt = getPoint(x,y);
        pts.push_back(wcpt);

        std::cout << TString::Format("(%+.3f,%+.3f) --> %.3f",x,y,wcpt.wgt) << std::endl;
    }
    std::cout << std::endl;
    for (uint i = 0; i < npts; i++) {
        pts.at(i).dump("pts",i);
    }
    
    return true;
}

// Tests how well the WCFit gets to the true quadratic coeffs
bool test_npts_fit(uint npts) {
    std::cout << "Begin npoint fitting unit test..." << std::endl;
    uint nevts = 5;
    TString info_str = TString::Format("nevts: %d, npts: %d",nevts,npts);
    std::cout << info_str << std::endl;
    for (uint i = 0; i < nevts; i++) {
        WCFit wc_fit = getFit(npts);
        // wc_fit.setTag(getCoeffErrorStr(wc_fit));
        wc_fit.setTag(getResidueStr(wc_fit));
        wc_fit.dump(i);
    }
    return true;
}

// Tests the effect of averaging over the sum of many identical WCFits
bool test_avg_sum_fit(uint nevts) {
    std::cout << "Begin fit summing unit test..." << std::endl;
    TString info_str;
    double avg_err;
    double avg_res;
    WCFit sum_fit;

    uint npts = 10;

    info_str = TString::Format("nevts: %d, npts: %d",nevts,npts);
    std::cout << info_str << std::endl;
    for (uint i = 0; i < nevts; i++) {
        WCFit wc_fit = getFit(npts);
        sum_fit.addFit(wc_fit);
        // wc_fit.setTag(getCoeffErrorStr(wc_fit));
        wc_fit.setTag(getResidueStr(wc_fit));
        wc_fit.dump(i);
    }
    std::cout << std::endl;
    sum_fit.scale(1.0 / nevts);
    // sum_fit.setTag(getCoeffErrorStr(sum_fit));
    sum_fit.setTag(getResidueStr(sum_fit));
    sum_fit.dump();

    return true;
}

// Test the filling of a TH1EFT histogram
bool test_hist_filling(uint nevts) {
    std::cout << "Begin hist filling unit test..." << std::endl;
    uint npts = 10;
    int nbins = 4;
    TH1EFT* h1 = new TH1EFT("","",nbins,0,nbins);
    for (uint i = 0; i < nevts; i++) {
        WCFit fit = getFit(npts);
        double x = (i % nbins) + 0.5;
        h1->Fill(x,1.0,fit);
    }
    double norm = 1.0 / nevts;
    h1->ScaleFits(norm);
    h1->DumpFits();

    delete h1;

    return true;
}

bool test_hist_adding(uint nevts) {
    std::cout << "Begin hist adding unit test..." << std::endl;
    uint npts = 10;
    uint nhists = 3;
    int nbins = 1;
    TH1EFT* hsum = new TH1EFT("hsum","hsum",nbins,0,nbins);
    std::vector<TH1EFT*> hists;
    for (uint i = 0; i < nhists; i++) {
        TString hname = TString::Format("h%d",i);
        TH1EFT* h = new TH1EFT(hname,hname,nbins,0,nbins);
        hists.push_back(h);
    }

    for (uint i = 0; i < nevts; i++) {
        WCFit fit = getFit(npts);
        uint hidx = (i % nhists);
        hists.at(hidx)->Fill(0.5,1.0,fit);
    }

    double norm = 1.0 / nevts;
    for (TH1EFT* h: hists) {
        h->ScaleFits(norm);
        hsum->Add(h,1.0);
        h->DumpFits();
    }
    hsum->DumpFits();

    for (uint i = 0; i < nhists; i++) {
        delete hists.at(i);
    }
    delete hsum;
    return true;
}

bool test_hist_cloning(uint nevts) {
    std::cout << "Begin hist cloning unit test..." << std::endl;
    std::string indent = "          ";
    uint npts = 10;
    uint nhists = 10;
    int nbins = 1;
    std::vector<TH1EFT*> hists;
    for (uint i = 0; i < nhists; i++) {
        TString hname = TString::Format("h%d",i);
        TH1EFT* h = new TH1EFT(hname,hname,nbins,0,nbins);
        hists.push_back(h);
    }
    for (uint i = 0; i < nevts; i++) {
        WCFit fit = getFit(npts);
        uint hidx = (i % nhists);
        hists.at(hidx)->Fill(0.5,1.0,fit);
    }
    double norm = 1.0 / nevts;
    for (TH1EFT* h: hists) h->ScaleFits(norm);
    TH1EFT* hsum = (TH1EFT*)hists.at(0)->Clone("hsum");
    for (uint i = 1; i < hists.size(); i++) {
        TH1EFT* h = hists.at(i);
        hsum->Add(h,1.0);
        h->DumpFits();
    }
    std::cout << indent << "------------------------------------------" << std::endl;
    hsum->DumpFits();
    for (uint i = 0; i < nhists; i++) {
        delete hists.at(i);
    }
    delete hsum;
    return true;
}

void test_th1eft() {
    gRandom->SetSeed();

    test_npts_fit(25);
    test_avg_sum_fit(15);
    test_hist_filling(40);
    test_hist_adding(40);
    test_hist_cloning(50);
}