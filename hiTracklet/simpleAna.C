#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>

void simpleAna(char *infile,TCut myCut = "1==1",char *title="")
{
   TFile *infMC = new TFile("hydjet/PixelTree-HYDJET-15k.root");
   TTree *tMC = (TTree*) infMC->FindObjectAny("PixelTree");

   TFile *infData = new TFile(infile);
   TTree *tData = (TTree*) infData->FindObjectAny("PixelTree");

   TH1D *hMC1 = new TH1D("hMC1","",12,-3,3);
   TH1D *hData1 = new TH1D("hData1","",12,-3,3);
   TH1D *hMC2 = new TH1D("hMC2","",12,-3,3);
   TH1D *hData2 = new TH1D("hData2","",12,-3,3);
   TH1D *hMC3 = new TH1D("hMC3","",12,-3,3);
   TH1D *hData3 = new TH1D("hData3","",12,-3,3);

   TH1D *hMCdNdeta = new TH1D("hMCdNdeta","",12,-3,3);
   TH1D *hDatadNdetaTruth = new TH1D("hDatadNdetaTruth","",12,-3,3);

   TCut csCut1 = "(abs(eta1)<=0.5&&cs1>=1)||(abs(eta1)>0.5&&abs(eta1)<=1.0&&cs1>=2)||(abs(eta1)>1.0&&abs(eta1)<=1.5&&cs1>=3)||(abs(eta1)>1.5&&abs(eta1)<=2.0&&cs1>=4)||(abs(eta1)>2.0&&abs(eta1)<=2.5&&cs1>=6)||(abs(eta1)>2.5&&abs(eta1)<=5.0&&cs1>=9)";
   TCut csCut2 = "(abs(eta2)<=0.5&&cs2>=1)||(abs(eta2)>0.5&&abs(eta2)<=1.0&&cs2>=2)||(abs(eta2)>1.0&&abs(eta2)<=1.5&&cs2>=3)||(abs(eta2)>1.5&&abs(eta2)<=2.0&&cs2>=4)||(abs(eta2)>2.0&&abs(eta2)<=2.5&&cs2>=6)||(abs(eta2)>2.5&&abs(eta2)<=5.0&&cs2>=9)";
   TCut csCut3 = "(abs(eta3)<=0.5&&cs3>=1)||(abs(eta3)>0.5&&abs(eta3)<=1.0&&cs3>=2)||(abs(eta3)>1.0&&abs(eta3)<=1.5&&cs3>=3)||(abs(eta3)>1.5&&abs(eta3)<=2.0&&cs3>=4)||(abs(eta3)>2.0&&abs(eta3)<=2.5&&cs3>=6)||(abs(eta3)>2.5&&abs(eta3)<=5.0&&cs3>=9)";

   tMC->Draw("eta1>>hMC1","abs(vz[1])<4"&&csCut1&&myCut);
   tMC->Draw("eta2>>hMC2","abs(vz[1])<4"&&csCut2&&myCut);
   tMC->Draw("eta3>>hMC3","abs(vz[1])<4"&&csCut3&&myCut);
   int nEvMC = tMC->GetEntries("abs(vz[1])<4"&&myCut);
   hMC1->Sumw2();
   hMC1->Scale(2./nEvMC);
   hMC2->Sumw2();
   hMC2->Scale(2./nEvMC);
   hMC3->Sumw2();
   hMC3->Scale(2./nEvMC);

   tMC->Draw("eta>>hMCdNdeta","abs(vz[1])<4"&&myCut);
   hMCdNdeta->Sumw2();

   hMCdNdeta->Scale(2./nEvMC);

   tData->Draw("eta1>>hData1","abs(vz[1])<4"&&csCut1&&myCut);
   tData->Draw("eta2>>hData2","abs(vz[1])<4"&&csCut2&&myCut);
   tData->Draw("eta3>>hData3","abs(vz[1])<4"&&csCut3&&myCut);
   tData->Draw("eta>>hDatadNdetaTruth","abs(vz[1])<4"&&myCut);
   hData1->Sumw2();
   hData2->Sumw2();
   hData3->Sumw2();

   int nEvData = tData->GetEntries("abs(vz[1])<4"&&myCut);
   hData1->Scale(2./nEvData);
   hData2->Scale(2./nEvData);
   hData3->Scale(2./nEvData);
   hDatadNdetaTruth->Scale(2./nEvData);
   TCanvas *c = new TCanvas("c","",600,600);
   TH1D* hDatadNdeta1 = (TH1D*)hMCdNdeta->Clone();
   hDatadNdeta1->SetName("hDatadNdeta1");
   hDatadNdeta1->Divide(hMC1);
   hDatadNdeta1->Multiply(hData1);
   hDatadNdeta1->SetXTitle(Form("#eta %s",title));
   hDatadNdeta1->SetYTitle("dN/d#eta");
   
   TH1D* hDatadNdeta2 = (TH1D*)hMCdNdeta->Clone();
   hDatadNdeta2->SetName("hDatadNdeta2");
   hDatadNdeta2->Divide(hMC2);
   hDatadNdeta2->Multiply(hData2);
   hDatadNdeta2->SetXTitle(Form("#eta %s",title));
   hDatadNdeta2->SetYTitle("dN/d#eta");

   TH1D* hDatadNdeta3 = (TH1D*)hMCdNdeta->Clone();
   hDatadNdeta3->SetName("hDatadNdeta3");
   hDatadNdeta3->Divide(hMC3);
   hDatadNdeta3->Multiply(hData3);
   hDatadNdeta3->SetXTitle(Form("#eta %s",title));
   hDatadNdeta3->SetYTitle("dN/d#eta");

   for (int i=0;i<12;i++)
   {
       double val;
       val=hDatadNdeta1->GetBinContent(i+1);
       hDatadNdeta1->SetBinError(i+1,val*0.1);
       val=hDatadNdeta2->GetBinContent(i+1);
       hDatadNdeta2->SetBinError(i+1,val*0.1);
       val=hDatadNdeta3->GetBinContent(i+1);
       hDatadNdeta3->SetBinError(i+1,val*0.1);
       if (i<=0||i>=11)
       {
         hDatadNdeta1->SetBinContent(i+1,0);
         hDatadNdeta1->SetBinError(i+1,0);
         hDatadNdeta2->SetBinContent(i+1,0);
         hDatadNdeta2->SetBinError(i+1,0);
         hDatadNdeta3->SetBinContent(i+1,0);
         hDatadNdeta3->SetBinError(i+1,0);
       }
   }

   hDatadNdeta1->SetMarkerColor(2);
   hDatadNdeta1->SetLineColor(2);
   hDatadNdeta1->SetMarkerStyle(20);
   hDatadNdeta1->SetMarkerSize(1.7);
   hDatadNdeta2->SetMarkerColor(4);
   hDatadNdeta2->SetLineColor(4);
   hDatadNdeta2->SetMarkerStyle(25);
   hDatadNdeta2->SetMarkerSize(1.7);
   hDatadNdeta3->SetMarkerColor(1);
   hDatadNdeta3->SetLineColor(1);
   hDatadNdeta3->SetMarkerStyle(26);
   hDatadNdeta3->SetMarkerSize(1.7);
   hDatadNdeta1->Draw();
   hDatadNdeta2->Draw("same");
   hDatadNdeta3->Draw("same");
   if (hDatadNdetaTruth->GetEntries()) hDatadNdetaTruth->Draw("same hist");
   TH1D *hDatadNdetaAvg = (TH1D*)hDatadNdeta1->Clone();
   hDatadNdetaAvg->SetName("hDatadNdetaAvg");
      
   TLegend *leg = new TLegend(0.2,0.18,1,0.35,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62); 
   leg->SetLineColor(1); 
   leg->SetLineStyle(1); 
   leg->SetLineWidth(1); 
   leg->SetFillColor(0); 
   leg->SetFillStyle(0); 
   leg->AddEntry(hDatadNdeta1,"Scaled (1st layer)","pl");
   leg->AddEntry(hDatadNdeta2,"Scaled (2nd layer)","pl");
   leg->AddEntry(hDatadNdeta3,"Scaled (3rd layer)","pl");
   leg->Draw();
   c->SaveAs(Form("plot/SimpleAna-3Layers-%s.C",title));
   c->SaveAs(Form("plot/SimpleAna-3Layers-%s.gif",title));
   c->SaveAs(Form("plot/SimpleAna-3Layers-%s.eps",title));

   TCanvas *c2 = new TCanvas("c2","",600,600);
   hDatadNdetaAvg->Add(hDatadNdeta2);
   hDatadNdetaAvg->Add(hDatadNdeta3);
   hDatadNdetaAvg->Scale(1./3.);
   hDatadNdetaAvg->SetLineColor(1);
   hDatadNdetaAvg->SetMarkerColor(1);
   hDatadNdetaAvg->Draw();
   if (hDatadNdetaTruth->GetEntries()) hDatadNdetaTruth->Draw("same hist");

   c2->SaveAs(Form("plot/SimpleAna-Avg-%s.C",title));
   c2->SaveAs(Form("plot/SimpleAna-Avg-%s.gif",title));
   c2->SaveAs(Form("plot/SimpleAna-Avg-%s.eps",title));
}
