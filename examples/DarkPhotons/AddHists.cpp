//------------------------------------------------------------------------------
/// \file AddHists.cpp.
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Add up hists made on lxplus
//------------------------------------------------------------------------------

// Stdlib header file for input and output.
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


void AddHists()
{
   int nbins = 99;
   double xmin = 0.1;
   double xmax = 10.;

   // DiMuonMass
   TH1D* h1_DiMuMass_CMS_eta2p4 = new TH1D("h1_DiMuMass_CMS_eta24", "M_{#mu#mu}; |#eta|<2.4",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_CMS_eta1p5 = new TH1D("h1_DiMuMass_CMS_eta15", "M_{#mu#mu}; |#eta|<1.5",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_CMS_eta2p4_dR1p4 = new TH1D("h1_DiMuMass_CMS_eta24_dR1p4", "M_{#mu#mu}; |#eta|<2.4 & #Delta R < 1.4",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_CMS_eta1p5_dR1p4 = new TH1D("h1_DiMuMass_CMS_eta15_dR1p4", "M_{#mu#mu}; |#eta|<1.5 & #Delta R < 1.4",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_LHCb = new TH1D("h1_DiMuMass_LHCb", "M_{#mu#mu}; 2<|#eta|<4.5 & pT > 0.5",nbins,xmin,xmax);
   TH1D* h1_DiMuMass_ALICE = new TH1D("h1_DiMuMass_ALICE", "M_{#mu#mu}; 2.5 <|#eta|<4 & 1 < p_{T} < 5",nbins,xmin,xmax);

   for (int i = 1; i < 10; ++i)
   {

      TFile *temp = new TFile(TString("out" + std::to_string(i) + ".root"));
      if (temp->IsOpen())
      {
         printf("File opened successfully \n");
         h1_DiMuMass_CMS_eta2p4->Add( (TH1*) temp->FindObjectAny("h1_DiMuMass_CMS_eta24"));
         h1_DiMuMass_CMS_eta1p5->Add( (TH1*) temp->FindObjectAny("h1_DiMuMass_CMS_eta15"));
         h1_DiMuMass_CMS_eta2p4_dR1p4->Add( (TH1*) temp->FindObjectAny("h1_DiMuMass_CMS_eta24_dR1p4"));
         h1_DiMuMass_CMS_eta1p5_dR1p4->Add( (TH1*) temp->FindObjectAny("h1_DiMuMass_CMS_eta15_dR1p4"));
         h1_DiMuMass_LHCb->Add( (TH1*) temp->FindObjectAny("h1_DiMuMass_LHCb"));
         h1_DiMuMass_ALICE->Add( (TH1*) temp->FindObjectAny("h1_DiMuMass_ALICE"));
      } 
      temp->Close();
   }

   TFile *mainfile = new TFile("AllHists.root","RECREATE");
   h1_DiMuMass_CMS_eta2p4->Write();
   h1_DiMuMass_CMS_eta1p5->Write();
   h1_DiMuMass_CMS_eta2p4_dR1p4->Write();
   h1_DiMuMass_CMS_eta1p5_dR1p4->Write();
   h1_DiMuMass_LHCb->Write();
   h1_DiMuMass_ALICE->Write();   

}                                                                                                                                                                       