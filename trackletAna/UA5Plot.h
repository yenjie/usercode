#include <TH1F.h>

TH1F* getUA5()
{


TH1F* hEta_UA5 = new TH1F("hEta_UA5","hEta_UA5",600,-3.,3.);

hEta_UA5->SetBinContent(hEta_UA5->FindBin(0.125),3.1);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(0.375),3.05);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(0.625),3.1);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(0.875),3.3);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(1.125),3.3);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(1.375),3.5);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(1.625),3.45);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(1.875),3.4);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(2.125),3.45);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(2.375),3.4);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(2.625),3.3);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(2.875),3.05);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(3.125),2.9);

hEta_UA5->SetBinContent(hEta_UA5->FindBin(-0.125),3.1);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-0.375),3.05);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-0.625),3.1);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-0.875),3.3);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-1.125),3.3);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-1.375),3.5);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-1.625),3.45);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-1.875),3.4);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-2.125),3.45);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-2.375),3.4);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-2.625),3.3);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-2.875),3.05);
hEta_UA5->SetBinContent(hEta_UA5->FindBin(-3.125),2.9);
hEta_UA5->SetMarkerColor(kGreen-2);
hEta_UA5->SetMarkerSize(0.5);
hEta_UA5->SetLineColor(2);
hEta_UA5->SetMarkerStyle(4);

hEta_UA5->SetLineColor(2);
hEta_UA5->SetMarkerStyle(4);
return hEta_UA5;
}

TH1F *getUA5NSD()
{
 TH1F* hEta_UA5_NSD = new TH1F("hEta_UA5_NSD",";#eta;dN/d#eta",
50,-3,3);

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

//   hEta_UA5_NSD->SetMarkerStyle(25);
//   hEta_UA5_NSD->SetMarkerSize(1.1);
 hEta_UA5_NSD->SetMarkerColor(kGreen-2);
 hEta_UA5_NSD->SetMarkerSize(0.5);
 hEta_UA5_NSD->SetLineColor(kGreen-2);
 hEta_UA5_NSD->SetMarkerStyle(4);
 return hEta_UA5_NSD;
}

TH1F *getUA5NSDScaled()
{
 TH1F* hEta_UA5_NSD = new TH1F("hEta_UA5_NSDScale",";#eta;dN/d#eta", 50,-3,3);
   //scaleFactor = 1.23625;
   double scaleFactor = 4.23/3.423;

   // positive eta
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*0.125),scaleFactor*3.48);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*0.375),scaleFactor*3.38);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*0.625),scaleFactor*3.52);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*0.875),scaleFactor*3.68);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*1.125),scaleFactor*3.71);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*1.375),scaleFactor*3.86);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*1.625),scaleFactor*3.76);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*1.875),scaleFactor*3.66);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*2.125),scaleFactor*3.72);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*2.375),scaleFactor*3.69);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*2.625),scaleFactor*3.56);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*2.875),scaleFactor*3.41);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*3.125),scaleFactor*3.15);

   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*0.125),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*0.375),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*0.625),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*0.875),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*1.125),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*1.375),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*1.625),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*1.875),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*2.125),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*2.375),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*2.625),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*2.875),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*3.125),scaleFactor*0.07);

   //negative eta
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-0.125),scaleFactor*3.48);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-0.375),scaleFactor*3.38);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-0.625),scaleFactor*3.52);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-0.875),scaleFactor*3.68);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-1.125),scaleFactor*3.71);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-1.375),scaleFactor*3.86);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-1.625),scaleFactor*3.76);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-1.875),scaleFactor*3.66);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-2.125),scaleFactor*3.72);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-2.375),scaleFactor*3.69);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-2.625),scaleFactor*3.56);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-2.875),scaleFactor*3.41);
   hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.1417*-3.125),scaleFactor*3.15);

   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-0.125),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-0.375),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-0.625),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-0.875),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-1.125),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-1.375),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-1.625),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-1.875),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-2.125),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-2.375),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-2.625),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-2.875),scaleFactor*0.07);
   hEta_UA5_NSD->SetBinError(hEta_UA5_NSD->FindBin(1.1417*-3.125),scaleFactor*0.07);

//   hEta_UA5_NSD->SetMarkerStyle(25);
//   hEta_UA5_NSD->SetMarkerSize(1.1);
 hEta_UA5_NSD->SetMarkerColor(kBlue-2);
 hEta_UA5_NSD->SetMarkerSize(0.5);
 hEta_UA5_NSD->SetLineColor(kBlue-2);
 hEta_UA5_NSD->SetMarkerStyle(4);
 return hEta_UA5_NSD;
}


TH1F* tracklet900GeV()
{
   Double_t xAxis5[13] = {-3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3};

   TH1F *hMeasuredFinal = new TH1F("hMeasuredFinal","",12, xAxis5);


   hMeasuredFinal->SetBinContent(7,(3.44304+3.40686)/2);
   hMeasuredFinal->SetBinContent(8,(3.61126+3.61123)/2);
   hMeasuredFinal->SetBinContent(9,(3.64045+3.77519)/2);
   hMeasuredFinal->SetBinContent(10,(3.69915+3.49038)/2);
   hMeasuredFinal->SetBinContent(6,(3.44304+3.40686)/2);
   hMeasuredFinal->SetBinContent(5,(3.61126+3.61123)/2);
   hMeasuredFinal->SetBinContent(4,(3.64045+3.77519)/2);
   hMeasuredFinal->SetBinContent(3,(3.69915+3.49038)/2);


   hMeasuredFinal->SetBinError(3,0.132634);
   hMeasuredFinal->SetBinError(4,0.143457);
   hMeasuredFinal->SetBinError(5,0.137227);
   hMeasuredFinal->SetBinError(6,0.129461);
   hMeasuredFinal->SetBinError(7,0.130835);
   hMeasuredFinal->SetBinError(8,0.137228);
   hMeasuredFinal->SetBinError(9,0.138337);
   hMeasuredFinal->SetBinError(10,0.140568);

   
   hMeasuredFinal->SetMarkerColor(kBlue);
   hMeasuredFinal->SetMarkerStyle(4);
   hMeasuredFinal->SetLineColor(4);
   hMeasuredFinal->SetMarkerSize(0.7); // use rootlogon size

   return hMeasuredFinal;
}


TH1F* tracklet2360GeVHF1()
{

   Double_t xAxis3[13] = {-3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3}; 
   
   TH1F *hAvg = new TH1F("hAvg","",12, xAxis3);
   hAvg->SetBinContent(3,4.802082);
   hAvg->SetBinContent(4,4.658647);
   hAvg->SetBinContent(5,4.569177);
   hAvg->SetBinContent(6,4.405703);
   hAvg->SetBinContent(7,4.405703);
   hAvg->SetBinContent(8,4.569177);
   hAvg->SetBinContent(9,4.658647);
   hAvg->SetBinContent(10,4.802082);
   hAvg->SetBinError(3,0.1824791);
   hAvg->SetBinError(4,0.1770286);
   hAvg->SetBinError(5,0.1736287);
   hAvg->SetBinError(6,0.1674167);
   hAvg->SetBinError(7,0.1674167);
   hAvg->SetBinError(8,0.1736287);
   hAvg->SetBinError(9,0.1770286);
   hAvg->SetBinError(10,0.1824791);
   hAvg->SetMinimum(0);
   hAvg->SetMaximum(5.5);
   hAvg->SetEntries(194100.4);
   hAvg->SetFillColor(1);
   hAvg->SetFillStyle(0);
   hAvg->SetLineStyle(0);
   hAvg->SetMarkerStyle(20);
   hAvg->GetXaxis()->SetTitle("#eta");
   hAvg->GetXaxis()->CenterTitle(true);
   hAvg->GetXaxis()->SetLabelFont(42);
   hAvg->GetXaxis()->SetLabelOffset(0.01);
   hAvg->GetXaxis()->SetLabelSize(0.045);
   hAvg->GetXaxis()->SetTitleSize(0.055);
   hAvg->GetXaxis()->SetTitleFont(42);
   hAvg->GetYaxis()->SetTitle("dN/d#eta");
   hAvg->GetYaxis()->CenterTitle(true);
   hAvg->GetYaxis()->SetLabelFont(42);
   hAvg->GetYaxis()->SetLabelOffset(0.01);
   hAvg->GetYaxis()->SetLabelSize(0.045);
   hAvg->GetYaxis()->SetTitleSize(0.055);
   hAvg->GetYaxis()->SetTitleOffset(1.25);
   hAvg->GetYaxis()->SetTitleFont(42);
   hAvg->GetZaxis()->SetLabelFont(42);
   hAvg->GetZaxis()->SetLabelSize(0.045);
   hAvg->GetZaxis()->SetTitleFont(42);

   hAvg->SetMarkerColor(kBlue);
   hAvg->SetMarkerStyle(4);
   hAvg->SetLineColor(4);
   hAvg->SetMarkerSize(0.7); // use rootlogon size
   return hAvg;
}
