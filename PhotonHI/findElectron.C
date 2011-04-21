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
#include <TStyle.h>
#include <TH2.h>
#include <TH3.h>

#include "common.h"
#include "histFunction2.C"
#include "multiPanel.h"



#define canvasX 1200
#define canvasY 1200


void findElectron()
{

   /*   
   0 	~ 	10%
   10	~	20%
   20 	~ 	30%
   30 	~	50%
   50	~ 	100 % 
   pT (6 bins)
   20	~	25GeV
   25	~	30GeV
   30	~	40GeV
   40	~	50GeV
   50	~ 	80GeV
   80    ~      140GeV 
   */
   gStyle->SetPaintTextFormat(".3g");
   // Data sample
   TFile *infData = new TFile("data/mpa_hiDATA_April02_EB.root");
   TTree *tData = (TTree*)infData->FindObjectAny("Analysis");

   // Signal sample
//   TFile *infSig = new TFile("PythiaData/mpaPhoton.root");
   TFile *infSig = new TFile("ampt/mpa_amptPhotonJet.root");
   TTree *tSig = (TTree*)infSig->FindObjectAny("Analysis");

   // W sample
//   TFile *infW = new TFile("PythiaData/mpaPhoton.root");
   TFile *infW = new TFile("ampt/mpa_w2ev_mix_ampt_april15.root");
   TTree *tW = (TTree*)infW->FindObjectAny("Analysis");


   // outfile
   TFile *outfile = new TFile("Wstudies.root","recreate");

   const int nCBin = 5;
   double cBins[nCBin+1] = {-0.5,3.5,7.5,11.5,19.5,39.5};
//   const int nEtBin = 15;
//   double etBins[nEtBin+1] = {20,21,22,23,24,25,27,30,35,40,45,50,60,80,100,140};
   const int nEtBin = 6;
   double etBins[nEtBin+1] = {20,25,30,40,50,80,140};


   selectionCriteria sel;

   cout <<"Add Centrality reweighting to signal ..."<<endl;
   addCentralityFriend(tSig,tData,sel.selectionCut2);
   addCentralityFriend(tW,tData,sel.selectionCut2);

   
   int count = 0;

   TH1D *hData = new TH1D("hData","",50,0,2);
   TH1D *hSig = new TH1D("hSig","",50,0,2);
   TH1D *hW = new TH1D("hW","",50,0,2);
   

   tData->Draw("t1PtCut/pt/r9>>hData",sel.selectionCut2&&"isEle&&et>20");
   tSig->Draw("t1PtCut/pt/r9>>hSig",(sel.selectionCut2&&"isEle&&et>20")*"cBinWeight");
   tW->Draw("t1PtCut/pt/r9>>hW",(sel.selectionCut2&&"isEle&&et>20")*"cBinWeight");
   
   hData->Scale(1./hData->GetEntries());
   hSig->Scale(1./hSig->GetEntries());
   hW->Scale(1./hW->GetEntries());
   hSig->SetLineColor(2);
   hW->SetLineColor(4);
   hData->Draw();
   hSig->Draw("same");
   hW->Draw("same");   
   
   
//   TCanvas *c0 = new TCanvas(Form("c%d",x),Form("%f<cBin&&cBin<%f",cBins[x],cBins[x+1]),canvasX,canvasY);

/*   
   for (int x=0;x<nCBin;x++)
   {
      for (int y=0;y<nEtBin;y++)
      {

         count++;
         c0->cd(count);
	 TH1D *htemp = new TH1D(Form("htemp%d",count),"",100,-0.4,0.4);
         TCut etCut = Form("%f<et&&et<%f",etBins[y],etBins[y+1]);
         TCut cBinCut = Form("%f<cBin&&cBin<%f",cBins[x],cBins[x+1]);
	 tSig->Draw(Form("(et-genMatchedPt)/et>>htemp%d",count),(sel.selectionCut2&&etCut&&cBinCut&&sel.sigCut&&"r9>0.94")*"cBinWeight");
	 hEtCorrectionUnc->SetBinContent(x+1,y+1,htemp->GetMean());
	 hEtCorrectionUnc->SetBinError(x+1,y+1,htemp->GetMeanError());
	 hEtCorrectionUnc->SetBinContent(x+1,y+1,htemp->GetMean());
	 hEtCorrectionUnc->SetBinError(x+1,y+1,htemp->GetMeanError());

	 tSig->Draw(Form("(et-genMatchedPt)/et>>htemp%d",count),(sel.selectionCut2&&etCut&&cBinCut&&sel.sigCut&&"r9<0.94")*"cBinWeight");
	 hEtCorrection->SetBinContent(x+1,y+1,htemp->GetMean());
	 hEtCorrection->SetBinError(x+1,y+1,htemp->GetMeanError());
	 hEtCorrection->SetBinContent(x+1,y+1,htemp->GetMean());
	 hEtCorrection->SetBinError(x+1,y+1,htemp->GetMeanError());
                  	 
      }
   }
   
   TCanvas *c1 = new TCanvas("c1","",800,600);
   hEtCorrectionUnc->Draw("col text");
   hEtCorrectionUnc->SetXTitle("Unconverted Photon        Centrality Bin");
   hEtCorrectionUnc->SetYTitle("Photon P_{T} GeV/c");
   
   TCanvas *c2 = new TCanvas("c2","",800,600);
   hEtCorrection->Draw("col text");
   hEtCorrection->SetXTitle("Converted Photon        Centrality Bin");
   hEtCorrection->SetYTitle("Photon P_{T} GeV/c");
   outfile->Write();
*/
}
