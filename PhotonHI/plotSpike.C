#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile.h>
#include <TNtuple.h>

#include "common.h"
#include "histFunction2.C"
#include "multiPanel.h"

#define canvasX 800
#define canvasY 400

void plotSpike(char *infname)
{
   // Data sample
   TFile *infData = new TFile(infname);
   TTree *tData = (TTree*)infData->FindObjectAny("Analysis");
   TTree *tDataGen = (TTree*)infData->FindObjectAny("Generator");

   // Signal sample
//   TFile *infSig = new TFile("PythiaData/mpaPhoton_correctedTree.root");
//   TFile *infSig = new TFile("ampt/mpa_photon15_ampt_april15_correctedTree.root");
//   TFile *infSig = new TFile("ampt/mpa_amptPhotonJet_correctedTree.root");
   TFile *infSig = new TFile("ampt/mpaEmJet30_mix_ampt.root");
   TTree *tSig = (TTree*)infSig->FindObjectAny("Analysis");
   TTree *tSigGen = (TTree*)infSig->FindObjectAny("Generator");


   tData->SetAlias("var","(1 - (eRight+eLeft+eTop+eBottom)/eMax)");
   tSig->SetAlias("var","(1 - (eRight+eLeft+eTop+eBottom)/eMax)");

   tData->SetAlias("var2","(eMax+e2nd)/e3x3");
   tSig->SetAlias("var2","(eMax+e2nd)/e3x3");


   // Output file
   TFile *outfile = new TFile(Form("tmp.root"),"recreate");
   
   selectionCriteria sel;
   addCentralityFriend(tSig,tData,"isEB&&et>20");

 
   TCanvas *c = new TCanvas("c","",600,600);
   c->SetLogy();

   TH1D *hData = new TH1D("hData","",62,-2,1.1);
   TH1D *hSig = new TH1D("hSig","",62,-2,1.1);
   hData->SetXTitle("1-E_{4}/E_{Max}");
   hData->SetYTitle("Arbitrary Normalization");

   tData->Draw("var>>hData","isEB&&et>20"&&sel.removeSpikeCut);
   tSig->Draw("var>>hSig",("isEB&&et>20"&&sel.removeSpikeCut));

   
   hData->Sumw2();
   hSig->Sumw2();
   
   hSig->SetLineColor(2);
   hSig->SetMarkerColor(2);
   hSig->SetMarkerStyle(4);
   hSig->Scale(1./hSig->GetEntries());
   hData->Scale(1./hData->GetEntries());
   double scaleData = ((double)hData->Integral(1,50))/hSig->Integral(1,50);
   hSig->Scale(scaleData);
   
   hData->Draw("");
   hSig->Draw("same");
   
      TLegend *leg1 ;
   leg1 = new TLegend(0.43,0.24,0.93,0.49);
   leg1->SetFillStyle(0);
   leg1->SetBorderSize(0);
   leg1->AddEntry(hData,"Pb+Pb data","pl");
   leg1->AddEntry(hSig,"PYTIHA + AMPT","pl");
   leg1->Draw();

 
   TCanvas *c2 = new TCanvas("c2","",600,600);
   c2->SetLogy();

   TH1D *hData2 = new TH1D("hData2","",55,0,1.1);
   TH1D *hSig2 = new TH1D("hSig2","",55,0,1.1);
   hData2->SetXTitle("E_{2}/E_{9}");
   hData2->SetYTitle("Arbitrary Normalization");

   tData->Draw("var2>>hData2","isEB&&et>20"&&sel.removeSpikeCut);
   tSig->Draw("var2>>hSig2",("isEB&&et>20"&&sel.removeSpikeCut));

   
   hData2->Sumw2();
   hSig2->Sumw2();
   
   hSig2->SetLineColor(2);
   hSig2->SetMarkerColor(2);
   hSig2->SetMarkerStyle(4);
   hSig2->Scale(1./hSig->GetEntries());
   hData2->Scale(1./hData->GetEntries());
//   double scaleData = ((double)hData2->Integral(1,50))/hSig2->Integral(1,50);
//   hSig2->Scale(scaleData);
   
   hData2->Draw("");
   hSig2->Draw("same");
   
      TLegend *leg2 ;
   leg2 = new TLegend(0.43,0.24,0.93,0.49);
   leg2->SetFillStyle(0);
   leg2->SetBorderSize(0);
   leg2->AddEntry(hData,"Pb+Pb data","pl");
   leg2->AddEntry(hSig,"PYTIHA + AMPT","pl");
   leg2->Draw();


   outfile->Write();
   
}
