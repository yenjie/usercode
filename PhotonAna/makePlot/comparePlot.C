#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <vector>
#include <string>
#include <TCanvas.h>

#include "selectionCut.h"

void fillTrees(vector<string> &fileName,vector<TTree*> &trees,string treeName)
{
   for (int i=0; i<fileName.size(); i++){
      TFile *f = new TFile(fileName[i].data());
      TTree *t = (TTree*)f->FindObjectAny(treeName.data());
      if (t==0) cout <<"Error!"<<endl;
      trees.push_back(t);
   }

}

void makePlot(vector<TTree*> sigTree,vector<double> sigWeight,
              char *var,TCut cut,TH1F* h,int idx=1)
{
   TH1F *hRes = (TH1F*)h->Clone();
   hRes->SetName("hRes");
      
   for (int i=0; i<sigTree.size(); i++)
   {
      TH1F *htmp = (TH1F*)h->Clone();
      htmp->SetName("htmp");
      sigTree[i]->Draw(Form("%s>>htmp",var),cut);
      htmp->Sumw2();
      htmp->Scale(sigWeight[i]);
      hRes->Add(htmp);
      delete htmp;
   }
   h->Add(hRes);
   delete hRes;
}


void comparePlot(char *var="ecalIso",double genIsoCut=1,bool scale = 1)
{
   vector<string> sigFile;
   vector<double> sigWeight;
   vector<TTree*> sigTree;
   
   sigFile.push_back("./sample/PhotonJet_Pt15.root");
//   sigFile.push_back("./sample/PhotonJet_Pt30.root");
   sigWeight.push_back(0.0157104);
//   sigWeight.push_back(0.00195463);

   fillTrees(sigFile,sigTree,"Analysis");
  
   vector<string> bckFile;
   vector<double> bckWeight;
   vector<TTree*>  bckTree;
   
   // to 100 nb-1
   //bckFile.push_back("./sample/PhotonJet_Pt15.root");
   bckFile.push_back("./sample/QCD_Pt15.root");
   bckFile.push_back("./sample/QCD_Pt30.root");
   bckFile.push_back("./sample/QCD_Pt80.root");
   //bckWeight.push_back(0.0157104);
   bckWeight.push_back(14.1539);
   bckWeight.push_back(1.14637);
   bckWeight.push_back(0.0286734);
   fillTrees(bckFile,bckTree,"Analysis");
   
   
   vector<string> mixFile;
   vector<double> mixWeight;
   vector<TTree*> mixTree;
   
   // to 100 nb-1
   mixFile.push_back("./sample/PhotonJet_Pt15.root");
   mixFile.push_back("./sample/QCD_Pt15.root");
   mixFile.push_back("./sample/QCD_Pt30.root");
   mixFile.push_back("./sample/QCD_Pt80.root");
   mixWeight.push_back(0.0157104);
   mixWeight.push_back(14.1539);
   mixWeight.push_back(1.14637);
   mixWeight.push_back(0.0286734);
   
   
   fillTrees(mixFile,mixTree,"Analysis");

   selectionCut myCut;
   
   TCut mycuts = Form("isGenMatched[0] && genCalIsoDR04[0] < %f",genIsoCut); 
   TCut mycutb = Form("!(isGenMatched[0] && genCalIsoDR04[0] < %f)",genIsoCut);
   
   TH1F *hEcalIsoSig = new TH1F("hEcalIsoSig","",55,-2,20);
   TH1F *hEcalIsoMix = new TH1F("hEcalIsoMix","",55,-2,20);
   TH1F *hEcalIsoBck = new TH1F("hEcalIsoBck","",55,-2,20);

   makePlot(sigTree,sigWeight,Form("%s[0]",var),myCut.EECut&&mycuts,hEcalIsoSig);
   makePlot(mixTree,mixWeight,Form("%s[0]",var),myCut.EECut&&mycuts,hEcalIsoMix);
   makePlot(mixTree,mixWeight,Form("%s[0]",var),myCut.EECut&&mycutb,hEcalIsoBck);
   
   hEcalIsoSig->SetMarkerColor(2);
   hEcalIsoSig->SetLineColor(2);
   hEcalIsoMix->SetMarkerColor(4);
   hEcalIsoMix->SetLineColor(4);

   
   TCanvas *c = new TCanvas("c","",600,600);

   if (scale) {
      hEcalIsoSig->Scale(1./hEcalIsoSig->Integral(0,1000));
      hEcalIsoBck->Scale(1./hEcalIsoBck->Integral(0,1000));
      hEcalIsoMix->Scale(1./hEcalIsoMix->Integral(0,1000));
      hEcalIsoSig->SetXTitle(var);
      hEcalIsoSig->Draw();
      hEcalIsoMix->Draw("same");
      hEcalIsoBck->Draw("same");
   } else {
      hEcalIsoBck->SetXTitle(var);
      hEcalIsoBck->Draw();
      hEcalIsoSig->Draw("same");
      hEcalIsoMix->Draw("same");
   
   }

   if (scale) {
      c->SaveAs(Form("plots/plot-%s-%.1fGeV.gif",var,genIsoCut));
      c->SaveAs(Form("plots/plot-%s-%.1fGeV.C",var,genIsoCut));
      c->SetLogy();
      c->Update();
      c->SaveAs(Form("plots/plot-%s-log-%.1fGeV.gif",var,genIsoCut));
      c->SaveAs(Form("plots/plot-%s-log-%.1fGeV.C",var,genIsoCut));
   } else {
      c->SaveAs(Form("plots/plot-%s-raw-%.1fGeV.gif",var,genIsoCut));
      c->SaveAs(Form("plots/plot-%s-raw-%.1fGeV.C",var,genIsoCut));
      c->SetLogy();
      c->Update();
      c->SaveAs(Form("plots/plot-%s-raw-log-%.1fGeV.gif",var,genIsoCut));
      c->SaveAs(Form("plots/plot-%s-raw-log-%.1fGeV.C",var,genIsoCut));
      
   }
}
