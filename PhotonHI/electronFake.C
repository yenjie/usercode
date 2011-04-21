#include <TFile.h>
#include <TCut.h>
#include <TH1D.h>
#include <TLegend.h>
#include <

void electronFake(){
TFile *inf = new TFile("ampt/mpaPhotonJet15_mix_ampt_correctedTree.root");
TTree *Analysis = (TTree*)inf->FindObjectAny("Analysis");
const int nEtBin = 6;
double myBin[nEtBin+1] = {20,25,30,40,50,80,140};

TCut  eventCut     = "( !TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && abs(vtxZ) <= 15)";
TCut   centralityCut = "cBin>=0&&cBin<40";
TCut   photonCut = "r9>0&&hadronicOverEm<0.2&&t1PtCut/et/r9<0.5&&abs(eta)<1.44&&rawEnergy/energy>0.5\
                    &&!isEBGap&&!isEEGap&&!isEBEEGap";   
TCut   removeElectronCut = "!isEle";
   TString swissCrx = "(1 - (eRight+eLeft+eTop+eBottom)/eMax)";
   TCut hiSpikeCut       = Form("(  %s < 0.90 && abs(seedTime)<4  && sigmaIetaIeta>0.002 )  || isEE",swissCrx.Data());
   
TCut   removeSpikeCut = hiSpikeCut;
TCut sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0";
  

TCut   selectionCut = photonCut&&centralityCut&&eventCut&&removeSpikeCut&&removeElectronCut; 
TCut   selectionCut1 = photonCut&&centralityCut&&eventCut&&removeSpikeCut; 
TCut   selectionCut2 =photonCut&&eventCut&&removeSpikeCut; 
TCut   selectionCut3 =photonCut&&eventCut; 


TH1D *h = new TH1D("h","",nEtBin,myBin);
TH1D *h0 = new TH1D("h0","",nEtBin,myBin);
TH1D *h1 = new TH1D("h1","",nEtBin,myBin);
TH1D *h2 = new TH1D("h2","",nEtBin,myBin);
Analysis->AddFriend("genParticleCounter/photon","ampt/genTree_w2ev_mix_ampt_april15.root");
Analysis->Draw("photon.et>>h","abs(photon.id)==11&&abs(photon.eta)<1.44&&photon.et>20&&status==1");
Generator->Draw("pt>>h","abs(photon.eta)<1.44&&photon.et>20&&status==1");
Analysis->Draw("etCorrected>>h0","isEB&&abs(eta)<1.44&&etCorrected>20&&sigmaIetaIeta<0.012&&cr4+cc4<15");
Analysis->Draw("etCorrected>>h1","isEB&&abs(eta)<1.44&&etCorrected>20&&sigmaIetaIeta<0.012&&cr4+cc4<15"&&selectionCut1);
Analysis->Draw("etCorrected>>h2","isEB&&abs(eta)<1.44&&etCorrected>20&&sigmaIetaIeta<0.012&&cr4+cc4<15&&!isEle"&&selectionCut);

bool isEff=0;
h->Sumw2();
h0->Sumw2();
h1->Sumw2();
h2->Sumw2();

h->SetXTitle("p_{T} (GeV)");
h->SetYTitle("Entries");
h->Draw("hist");
if (!isEff)h0->Draw("same");
h1->Draw("same");
h2->Draw("same");
h1->SetMarkerColor(4);
h1->SetMarkerStyle(4);
h1->SetLineColor(4);

h2->SetMarkerColor(2);
h2->SetMarkerStyle(4);
h2->SetLineColor(2);


if (isEff) {
   h0->Divide(h);
   h1->Divide(h);
   h2->Divide(h);
   h->Divide(h);
   h->SetYTitle("Efficiency");
   h->SetAxisRange(0,1.2,"Y");
}   

TLegend *leg = new TLegend(0.4,0.6,0.8,0.9);
leg->SetBorderSize(0);
leg->SetFillStyle(0);
if (!isEff)leg->AddEntry(h,"Generator Truth","l");
leg->AddEntry(h0,"Reconstructed","pl");
leg->AddEntry(h1,"Pass selection","pl");
leg->AddEntry(h2,"Selection + Ele veto","pl");
leg->Draw();



}
