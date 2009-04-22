#include <TFile.h>
#include <TNtuple.h>
#include <TH1F.h>
#include <TCanvas.h>


void plotVtx(){
   TFile *a = new TFile("tracklet_10TeV_official_1.root") ;
   TNtuple *ta = (TNtuple*) a->FindObjectAny("TrackletTree");
   TFile *b = new TFile("tracklet-900GeV-official-1-20081118.root") ;   
   TNtuple *tb = (TNtuple*) b->FindObjectAny("TrackletTree");

   TH1F *ha = new TH1F("ha","",100,-10,10);
   TH1F *hb = new TH1F("hb","",100,-10,10);

   TCanvas *c = new TCanvas("c","",600,600);
   ta->Draw("vz[1]>>ha");
   tb->Draw("vz[1]>>hb");
 
   ha->Fit("gaus");
   hb->Fit("gaus");
}
