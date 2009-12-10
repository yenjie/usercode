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
 TH1F* hEta_UA5_NSD = new TH1F("hEta_UA5_NSD","hEta_UA5_NSD",600,-3.,3.);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.125),3.45);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.375),3.4);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.625),3.55);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(0.875),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.125),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.375),3.8);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.625),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(1.875),3.6);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.125),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.375),3.65);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.625),3.55);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(2.875),3.4);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(3.125),3.2);

 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.125),3.45);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.375),3.4);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.625),3.55);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-0.875),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.125),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.375),3.8);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.625),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-1.875),3.6);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.125),3.7);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.375),3.65);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.625),3.55);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-2.875),3.4);
 hEta_UA5_NSD->SetBinContent(hEta_UA5_NSD->FindBin(-3.125),3.2);
 hEta_UA5_NSD->SetMarkerColor(kGreen-2);
 hEta_UA5_NSD->SetMarkerSize(0.5);
 hEta_UA5_NSD->SetLineColor(2);
 hEta_UA5_NSD->SetMarkerStyle(4);
 return hEta_UA5_NSD;
}
