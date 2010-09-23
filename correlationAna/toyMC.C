#include <iostream>
#include <TH1D.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>
#include "nchCommon.h"


void toyMC(char *infname = "minBias_test.root")
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*) inf->FindObjectAny("PixelTree");
   TFile *outf = new TFile("toyMC.root","recreate");
   TH1D *hVz = new TH1D("hVz","",100,-15,15);
   
   // Treatment of Vz PDF
   TCanvas *c = new TCanvas("c","Vz",600,600);
   TF1 *f = new TF1("f","[0]*exp(-(x-[1])*(x-[1])/2./[2]/[2])+[3]*exp(-(x-[4])*(x-[4])/2./[5]/[5])+[6]*exp(-(x-[7])*(x-[7])/2./[8]/[8])");
   t->Draw("vz[1]>>hVz");  
   hVz->Sumw2();
//   hVz->Scale(1./hVz->GetEntries());

   TH1D *hVzFit = (TH1D*)hVz->Clone();
   hVzFit->SetName("hVzFit");
   f->SetParameters(10e6,-0.146,6.438,0.000,-0.146,6.438,0.0000,-0.146,6.438);
//   f->SetParLimits(0,0,10e10);
//   f->SetParLimits(3,0,10e10);
//   f->SetParLimits(6,0,10e10);
   f->SetLineColor(2);
   hVzFit->Fit("f");
   hVzFit->Fit("f");
   hVzFit->Fit("f","L");
   f->SetTitle("");
   f->SetRange(-30,30);
   f->SetLineStyle(1);
   hVz->SetXTitle("V_{z} (cm)");
   hVz->SetYTitle("Arbitrary Unit");
   hVz->SetMarkerStyle(24);
   hVz->SetMarkerSize(0.5);
   hVz->Draw();
   f->Draw("same");
   hVz->Draw("same");

   // nTrk
   TCanvas *c2 = new TCanvas("c2","Nch",600,600);
   TH1D *hNch = new TH1D("hNch","",150,0,150);
   TH1D *hNch1 = new TH1D("hNch1","",150,0,150);
   TH1D *hNch2 = new TH1D("hNch2","",150,0,150);
   TH1D *hNch3 = new TH1D("hNch3","",150,0,150);
   t->Draw("nTrk>>hNch");
   //TF1 *f2 = new TF1("f2",nbd,0,200,3);
   //f2->SetParameters(hVz->GetEntries(),3.9,1.6);
   TF1 *f2 = new TF1("f2",twonbd,0,150,6);
   f2->SetParameters(1,3.9,1.6,10,6,2);
   f2->SetLineColor(2);   
//   TF1 *f2 = new TF1("f2","[0]*exp([1]+[2]*x+[3]*x*x)+[4]*exp([5]+[6]*x+[7]*x*x)");
   hNch->Sumw2();
   hNch->Scale(1./hNch->GetEntries());
   hNch->Fit("f2","","",5,150);
   hNch->Fit("f2","","",5,150);
   hNch->Fit("f2","","",5,150);
   hNch->SetMarkerStyle(24);
   hNch->SetMarkerSize(0.5);
   hNch->SetXTitle("N");
   hNch->Draw();
   double longVz  = f->Integral(-30,30);
   double longNch = f2->Integral(0,150);

   double avg = 1.4;
   TH1D *h = new TH1D("h","",9,1,10);
   for (long i=0;i<9;i++)
   {
      h->Fill(i,TMath::Poisson(i,avg));
   }

   TNtuple *nt = new TNtuple("nt","","vz:big:nch:nchCut:nchNoCut:n2:n3:n4:n5:np:npCut");

   for (long i=0;i<100000000;i++)
   {
       long n=0; 
       long n1=0; 
       if (i% 100000 ==0) cout <<i<<endl;
       double vz = f->GetRandom();
       long n0= f2->GetRandom();
       n=n0;
       n1=n0;
       hNch1->Fill(n);
       long big =n0;
       long ntrack[10];
       for (long k=0;k<10;k++)
       {
          ntrack[k]=0;
       }
       long np=h->GetRandom();
       long npCut=0;

       for (long j=0;j<np-1;j++) 
       {
           long mult = f2->GetRandom();
           if (fabs(f->GetRandom()-vz)<0.15){
              n+= mult;
              ntrack[npCut]=mult;
              npCut++;
              if ((mult)>big) big=mult;
           }
           n1+=mult;

       }
       hNch2->Fill(n);
       hNch3->Fill(n1);
       nt->Fill(vz,big,n0,n,n1,ntrack[0],ntrack[1],ntrack[2],ntrack[3],np,npCut+1);
   }

   TCanvas *c3 = new TCanvas("c3","",600,600);
   c3->SetLogy();
   hNch2->SetLineColor(2);
   hNch3->SetLineColor(4);

   TLegend *l = new TLegend(0.5,0.6,0.9,0.9);
   l->SetBorderSize(0);
   l->SetFillStyle(0);
   l->AddEntry(hNch1,"No Pileup","l");
   l->AddEntry(hNch2,"Pileup within |dvz|<0.15","l");
   l->AddEntry(hNch3,"Pileup","l");
   hNch1->Draw();
   hNch2->Draw("same");
   hNch3->Draw("same");
   l->Draw();
   TCanvas *c4 = new TCanvas("c4","",600,600);
   TProfile *p = new TProfile("p","",15,0,150);
   nt->Draw("npCut-1:nchCut>>p","nchCut<150","prof");
   p->SetXTitle("N");
   p->SetYTitle("Pileup rate");   

}
