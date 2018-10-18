//------------------------------------------------------------------------------
/// \file MakePlots.cpp.
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Make plots from root file
//------------------------------------------------------------------------------

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

   TFile* file = new TFile("../out22.root");
   TString shists[14] = {"443","100443","30443","553","100553","200553","111","130","113","213","221","331","223","333"};   
   int pdghists[14] = {443,100443,30443,553,100553,200553,111,310,113,213,221,331,223,333};   
   Color_t chists[14] = {kRed, kRed+1, kRed+2, kBlue, kBlue+1, kBlue+2,kGreen,kBlack,kOrange,kOrange+8,kViolet,kViolet+1,kTeal,kTeal+2};
   THStack* stack = new THStack("hmain", "M_{#mu#mu}");
   TCanvas* c = new TCanvas();
   TLegend *legend = new TLegend(.60, .60, .8,.8);

   TH1* hall = (TH1*) file->Get("h1_DiMuMass_ALICE");
   hall->SetMarkerStyle(20);
   hall->SetMarkerSize(1);
   hall->SetTitle("M_{#mu#mu}");
   hall->Draw("same p e");

   for (int i = 0; i < 14; ++i)
   {
     // ((TH1*) file->FindObjectAny(shists[i]))->SetLineColor(chists[i]);
     // ((TH1*) file->FindObjectAny(shists[i]))->SetFillColor(chists[i]);
     // ((TH1*) file->FindObjectAny(shists[i]))->SetFillStyle(4051);
     // ((TH1*) file->FindObjectAny(shists[i]))->SetStats(0);
     // // stack->Add( (TH1*) file->FindObjectAny(shists[i]));
     // ((TH1*) file->FindObjectAny(shists[i]))->Draw("same");
      TH1* htemp = (TH1*) file->FindObjectAny(shists[i]) ;
      htemp->SetLineColor(chists[i]);
      htemp->SetFillColor(chists[i]);
      htemp->SetTitle("M_{#mu#mu}");
      // htemp->SetFillColorAlpha(chists[i],0.35);
      htemp->SetFillStyle(3001);
      htemp->SetStats(0);
      htemp->Draw("same");
      legend->AddEntry(htemp,TString(pdgParticle[pdghists[i]]),"lpf");
   }

   // stack->Draw("nostack");
   legend->Draw("same");
   // for (int i = 0; i < count; ++i)
   // {
   //    file->FindObjectAny()->Draw("hist same");
   //    /* code */
   // }

}