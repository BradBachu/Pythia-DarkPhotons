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
#include "DarkPhotons/DiMuonMass.h"

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


void SortMothers(DiMuonMass dimuonmass, TH1D* hSameMother[], int size, TH1D* hDiffMother , TH1D* hSameMotherExtra)
{
   // if the mothers are different fill in hDiffMother
   std::cout<<"Mu- Mother PDG = " << dimuonmass.getLeadingMuMotherPDG() <<std::endl;
   std::cout<<"Mu+ Mother PDG = " << dimuonmass.getLeadingMuBarMotherPDG() <<std::endl;
   bool matched = false;
   if (dimuonmass.getLeadingMuMotherPDG() != dimuonmass.getLeadingMuBarMotherPDG() )
   {
      std::cout<<"diff mother" <<std::endl;
      hDiffMother->Fill(dimuonmass.getDiMuMass());
      return;
   }
   // they have the same mother and we have the ID
   for (int i = 0; i < size; ++i)
   {
      if ((!matched)&&(std::to_string(dimuonmass.getLeadingMuMotherPDG()) == hSameMother[i]->GetName()))
      {
         std::cout<<"Matched to known mother" << std::endl;
         hSameMother[i]->Fill(dimuonmass.getDiMuMass());
         return;
      }
   }
   // they have the same mother and we did not label it
   std::cout<< "Matched but not accounted for ID = " << std::to_string(dimuonmass.getLeadingMuMotherPDG())  << std::endl;
   hSameMotherExtra->Fill(dimuonmass.getDiMuMass());

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

int main(int argc, char* argv[]) 
{ 
   // Create the ROOT application environment. 
   TApplication theApp("hist", &argc, argv);

   int seed = std::stoi(argv[1],nullptr,10);

   // Create file on which histogram(s) can be saved.
   TString outname = "out" + TString(argv[1]) + ".root";
   TFile *outFile = new TFile(outname, "RECREATE");   

   Pythia pythia;
   pythia.readFile("DarkPhotons/pythiaSettings.cmnd");
   // pythia.readFile("DarkPhotons/ALICE_settings.cmnd");
   pythia.readString("Random:seed = " + std::to_string(seed));
   pythia.init();
   int nEvents =20000;

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

   // Histograms
   // pt eta phase space 
   TH2D* h2_MuPtEta_all = new TH2D("h2_MuPtEta_all", "#mu^{#pm}: p_{T} / #eta Phase space", 100,0.,10., 14,-7.,7.);
   TH2D* h2_MuPtEta_t1 = new TH2D("h2_MuPtEta_t1", "#mu^{#pm}: p_{T} / #eta Phase space", 100,0.,10., 14,-7.,7.);

   int nbins = 99;
   double xmin = 0.1;
   double xmax = 10.;

   // DiMuonMass
   TH1D* h1_DiMuMass_all = new TH1D("h1_DiMuMass_all", "M_{#mu#mu}",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_CMS_eta2p4 = new TH1D("h1_DiMuMass_CMS_eta24", "M_{#mu#mu}; |#eta|<2.4",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_CMS_eta1p5 = new TH1D("h1_DiMuMass_CMS_eta15", "M_{#mu#mu}; |#eta|<1.5",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_CMS_eta2p4_dR1p4 = new TH1D("h1_DiMuMass_CMS_eta24_dR1p4", "M_{#mu#mu}; |#eta|<2.4 & #Delta R < 1.4",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_CMS_eta1p5_dR1p4 = new TH1D("h1_DiMuMass_CMS_eta15_dR1p4", "M_{#mu#mu}; |#eta|<1.5 & #Delta R < 1.4",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_LHCb = new TH1D("h1_DiMuMass_LHCb", "M_{#mu#mu}; 2<|#eta|<4.5 & pT > 0.5",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_ALICE = new TH1D("h1_DiMuMass_ALICE", "M_{#mu#mu}; 2.5 <|#eta|<4 & 1 < p_{T} < 5",nbins,xmin,xmax);

   // PDG Mothers
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
   TH1D* hDiffMother    = new TH1D("hDiffMother",    "M_{xy}#rightarrow#mu#mu)", nbins,xmin,xmax);
   TH1D* hSameMotherExtra    = new TH1D("hSameMotherExtra",    "M_{xx}#rightarrow#mu#mu)", nbins,xmin,xmax);

   TH1D* hAll[14] = {h443,h100443,h30443,h553,h100553,h200553,h111,h130,h113,h213,h221,h331,h223,h333};

   // Histogram for Mother PDG ID's
   // TH2I* hAllMothers 

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
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 triggerHLTCMS_eta2p4(2.4);
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 triggerHLTCMS_eta1p5(1.5);
   LeadingMu_OS_Good triggerCMS_eta2p4(0.,2.4,3.,0.);
   LeadingMu_OS_Good triggerCMS_eta1p5(0.,1.5,3.,0.);
   LeadingMu_OS_Good triggerLHCb(2.,4.5,0.5,0.); 
   LeadingMu_OS_Good triggerALICE(2.5,4.,1.,5.);

   // Mass Analysis
   // Note that the mass analysis does not use deltaR as a requirement.
   DiMuonMass diMuMass_CMS_eta1p5(h1_DiMuMass_CMS_eta1p5,0.,1.5,3.,0.);
   DiMuonMass diMuMass_CMS_eta2p4(h1_DiMuMass_CMS_eta2p4,0.,2.4,3.,0.);
   DiMuonMass diMuMass_CMS_eta1p5_dR1p4(h1_DiMuMass_CMS_eta1p5_dR1p4,0.,1.5,3.,0.);
   DiMuonMass diMuMass_CMS_eta2p4_dR1p4(h1_DiMuMass_CMS_eta2p4_dR1p4,0.,2.4,3.,0.);
   DiMuonMass diMuMass_ALICE(h1_DiMuMass_ALICE,2.5,4.,1.,5.);
   DiMuonMass diMuMass_LHCb(h1_DiMuMass_LHCb,2.,4.5,0.5,0.);

   // Begin event loop. Generate event; skip if generation aborted.
   for (int iEvent = 0; iEvent < nEvents; ++iEvent) 
   {
      // Generate event. Skip if error
      if (!pythia.next()) {std::cout<<"Error in event generation" << std::endl; continue;} 
      // std::cout<< "--- Event " << iEvent << std::endl;
      // run the events through each analysis
      // set to true to get all events

      // mass
      diMuMass_CMS_eta1p5.analyze(pythia.event,triggerCMS_eta1p5.fired(pythia.event));
      diMuMass_CMS_eta2p4.analyze(pythia.event,triggerCMS_eta2p4.fired(pythia.event));
      diMuMass_CMS_eta1p5_dR1p4.analyze(pythia.event,triggerHLTCMS_eta1p5.fired(pythia.event));
      diMuMass_CMS_eta2p4_dR1p4.analyze(pythia.event,triggerHLTCMS_eta2p4.fired(pythia.event));
      diMuMass_ALICE.analyze(pythia.event,triggerALICE.fired(pythia.event));
      diMuMass_LHCb.analyze(pythia.event,triggerLHCb.fired(pythia.event));
      
      // Fill the histograms
      h1_DiMuMass_CMS_eta1p5->Fill(diMuMass_CMS_eta1p5.getDiMuMass());
      h1_DiMuMass_CMS_eta2p4->Fill(diMuMass_CMS_eta2p4.getDiMuMass());
      h1_DiMuMass_CMS_eta1p5_dR1p4->Fill(diMuMass_CMS_eta1p5_dR1p4.getDiMuMass());
      h1_DiMuMass_CMS_eta2p4_dR1p4->Fill(diMuMass_CMS_eta2p4_dR1p4.getDiMuMass());
      h1_DiMuMass_LHCb->Fill(diMuMass_LHCb.getDiMuMass());
      h1_DiMuMass_ALICE->Fill(diMuMass_ALICE.getDiMuMass());

      if (triggerHLTCMS_eta2p4.fired(pythia.event))
      {
         std::cout<<"fired HLT trigger" << std::endl;
         std::cout<<"DiMuMass = " << diMuMass_CMS_eta2p4_dR1p4.getDiMuMass() << std::endl;
         SortMothers(diMuMass_CMS_eta2p4_dR1p4, hAll, 14, hDiffMother ,hSameMotherExtra);
      }      
   }

   OppSignSpectrum(hAll,14,hDiffMother,hSameMotherExtra);

   std::cout<<"L1 Trigger Efficiency = " << triggerHLTCMS_eta2p4.getEfficiency() << std::endl;
   std::cout<<"N fired = "<< triggerHLTCMS_eta2p4.getNFired() << std::endl;
   std::cout << "N Events = " << triggerHLTCMS_eta2p4.getNEvents() << std::endl;
   std::cout<<"LHCb Trigger Efficiency = " << triggerLHCb.getEfficiency() << std::endl;
   std::cout<<"ALICE Trigger Efficiency = " << triggerALICE.getEfficiency() << std::endl;

   // Fill the histograms
   h1_DiMuMass_CMS_eta1p5->Write();
   h1_DiMuMass_CMS_eta2p4->Write();
   h1_DiMuMass_CMS_eta1p5_dR1p4->Write();
   h1_DiMuMass_CMS_eta2p4_dR1p4->Write();
   h1_DiMuMass_LHCb->Write();
   h1_DiMuMass_ALICE->Write();

   delete outFile;
}

