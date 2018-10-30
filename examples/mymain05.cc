// Created by Brad Bachu 06/28/18
// Generation of di-muon events at the LHC

// Stdlib header file for input and output.
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"
#include "TH2.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TAxis.h"

// ROOT for stacking
#include "THStack.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TCanvas.h"
#include <ROOT/TDataFrame.hxx>

// ROOT Colors
#include "TAttFill.h"
#include "TColor.h"

// DarkPhoton Analysis
#include "DarkPhotons/MuonCounter.h"
#include "DarkPhotons/MuonPt.h"
#include "DarkPhotons/MuonPtEta.h"
#include "DarkPhotons/LeadingMuonPtEta.h"
#include "DarkPhotons/DiMuonMass.h"
#include "DarkPhotons/LHCbDiMuMass.h"
#include "DarkPhotons/DiMu2LpTOSMass.h"

// DarkPhoton Triggers
#include "DarkPhotons/L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4.h"
#include "DarkPhotons/LeadingMu_OS_Good.h"
#include "DarkPhotons/ALICE_DoubleMu.h"
#include "DarkPhotons/LHCb_DoubleMu.h"


using namespace Pythia8; // Let Pythia8:: be implicit.
using namespace DarkPhotons; // Let DarkPhotons be implicit

void DrawHists(TH1D* h1, TH1D* h2, TH1D* h3, TH1D* h4)
{
   TCanvas* c = new TCanvas();
   h1->SetLineColor(kBlue);
   h2->SetLineColor(kRed);
   h3->SetLineColor(kGreen);
   h4->SetLineColor(kBlack);

   h1->Draw();
   h2->Draw("same");
   h3->Draw("same");
   h4->Draw("same");

   h1->Write();
   h2->Write();
   h3->Write();
   h4->Write();
   c->Write();
}


void SortMothers(DiMu2LpTOSMass dimuonmass, TH1D* hSameMother[],TH1D* hSameMother_var[], int size)
{
   // if the mothers are different fill in hDiffMother
   std::cout<<"Leading Mother PDG = " << dimuonmass.getLeadingMotherPDG() <<std::endl;
   std::cout<<"Second Leading Mother PDG = " << dimuonmass.getSLeadingMotherPDG() <<std::endl;
   bool matched = false;
   if (dimuonmass.getLeadingMotherPDG() != dimuonmass.getSLeadingMotherPDG() )
   {
      return;
   }
   // they have the same mother and we have the ID
   for (int i = 0; i < size; ++i)
   {
      if ((!matched)&&(std::to_string(dimuonmass.getLeadingMotherPDG()) == hSameMother[i]->GetName()))
      {
         std::cout<<"Matched to known mother" << std::endl;
         dimuonmass.fillBEMMass(hSameMother[i]);
         dimuonmass.fillBEMMass(hSameMother_var[i]);
         return;
      }
   }
}

void OppSignSpectrum(TH1D* hSameMother[], int size, TH1D* hDiffMother , TH1D* hSameMotherExtra)
{

   hSameMotherExtra->SetFillColor(kGray);

   TCanvas* c = new TCanvas();
   THStack* hs = new THStack("hOSMuonSpectrum", "M_{#mu#mu}");
   TLegend *legend = new TLegend(.60, .60, .8,.8);

   for (int i = 0; i < size; ++i)
  {
     hs->Add(hSameMother[i]);
     legend->AddEntry(hSameMother[i], hSameMother[i]->GetName());
     hSameMother[i]->Write();
  }  

   legend->AddEntry(hDiffMother, "Different Mothers");
   legend->AddEntry(hSameMotherExtra, "Same Mothers Other PDG");

   hs->Add(hDiffMother);
   hs->Add(hSameMotherExtra);
   hs->Draw();
   legend->Draw("same");

   hs->Write();
   c->Write();
}

void FillBEM(TH1D* h1_bem, DiMuonMass diMuAnalysis)
{
   if ((diMuAnalysis.getLeadingMuMotherPDG()!=0.)&&(diMuAnalysis.getLeadingMuMotherPDG() == diMuAnalysis.getLeadingMuBarMotherPDG()))
   {
      h1_bem->Fill(diMuAnalysis.getDiMuMass());
      std::cout<<"BEM MASS = " << diMuAnalysis.getDiMuMass() << std::endl;
   }
}

void RapidityPlots(TH1D* h1_etas[], DiMu2LpTOSMass analysis)
{
   if (!(analysis.twoOSMuons())) return;
   if (analysis.getLeadingMotherPDG() == analysis.getSLeadingMotherPDG())
   {
      double mass = analysis.getDiMuMass();
      if ((mass >0)&&(mass <=0.8))
      {
         h1_etas[0]->Fill(abs(analysis.getLeadingEta()));
      }
      else if ((mass>0.8)&&(mass<2.))
      {
         h1_etas[1]->Fill(abs(analysis.getLeadingEta()));
      }
      else if (mass>2)
      {
         h1_etas[2]->Fill(abs(analysis.getLeadingEta()));
      }
      else return;
   }
   return;
}

void DeltaRvsMassPlots(DiMu2LpTOSMass analysis, TH2D* h2, Event& event)
{
   if (analysis.twoOSMuons() && (analysis.getLeadingMotherPDG()==analysis.getSLeadingMotherPDG()) && (analysis.getLeadingMotherPDG()!=0))
   {
      h2->Fill(analysis.getDiMuMass(),analysis.getDeltaR(event));
   }
}


int main(int argc, char* argv[]) 
{ 
   // Create the ROOT application environment. 
   TApplication theApp("hist", &argc, argv);

   int seed = std::stoi(argv[1],nullptr,10);

   // Create file on which histogram(s) can be saved.
   TString outname = "out" + TString(argv[1]) + ".root";

   Pythia pythia;
   pythia.readFile("DarkPhotons/pythiaSettings.cmnd");
   // pythia.readFile("DarkPhotons/JPsi_only.cmnd");
   pythia.readString("Random:seed = " + std::to_string(seed));
   pythia.init();
   int nEvents =3000000;

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
   pdgParticle[130] = "K_{S}^{0}";

   // eta phase space
   // we use these to help understand why LHCb might get more data because of the rapidity range
   int nbinseta = 20;
   double xetalow = 0;
   double xetaup = 10;
   TH1D* h1_leadMuEta_0_0p8 = new TH1D("h1_leadMuEta_0_0p8", " 0.0 #geq m < 0.8 GeV", nbinseta, xetalow, xetaup);
   TH1D* h1_leadMuEta_0p8_2 = new TH1D("h1_leadMuEta_0p8_2", " 0.8 #geq m < 2 GeV", nbinseta, xetalow, xetaup);
   TH1D* h1_leadMuEta_2_10 = new TH1D("h1_leadMuEta_2_10", " 2 #geq m < 10 GeV", nbinseta, xetalow, xetaup);

   TH1D* h1_etas[3] = {h1_leadMuEta_0_0p8,h1_leadMuEta_0p8_2,h1_leadMuEta_2_10};
   // want a delta R plot to see how it changes with mass
   TH2D* h2_deltaR_Mass = new TH2D("h2_deltaR_Mass", "#Delta R of leading #mu^{#pm} OS",100,0,10,10,0,99);

   // variable binning
   double xbinsvar[100];
   double i = 0.2;
   for (int m = 0; m < 100; ++m)
   {
      i  = i + (i*0.04);
      xbinsvar[m] = i;
   }



   // DiMuonMass
   int nbins = 99; // 5 MeV bin width
   double xmin = 0.1;
   double xmax = 10.;
   
   TH1D* h1_DiMuMass_BEM_all = new TH1D("h1_DiMuMass_BEM_all", "BEM no cuts",nbins,xmin,xmax);
   // DiMuMass selects the leading mu and leading mubar and reconstructs it
   TH1D* h1_DiMuMass_BEM_CMS_eta2p4 = new TH1D("h1_DiMuMass_BEM_CMS_eta2p4", "|#eta|<2.4, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_BEM_CMS_eta1p5 = new TH1D("h1_DiMuMass_BEM_CMS_eta1p5", "|#eta|<1.5, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_BEM_CMS_eta1p5_dR1p4 = new TH1D("h1_DiMuMass_BEM_CMS_eta1p5_dR1p4", "|#eta|<1.5, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_BEM_CMS_eta2p4_dR1p4 = new TH1D("h1_DiMuMass_BEM_CMS_eta2p4_dR1p4", "|#eta|<2.4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   // DiMu2LpTOS selects the 2 leading pT, if OS then computes mass. but also SS
   // these should use the L1 triggers only
   TH1D* h1_DiMu2LpTOSMass_BEM_all = new TH1D("h1_DiMu2LpTOSMass_BEM_all", "No Cuts",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_all_var = new TH1D("h1_DiMu2LpTOSMass_BEM_all_var", "No Cuts",99,xbinsvar);
   TH1D* h1_DiMu2LpTOSMass_all_SS = new TH1D("h1_DiMu2LpTOSMass_all_SS", "p_{T}>3GeV",nbins,xmin,xmax);
   // first get BEM only
   // the title here represents L1 trigger selections, but we always do eta 2.4, pT >3 to reconstruct mass
   TH1D* h1_DiMu2LpTOSMass_BEM_CMS_eta2p4 = new TH1D("h1_DiMu2LpTOSMass_BEM_CMS_eta2p4", "|#eta|<2.4, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_CMS_eta1p5 = new TH1D("h1_DiMu2LpTOSMass_BEM_CMS_eta1p5", "|#eta|<1.5, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p4 = new TH1D("h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p4", "|#eta|<1.5, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p4 = new TH1D("h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p4", "|#eta|<2.4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p2 = new TH1D("h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p2", "|#eta|<1.5, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p2",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2 = new TH1D("h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2", "|#eta|<2.4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p2",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2_var = new TH1D("h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2_var", "|#eta|<2p4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p2",99,xbinsvar);
   TH1D* h1_DiMu2LpTOSMass_BEM_LHCb = new TH1D("h1_DiMu2LpTOSMass_BEM_LHCb", "p_{T}(#mu^{#pm})>1GeV, 2<#eta(#mu^{#pm})<4.5, p(#mu^{#pm})>20GeV, p_{T}(A')>1GeV, 2<#eta(A')<4.5",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_BEM_LHCb_var = new TH1D("h1_DiMu2LpTOSMass_BEM_LHCb_var", "p_{T}(#mu^{#pm})>1GeV, 2<#eta(#mu^{#pm})<4.5, p(#mu^{#pm})>20GeV, p_{T}(A')>1GeV, 2<#eta(A')<4.5",99,xbinsvar);
   // We can also get the same sign distribution from the above set of analysis
   TH1D* h1_DiMu2LpTOSMass_CMS_eta2p4_SS = new TH1D("h1_DiMu2LpTOSMass_CMS_eta2p4_SS", "|#eta|<2.4, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta1p5_SS = new TH1D("h1_DiMu2LpTOSMass_CMS_eta1p5_SS", "|#eta|<1.5, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p4_SS = new TH1D("h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p4_SS", "|#eta|<1.5, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p4_SS = new TH1D("h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p4_SS", "|#eta|<2.4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p2_SS = new TH1D("h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p2_SS", "|#eta|<1.5, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p2",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p2_SS = new TH1D("h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p2_SS", "|#eta|<2.4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p2",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_LHCb_SS = new TH1D("h1_DiMu2LpTOSMass_LHCb_SS", "p_{T}(#mu^{#pm})>0.5GeV, 2<#eta(#mu^{#pm})<4.5, p(#mu^{#pm})>1GeV, p_{T}(A')>1GeV, 2<#eta(A')<4.5, p(A')>1GeV",nbins,xmin,xmax);   
   // We can also get the opp sign distribution but not necessairly BEM
   TH1D* h1_DiMu2LpTOSMass_CMS_eta2p4 = new TH1D("h1_DiMu2LpTOSMass_CMS_eta2p4", "|#eta|<2.4, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta1p5 = new TH1D("h1_DiMu2LpTOSMass_CMS_eta1p5", "|#eta|<1.5, p_{T}>3GeV",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p4 = new TH1D("h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p4", "|#eta|<1.5, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p4 = new TH1D("h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p4", "|#eta|<2.4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p2 = new TH1D("h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p2", "|#eta|<1.5, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p2",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p2 = new TH1D("h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p2", "|#eta|<2.4, p_{T}>3GeV &  L1_DoubleMu0er1p5_SQ_OS_dR_Max1p2",nbins,xmin,xmax);
   TH1D* h1_DiMu2LpTOSMass_LHCb = new TH1D("h1_DiMu2LpTOSMass_LHCb", "p_{T}(#mu^{#pm})>0.5GeV, 2<#eta(#mu^{#pm})<4.5, p(#mu^{#pm})>1GeV, p_{T}(A')>1GeV, 2<#eta(A')<4.5, p(A')>1GeV",nbins,xmin,xmax);   

   // Throw everything in this array
   TH1D* hallhists[31] = {h1_DiMuMass_BEM_all,
                        h1_DiMuMass_BEM_CMS_eta2p4,
                        h1_DiMuMass_BEM_CMS_eta1p5,
                        h1_DiMuMass_BEM_CMS_eta1p5_dR1p4,
                        h1_DiMuMass_BEM_CMS_eta2p4_dR1p4,
                        h1_DiMu2LpTOSMass_BEM_all,
                        h1_DiMu2LpTOSMass_BEM_all_var,
                        h1_DiMu2LpTOSMass_all_SS,
                        h1_DiMu2LpTOSMass_BEM_CMS_eta2p4,
                        h1_DiMu2LpTOSMass_BEM_CMS_eta1p5,
                        h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p4,
                        h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p4,
                        h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p2,
                        h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2,
                        h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2_var,
                        h1_DiMu2LpTOSMass_BEM_LHCb,
                        h1_DiMu2LpTOSMass_BEM_LHCb_var,
                        h1_DiMu2LpTOSMass_CMS_eta2p4_SS,
                        h1_DiMu2LpTOSMass_CMS_eta1p5_SS,
                        h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p4_SS,
                        h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p4_SS,
                        h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p2_SS,
                        h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p2_SS,
                        h1_DiMu2LpTOSMass_LHCb_SS,
                        h1_DiMu2LpTOSMass_CMS_eta2p4,
                        h1_DiMu2LpTOSMass_CMS_eta1p5,
                        h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p4,
                        h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p4,
                        h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p2,
                        h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p2,
                        h1_DiMu2LpTOSMass_LHCb
                     };

   // PDG Mothers to identify BEM
   TH1D* h443    = new TH1D("443",    "M_{#mu#mu}("+TString(pdgParticle[443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h100443 = new TH1D("100443", "M_{#mu#mu}("+TString(pdgParticle[100443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h30443  = new TH1D("30443",  "M_{#mu#mu}("+TString(pdgParticle[30443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h553    = new TH1D("553",    "M_{#mu#mu}("+TString(pdgParticle[553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h100553 = new TH1D("100553", "M_{#mu#mu}("+TString(pdgParticle[100553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h200553 = new TH1D("200553", "M_{#mu#mu}("+TString(pdgParticle[200553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h111    = new TH1D("111",    "M_{#mu#mu}("+TString(pdgParticle[111])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h130    = new TH1D("130",    "M_{#mu#mu}("+TString(pdgParticle[130])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h113    = new TH1D("113",    "M_{#mu#mu}("+TString(pdgParticle[113])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h213    = new TH1D("213",    "M_{#mu#mu}("+TString(pdgParticle[213])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h221    = new TH1D("221",    "M_{#mu#mu}("+TString(pdgParticle[221])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h331    = new TH1D("331",    "M_{#mu#mu}("+TString(pdgParticle[331])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h223    = new TH1D("223",    "M_{#mu#mu}("+TString(pdgParticle[223])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h333    = new TH1D("333",    "M_{#mu#mu}("+TString(pdgParticle[333])+"#rightarrow#mu#mu)", nbins,xmin,xmax);

   TH1D* h443_var    = new TH1D("443_var",    "M_{#mu#mu}("+TString(pdgParticle[443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h100443_var = new TH1D("100443_var", "M_{#mu#mu}("+TString(pdgParticle[100443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h30443_var  = new TH1D("30443_var",  "M_{#mu#mu}("+TString(pdgParticle[30443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h553_var    = new TH1D("553_var",    "M_{#mu#mu}("+TString(pdgParticle[553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h100553_var = new TH1D("100553_var", "M_{#mu#mu}("+TString(pdgParticle[100553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h200553_var = new TH1D("200553_var", "M_{#mu#mu}("+TString(pdgParticle[200553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h111_var    = new TH1D("111_var",    "M_{#mu#mu}("+TString(pdgParticle[111])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h130_var    = new TH1D("130_var",    "M_{#mu#mu}("+TString(pdgParticle[130])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h113_var    = new TH1D("113_var",    "M_{#mu#mu}("+TString(pdgParticle[113])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h213_var    = new TH1D("213_var",    "M_{#mu#mu}("+TString(pdgParticle[213])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h221_var    = new TH1D("221_var",    "M_{#mu#mu}("+TString(pdgParticle[221])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h331_var    = new TH1D("331_var",    "M_{#mu#mu}("+TString(pdgParticle[331])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h223_var    = new TH1D("223_var",    "M_{#mu#mu}("+TString(pdgParticle[223])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* h333_var    = new TH1D("333_var",    "M_{#mu#mu}("+TString(pdgParticle[333])+"#rightarrow#mu#mu)", nbins,xmin,xmax);

   TH1D* hAll[14] = {h443,h100443,h30443,h553,h100553,h200553,h111,h130,h113,h213,h221,h331,h223,h333};
   TH1D* hAll_var[14] = {h443_var,h100443_var,h30443_var,h553_var,h100553_var,h200553_var,h111_var,h130_var,h113_var,h213_var,h221_var,h331_var,h223_var,h333_var};

   h443->SetFillColor(kRed);        //   h443->Write();
   h100443->SetFillColor(kRed+1);   //   h100443->Write();
   h30443->SetFillColor(kRed+2);    //   h30443->Write();
   h553->SetFillColor(kBlue);       //   h553->Write();
   h100553->SetFillColor(kBlue+1);  //   h100553->Write();
   h200553->SetFillColor(kBlue+2);  //   h200553->Write();
   h111->SetFillColor(kGreen);      //   h111->Write();
   h130->SetFillColor(kBlack);      //   h130->Write();
   h113->SetFillColor(kOrange);     //   h113->Write();
   h213->SetFillColor(kOrange+8);   //   h213->Write();
   h221->SetFillColor(kViolet);     //   h221->Write();
   h331->SetFillColor(kViolet+1);    //  h331->Write();
   h223->SetFillColor(kTeal);        //  h223->Write();
   h333->SetFillColor(kTeal+2);      //  h333->Write();

   // triggers
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 triggerL1_CMS_eta2p4_dR1p4(2.4);
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 triggerL1_CMS_eta1p5_dR1p4(1.5);
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 triggerL1_CMS_eta2p4_dR1p2(2.4,1.2); // THIS IS THE MAIN ONE
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 triggerL1_CMS_eta1p5_dR1p2(1.5,1.2); 
   // need 2 of these triggers 
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 trigger_CMS_eta2p4(2.4,pow(10,100000));
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 trigger_CMS_eta1p5(1.5,pow(10,100000));

   // Mass Analysis
   // Note that the mass analysis does not use deltaR as a requirement.
   // DiMu2LpTOSMass selects the leading 2 muons, then asks if OS or SS
   DiMu2LpTOSMass diMu2LpTOSMass_NoEta_NoPT(0.0, pow(10,100),0.0,0.0); // needed to get the fiducial
   DiMu2LpTOSMass diMu2LpTOSMass_CMS_eta2p4_eta1p5(0.0,2.4,3.0,0.0); // THIS IS A MAIN ONE
   DiMu2LpTOSMass diMu2LpTOSMass_CMS_eta2p4_eta2p4(0.0,2.4,3.0,0.0); // THIS IS A MAIN ONE
   DiMu2LpTOSMass diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p4(0.0,2.4,3.0,0.0); // THIS IS A MAIN ONE
   DiMu2LpTOSMass diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p2(0.0,2.4,3.0,0.0); // THIS IS A MAIN ONE
   DiMu2LpTOSMass diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p4(0.0,2.4,3.0,0.0); // THIS IS A MAIN ONE
   DiMu2LpTOSMass diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p2(0.0,2.4,3.0,0.0); // THIS IS THE MAIN ONE
   // DiMuMass selects the leading mu+ and leading mu- then reconstructs
   DiMuonMass diMuMass_CMS_eta1p5(0.,2.4,3.,0.);
   DiMuonMass diMuMass_CMS_eta2p4(0.,2.4,3.,0.);
   DiMuonMass diMuMass_CMS_eta1p5_dR1p4(0.,2.4,3.,0.);
   DiMuonMass diMuMass_CMS_eta2p4_dR1p4(0.,2.4,3.,0.);
   //LHCB
   LHCbDiMuMass diMuMassLHCb(2., 4.5, 1.,20.); // see fig 7 of LHCb exp paper

   // set up root file to store events
   // TFile *fileEvents = TFile::Open("events" + TString(argv[1]) + ".root","RECREATE");
   // Event *event = &pythia.event;diMu2LpTOSMass_NoEta_NoPT
   // TTree *T = new TTree("T","ev1 Tree");
   // T->Branch("event",&event);

   int nB_EM = 0;
   int nSuccessfulEvents = 0;
   // Begin event loop. Generate event; skip if generation aborted.
   for (int iEvent = 0; iEvent < nEvents; ++iEvent) 
   {
      // Generate event. Skip if error
      if (!pythia.next()) {std::cout<<"Error in event generation" << std::endl; continue;} 
      nSuccessfulEvents+=1;
      // fill all events
      // T->Fill();

      // run the events through each analysis
      // set to true to get all events
      // mass
      // need an all true one for fiducial stuff
      diMu2LpTOSMass_NoEta_NoPT.analyze(pythia.event, true);
      diMu2LpTOSMass_CMS_eta2p4_eta1p5.analyze(pythia.event, trigger_CMS_eta1p5.fired(pythia.event));
      diMu2LpTOSMass_CMS_eta2p4_eta2p4.analyze(pythia.event, trigger_CMS_eta2p4.fired(pythia.event));
      diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p4.analyze(pythia.event, triggerL1_CMS_eta1p5_dR1p4.fired(pythia.event));
      diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p2.analyze(pythia.event, triggerL1_CMS_eta1p5_dR1p2.fired(pythia.event));
      diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p4.analyze(pythia.event, triggerL1_CMS_eta2p4_dR1p4.fired(pythia.event));
      diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p2.analyze(pythia.event, triggerL1_CMS_eta2p4_dR1p2.fired(pythia.event));

      diMuMass_CMS_eta1p5.analyze(pythia.event,trigger_CMS_eta1p5.fired(pythia.event));
      diMuMass_CMS_eta2p4.analyze(pythia.event,trigger_CMS_eta2p4.fired(pythia.event));
      diMuMass_CMS_eta1p5_dR1p4.analyze(pythia.event,triggerL1_CMS_eta1p5_dR1p4.fired(pythia.event));
      diMuMass_CMS_eta2p4_dR1p4.analyze(pythia.event,triggerL1_CMS_eta2p4_dR1p4.fired(pythia.event));

      diMuMassLHCb.analyze(pythia.event,true);
      
      // Fill the BEM histograms
      diMuMass_CMS_eta1p5.fillBEMMass(h1_DiMuMass_BEM_CMS_eta1p5);      
      diMuMass_CMS_eta2p4.fillBEMMass(h1_DiMuMass_BEM_CMS_eta2p4);
      diMuMass_CMS_eta1p5_dR1p4.fillBEMMass(h1_DiMuMass_BEM_CMS_eta1p5_dR1p4);
      diMuMass_CMS_eta2p4_dR1p4.fillBEMMass(h1_DiMuMass_BEM_CMS_eta2p4_dR1p4);
      diMu2LpTOSMass_NoEta_NoPT.fillBEMMass(h1_DiMu2LpTOSMass_BEM_all);
      diMu2LpTOSMass_NoEta_NoPT.fillBEMMass(h1_DiMu2LpTOSMass_BEM_all_var);
      diMu2LpTOSMass_CMS_eta2p4_eta1p5.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta1p5);
      diMu2LpTOSMass_CMS_eta2p4_eta2p4.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta2p4);
      diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p4.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p4);
      diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p2.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2_var);
      // diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p4.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2_var);
      diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p2.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta1p5_dR1p2);
      diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p4.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p4);
      diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p2.fillBEMMass(h1_DiMu2LpTOSMass_BEM_CMS_eta2p4_dR1p2);
      diMuMassLHCb.fillBEMMass(h1_DiMu2LpTOSMass_BEM_LHCb);
      diMuMassLHCb.fillBEMMass(h1_DiMu2LpTOSMass_BEM_LHCb_var);
      // Fill the SS histograms
      diMu2LpTOSMass_NoEta_NoPT.fillMassSS(h1_DiMu2LpTOSMass_all_SS);
      diMu2LpTOSMass_CMS_eta2p4_eta1p5.fillMassSS(h1_DiMu2LpTOSMass_CMS_eta2p4_SS);
      diMu2LpTOSMass_CMS_eta2p4_eta2p4.fillMassSS(h1_DiMu2LpTOSMass_CMS_eta1p5_SS);
      diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p4.fillMassSS(h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p4_SS);
      diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p2.fillMassSS(h1_DiMu2LpTOSMass_CMS_eta1p5_dR1p2_SS);
      diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p4.fillMassSS(h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p4_SS);
      diMu2LpTOSMass_CMS_eta2p4_L1eta2p4dR1p2.fillMassSS(h1_DiMu2LpTOSMass_CMS_eta2p4_dR1p2_SS);
      diMuMassLHCb.fillMassSS(h1_DiMu2LpTOSMass_LHCb_SS);

      if (diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p4.twoOSMuons())
      {
         // std::cout<<"fired HLT trigger" << std::endl;
         // std::cout<<"DiMuMass = " << diMuMass_CMS_eta2p4_dR1p4.getDiMuMass() << std::endl;
         SortMothers(diMu2LpTOSMass_CMS_eta2p4_L1eta1p5dR1p4, hAll, hAll_var, 14);
      }      

      // make rapidity plots
      RapidityPlots(h1_etas,diMu2LpTOSMass_NoEta_NoPT);
      // plot delta R versus mass for analysis with no cuts
      DeltaRvsMassPlots(diMu2LpTOSMass_NoEta_NoPT, h2_deltaR_Mass, pythia.event);

   }

   // T->Print();
   // T->Write();
   // delete fileEvents;

   // OppSignSpectrum(hAll,14,hDiffMother,hSameMotherExtra);

   // std::cout<<"L1 Trigger Efficiency = " << triggerHLTCMS_eta2p4.getEfficiency() << std::endl;
   // std::cout<<"N fired = "<< triggerHLTCMS_eta2p4.getNFired() << std::endl;
   // std::cout << "N Events = " << triggerHLTCMS_eta2p4.getNEvents() << std::endl;
   // std::cout<<"LHCb Trigger Efficiency = " << triggerLHCb.getEfficiency() << std::endl;
   // std::cout<<"ALICE Trigger Efficiency = " << triggerALICE.getEfficiency() << std::endl;

   TFile *outFile = new TFile(outname, "RECREATE");  
   // write histograms to file
   for (int i = 0; i < 14; ++i)
   {
      hAll[i]->Write();
      hAll_var[i]->Write();
   }
   for (int i = 0; i < 31; ++i)
   {
      hallhists[i]->Write();
   }
   for (int i = 0; i < 3; ++i)
   {
      h1_etas[i]->Write();
   }
   h2_deltaR_Mass->Write();
   delete outFile;

   std::cout << "N B_EM = " << nB_EM << std::endl;
   // std::cout << "N Signal = " << diMuMassLHCb_1.getNSignal() << std::endl;
   std::cout << "N Events generated = " << nEvents << std::endl;
   
   std::cout << "Process summed estimated cross-section = " << pythia.info.sigmaGen()<< " mb" << std::endl; 
   pythia.stat() ;
}

