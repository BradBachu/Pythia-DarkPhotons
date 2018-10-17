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

   // h443->SetFillColor(kRed);           h443->Write();
   // h100443->SetFillColor(kRed+1);      h100443->Write();
   // h30443->SetFillColor(kRed+2);       h30443->Write();
   // h553->SetFillColor(kBlue);          h553->Write();
   // h100553->SetFillColor(kBlue+1);     h100553->Write();
   // h200553->SetFillColor(kBlue+2);     h200553->Write();
   // h111->SetFillColor(kGreen);         h111->Write();
   // h130->SetFillColor(kBlack);         h130->Write();
   // h113->SetFillColor(kOrange);        h113->Write();
   // h213->SetFillColor(kOrange+8);      h213->Write();
   // h221->SetFillColor(kViolet);        h221->Write();
   // h331->SetFillColor(kViolet+1);      h331->Write();
   // h223->SetFillColor(kTeal);          h223->Write();
   // h333->SetFillColor(kTeal+2);        h333->Write();

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
   // pythia.readFile("DarkPhotons/pythiaSettings.cmnd");
   pythia.readFile("DarkPhotons/ALICE_settings.cmnd");
   pythia.init();
   int nEvents =500000;

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
   // Number of Muons
   TH1D* h_MuTot_all = new TH1D("hMuTot_all", "Total No. of #mu^{#pm}", 20, 0, 20);
   TH1D* h_NMu_all = new TH1D("h_NMu_all", "No. of #mu^{-}", 20, 0, 20);
   TH1D* h_NMuBar_all = new TH1D("h_NMuBar", "No. of #mu^{-}", 20, 0, 20);

   TH1D* h_MuTot_t1 = new TH1D("h_MuTot_t1", "Total No. of Muons", 10, 0, 10);
   TH1D* h_NMu_t1 = new TH1D("h_NMu_t1", "No. of #mu^{-}", 10, 0, 10);
   TH1D* h_NMuBar_t1 = new TH1D("h_NMuBar_t1", "No. of #mu^{-}", 10, 0, 10);

   // Muon pT
   TH1D* h_MuPt_all = new TH1D("h_MuPt_all", "#mu^{-} p_{T}", 100,0,20);
   TH1D* h_MuPt_t1 = new TH1D("h_MuPt_t1", "#mu^{-} p_{T}", 100,0,20);
   TH1D* h_MuPt_t2 = new TH1D("h_MuPt_t2", "#mu^{-} p_{T}", 100,0,20);
   TH1D* h_MuPt_t1_t2 = new TH1D("h_MuPt_t1_t2", "#mu^{-} p_{T}", 100,0,20);

   // MuonBar pT
   TH1D* h_MuBarPt_all = new TH1D("h_MuBarPt_all", "#mu^{+} p_{T}", 100,0,20);
   TH1D* h_MuBarPt_t1 = new TH1D("h_MuBarPt_t1", "#mu^{+} p_{T}", 100,0,20);

   // pt eta phase space 
   TH2D* h2_MuPtEta_all = new TH2D("h2_MuPtEta_all", "#mu^{#pm}: p_{T} / #eta Phase space", 100,0.,10., 14,-7.,7.);
   TH2D* h2_MuPtEta_t1 = new TH2D("h2_MuPtEta_t1", "#mu^{#pm}: p_{T} / #eta Phase space", 100,0.,10., 14,-7.,7.);


   int nbins = 100;
   double xmin = 0.1;
   double xmax = 10.;

   // DiMuonMass
   TH1D* h1_DiMuMass_all = new TH1D("h1_DiMuMass_all", "M_{#mu#mu}",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_t1 = new TH1D("h1_DiMuMass_t1", "M_{#mu#mu}",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_t2 = new TH1D("h1_DiMuMass_t2", "M_{#mu#mu}",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_t1_t2 = new TH1D("h1_DiMuMass_t1_t2", "M_{#mu#mu}",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_ALICE = new TH1D("h1_DiMuMass_ALICE", "M_{#mu#mu}",nbins,xmin,xmax);

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
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 trigger1;
   LeadingMu_OS_Good trigger2;
   ALICE_DoubleMu triggerALICE;

   // Analysis
   MuonCounter mucount_all;
   MuonCounter mucount_t1;
   MuonPt muPt_all(h_MuPt_all);
   MuonPt muPt_t1(h_MuPt_t1);
   MuonPt muPt_t2(h_MuPt_t2);
   MuonPt muPt_t1_t2(h_MuPt_t1_t2);
   MuonPtEta muPtEta_all(h2_MuPtEta_all);
   MuonPtEta muPtEta_t1(h2_MuPtEta_t1);

   // Mass Analysis
   DiMuonMass diMuMass_all(h1_DiMuMass_all);
   DiMuonMass diMuMass_t1(h1_DiMuMass_t1);
   DiMuonMass diMuMass_t2(h1_DiMuMass_t2);
   DiMuonMass diMuMass_t1_t2(h1_DiMuMass_t1_t2);
   DiMuonMass diMuMass_ALICE(h1_DiMuMass_ALICE);

   // Begin event loop. Generate event; skip if generation aborted.
   for (int iEvent = 0; iEvent < nEvents; ++iEvent) 
   {
      // Generate event. Skip if error
      if (!pythia.next()) {std::cout<<"Error in event generation" << std::endl; continue;} 
      // std::cout<< "--- Event " << iEvent << std::endl;

      bool firedt1 = trigger1.fired(pythia.event);
      bool firedt2 = trigger2.fired(pythia.event);
      bool firedALICE = triggerALICE.fired(pythia.event);
     
      // run the events through each analysis
      // set to true to get all events
      // muon count
      mucount_all.analyze(pythia.event,true);
      mucount_t1.analyze(pythia.event,firedt1);
      // muon pt
      muPt_all.analyze(pythia.event, true);
      muPt_t1.analyze(pythia.event, firedt1);
      muPt_t2.analyze(pythia.event, firedt2);
      muPt_t1_t2.analyze(pythia.event, (firedt1 && firedt2) );
      // muon pt/eta
      muPtEta_all.analyze(pythia.event,true);
      muPtEta_t1.analyze(pythia.event,firedt1);
      // mass
      diMuMass_all.analyze(pythia.event,true);
      diMuMass_t1.analyze(pythia.event,firedt1);
      diMuMass_t2.analyze(pythia.event,firedt2);
      diMuMass_ALICE.analyze(pythia.event,(firedALICE));

      // Fill the histograms
      h_MuTot_all->Fill(mucount_all.getTotMuons());
      h_MuTot_t1->Fill(mucount_t1.getTotMuons());

      h1_DiMuMass_all->Fill(diMuMass_all.getDiMuMass());
      h1_DiMuMass_t1->Fill(diMuMass_t1.getDiMuMass());
      h1_DiMuMass_t2->Fill(diMuMass_t2.getDiMuMass());
      h1_DiMuMass_t1_t2->Fill(diMuMass_t1_t2.getDiMuMass());
      h1_DiMuMass_ALICE->Fill(diMuMass_ALICE.getDiMuMass());

      // hNMuons_vanilla->Fill(mucount_vanilla.getNMuons());
      // hNAMuons_vanilla->Fill(mucount_vanilla.getNAMuons());

      // hNMuons_scoutTrigger->Fill(mucount_scouting.getNMuons());
      // hNAMuons_scoutTrigger->Fill(mucount_scouting.getNAMuons());
      if (firedt1&&firedt2)
      {
         std::cout<<"fired t1 and t2" << std::endl;
         SortMothers(diMuMass_ALICE, hAll, 14, hDiffMother ,hSameMotherExtra);
      }      
   }


   OppSignSpectrum(hAll,14,hDiffMother,hSameMotherExtra);

   std::cout<<"L1 Trigger Efficiency = " << trigger1.getEfficiency() << std::endl;
   std::cout<<"N fired = "<< trigger1.getNFired() << std::endl;
   std::cout << "N Events = " << trigger1.getNEvents() << std::endl;
   std::cout<<"t2: N Events = " << trigger2.getNFired() << std::endl;
   std::cout<<"ALICE Trigger Efficiency = " << triggerALICE.getEfficiency() << std::endl;


   DrawHists(h_MuPt_all,h_MuPt_t1,h_MuPt_t2,h_MuPt_t1_t2);
   DrawHists(h1_DiMuMass_all, h1_DiMuMass_t1, h1_DiMuMass_t2, h1_DiMuMass_t1_t2);
   // TCanvas* c3 = new TCanvas();
   // hNMuons_vanilla->Draw();
   h2_MuPtEta_all->Write();
   h2_MuPtEta_t1->Write();

   TCanvas* c2 = new TCanvas(2);
   c2->Divide(2,1);
   c2->cd(1);
   h2_MuPtEta_all->Draw("COLZ");
   c2->cd(2);
   h2_MuPtEta_t1->Draw("COLZ");
   c2->Write();

   TCanvas* c3 = new TCanvas();

   delete outFile;
}

