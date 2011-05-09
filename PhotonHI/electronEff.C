#include <TFile.h>
#include <TCut.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TCanvas.h>


TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2,char *hName="hEff")
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(h2,h1);
   return gEfficiency;
}


void electronEff(bool isEff=0){
//TFile *inf = new TFile("ampt/mpa_w2ev_mix_ampt_april15_correctedTree.root");
TFile *inf = new TFile("PythiaData/mpa_wEv_mix_hiData_correctedTree.root");
TTree *Analysis = (TTree*)inf->FindObjectAny("Analysis");
const int nEtBin = 5;
double myBin[nEtBin+1] = {20,25,30,40,50,80};

TCut  eventCut     = "( !TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && abs(vtxZ) <= 15)";
TCut   centralityCut = "cBin>=0&&cBin<40";
TCut   photonCut = "r9>0&&hadronicOverEm<0.2&&abs(eta)<1.44&&rawEnergy/energy>0.5\
                   ";   
TCut   removeElectronCut = "!isEle";
   TString swissCrx = "(1 - (eRight+eLeft+eTop+eBottom)/eMax)";
   TCut hiSpikeCut       = Form("(  %s < 0.90 && abs(seedTime)<3  && sigmaIetaIeta>0.002 )  || isEE",swissCrx.Data());
   
TCut   removeSpikeCut = hiSpikeCut;
  

TCut   selectionCut = photonCut&&centralityCut&&eventCut&&removeSpikeCut&&removeElectronCut; 
TCut   selectionCut1 = photonCut&&centralityCut&&eventCut&&removeSpikeCut; 
TCut   selectionCut2 =photonCut&&eventCut&&removeSpikeCut; 
TCut   selectionCut3 =photonCut&&eventCut; 


TCanvas *c = new TCanvas("c","",600,600);
TH1D *h = new TH1D("h","",nEtBin,myBin);
TH1D *h0 = new TH1D("h0","",nEtBin,myBin);
TH1D *h1 = new TH1D("h1","",nEtBin,myBin);
TH1D *h2 = new TH1D("h2","",nEtBin,myBin);
//Analysis->AddFriend("genParticleCounter/photon","ampt/genTree_w2ev_mix_ampt_april15.root");
Analysis->AddFriend("genParticleCounter/photon","PythiaData/genTree_wEv_mix_hiData.root");
Analysis->Draw("photon.et>>h","abs(photon.id)==11&&abs(photon.eta)<1.44&&photon.et>20&&status==1"&&centralityCut);
Analysis->Draw("etCorrected>>h0","isEB&&abs(eta)<1.44&&etCorrected>20&&sigmaIetaIeta<0.012&&cr4+cc4+ct4PtCut<5*0.9");
Analysis->Draw("etCorrected>>h1","isEB&&abs(eta)<1.44&&etCorrected>20&&sigmaIetaIeta<0.012&&cr4+cc4+ct4PtCut<5*0.9"&&selectionCut1);
Analysis->Draw("etCorrected>>h2","isEB&&abs(eta)<1.44&&etCorrected>20&&sigmaIetaIeta<0.012&&cr4+cc4+ct4PtCut<5*0.9&&!isEle"&&selectionCut);

h->Sumw2();
h0->Sumw2();
h1->Sumw2();
h2->Sumw2();

h->SetXTitle("p_{T} (GeV)");
h->SetYTitle("Entries");


if (isEff) {
   TGraphAsymmErrors *g1 = calcEff(h,h1);
   TGraphAsymmErrors *g2 = calcEff(h,h2);
   h->Divide(h);
   h->SetYTitle("Efficiency");
   h->SetAxisRange(0,1.4,"Y");
   h->Draw("hist");
   if (!isEff)h0->Draw("same");

   g1->SetMarkerColor(4);
   g1->SetMarkerStyle(4);
   g1->SetLineColor(4);

   g2->SetMarkerColor(2);
   g2->SetMarkerStyle(4);
   g2->SetLineColor(2);
   g1->Draw("p same");
   g2->Draw("p same");
   h1->SetMarkerColor(4);
   h1->SetMarkerStyle(4);
   h1->SetLineColor(4);

   h2->SetMarkerColor(2);
   h2->SetMarkerStyle(4);
   h2->SetLineColor(2);

} else {
   h->Draw("hist");
   h0->Draw("same");
   h1->Draw("same");
   h2->Draw("same");
   h1->SetMarkerColor(4);
   h1->SetMarkerStyle(4);
   h1->SetLineColor(4);

   h2->SetMarkerColor(2);
   h2->SetMarkerStyle(4);
   h2->SetLineColor(2);
}   

TLegend *leg;

if (!isEff) {
   leg = new TLegend(0.4,0.6,0.95,0.95);
} else {
   leg = new TLegend(0.4,0.8,0.95,0.94);

}   
 
leg->SetBorderSize(0);
leg->SetFillStyle(0);

leg->AddEntry(h,"W#rightarrow e#nu embeded in Data","");
if (!isEff)leg->AddEntry(h,"Generator Truth","l");
if (!isEff)leg->AddEntry(h0,"Reconstructed","pl");
leg->AddEntry(h1,"Pass Photon selection","pl");
leg->AddEntry(h2,"Selection + Ele veto","pl");
leg->Draw();

if (!isEff) {
   c->SaveAs("plot/electronStudy.gif");
   c->SaveAs("plot/electronStudy.eps");
   c->SaveAs("plot/electronStudy.C");
} else {
   c->SaveAs("plot/electronEfficiency.gif");
   c->SaveAs("plot/electronEfficiency.eps");
   c->SaveAs("plot/electronEfficiency.C");

}

if (!isEff) c->SaveAs("plot/electronStudy.gif");
if (!isEff) c->SaveAs("plot/electronStudy.eps");
if (!isEff) c->SaveAs("plot/electronStudy.C");


}
