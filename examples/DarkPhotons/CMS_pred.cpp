//------------------------------------------------------------------------------
/// \file CMS_pred.cpp.
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    CMS production predictions
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string.h>
#include <TFile.h>
#include "TH1D.h"
#include <TCanvas.h>
#include <TGraph.h>

// void CMS_pred(int argc, char* argv[])
void CMS_pred()
{
   // double epsilonSquare = std::stod(argv[1],nullptr);
   double epsilonSquare = 1;
   // open root file and get histogram with B_EM
   TFile* file = new TFile("../out35.root");
   TH1D* h = (TH1D*) file->FindObjectAny("h1_BEM_CMS");


   int nbins = h->GetNbinsX();
   double x[nbins+1], y[nbins+1];
   double xlow = h->GetXaxis()->GetBinLowEdge(1);
   std::cout << "Bin low edge = " << xlow << std::endl;
   double width = h->GetBinWidth(1);
   int ntotal = h->Integral(1,nbins);
   std::cout <<"total = " << ntotal << std::endl;
   double pythia_xs = 1.181;  // mb
   h->Scale(pythia_xs/ntotal);
   for (int i = 0; i <= nbins; ++i)
   {
      double mA = xlow + (i*width) + (width/2);
      std::cout<<"mass = " << mA << std::endl;
      double massRes = 0.01*mA;
      double pred = (epsilonSquare*mA*(h->GetBinContent(i)))/massRes ;
      std::cout<< "Predction = " << pred << std::endl;
      x[i] = mA;
      y[i] = pred;
   }
   TGraph *g_pred = new TGraph(nbins,x,y);
   TCanvas* c = new TCanvas();
   h->Draw();
   TCanvas* c2 = new TCanvas();
   // g_pred->SetMarkerStyle(20);
   g_pred->Draw();
}