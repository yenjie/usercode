// For plotting
#include "GraphErrorsBand.h"
#include "common.h"
// Standard library
#include <cmath>
#include <iostream>
#include <fstream>

// ROOT Library
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TLine.h>
#include <TF1.h>
#include <TCut.h>
#include <TPad.h>
#include <TText.h>
#include <TLatex.h>
#include <TLegendEntry.h>

//void Draw(){
void dNdEta_ThreeMethodsCombined_BU_Compile(){
//=========Macro generated from canvas: MyCanvas/MyCanvas
//=========  (Thu Dec 10 11:52:00 2009) by ROOT version5.22/00d

   gROOT->Reset();
   gROOT->ProcessLine(".x rootlogon.C");

   TCanvas *MyCanvas = new TCanvas("MyCanvas", "Final result",1,360,550,600);
   /// ==================================================== Weighted mean of all three method!

   Double_t xAxis7[13] = {-3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3};
   TH1 *hMeasuredFinal3 = new TH1D("hMeasuredFinal3","",12, xAxis7);
   
   hMeasuredFinal3->GetXaxis()->SetRange(1,13);

   hMeasuredFinal3->SetMinimum(0);
   hMeasuredFinal3->SetMaximum(6.0);

   hMeasuredFinal3->SetBinContent(2,3.60278);  // -2.4 to -2.0    
   hMeasuredFinal3->SetBinContent(3,3.731399616); //-2 to -1.5 
   hMeasuredFinal3->SetBinContent(4,3.618227783);        // -1.5 to -1.0 
   hMeasuredFinal3->SetBinContent(5,3.538766473); // -1.0 to -0.5 
   hMeasuredFinal3->SetBinContent(6,3.477522368);        // -0.5 to 0

   /*
   hMeasuredFinal3->SetBinContent(2,3.65413);  // -2.4 to -2.0    
   hMeasuredFinal3->SetBinContent(3,3.744095779); //-2 to -1.5 
   hMeasuredFinal3->SetBinContent(4,3.726157315);        // -1.5 to -1.0 
   hMeasuredFinal3->SetBinContent(5,3.6087225); // -1.0 to -0.5 
   hMeasuredFinal3->SetBinContent(6,3.489931761);        // -0.5 to 0
   */


   /*
   hMeasuredFinal3->SetBinContent(2,3.6541);  // -2.4 to -2.0 
   hMeasuredFinal3->SetBinContent(3,3.7376); //-2 to -1.5 
   hMeasuredFinal3->SetBinContent(4,3.7255);        // -1.5 to -1.0 
   hMeasuredFinal3->SetBinContent(5,3.6123); // -1.0 to -0.5
   hMeasuredFinal3->SetBinContent(6,3.4977);        // -0.5 to 0
   */

   hMeasuredFinal3->SetBinContent(7,hMeasuredFinal3->GetBinContent(6)); // 0 to 0.5 
   hMeasuredFinal3->SetBinContent(8,hMeasuredFinal3->GetBinContent(5)); // 0.5 to 1.0
   hMeasuredFinal3->SetBinContent(9,hMeasuredFinal3->GetBinContent(4)); // 1.0 to 1.5 
   hMeasuredFinal3->SetBinContent(10,hMeasuredFinal3->GetBinContent(3)); // 1.5 to 2.0 
   hMeasuredFinal3->SetBinContent(11,hMeasuredFinal3->GetBinContent(2)); // 2.0 to 2.4 


   // --- Stat error ---
   double statError= 0.01767767;
   

   for (int i=2; i<12; ++i) {
     hMeasuredFinal3->SetBinError(i,hMeasuredFinal3->GetBinContent(i)*statError);
     cout << "stat error (" << i << "): " << hMeasuredFinal3->GetBinError(i) << endl;
   }

   hMeasuredFinal3->GetYaxis()->SetTitle("dN_{ch}/d#eta");
   hMeasuredFinal3->GetXaxis()->SetTitle("#eta");
   hMeasuredFinal3->GetXaxis()->CenterTitle();
   hMeasuredFinal3->GetYaxis()->CenterTitle();


   hMeasuredFinal3->GetXaxis()->SetNdivisions(405);
   //hMeasuredFinal3->GetYaxis()->SetNdivisions(1005);
   hMeasuredFinal3->GetYaxis()->SetNdivisions(506);


   hMeasuredFinal3->SetMarkerColor(kRed);
   hMeasuredFinal3->SetMarkerStyle(20);
   hMeasuredFinal3->SetLineColor(kRed);
   hMeasuredFinal3->SetMarkerSize(1.8); 
   
   hMeasuredFinal3->Draw("pz");

   // Clone above for systematic band!
   /// ==================================================== Weighted mean of all three method! 
   //Double_t xAxis8[13] = {-3, -2.4, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.4, 3};
   TH1D * hMeasuredFinal4 = (TH1D*)hMeasuredFinal3->Clone("hMeasuredFinal4");
   //   double sysError = 0.0603;
   //double sysError = 0.035;
   double sysError = 0.037;
   

   TGraph *gErrorBand = GetErrorBand((TH1F*)hMeasuredFinal4,sysError,sysError,0.25,0.06); 
   gErrorBand->Draw("f");

   hMeasuredFinal3->Draw("pzsame");  


   /// ==================================================== CMS 2.36 GeV
   TH1F *hMeasuredFinal236 = (TH1F*)hMeasuredFinal3->Clone("hMeasuredFinal236");
   //hMeasuredFinal236->GetXaxis()->SetRange(3,10);
   hMeasuredFinal236->GetXaxis()->SetRange(1,13);
   hMeasuredFinal236->SetMarkerColor(kRed);
   hMeasuredFinal236->SetLineColor(kRed);
   hMeasuredFinal236->SetMarkerStyle(4);
   hMeasuredFinal236->SetMarkerSize(1.8);

   hMeasuredFinal236->SetBinContent(2,4.78296);  // -2.4 to -2.0  
   hMeasuredFinal236->SetBinContent(3,4.806189822); //-2 to -1.5
   hMeasuredFinal236->SetBinContent(4,4.662502625);        // -1.5 to -1.0 
   hMeasuredFinal236->SetBinContent(5,4.614742045); // -1.0 to -0.5 
   hMeasuredFinal236->SetBinContent(6,4.469256947);        // -0.5 to 0

   /*
   hMeasuredFinal236->SetBinContent(2,4.968905);  // -2.4 to -2.0  
   hMeasuredFinal236->SetBinContent(3,4.906105606); //-2 to -1.5
   hMeasuredFinal236->SetBinContent(4,4.695318198);        // -1.5 to -1.0 
   hMeasuredFinal236->SetBinContent(5,4.635741157); // -1.0 to -0.5 
   hMeasuredFinal236->SetBinContent(6,4.495389263);        // -0.5 to 0
   */
   /*
   hMeasuredFinal236->SetBinContent(2,4.9689);  // -2.4 to -2.0 
   hMeasuredFinal236->SetBinContent(3,4.9122); //-2 to -1.5 
   hMeasuredFinal236->SetBinContent(4,4.7081);        // -1.5 to -1.0 
   hMeasuredFinal236->SetBinContent(5,4.6404); // -1.0 to -0.5
   hMeasuredFinal236->SetBinContent(6,4.4921);        // -0.5 to 0
   */

   hMeasuredFinal236->SetBinContent(7,hMeasuredFinal236->GetBinContent(6)); // 0 to 0.5 
   hMeasuredFinal236->SetBinContent(8,hMeasuredFinal236->GetBinContent(5)); // 0.5 to 1.0
   hMeasuredFinal236->SetBinContent(9,hMeasuredFinal236->GetBinContent(4)); // 1.0 to 1.5 
   hMeasuredFinal236->SetBinContent(10,hMeasuredFinal236->GetBinContent(3)); // 1.5 to 2.0 
   hMeasuredFinal236->SetBinContent(11,hMeasuredFinal236->GetBinContent(2)); // 2.0 to 2.4 

   /// --- draw the error bands ---
   TH1F *hMeasuredFinal236EB = (TH1F*)hMeasuredFinal236->Clone("hMeasuredFinal236EB");
   //hMeasuredFinal236EB->SetBinContent(2,4.32647);
   //hMeasuredFinal236EB->SetBinContent(11,4.32647);
   TGraph *gErrorBand236 = GetErrorBand((TH1F*)hMeasuredFinal236EB,sysError,sysError,0.25);
   gErrorBand236->Draw("f");

   hMeasuredFinal236->Draw("pzsame");



   /// ====================================================  ALICE 
   //Double_t xAxis9[9] = {-1.4, -1.0, -0.6, -0.2, 0.2, 0.6, 1.0, 1.4};
   //Double_t xAxis9[9] = {-1.4, -1.0, -0.6, -0.2, 0.2, 0.6, 1.0, 1.4};
   Double_t xAxis9[9] = {-1.6, -1.2, -0.8, -0.4, 0, 0.4, 0.8, 1.2, 1.6};
   TH1 *hEta_ALICE_NSD = new TH1D("hEta_ALICE_NSD","",8, xAxis9);

   // ALICE data points! 
   /*
     ALICE, NSD:
     -1.400000: 3.880678  +- 0.448263
     -1.000000: 3.863622  +- 0.279495
     -0.600000: 3.466114  +- 0.235660
     -0.200000: 3.408768  +- 0.233457
     0.200000: 3.619309  +- 0.241938
     0.600000: 3.905048  +- 0.251426
     1.000000: 3.590869  +- 0.249225
     1.400000: 3.592486  +- 0.320071
   */

   hEta_ALICE_NSD->SetBinContent(1,3.880678);
   hEta_ALICE_NSD->SetBinContent(2,3.863622);
   hEta_ALICE_NSD->SetBinContent(3,3.466114);
   hEta_ALICE_NSD->SetBinContent(4,3.408768);
   hEta_ALICE_NSD->SetBinContent(5,3.619309);
   hEta_ALICE_NSD->SetBinContent(6,3.905048);
   hEta_ALICE_NSD->SetBinContent(7,3.590869);
   hEta_ALICE_NSD->SetBinContent(8,3.592486);

   hEta_ALICE_NSD->SetBinError(1,0.448263);
   hEta_ALICE_NSD->SetBinError(2,0.279495);
   hEta_ALICE_NSD->SetBinError(3,0.235660);
   hEta_ALICE_NSD->SetBinError(4,0.233457);
   hEta_ALICE_NSD->SetBinError(5,0.241938);
   hEta_ALICE_NSD->SetBinError(6,0.251426);
   hEta_ALICE_NSD->SetBinError(7,0.249225);
   hEta_ALICE_NSD->SetBinError(8,0.320071);


   hEta_ALICE_NSD->SetMarkerColor(1);
   hEta_ALICE_NSD->SetMarkerStyle(26);
   hEta_ALICE_NSD->SetLineColor(kBlack);
   hEta_ALICE_NSD->SetMarkerSize(1.5);
   //hEta_ALICE_NSD->SetMarkerSize(1.25);
   hEta_ALICE_NSD->Draw("pzsame");


   /// ====================================================  UA5 Data

   //TH1F* hEta_UA5_NSD = new TH1F("hEta_UA5_NSD",";#eta;dN/d#eta",50,-3,3);
   //TH1F* hEta_UA5_NSD = new TH1F("hEta_UA5_NSD",";#eta;dN/d#eta",100,-3,3);
   TH1F* hEta_UA5_NSD = new TH1F("hEta_UA5_NSD",";#eta;dN/d#eta",26,-3.25,3.25);  

   // positive eta
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.125),3.48);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.375),3.38);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.625),3.52);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.875),3.68);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.125),3.71);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.375),3.86);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.625),3.76);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.875),3.66);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.125),3.72);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.375),3.69);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.625),3.56);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.875),3.41);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(3.125),3.15);

   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(0.125),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(0.375),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(0.625),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(0.875),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.125),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.375),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.625),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.875),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(2.125),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(2.375),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(2.625),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(2.875),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(3.125),0.07);


   //negative eta
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.125),3.48);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.375),3.38);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.625),3.52);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.875),3.68);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.125),3.71);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.375),3.86);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.625),3.76);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.875),3.66);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.125),3.72);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.375),3.69);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.625),3.56);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.875),3.41);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-3.125),3.15);

   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-0.125),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-0.375),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-0.625),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-0.875),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-1.125),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-1.375),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-1.625),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-1.875),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-2.125),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-2.375),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-2.625),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-2.875),0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(-3.125),0.07);

   hEta_UA5_NSD->SetMarkerStyle(25);
   hEta_UA5_NSD->SetMarkerSize(1.25);
   hEta_UA5_NSD->Draw("psame");

   // Draw one more time to put CMS point on top of every other experiemnt's data points!
   hMeasuredFinal3->Draw("pzsame");


   
   Float_t ywidth = 0.045*5;

   //TLegend *leg = new TLegend(0.20,0.27,0.53,0.27+ywidth,"","brNDC");   
   //TLegend *leg = new TLegend(0.48,0.27,0.81,0.27+ywidth,"","brNDC");
   TLegend *leg = new TLegend(0.27,0.21,0.699,0.21+ywidth,NULL,"brNDC");
   leg->SetMargin(0.37);
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->SetTextSize(0.033);  
   leg->SetHeader("  0.9 TeV");
   
   //leg->AddEntry(hMeasuredFinal236,"CMS NSD (2.36 TeV)","P");
   leg->AddEntry(hMeasuredFinal3,"","P");
   leg->AddEntry(hEta_ALICE_NSD,"","P");
   leg->AddEntry(hEta_UA5_NSD,"","P");
   
   Float_t ywidth2 = 0.045*2;
   
   //TLegend *leg2 = new TLegend(0.38,0.27,0.81,0.27+ywidth2,"","brNDC");
   TLegend *leg2 = new TLegend(0.39,0.21,0.82,0.21+ywidth,NULL,"brNDC");

   leg2->SetMargin(0.5);
   leg2->SetBorderSize(0);
   leg2->SetTextFont(62);
   leg2->SetLineColor(1);
   leg2->SetLineStyle(1);
   leg2->SetLineWidth(1);
   leg2->SetFillColor(0);
   //leg->SetFillStyle(1001);
   leg2->SetFillStyle(0);
   leg2->SetTextSize(0.033);
   leg2->SetHeader("     2.36 TeV");
   leg2->AddEntry(hMeasuredFinal236,"CMS NSD","P");
   leg2->AddEntry(hEta_ALICE_NSD,"ALICE NSD","");
   leg2->AddEntry(hEta_UA5_NSD,"UA5 NSD","");


   leg2->Draw();
   leg->Draw();


   printFinalCanvases(MyCanvas,"dNdeta_ThreeMethodsCombined",0,2);
}
