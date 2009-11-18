#include <iostream>
#include <TFile.h>
#include <TNtuple.h>
#include <TMatrixD.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLine.h>
#include <TCanvas.h>
#include "rootTool/histFunction2D.C"

void unfoldPt(int mode=0)
{

   // Matched Tracklets
   TFile *inf = new TFile("match-10TeV-12.root");
   TNtuple *nt = (TNtuple*)inf->FindObjectAny("nt");

   // Test sample
   TFile *infTest = new TFile("TrackletTree-7TeV-Herwig-Test.root");
   TNtuple *ntTest = (TNtuple*)infTest->FindObjectAny("TrackletTree12");
   
   TFile *pdfFile;
   if (mode==0) pdfFile = new TFile("pdf.root","recreate");
           else pdfFile = new TFile("pdf.root");

   double nPtBin=15;
   double minPt=log(0.05);
   double maxPt=log(10);
   double nDphiBin=600;
   double maxDphi=0.4;
   
   char* mycut = Form("abs(eta)<2&&pt>%f&&pt<%f",minPt,maxPt);
   char* mycut1=Form("abs(eta)<2&&pt>%f&&pt<%f&&abs(eta-eta1)<0.01&&abs(deta)<0.01",minPt,maxPt);

   TH2F *h;
   TH1F *hdphi = new TH1F("hdphi","",nDphiBin,0,maxDphi);
   TH1F *hdphi2;
   TH1F *hpt;
   TH1F *hptH = new TH1F("hptH","",nPtBin,minPt,maxPt);
   
   TH1F *hptUnfold = new TH1F("hptUnfold","",nPtBin,minPt,maxPt);
   TH1F *hptMC = new TH1F("hptMC","",nPtBin,minPt,maxPt);
   TH1F *hptTemp = new TH1F("hptTemp","",nPtBin,minPt,maxPt);

   // Delta phi as a function of matched genparticle transverse momentum
   TCanvas *c = new TCanvas("c","",600,600);
   
   if (mode == 0) {
      h = new TH2F("h","",nPtBin,minPt,maxPt,nDphiBin,0,maxDphi);
      hdphi2 = new TH1F("hdphiMC","",nDphiBin,0,maxDphi);
      hpt = new TH1F("hpt","",nPtBin,minPt,maxPt);      
      h->SetXTitle("ln(P_{T}) GeV/c");
      h->SetYTitle("|#Delta#phi|");
      nt->Draw("abs(dphi):log(pt)>>h",mycut1,"col");
      // used to generate pdf
      nt->Draw("abs(dphi)>>hdphiMC",mycut,"");
      nt->Draw("log(pt)>>hpt",mycut,"");
      h->Write();      
      hpt->Write();      
      hdphi2->Write();      
   } else {
      h = (TH2F*) pdfFile->FindObjectAny("h");
      hdphi2 = (TH1F*) pdfFile->FindObjectAny("hdphiMC");
      hpt = (TH1F*) pdfFile->FindObjectAny("hpt");
   }
   // Delta phi fit
   TCanvas *c2 = new TCanvas("c2","",600,600);
   c2->SetLogy();
   c2->SetLogx();

 
   // dphi for unfolding and MC truth:  
   ntTest->Draw("abs(dphi)>>hdphi","abs(eta1)<2&&abs(deta)<0.1","",200000);
   ntTest->Draw("log(pt)>>hptH",mycut,"",200000);
   
   histFunction2D *myfun = new histFunction2D(h);
   
   TF1 *test = new TF1("histFun",myfun,&histFunction2D::evaluate,0,maxDphi,nPtBin+1);
   TF1 *test2 = new TF1("histFunMC",myfun,&histFunction2D::evaluate,0,maxDphi,nPtBin+1);

   for (int i=0;i<nPtBin+1;i++)
   {  
      test->SetParameter(i,1);   
   }


   hdphi2->SetXTitle("|#Delta#phi|");
   hdphi2->SetYTitle("Arbitrary Normalization");
   hdphi2->Fit("histFunMC","M");

   hdphi->SetXTitle("|#Delta#phi|");
   hdphi->SetYTitle("Arbitrary Normalization");
   hdphi->Fit("histFun","M");
   hdphi->SetStats(0);
   hdphi->Draw();

   
   for (int i=0;i<nPtBin+1;i++) {
      TF1 *testPlot = new TF1(Form("histFun%d",i),myfun,&histFunction2D::evaluate,0,maxDphi,nPtBin+1);

      testPlot->SetParameter(i,test->GetParameter(i));
      testPlot->SetLineColor(i+2);
      testPlot->Draw("same");
   }
   
   int total=0,totalMC=0;


   for (int i=0;i<nPtBin;i++){
      if (test->GetParameter(i)==0) continue;
      hptUnfold->SetBinContent(i+1,fabs(test->GetParameter(i)));
      hptUnfold->SetBinError(i+1,test->GetParError(i));

      hptMC->SetBinContent(i+1,fabs(test2->GetParameter(i)));
      hptMC->SetBinError(i+1,test2->GetParError(i));

      total+=fabs(test->GetParameter(i));
      totalMC+=fabs(test2->GetParameter(i));
   }

   hptUnfold->SetEntries(total);
   hptMC->SetEntries(totalMC);
   
   TCanvas *c3 = new TCanvas("c3","",600,600);
   hpt->Sumw2();
   hptH->Sumw2();
   //hptMC->Sumw2();
   
   double normMC=0;
   double norm=0;
   double normTruth=0;
   

   hptUnfold->SetMarkerColor(2);
   hptUnfold->SetMarkerStyle(4);
//   hptUnfold->Scale(1./hptUnfold->GetEntries());
   TH1F *hptCorrected = (TH1F*)hptUnfold->Clone();
   hptCorrected->SetName("hptCorrected");
   hptMC->Divide(hpt);
   hptCorrected->Divide(hptMC);
   
   for (int i=0;i<nPtBin;i++){
      if (hptMC->GetBinContent(i)<=0.001)hptCorrected->SetBinContent(i,0);
   }
   hptCorrected->Scale(1./(hptCorrected->GetSum()));
   hptCorrected->SetMarkerStyle(20);

   hpt->Scale(1./hpt->GetEntries());
   hptH->Scale(1./hptH->GetEntries());

   hptTemp->SetXTitle("ln(P_{T}) GeV/c");
   hptTemp->SetYTitle("Arbitrary Normalization");
   hptTemp->Draw();
   

   hptH->SetXTitle("ln(P_{T}) GeV/c");
   hptH->SetYTitle("Arbitrary Normalization");
   hptH->Draw("hist");
   hptH->SetLineColor(4);
   
   hpt->Draw("hist same ");
   
   hptCorrected->Draw("same");
   
   TH1F *hptUnfoldRatio = (TH1F*)hptUnfold->Clone();
   hptUnfoldRatio->SetName("hptUnfoldRatio");
   hptUnfoldRatio->Scale(1./hptUnfoldRatio->GetSum());
   hptUnfoldRatio->Divide(hptH);

   TH1F *hptCorrectedRatio = (TH1F*)hptCorrected->Clone();
   hptCorrectedRatio->SetName("hptCorrectedRatio");
   hptCorrectedRatio->SetMarkerColor(2);
   hptCorrectedRatio->Divide(hptH);

   TCanvas *c4 = new TCanvas("c4","",600,600);
   TLine *l = new TLine(-2.5,1,2.5,1);
   hptUnfoldRatio->Draw();
   hptMC->Draw("same");
   hptCorrectedRatio->Draw("same");
   l->Draw("same");
}
