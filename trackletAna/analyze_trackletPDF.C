#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TNtuple.h"
#include "Tracklet.h"
#include "Math/Vector3D.h"

using namespace std;

void analyze_trackletPDF(char * infile, char * outfile = "output.root"){
  TFile* inf = new  TFile(infile);
  TTree* t = dynamic_cast<TTree*>(inf->Get("ana/PixelTree"));

  int zbins = 1;
  int hitbins = 100;
  int nbins = zbins*hitbins;
  
  // Selection on Hits and events
  SelectionCriteria cuts;
  cuts.drCut   = 0.4;      // to remove double hit
  cuts.dPhiCut = 0.04;     // to remove double hit
  cuts.vzCut   = 10;       // vertex cut

  cuts.verbose_ = false;
  cuts.useDeltaPhi_ = false;
  cuts.checkSecondLayer_ = true;
  
  // Output PDF
  TFile* outf = new TFile(outfile,"recreate");

  vector<TH1D*> layer1HitEta;
  layer1HitEta.reserve(nbins);
  vector<TH1D*> layer1HitPhi;
  layer1HitPhi.reserve(nbins);
  vector<TH3D*> layer13D;
  layer13D.reserve(nbins);

  vector<TH1D*> layer2HitEta;
  layer2HitEta.reserve(nbins);
  vector<TH1D*> layer2HitPhi;
  layer2HitPhi.reserve(nbins);
  vector<TH3D*> layer23D;
  layer23D.reserve(nbins);
    
  for(int i = 0; i< nbins; ++i){
    layer1HitEta[i] = new TH1D(Form("dNdEtaHits1_%02d",i),"dNdEta Hits Layer 1",500,-3,3);
    layer2HitEta[i] = new TH1D(Form("dNdEtaHits2_%02d",i),"dNdEta Hits Layer 2",500,-3,3);
    layer1HitPhi[i] = new TH1D(Form("dNdPhiHits1_%02d",i),"dNdPhi Hits Layer 1",500,-3.2,3.2);
    layer2HitPhi[i] = new TH1D(Form("dNdPhiHits2_%02d",i),"dNdPhi Hits Layer 2",500,-3.2,3.2);
    layer13D[i] = new TH3D(Form("dNdEtadPhiHits1_%02d",i),"dNdPhidEta Hits Layer 1",100,-3,3,100,-3.2,3.2,20,3,5);
    layer23D[i] = new TH3D(Form("dNdEtadPhiHits2_%02d",i),"dNdPhidEta Hits Layer 2",100,-3,3,100,-3.2,3.2,20,6,8);
  }

  TH3F* nhits = new TH3F("nhits","",100,0,100,100,0,100,100,-10,10);

  
  // Parameters for the tree:
  Parameters par;  

  t->SetBranchAddress("eta1",par.eta1);
  t->SetBranchAddress("phi1",par.phi1);
  t->SetBranchAddress("r1",par.r1);
  t->SetBranchAddress("eta2",par.eta2);
  t->SetBranchAddress("phi2",par.phi2);
  t->SetBranchAddress("r2",par.r2);
  t->SetBranchAddress("nhits1",&par.nhits1);
  t->SetBranchAddress("nhits2",&par.nhits2);
  t->SetBranchAddress("mult",&par.mult);
  t->SetBranchAddress("vz",par.vz);
  t->SetBranchAddress("nv",&par.nv);
  t->SetBranchAddress("npart",&par.npart);
  t->SetBranchAddress("eta",&par.eta);
  t->SetBranchAddress("phi",&par.phi);
  t->SetBranchAddress("chg",&par.chg);
  t->SetBranchAddress("pdg",&par.pdg);

  cout <<"Number of Events: "<<t->GetEntries()<<endl;

  // Main loop
  for(int i = 0; i< 100000 && i<t->GetEntries(); ++i){    
    t->GetEntry(i);
    if (i % 1000 == 0) cout <<"Event "<<i<<endl;    
    // Selection on Events
    if (fabs(par.vz[1])>cuts.vzCut) continue;
    
    // Fill vertex information
    
    // Process the first layer
    vector<RecoHit> layer1 = removeDoubleHitsRaw(par, cuts,1);
    double mult = 0;
    for(int ihit = 0; ihit< (int)layer1.size(); ++ihit) {
      int hitbin1 = (int)layer1.size();
      if (hitbin1 > 99) hitbin1 = 99;
      layer1HitEta[hitbin1]->Fill(layer1[ihit].eta);
      layer1HitPhi[hitbin1]->Fill(layer1[ihit].phi);
      layer13D[hitbin1]->Fill(layer1[ihit].eta,layer1[ihit].phi,layer1[ihit].r);
      if(fabs(layer1[ihit].eta)<1) mult++;
    }

    // Process the second layer
    vector<RecoHit> layer2 = removeDoubleHitsRaw(par, cuts,2);

    for(int ihit = 0; ihit< (int)layer2.size(); ++ihit) {
      int hitbin2 = (int)layer2.size();
      if (hitbin2 > 99) hitbin2 = 99;
      layer2HitEta[hitbin2]->Fill(layer2[ihit].eta);
      layer2HitPhi[hitbin2]->Fill(layer2[ihit].phi);
      layer23D[hitbin2]->Fill(layer2[ihit].eta,layer2[ihit].phi,layer2[ihit].r);
    }

    nhits->Fill(layer1.size(),layer2.size(),par.vz[1]);

  }

  outf->Write();
  outf->Close(); 
}

