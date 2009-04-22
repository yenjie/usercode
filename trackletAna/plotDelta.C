#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream.h>

void plotDelta(char* infile,int type = 1)
{
   cout <<infile<<endl;
   TFile *f = new TFile(infile);
   TTree * myTree = (TTree *)(f->FindObjectAny("TrackletTree"));
   TNtuple * myNtuple = (TNtuple *)(f->FindObjectAny("ntmatched"));

   TCanvas *c = new TCanvas("c","#Delta#eta",400,400);
   TH1F *h = new TH1F("h","",200,-5,5);
   if (type) myTree->Draw("deta>>h"); else myNtuple->Draw("deta>>h");
   h->Scale(1./h->GetEntries());
   if (type) h->SetXTitle("#Delta#eta"); else h->SetXTitle("#Delta#eta (ToyMC)");
   h->SetTitleOffset(1.4,"Y");
   h->SetYTitle("Random normalization");
   h->Draw();

   TCanvas *c2 = new TCanvas("c2","#Delta#phi",400,400);
   TH1F *h2 = new TH1F("h2","",200,-3.2,3.2);
   if (type) myTree->Draw("dphi>>h2"); else myNtuple->Draw("dphi>>h2");
   h2->Scale(1./h2->GetEntries());
   if (type) h2->SetXTitle("#Delta#phi"); else h2->SetXTitle("#Delta#phi (ToyMC)");
   h2->SetYTitle("Random normalization");
   h2->Draw();

   TCanvas *c3 = new TCanvas("c3","Scatter",400,400);
   TH2F *h3 = new TH2F("h3","",200,-3.2,3.2,200,-5,5);
   if (type) myTree->Draw("deta:dphi>>h3"); else myNtuple->Draw("deta:dphi>>h3");
  // h3->Scale(1./h3->GetEntries());
   if (type) h3->SetXTitle("#Delta#phi"); else h3->SetXTitle("#Delta#phi (ToyMC)");
   if (type) h3->SetYTitle("#Delta#eta"); else h3->SetYTitle("#Delta#eta (ToyMC)");
   h3->SetTitleOffset(1.4,"Y");
   h3->Draw("col");

   TCanvas *c4 = new TCanvas("c4","#Delta#eta",400,400);
   TH1F *h4 = new TH1F("h4","",200,-5,5);
   if (type) myTree->Draw("deta>>h4","abs(dphi)<1"); else myNtuple->Draw("deta>>h4","abs(dphi)<1");
   h4->Scale(1./h4->GetEntries());
   if (type) h4->SetXTitle("#Delta#eta (Grand Signal Region)"); else h4->SetXTitle("#Delta#eta (ToyMC) (Grand Signal Region)");
   h4->SetYTitle("Random normalization");
   h4->SetTitleOffset(1.4,"Y");
   h4->Draw();

   TCanvas *c5 = new TCanvas("c5","#Delta#eta",400,400);
   TH1F *h5 = new TH1F("h5","",200,-5,5);
   if (type) myTree->Draw("deta>>h5","abs(dphi)>1&&abs(dphi)<2"); else myNtuple->Draw("deta>>h5","abs(dphi)>1&&abs(dphi)<2");
   h5->Scale(1./h5->GetEntries());
   if (type) h5->SetXTitle("#Delta#eta (Side Band region)"); else h5->SetXTitle("#Delta#eta (ToyMC) (Side Band Region)");
   h5->SetYTitle("Random normalization");
   h5->SetTitleOffset(1.4,"Y");
   h5->Draw();
   

}
