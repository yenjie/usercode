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

void scaleBin(TH1F *h,int i,double width)
{
   double val = h->GetBinContent(i)/width;
   double valErr = h->GetBinError(i)/width;
   h->SetBinContent(i,val);
   h->SetBinError(i,valErr);
}


double doFit(parameter par, double minEt, double maxEt, double &result, double &resultErr)
{
   static int count=0;
   count++;

   // Selection cut
   TCut etCut = Form("abs(eta)<1.44&&(et)>%f&&(et)<%f",minEt,maxEt);
   TCut sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0";

   char *title = Form("%.0f<E_{T}<%.0f GeV",minEt,maxEt);

   // Histograms
   TH1F *hData = new TH1F(Form("hData%d",count),"",par.nBin,par.binL,par.binH);
   TH1F *hSig = new TH1F(Form("hSig%d",count),"",par.nBin,par.binL,par.binH);
   TH1F *hBck = new TH1F(Form("hBck%d",count),"",par.nBin,par.binL,par.binH);

   par.tData->Draw(Form("%s>>hData%d",par.var,count),etCut&&par.selectionCut&&par.dataCut);
   par.tSig->Draw(Form("%s>>hSig%d",par.var,count),   (etCut&&par.selectionCut&&sigCut&&par.dataCut)*"cBinWeight");
   // par.tBck->Draw(Form("%s>>hBck%d",par.var,count),   etCut&&par.selectionCut&&!sigCut&&par.dataCut);//*"(49.7014*(exp((-0.943492e-3)*cBin*cBin*cBin+0.09106*cBin*cBin-0.44891*cBin+2.2789))+31.5117)");
   par.tData->Draw(Form("%s>>hBck%d",par.var,count), etCut&&par.selectionCut&&par.dataSidebandCut);

   TH1F *hFinal = (TH1F*)hData->Clone();
   hFinal->SetName(Form("hFinal%d",count));

   histFunction2 *myfun = new histFunction2(hSig,hBck);
   TF1 *f = new TF1("f",myfun,&histFunction2::evaluate,par.binL,par.binH,2);
   f->SetParameters(hData->GetEntries(),0.9);
   f->SetParLimits(1,0,1);
   hData->Fit("f","LL m");
   hData->Draw("pe");
   
   result=0;
   resultErr=0;
   if (hData->GetEntries()==0) return 0;
   
   double nev = f->GetParameter(0);
   double ratio = f->GetParameter(1);
   double ratioErr = f->GetParError(1);

   mcStyle(hSig);
   sbStyle(hBck);
   TH1F *hSigPdf = (TH1F*)hSig->Clone();
   hSigPdf->SetName(Form("hSigPdf%d",count));
   hSigPdf->Scale(nev*ratio/hSigPdf->GetEntries());

   TH1F *hBckPdf = (TH1F*)hBck->Clone();
   hBckPdf->SetName(Form("hBckPdf%d",count));
   hBckPdf->Scale(nev*(1-ratio)/hBckPdf->GetEntries());
//   hSigPdf->SetLineColor(1);
   hSigPdf->Add(hBckPdf);

   hSigPdf->SetXTitle(title);
   hSigPdf->SetNdivisions(505);
   hFinal->SetXTitle(par.xTitle);

   hFinal->SetYTitle("Entries");

   hFinal->Draw("e");

/*
   hBckPdf->SetFillColor(4);
   hBckPdf->SetLineColor(4);
   hBckPdf->SetFillStyle(3004);*/
   
   hSigPdf->Draw("hist same");
   hBckPdf->Draw("hist same");
   
   TLegend *leg ;
   leg = new TLegend(0.5,0.7,0.9,0.9);

   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->AddEntry(hFinal,title,"");
   leg->AddEntry(hFinal,"Data","pl");
   leg->AddEntry(hSigPdf,"Signal+Background","l");
   leg->AddEntry(hBckPdf,"Background","l");
   leg->Draw();

   if (ratio>0) {
     result = nev*ratio;
     resultErr = nev*ratio*sqrt(1./nev+(ratioErr/ratio)*(ratioErr/ratio));
     cout <<minEt<<" "<<maxEt<<"Nsig : "<<result<<" +- "<<resultErr<<endl;
   }
   
   
   return 1;
}

int applyCorrectionTable(char* infname = "data/mpaData_march17th.root")
{

   // Data sample
   TFile *infData = new TFile(infname);
   TTree *tData = (TTree*)infData->FindObjectAny("Analysis");
   TTree *tDataGen = (TTree*)infData->FindObjectAny("Generator");
   TTree* newtree = tData->CloneTree(0);

   // Correction file
   
   TFile *correctionFile = new TFile("correction/correction-PYTHIAData.root");
   TH2D *hEtCorrection = (TH2D*)correctionFile->FindObjectAny("hEtCorrection");
   
   // Output file
   TFile *outfile = new TFile("output.root","recreate");

   cout <<"Add correctedEt to data ..."<<endl;
   addCorrectedEtFriend(tData,hEtCorrection);
   

   
   if (tDataGen) {
      TTree* newtreeGen = tDataGen->CloneTree(0);
   }


//   tData->Draw("et:etCorrected");
//   tData->AutoSave();
//   tData->Write();
   outfile->Write();   
   outfile->Close();   
   
}

