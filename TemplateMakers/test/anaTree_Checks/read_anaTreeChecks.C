#include "utils.h"
#include "categories.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// std::vector<Analysis::Category> cats_2los = Analysis::getLeptonChildCategories(Lepton::TwoLepOS);
// std::vector<Analysis::Category> cats_2lss = Analysis::getLeptonChildCategories(Lepton::TwoLepSS);
// std::vector<Analysis::Category> cats_3lep = Analysis::getLeptonChildCategories(Lepton::ThreeLep);
// std::vector<Analysis::Category> cats_all;
// cats_all.insert(cats_all.end(),cats_2los.begin(),cats_2los.end());
// cats_all.insert(cats_all.end(),cats_2lss.begin(),cats_2lss.end());
// cats_all.insert(cats_all.end(),cats_3lep.begin(),cats_3lep.end());
////////////////////////////////////////////////////////////////////////////////////////////////

// I believe this is the correct way to normalize over a sum of histogram bins
void norm1(TH1EFT* H, TH1EFT* G, double norm) {
// void norm1(TH1D* H, TH1D* G, double norm) {
    double g_sum = G->Integral();
    double g_err = 0.0;
    for (Int_t i = 1; i <= G->GetNbinsX(); i++) {
        double bin_err = G->GetBinError(i);
        g_err += bin_err*bin_err;
    }
    g_err = sqrt(g_err);

    double h_sum = H->Integral();
    double h_err = 0.0;
    for (Int_t i = 1; i <= H->GetNbinsX(); i++) {
        double bin_err = H->GetBinError(i);
        h_err += bin_err*bin_err;
    }
    h_err = sqrt(h_err);

    double f_sum = norm*h_sum / g_sum;
    double f_t1 = h_err*g_sum;
    double f_t2 = g_err*h_sum;
    double f_err = norm*sqrt(f_t1*f_t1 + f_t2*f_t2) / (g_sum*g_sum);

    TString sum_str = TString::Format("%+.2f +/- %+.2f",f_sum,f_err);
    std::cout << "norm1: " << sum_str << std::endl;
}

// This is incorrect
void norm2(TH1EFT* H, TH1EFT* G, double norm) {
// void norm2(TH1D* H, TH1D* G, double norm) {
    double g_sum = G->Integral();
    double g_err = 0.0;
    for (Int_t i = 1; i <= G->GetNbinsX(); i++) {
        double bin_err = G->GetBinContent(i);
        g_err += bin_err*bin_err;
    }
    g_err = sqrt(g_err);

    double f_sum = 0.0;
    double f_err = 0.0;
    for (Int_t i = 1; i <= H->GetNbinsX(); i++) {
        double bin_cont = H->GetBinContent(i);
        double bin_err = H->GetBinError(i);
        // bin_err = bin_err*bin_err;

        double e1 = bin_err*g_sum;
        double e2 = g_err*bin_cont;

        double h_cont = bin_cont / g_sum;
        double h_err = sqrt(e1*e1 + e2*e2) / (g_sum*g_sum);

        // H->SetBinContent(i,h_cont);
        // H->SetBinError(i,h_err);

        f_sum += h_cont;
        f_err += h_err*h_err;
    }
    f_sum = norm*f_sum;
    f_err = norm*sqrt(f_err);

    TString sum_str = TString::Format("%+.2f +/- %+.2f",f_sum,f_err);
    std::cout << "norm2: " << sum_str << std::endl;
}

// Gets a histogram from a file and adds it to the hists vector
void getEFTHistogram(TFile* f, TString name, double norm, std::vector<TH1EFT*> & hists) {
    TString htitle = f->GetName();
    int str_idx = htitle.Index(".root");
    htitle = htitle(0,str_idx);

    TH1EFT* h = (TH1EFT*)f->Get(name);
    if (!h) {
        return;
    }
    h->Scale(norm);
    h->ScaleFits(norm);
    h->SetTitle(htitle);
    hists.push_back(h);
    return;
}

// Note: Don't normally normalize the TH1D histograms
void getTH1DHistogram(TFile* f, TString name, double norm, bool make_shape, std::vector<TH1D*> & hists) {
    TString htitle = f->GetName();
    int str_idx = htitle.Index(".root");
    htitle = htitle(0,str_idx);
    TH1D* h = (TH1D*)f->Get(name);
    if (!h) {
        return;
    }
    // We might not always want to turn these histograms into shape only ones
    if (make_shape) {    
        double intg = h->Integral();
        h->Scale(1./intg);
    }
    h->SetTitle(htitle);
    hists.push_back(h);
    return;
}

// Returns a histogram that is the ratio of h1/h2
template<typename T>
TH1D* makeRatioHistogram(TString name,T* h1,T* h2) {
    if (h1->GetXaxis()->GetNbins() != h2->GetXaxis()->GetNbins()) {
        std::cout << "[Error] makeRatioHistogram() - bin mismatch between ratio of hists" << std::endl;
        throw;
    }
    TAxis* xaxis = h1->GetXaxis();
    int bins = xaxis->GetNbins();
    Double_t low = xaxis->GetXmin();
    Double_t high = xaxis->GetXmax();

    TAxis* yaxis = h1->GetYaxis();
    Double_t yaxis_sz = yaxis->GetLabelSize();

    // std::cout << "Ratio Name: " << name << std::endl;
    TH1D* h_ratio = new TH1D(name,"",bins,low,high);    // Make sure the title is empty!
    h_ratio->GetYaxis()->SetLabelSize(yaxis_sz*1.5);
    h_ratio->GetXaxis()->SetLabelSize(yaxis_sz*2.5);    // Note: If this is done after alphanumeric, it does nothing
    for (int i = 1; i <= bins; i++) {
        if (xaxis->IsAlphanumeric()) {
            TString bin_label = xaxis->GetBinLabel(i);
            h_ratio->GetXaxis()->SetBinLabel(i,bin_label);
        }
        double val1 = h1->GetBinContent(i);
        double err1 = h1->GetBinError(i);

        double val2 = h2->GetBinContent(i);
        double err2 = h2->GetBinError(i);

        double ratio;
        double ratio_err;
        if (val2 != 0) {
            ratio = abs(val1 / val2);
        } else if (val1 == 0 && val2 == 0) {
            ratio = 1.0;
        } else {
            ratio = -1;
        }
        ratio_err = 0.0;    // Ignore the error for now
        h_ratio->SetBinContent(i,ratio);
        h_ratio->SetBinError(i,ratio_err);
    }
    h_ratio->GetYaxis()->SetRangeUser(0.0,2.3);
    return h_ratio;
}

//TODO: Add optional ratio inset
template<typename T>
void makePlot(std::vector<T*> hists,TString name,int ratio_idx = -1) {
    std::vector<int> clrs {
        kBlack,
        kBlue,
        kRed+1,
        kGreen+1,
        kMagenta,
        kCyan,
        kPink+1,
        kSpring+1,
        kAzure-9,
    };
    bool incl_ratio = (ratio_idx >= 0);

    double ymax = 0.0;
    for (uint i=0; i <hists.size(); i++) {
        T* h = hists.at(i);
        ymax = std::max(ymax,h->GetMaximum());
    }

    TCanvas* canv = new TCanvas(name,name,1300,1000);
    Float_t small = 1.e-5;
    const float padding = 1e-5;
    if (incl_ratio) {
        const float ydiv = 0.3;
        canv->Divide(1,2,small,small);
        canv->GetPad(1)->SetPad(padding,ydiv+padding,1-padding,1-padding);
        canv->GetPad(1)->SetLeftMargin(.11);
        canv->GetPad(1)->SetRightMargin(.05);
        canv->GetPad(1)->SetBottomMargin(.3);
        canv->GetPad(1)->Modified();

        canv->GetPad(2)->SetPad(padding,padding,1-padding,ydiv-padding);
        canv->GetPad(2)->SetLeftMargin(.11);
        canv->GetPad(2)->SetRightMargin(.05);
        canv->GetPad(2)->SetBottomMargin(.3);
        canv->GetPad(2)->Modified();

        canv->cd(1);
        gPad->SetBottomMargin(small);
        gPad->Modified();

        canv->cd(2);
        gPad->SetTopMargin(small);
        gPad->SetTickx();
        gPad->Modified();
    } else {
        canv->Divide(1,1,small,small);
        //canv->GetPad(1)->SetPad(xlow,ylow,xup,yup);
        canv->GetPad(1)->SetPad(padding,padding,1-padding,1-padding);
        canv->GetPad(1)->SetLeftMargin(.11);
        canv->GetPad(1)->SetRightMargin(.05);
        //canv->GetPad(1)->SetBottomMargin(.3);
        canv->GetPad(1)->SetBottomMargin(.1);
        canv->GetPad(1)->Modified();

        canv->cd(1);
        gPad->Modified();
    }

    TLegend *leg = new TLegend(0.14,0.75,0.94,0.89);
    leg->SetFillColor(kWhite);
    leg->SetLineColor(kWhite);
    leg->SetShadowColor(kWhite);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    leg->SetNColumns(4);

    for (uint i=0; i < hists.size(); i++) {
        T* h = hists.at(i);
        int clr_idx = (i % clrs.size());
        h->SetLineColor(clrs.at(clr_idx));
        h->GetYaxis()->SetRangeUser(0.0,1.25*ymax);

        leg->AddEntry(h,h->GetTitle(),"l");

        // Note: This creates a memory leak!
        TH1D* h_ratio;
        if (incl_ratio) {
            TString ratio_name = "r_" + name + "_" + h->GetTitle();
            h_ratio = makeRatioHistogram(ratio_name,h,hists.at(ratio_idx));
            h_ratio->SetLineColor(clrs.at(clr_idx));
        }

        canv->cd(1);

        if (i == 0) {
            h->SetTitle(name);
            h->Draw("e hist");
        } else {
            h->Draw("e hist same");
        }

        if (incl_ratio) {
            canv->cd(2);
            if (i == 0) {
                h_ratio->Draw("e2");
            } else {
                h_ratio->Draw("e2 same");
            }
            canv->GetPad(1)->RedrawAxis();
            canv->GetPad(2)->RedrawAxis();
        } else {
            canv->GetPad(1)->RedrawAxis();
        }
    }
    canv->cd(1);
    leg->Draw();

    TString outf = name + ".png";
    canv->Print(outf,"png");

    delete leg;
    delete canv;
}

void runit(std::vector<TFile*> files, double sm_xsec) {
    WCPoint* sm_pt = new WCPoint("sm",0.0);
    double lumi2017 = 41530.;
    double hadFilter_factor = 1.0;
    // double hadFilter_factor = 0.823;
    // double hadFilter_factor = 0.722;

    std::vector<TH1EFT*> njet_incl_hists;
    std::vector<TH1EFT*> njet_sr_hists;
    std::vector<TH1EFT*> yield_hists;
    std::vector<TH1EFT*> invmass_hists;
    std::vector<TH1EFT*> invmass_eles_hists;
    std::vector<TH1EFT*> invmass_muons_hists;
    std::vector<TH1EFT*> invmass_taus_hists;
    std::vector<TH1EFT*> nleps_hists;
    std::vector<TH1EFT*> neles_hists;
    std::vector<TH1EFT*> nmuons_hists;
    std::vector<TH1EFT*> ntaus_hists;
    std::vector<TH1EFT*> lep1_pt_hists;
    std::vector<TH1EFT*> lep1_eta_hists;
    std::vector<TH1EFT*> lep1_mva_hists;
    std::vector<TH1EFT*> lep2_pt_hists;
    std::vector<TH1EFT*> lep2_eta_hists;
    std::vector<TH1EFT*> lep2_mva_hists;
    std::vector<TH1EFT*> jet1_pt_hists;
    std::vector<TH1EFT*> jet1_eta_hists;
    std::vector<TH1EFT*> jet2_pt_hists;
    std::vector<TH1EFT*> jet2_eta_hists;

    std::vector<TH1D*> muRUp_hists;
    std::vector<TH1D*> muRDown_hists;
    std::vector<TH1D*> muFUp_hists;
    std::vector<TH1D*> muFDown_hists;
    std::vector<TH1D*> muRmuFUp_hists;
    std::vector<TH1D*> muRmuFDown_hists;
    std::vector<TH1D*> nnpdfUp_hists;
    std::vector<TH1D*> nnpdfDown_hists;

    for (uint i=0; i < files.size(); i++) {
        TFile* f = files.at(i);

        TH1EFT* h_norm = (TH1EFT*)f->Get("h_sumSM_incl");
        TH1EFT* h_cats = (TH1EFT*)f->Get("h_all_cats");
        TH1D* h_wgts = (TH1D*)f->Get("h_smwgts_incl");

        double event_norm = h_norm->Integral();
        double norm = lumi2017*sm_xsec / event_norm;
        if (i != 0) {
            // Ad-hoc way of not applying the hadFilter norm to the central sample
            norm *= hadFilter_factor;
        }

        // double h_sum = 0.0;
        // double h_err = 0.0;
        // for (Int_t bin_idx = 1; bin_idx <= h_cats->GetNbinsX(); bin_idx++) {
        //     double bin_val = h_cats->GetBinFit(bin_idx).evalPoint(sm_pt);
        //     double bin_err = h_cats->GetBinError(bin_idx);
        //     // double bin_val = h_cats->GetBinContent(bin_idx);
        //     // double bin_err = h_cats->GetBinError(bin_idx);
        //     h_sum += bin_val;
        //     h_err += bin_err*bin_err;
        // }
        // h_sum = norm*h_sum;
        // h_err = norm*sqrt(h_err);
        // TString sum_str = TString::Format("%+.2f +/- %+.2f",h_sum,h_err);
        // std::cout << "Baseline: " << sum_str << std::endl;
        // norm1(h_cats,h_norm,lumi2017*sm_xsec);
        // norm2(h_cats,h_norm,lumi2017*sm_xsec);

        getEFTHistogram(f,"h_all_cats"  ,norm,yield_hists);
        getEFTHistogram(f,"h_njets_sr"  ,norm,njet_sr_hists);
        getEFTHistogram(f,"h_njets_incl",norm,njet_incl_hists);
        getEFTHistogram(f,"h_invmass_dilep_incl" ,norm,invmass_hists);
        getEFTHistogram(f,"h_invmass_diele_incl" ,norm,invmass_eles_hists);
        getEFTHistogram(f,"h_invmass_dimuon_incl",norm,invmass_muons_hists);
        getEFTHistogram(f,"h_invmass_ditau_incl" ,norm,invmass_taus_hists);
        getEFTHistogram(f,"h_nleps_incl"  ,norm,nleps_hists);
        getEFTHistogram(f,"h_neles_incl"  ,norm,neles_hists);
        getEFTHistogram(f,"h_nmuons_incl" ,norm,nmuons_hists);
        getEFTHistogram(f,"h_ntaus_incl"  ,norm,ntaus_hists);
        
        getEFTHistogram(f,"h_lep1_pt_incl" ,norm,lep1_pt_hists);
        getEFTHistogram(f,"h_lep1_eta_incl",norm,lep1_eta_hists);
        getEFTHistogram(f,"h_lep1_mva_incl",norm,lep1_mva_hists);
        getEFTHistogram(f,"h_lep2_pt_incl" ,norm,lep2_pt_hists);
        getEFTHistogram(f,"h_lep2_eta_incl",norm,lep2_eta_hists);
        getEFTHistogram(f,"h_lep2_mva_incl",norm,lep2_mva_hists);
        
        getEFTHistogram(f,"h_jet1_pt_incl" ,norm,jet1_pt_hists);
        getEFTHistogram(f,"h_jet1_eta_incl",norm,jet1_eta_hists);
        getEFTHistogram(f,"h_jet2_pt_incl" ,norm,jet2_pt_hists);
        getEFTHistogram(f,"h_jet2_eta_incl",norm,jet2_eta_hists);

        bool make_shape = true;
        getTH1DHistogram(f,"h_muRUp"     ,1.0,make_shape,muRUp_hists);
        getTH1DHistogram(f,"h_muRDown"   ,1.0,make_shape,muRDown_hists);
        getTH1DHistogram(f,"h_muFUp"     ,1.0,make_shape,muFUp_hists);
        getTH1DHistogram(f,"h_muFDown"   ,1.0,make_shape,muFDown_hists);
        getTH1DHistogram(f,"h_muRmuFUp"  ,1.0,make_shape,muRmuFUp_hists);
        getTH1DHistogram(f,"h_muRmuFDown",1.0,make_shape,muRmuFDown_hists);
        getTH1DHistogram(f,"h_nnpdfUp"   ,1.0,make_shape,nnpdfUp_hists);
        getTH1DHistogram(f,"h_nnpdfDown" ,1.0,make_shape,nnpdfDown_hists);

        bool incl_header = (i == 0);
        TString row_name = f->GetName();
        int str_idx = row_name.Index(".root");
        row_name = row_name(0,str_idx);
        print_table(h_cats,row_name,-1,incl_header);
    }

    // makePlot(njet_sr_hists,"njets_sr",0);
    // makePlot(njet_incl_hists,"njets_incl",0);
    // makePlot(invmass_hists,"invmass_all",0);
    // makePlot(invmass_eles_hists,"invmass_eles",0);
    // makePlot(invmass_muons_hists,"invmass_muons",0);
    // makePlot(invmass_taus_hists,"invmass_taus",0);
    // makePlot(yield_hists,"yields",0);
    // makePlot(nleps_hists,"nleps",0);
    // makePlot(neles_hists,"neles",0);
    // makePlot(nmuons_hists,"nmuons",0);
    // makePlot(ntaus_hists,"ntaus",0);
    // makePlot(lep1_pt_hists,"lep1_pt",0);
    // makePlot(lep2_pt_hists,"lep2_pt",0);
    // makePlot(jet1_pt_hists,"jet1_pt",0);
    // makePlot(jet2_pt_hists,"jet2_pt",0);

    // makePlot(muRUp_hists     ,"muRUp",-1);
    // makePlot(muRDown_hists   ,"muRDown",-1);
    // makePlot(muFUp_hists     ,"muFUp",-1);
    // makePlot(muFDown_hists   ,"muFDown",-1);
    // makePlot(muRmuFUp_hists  ,"muRmuFUp",-1);
    // makePlot(muRmuFDown_hists,"muRmuFDown",-1);
    // makePlot(nnpdfUp_hists   ,"nnpdfUp",-1);
    // makePlot(nnpdfDown_hists ,"nnpdfDown",-1);
}

// Note: fname should point to a root file that has already been fully merged (i.e. with hadd)
void read_anaTreeChecks(std::vector<TString> fpaths, double sm_xsec) {
    gStyle->SetOptStat(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);

    std::vector<TFile*> files;
    for (TString fpath: fpaths) {
        TFile* f = TFile::Open(fpath);
        files.push_back(f);
    }

    runit(files,sm_xsec);

    for (TFile* f: files) f->Close();
}