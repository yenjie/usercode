#define match_cxx
#include "match.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TFile.h>
#include <iostream>

double match::calcDphi(double phi1_,double phi2_)
{
   double pi = 3.14159265358979;
   double dphi=phi1_-phi2_;

   if (dphi>0){
      while (dphi>2*pi) dphi-=2*pi;
      if (dphi>pi) dphi=2*pi-dphi; 
   } else {
      while (dphi<-2*pi) dphi+=2*pi;
      if (dphi<-pi) dphi=-2*pi-dphi;
   }


   return dphi;
}

void match::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   TFile *output = new TFile("output.root","recreate");
   TNtuple *nt = new TNtuple("nt","","pt:eta:phi:eta1:deta:phi1:dphi");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      if (jentry % 1000 ==0) cout <<jentry<<" / "<<nentries<<endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (vz[1]<-99) continue;
      // loop over genParticles to do matching
      for (int j=0;j<npart;j++) {
         double minDEta=100;
         int selected=-1;
         for (int i=0;i<nTracklets;i++) {
	    double deltaEta=fabs(eta[j]-eta1[i]);
	    double deltaPhi=fabs(phi[j]-phi1[i]);
           
            // select the one with smallest delta eta      
	    if (minDEta>deltaEta) {
               selected = i;
               minDEta=deltaEta;
            }
         }             
         // fill ntuple
         if (selected>-1) nt->Fill(pt[j],eta[j],phi[j],eta1[selected],deta[selected],phi1[selected],dphi[selected]);
      }
   }
   output->Write();
   output->Close();
}
