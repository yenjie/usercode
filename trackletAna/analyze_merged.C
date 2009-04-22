#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TNtuple.h"

using namespace std;

class RecoHit
{
   public:

   RecoHit(double _eta,double _phi,double _r) 
   { 
      eta = _eta;
      phi = _phi;
      r = _r;
   }; 
   ~RecoHit(){};
   
      double eta;
      double phi;
      double r;
};

bool compareEta(RecoHit a,RecoHit b) { return a.eta<b.eta;}
bool comparePhi(RecoHit a,RecoHit b) { return a.phi<b.phi;}


void analyze_merged(){
  TFile* inf = new  TFile("noParticleInfo/pythia_mb_900GeV_vtxFlat_HitTree_d20081015_500k_from0.root");
  TTree* t = dynamic_cast<TTree*>(inf->Get("ana/PixelTree"));

  int zbins = 1;
  int hitbins = 100;
  int nbins = zbins*hitbins;
  
  // Selection on Hits and events
  double drCut   = 0.4;      // to remove double hit
  double dphiCut = 0.04;     // to remove double hit
  double vzCut   = 10;       // vertex cut
  
  // Output PDF
  TFile* outf = new TFile("pdf.root","recreate");
  vector<TH1D*> layer1HitEta;
  layer1HitEta.reserve(nbins);
  vector<TH1D*> layer1HitPhi;
  layer1HitPhi.reserve(nbins);

  vector<TH1D*> layer2HitEta;
  layer2HitEta.reserve(nbins);
  vector<TH1D*> layer2HitPhi;
  layer2HitPhi.reserve(nbins);
  
  TNtuple *nthit1 = new TNtuple("nthit1","","eta:phi:r:dr:dphi:deta");  
  TNtuple *nthit2 = new TNtuple("nthit2","","eta:phi:r:dr:dphi:deta");  
  
  for(int i = 0; i< nbins; ++i){
    layer1HitEta[i] = new TH1D(Form("dNdEtaHits1_%02d",i),"dNdEta Hits Layer 1",500,-3,3);
    layer2HitEta[i] = new TH1D(Form("dNdEtaHits2_%02d",i),"dNdEta Hits Layer 2",500,-3,3);
    layer1HitPhi[i] = new TH1D(Form("dNdPhiHits1_%02d",i),"dNdPhi Hits Layer 1",500,-3.2,3.2);
    layer2HitPhi[i] = new TH1D(Form("dNdPhiHits2_%02d",i),"dNdPhi Hits Layer 2",500,-3.2,3.2);
  }
  
  TH1D* hm1 = new TH1D("hm1","Number of Hits Layer 1",50,0,50);

  TH3F* nhits = new TH3F("nhits","",100,0,100,100,0,100,100,0,100);

  float eta1[1000],phi1[1000],r1[1000],eta2[1000],phi2[1000],r2[1000],vz[100];

  int nhits1,nhits2,mult,nv;

  t->SetBranchAddress("eta1",eta1);
  t->SetBranchAddress("phi1",phi1);
  t->SetBranchAddress("r1",r1);
  t->SetBranchAddress("eta2",eta2);
  t->SetBranchAddress("phi2",phi2);
  t->SetBranchAddress("r2",r2);
  t->SetBranchAddress("nhits1",&nhits1);
  t->SetBranchAddress("nhits2",&nhits2);
  t->SetBranchAddress("mult",&mult);
  t->SetBranchAddress("vz",vz);
  t->SetBranchAddress("nv",&nv);

  cout <<"Number of Events: "<<t->GetEntries()<<endl;

  // Main loop
  for(int i = 0; i< t->GetEntries(); ++i){    
    t->GetEntry(i);
    if (fabs(vz[1])>vzCut) continue;
    nhits->Fill(mult,nhits1,nhits2);

    hm1->Fill(mult);

    // Process the first layer
    vector<RecoHit> layer1;
    int hitbin1 = (int)nhits1;
    if (hitbin1>99) hitbin1=99;

    for(int ihit = 0; ihit < nhits1; ++ihit){
      RecoHit tmp(eta1[ihit],phi1[ihit],r1[ihit]);
      layer1.push_back(tmp);
    }

    sort (layer1.begin(),layer1.end(),compareEta);
    
    for(int ihit = 0; ihit < (int)layer1.size(); ++ihit) {
      double dr=0;
      double dphi=10;
      if (ihit !=0) {
         dphi = fabs(layer1[ihit-1].phi - layer1[ihit].phi);
	 dr   = fabs(layer1[ihit-1].r - layer1[ihit].r);
      }
      
      if (dr>drCut && dphi < dphiCut) continue;
      layer1HitEta[hitbin1]->Fill(layer1[ihit].eta);
      layer1HitPhi[hitbin1]->Fill(layer1[ihit].phi);
     
      if (ihit!=0) nthit1->Fill(layer1[ihit].eta,layer1[ihit].phi,layer1[ihit].r,layer1[ihit-1].r-layer1[ihit].r,layer1[ihit].phi-layer1[ihit-1].phi,layer1[ihit].eta-layer1[ihit-1].eta);
              else nthit1->Fill(layer1[ihit].eta,layer1[ihit].phi,layer1[ihit].r,7,7,7);
    }

    // Process the second layer
    vector<RecoHit> layer2;
    int hitbin2 = (int)nhits2;
    if (hitbin2>99) hitbin2=99;
    for(int ihit = 0; ihit < nhits2; ++ihit){
      RecoHit tmp(eta2[ihit],phi2[ihit],r2[ihit]);
      layer2.push_back(tmp);
    }

    sort (layer2.begin(),layer2.end(),comparePhi);
    
    for(int ihit = 0; ihit < (int)layer2.size(); ++ihit) {
      double dr=0;
      double dphi=10;
      if (ihit !=0) {
         dphi = fabs(layer2[ihit-1].phi - layer2[ihit].phi);
	 dr   = fabs(layer2[ihit-1].r - layer2[ihit].r);
      }

      if (dr>drCut && dphi < dphiCut) continue;
      layer2HitEta[hitbin2]->Fill(layer2[ihit].eta);
      layer2HitPhi[hitbin2]->Fill(layer2[ihit].phi);
     
      if (ihit!=0) nthit2->Fill(layer2[ihit].eta,layer2[ihit].phi,layer2[ihit].r,layer2[ihit-2].r-layer2[ihit].r,layer2[ihit].phi-layer2[ihit-2].phi,layer2[ihit].eta-layer2[ihit-2].eta);
              else nthit2->Fill(layer2[ihit].eta,layer2[ihit].phi,layer2[ihit].r,7,7,7);
    }

  }

  outf->Write();
  outf->Close(); 
}
