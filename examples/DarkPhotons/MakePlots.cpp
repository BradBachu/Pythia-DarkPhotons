#include <iostream>
#include <fstream>
#include <TFile.h>
#include <map>
#include "TCanvas.h"
#include "TColor.h"
#include "TString.h"
#include "TAttFill.h"
#include "TH1D.h"
#include "THStack.h"
#include "TAttMarker.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TString.h"
#include "TMultiGraph.h"

double R_mu(double mA)
{
   if (mA < 1)
   {
      return 1;
   }
   else if ((mA >= 1)&&(mA <4))
   {
      return 2;
   }
   else
   {
      return 3;
   }
}

double N_l(double mA)
{
   if (mA/2 < 1.7)
   {
      return 1;
   }
   else 
   {
      return 2;
   }
}

void MakePred(TH1D* h1, TGraph* g, double epsilon)
{
   double pdgMass[14] = {3.096, // 443
                         3.686, // 100443
                         3.773, // 30443
                         9.460, // 553 
                         1.0023, // 100553
                         1.0355, // 200553 
                         0.135, // 111
                         0.1396, //211
                         0.775, // 113
                         0.775, // 213
                         0.5479, // 221
                         0.958, // 331
                         0.7827, // 223
                         1.019}; // 1.019

   TH1D* h2 = (TH1D*) h1->Clone("h2");
   int nbins = h2->GetNbinsX();
   std::cout<< "nbins = " << nbins << std::endl;
   double xlow = h2->GetXaxis()->GetBinLowEdge(1);
   std::cout<< "xlow = " << xlow << std::endl;
   double width = h2->GetBinWidth(1);
   std::cout<< "width = " << width << std::endl;

   for (int i = 0; i < nbins; ++i)
   {
      double mA = xlow + (width/2) + (width*i)  ; // make mA the center of a bin
      std::cout<< "mA " << mA << std::endl;
      // check if mA is in a bin with a resonance.
      // this happens if the distance to a resonacne is less than a bin width
      bool onResonance = false;
      for (int r = 0; r < 14; ++r)
      {
         if (abs(pdgMass[r] - mA) <= width)
         {
            std::cout<< "near resonance  = "<< pdgMass[r] << std::endl;
            onResonance = true;
         }
      }
      double sigma_m = 4*0.01*mA;
      double b_em = h2->GetBinContent(i) * (4*sigma_m) /width ;
      double pred = 0;
      if (!onResonance)
      {
         pred = (epsilon*epsilon)*3*(3.14159265)*mA/(8 * sigma_m)*(137/(N_l(mA) + R_mu(mA))) * b_em;
         std::cout<< "pred = " << pred << std::endl;
      }
      g->SetPoint(i,mA,pred*pow(10,9)); // convert to pb
   }
   g->GetYaxis()->SetTitle("#sigma(A')#times Br(A'#Rightarrow#mu#mu)#times Acc [pb]");
   g->GetXaxis()->SetTitle("m_{A}[GeV]");
   g->SetTitle(TString("#epsilon = " + std::to_string(epsilon)));
// return g;
}

void MakePlots()
{
   std::map<int,std::string> pdgParticle;
   pdgParticle[443] = "J/#Psi";
   pdgParticle[100443] = "#Psi(2S)";
   pdgParticle[30443] = "#Psi(3770)";

   pdgParticle[553] = "#Upsilon";
   pdgParticle[100553] = "#Upsilon(2S)";
   pdgParticle[200553] = "#Upsilon(3S)";

   pdgParticle[111] = "#pi^{0}";
   pdgParticle[211] = "#pi^{+}";

   pdgParticle[113] = "#rho(770)^{0}";
   pdgParticle[213] = "#rho(770)^{+}";

   pdgParticle[221] = "#eta";
   pdgParticle[331] = "#eta'";

   pdgParticle[223] = "#omega(782)";
   pdgParticle[333] = "#phi(1020)";
   pdgParticle[310] = "K_{S}^{0}";

   double pdgMass[14] = {3.096, // 443
                         3.686, // 100443
                         3.773, // 30443
                         9.460, // 553 
                         1.0023, // 100553
                         1.0355, // 200553 
                         0.135, // 111
                         0.1396, //211
                         0.775, // 113
                         0.775, // 213
                         0.5479, // 221
                         0.958, // 331
                         0.7827, // 223
                         1.019}; // 1.019

   TFile* file = new TFile("~/Dropbox/Research/DarkPhotons/montecarlo/v1/AllHists.root");
   TString shists[14] = {"443","100443","30443","553","100553","200553","111","130","113","213","221","331","223","333"};   
   int pdghists[14] = {443,100443,30443,553,100553,200553,111,310,113,213,221,331,223,333};   
   Color_t chists[14] = {kRed, kRed+1, kRed+2, kBlue, kBlue+1, kBlue+2,kGreen,kBlack,kOrange,kOrange+8,kViolet,kViolet+1,kTeal,kTeal+2};
   THStack* stack = new THStack("hmain", "M_{#mu#mu}");
   TCanvas* c = new TCanvas();
   TLegend *legend = new TLegend(.60, .60, .8,.8);

   TString s_dimuall[6] = {"h1_DiMuMass_CMS_eta24","h1_DiMuMass_CMS_eta15","h1_DiMuMass_CMS_eta24_dR1p4","h1_DiMuMass_CMS_eta15_dR1p4","h1_DiMuMass_LHCb","h1_DiMuMass_ALICE"};
   for (int i = 0; i < 6; ++i)
   {
      TH1* htemp = (TH1*) file->FindObjectAny(s_dimuall[i]);
      // htemp->SetMarkerStyle(20);
      htemp->GetXaxis()->SetTitle("M_{#mu#mu}[GeV]");
      htemp->GetYaxis()->SetTitle("N events");
      // htemp->SetStats(0);
      TCanvas* c = new TCanvas();
      c->SetLogy();
      htemp->Draw();
   }

   TCanvas* c_bem = new TCanvas();
   c_bem->SetLogy();
   c_bem->SetLogx();
   TH1D* h1_BEM_CMS =(TH1D*) ((TH1D*) file->FindObjectAny(shists[1]))->Clone("h1_BEM_CMS");
   h1_BEM_CMS->Reset();
   h1_BEM_CMS->SetTitle("pp#Rightarrow X #gamma* #Rightarrow X #mu^{+}#mu^{-}");
   h1_BEM_CMS->SetMarkerStyle(20);
   h1_BEM_CMS->Draw("ep");
   h1_BEM_CMS->GetXaxis()->SetTitle("#mu^{+}#mu^{-} invariant mass [GeV]");
   h1_BEM_CMS->GetYaxis()->SetTitle("Events/GeV");
   for (int i = 0; i < 14; ++i)
   {
      TH1* htemp = (TH1*) ((TH1*) file->FindObjectAny(shists[i]))->Clone("htemp") ;
      h1_BEM_CMS->Add(htemp);
      htemp->SetLineColor(chists[i]);
      htemp->SetFillColor(chists[i]);
      htemp->SetTitle("M_{#mu#mu}");
      htemp->SetFillColorAlpha(chists[i],0.35);
      htemp->SetFillStyle(3001);
      htemp->SetStats(0);
      htemp->Draw("same");
      legend->AddEntry(htemp,TString(pdgParticle[pdghists[i]]),"lpf");
   }
   // stack->Draw("nostack");
   legend->Draw("same");

   // now normalize everything to 1
   TCanvas* c_normalize = new TCanvas();
   c_normalize->SetLogy();
   c_normalize->SetLogx();
   TH1D* h1_BEM_CMS_norm =(TH1D*) h1_BEM_CMS->Clone("h1_BEM_CMS_norm");
   int n_bem = h1_BEM_CMS->Integral(1,h1_BEM_CMS->GetNbinsX()+1   );
   std::cout << "n_bem = " << n_bem << std::endl;
   std::cout << "n_entries = " << h1_BEM_CMS->GetEntries() << std::endl;
   // normalize everything to xs/nentries
   double pythia_xs = 1.181;  // mb
   h1_BEM_CMS_norm->Scale(pythia_xs/n_bem);
   h1_BEM_CMS_norm->GetYaxis()->SetTitle("#sigma #times Br #times Acceptance /GeV [mb]");
   h1_BEM_CMS_norm->Draw();
   for (int i = 0; i < 14; ++i)
   {
      TH1* htemp1 = (TH1*) ((TH1*) file->FindObjectAny(shists[i]))->Clone("htemp1") ;
      // TH1* htemp = (TH1*) file->FindObjectAny(shists[i]) ;
      htemp1->SetLineColor(chists[i]);
      htemp1->SetFillColor(chists[i]);
      // htemp->SetTitle("M_{#mu#mu}");
      htemp1->SetFillStyle(3001);    
      htemp1->SetStats(0);
      htemp1->Scale(pythia_xs/n_bem);
      // htemp1->SetFillColorAlpha(chists[i],0.35);
      htemp1->Draw("same");
   }
   legend->Draw("same");

   // TString s_epsilon[4] = {"1","10e-2","10e-3","10e-3.5"};
   double epsilon[4] = {1,pow(10,-2),pow(10,-3),pow(10,-3.5)};
   int nbins = h1_BEM_CMS_norm->GetNbinsX()+1;
   TGraph* g1 = new TGraph(nbins);
   TGraph* g2 = new TGraph(nbins);
   TGraph* g3 = new TGraph(nbins);
   TGraph* g4 = new TGraph(nbins);
   TGraph* g[4] = {g1,g2,g3,g4};

   TCanvas* c_pred_all = new TCanvas();
   // g1->Draw("A");
   TMultiGraph *mg = new TMultiGraph();
   mg->GetYaxis()->SetTitle("#sigma(A')#times Br(A'#Rightarrow#mu#mu)#times Acc [pb]");
   mg->GetXaxis()->SetTitle("m(A) [GeV]");
   for (int i = 0; i < 4; ++i)
   {
      MakePred(h1_BEM_CMS_norm,g[i],epsilon[i]);
      g[i]->SetMarkerStyle(20);
      g[i]->SetMarkerColor(chists[i]);
      mg->Add(g[i]);
      //c->SetLogy();
      //c->SetLogx();
   }
   c_pred_all->SetLogy();
   c_pred_all->SetLogx();
   mg->SetMinimum(10);
   mg->Draw("ap");
   c_pred_all->Update();
}