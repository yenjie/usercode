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


void makeEtCorrectionTable()
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
   TFile *infData = new TFile("data/mpaData_photon15Trig_april26_EB.root");
   TTree *tData = (TTree*)infData->FindObjectAny("Analysis");

   // Signal sample
   TFile *infSig = new TFile("PythiaData/mpa_photonAll_hiData_april26.root");
//   TFile *infSig = new TFile("ampt/mpaPhotonJet_0417.root");
   TTree *tSig = (TTree*)infSig->FindObjectAny("Analysis");
   TTree *tSigGen = (TTree*)infSig->FindObjectAny("Generator");

   // outfile
   TFile *outfile = new TFile("correction.root","recreate");

   const int nCBin = 5;
   double cBins[nCBin+1] = {-0.5,3.5,7.5,11.5,19.5,39.5};
//   const int nEtBin = 15;
//   double etBins[nEtBin+1] = {20,21,22,23,24,25,27,30,35,40,45,50,60,80,100,140};
   const int nEtBin = 6;
   double etBins[nEtBin+1] = {20,25,30,40,50,80,140};


   // Selection cut
   selectionCriteria sel;
   sel.eventCut     = "( !TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && abs(vtxZ) <= 15)";
//   sel.centralityCut = Form("cBin>=%d&&cBin<%d",cBinL,cBinH);
   TString swissCrx = "(1 - (eRight+eLeft+eTop+eBottom)/eMax)";
   TCut hiSpikeCut       = Form("(  %s < 0.9 && abs(seedTime)<3  && sigmaIetaIeta>0.002 )  || isEE",swissCrx.Data());
   
   sel.removeSpikeCut = hiSpikeCut;
   sel.photonCut ="r9>0.&&hadronicOverEm<0.2&&abs(eta)<1.44&&rawEnergy/energy>0.5";
   sel.removeElectronCut = "!isEle";//"!isEle";//"!isEle";
   
   sel.selectionCut = sel.photonCut&&sel.centralityCut&&sel.eventCut&&sel.removeSpikeCut&&sel.removeElectronCut; 
   sel.selectionCutWOElectronCut = sel.photonCut&&sel.centralityCut&&sel.eventCut&&sel.removeSpikeCut; 
   sel.selectionCut2 = sel.photonCut&&sel.eventCut&&sel.removeSpikeCut&&sel.removeElectronCut; 


   sel.sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0 && genMatchedCollId ==0";

   cout <<"Add Centrality reweighting to signal ..."<<endl;
   addCentralityFriend(tSig,tSigGen,tData,sel.selectionCut2&&"sigmaIetaIeta<0.0125&&(cr4+cc4)<5");

   TH2D *hEtCorrection = new TH2D("hEtCorrection","",nCBin,cBins,nEtBin,etBins);
   TH2D *hEtCorrectionUnc = new TH2D("hEtCorrectionUnc","",nCBin,cBins,nEtBin,etBins);
   
   int count = 0;

   TCanvas *c0 = new TCanvas("c0","Unconverted",canvasX,canvasY);
   c0->Divide(nEtBin,nCBin);
   TCanvas *c1 = new TCanvas("c1","Converted",canvasX,canvasY);
   c1->Divide(nEtBin,nCBin);

//   TCanvas *c0 = new TCanvas(Form("c%d",x),Form("%f<cBin&&cBin<%f",cBins[x],cBins[x+1]),canvasX,canvasY);
   
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
         /*
	 hEtCorrectionUnc->SetBinContent(x+1,y+1,htemp->GetMean());
	 hEtCorrectionUnc->SetBinError(x+1,y+1,htemp->GetMeanError());
	 hEtCorrectionUnc->SetBinContent(x+1,y+1,htemp->GetMean());
	 hEtCorrectionUnc->SetBinError(x+1,y+1,htemp->GetMeanError());
         */
	 htemp->Fit("gaus","LL","",htemp->GetMean()-1.5*htemp->GetRMS(),htemp->GetMean()+1.5*htemp->GetRMS());
	 hEtCorrectionUnc->SetBinContent(x+1,y+1,htemp->GetFunction("gaus")->GetParameter(1));
	 hEtCorrectionUnc->SetBinError(x+1,y+1,htemp->GetFunction("gaus")->GetParError(1));

	 htemp = new TH1D(Form("htempU%d",count),"",100,-0.4,0.4);
         c1->cd(count);
	 tSig->Draw(Form("(et-genMatchedPt)/et>>htempU%d",count),(sel.selectionCut2&&etCut&&cBinCut&&sel.sigCut&&"r9<0.94")*"cBinWeight");
	 htemp->Fit("gaus","LL","",htemp->GetMean()-1.5*htemp->GetRMS(),htemp->GetMean()+1.5*htemp->GetRMS());
	 hEtCorrection->SetBinContent(x+1,y+1,htemp->GetFunction("gaus")->GetParameter(1));
	 hEtCorrection->SetBinError(x+1,y+1,htemp->GetFunction("gaus")->GetParError(1));
                  	 
      }
   }
   
   TCanvas *c2 = new TCanvas("c2","",800,600);
   hEtCorrectionUnc->Draw("col text");
   hEtCorrectionUnc->SetXTitle("Unconverted Photon        Centrality Bin");
   hEtCorrectionUnc->SetYTitle("Photon P_{T} GeV/c");
   
   TCanvas *c3 = new TCanvas("c3","",800,600);
   hEtCorrection->Draw("col text");
   hEtCorrection->SetXTitle("Converted Photon        Centrality Bin");
   hEtCorrection->SetYTitle("Photon P_{T} GeV/c");
   outfile->Write();
}
