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

void analyze_trackletTree(char * infile, char * outfile = "output.root", int makeVzCut = 1,
                          int addL1Bck = 0, int addL2Bck = 0)
{
  TFile* inf = new  TFile(infile);
  TTree* t = dynamic_cast<TTree*>(inf->Get("ana/PixelTree"));

  int zbins = 1;
  int hitbins = 100;
  int nbins = zbins*hitbins;
  
  // Selection on Hits and events =====================================================================
  SelectionCriteria cuts;
  cuts.drCut   = 0.4;      // to remove double hit
  cuts.dPhiCut = 0.04;     // to remove double hit
  cuts.dEtaCut = 0.2;     // to remove double hit
  cuts.vzCut   = 10;       // vertex cut
  

  // Settings =========================================================================================
  cuts.verbose_ = false;
  cuts.useDeltaPhi_ = false;
  cuts.checkSecondLayer_ = true;
  
  // Output PDF =======================================================================================
  TFile* outf = new TFile(outfile,"recreate");
  TNtuple *ntmult = new TNtuple("ntmult","","mult:nhit1:nhit2");
  TNtuple *nthit = new TNtuple("nthit","","phi1:layer");
  TTree *trackletTree = new TTree("TrackletTree","Tree of Reconstructed Tracklets");

  // Tracklet Tree data format ========================================================================
  TrackletData tdata;

  trackletTree->Branch("nTracklets",&tdata.nTracklet,"nTracklets/I");
  trackletTree->Branch("nhit1",&tdata.nhit1,"nhit1/I");
  trackletTree->Branch("nhit2",&tdata.nhit2,"nhit2/I");
  trackletTree->Branch("mult",&tdata.mult,"mult/I");
  trackletTree->Branch("nv",&tdata.nv,"nv/I");
  trackletTree->Branch("vz",tdata.vz,"vz[nv]/F");
  trackletTree->Branch("eta1",tdata.eta1,"eta1[nTracklets]/F");
  trackletTree->Branch("phi1",tdata.phi1,"phi1[nTracklets]/F");
  trackletTree->Branch("eta2",tdata.eta2,"eta2[nTracklets]/F");
  trackletTree->Branch("phi2",tdata.phi2,"phi2[nTracklets]/F");
  trackletTree->Branch("deta",tdata.deta,"data[nTracklets]/F");
  trackletTree->Branch("dphi",tdata.dphi,"data[nTracklets]/F");
  
  trackletTree->Branch("npart",&tdata.npart,"npart/I");
  trackletTree->Branch("eta",tdata.eta,"eta[npart]/F");
  //trackletTree->Branch("phi",tdata.phi,"phi[npart]/F");
  trackletTree->Branch("pdg",tdata.pdg,"pdg[npart]/I");
  trackletTree->Branch("chg",tdata.chg,"chg[npart]/I");
  trackletTree->Branch("nhad",tdata.nhad,"nhad[12]/F");
  trackletTree->Branch("evtType",&tdata.evtType,"evtType/I");


  // Hit vectors & pdfs ===================================================================================
  vector<TH1D*> layer1HitEta;
  layer1HitEta.reserve(nbins);

  vector<TH1D*> layer1HitPhi;
  layer1HitPhi.reserve(nbins);

  vector<TH2D*> layer12D;
  layer12D.reserve(nbins);

  vector<TH1D*> layer2HitEta;
  layer2HitEta.reserve(nbins);

  vector<TH1D*> layer2HitPhi;
  layer2HitPhi.reserve(nbins);

  vector<TH2D*> layer22D;
  layer22D.reserve(nbins);
    
  for(int i = 0; i< nbins; ++i){
    layer1HitEta[i] = new TH1D(Form("dNdEtaHits1_%02d",i),"dNdEta Hits Layer 1",500,-3,3);
    layer2HitEta[i] = new TH1D(Form("dNdEtaHits2_%02d",i),"dNdEta Hits Layer 2",500,-3,3);
    layer1HitPhi[i] = new TH1D(Form("dNdPhiHits1_%02d",i),"dNdPhi Hits Layer 1",500,-3.2,3.2);
    layer2HitPhi[i] = new TH1D(Form("dNdPhiHits2_%02d",i),"dNdPhi Hits Layer 2",500,-3.2,3.2);
    layer12D[i] = new TH2D(Form("dNdEtadPhiHits1_%02d",i),"dNdPhidEta Hits Layer 1",500,-3,3,500,-3.2,3.2);
    layer22D[i] = new TH2D(Form("dNdEtadPhiHits2_%02d",i),"dNdPhidEta Hits Layer 2",500,-3,3,500,-3.2,3.2);
  }


  TH3F* nhits = new TH3F("nhits","",100,0,100,100,0,100,100,0,100);
  
  // Parameters for the tree =============================================================================
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
  if(!makeVzCut) t->SetBranchAddress("evtType",&par.evtType);
  cout <<"Number of Events: "<<t->GetEntries()<<endl;

  // Main loop ===========================================================================================
  for(int i = 0;  i<t->GetEntries() && i < 100000 ; ++i){    
    t->GetEntry(i);
    if (i % 1000 == 0) cout <<"Event "<<i<<endl;    
    // Selection on Events
    
    // Fill vertex information
    tdata.nv = par.nv;  
    for(int j = 0; j<par.nv;j++) {
       tdata.vz[j] = par.vz[j];
    }
    
    // add background 
    if (addL1Bck!=0) {
       double etaMin = -2.5;
       double etaMax = 2.5;
       double phiMin = -3.1415926;
       double phiMax = 3.1415926;
       
       for (int i = par.nhits1; i < par.nhits1+addL1Bck; i++) {

          double eta = etaMin + (etaMax-etaMin)*gRandom->Rndm();
          double phi = phiMin + (phiMax-phiMin)*gRandom->Rndm();
	  double r = 4.15;
	  par.eta1[i] = eta;
	  par.phi1[i] = phi;
	  par.r1[i] = r;

       }
       par.nhits1+=addL1Bck;
    }

    if (addL2Bck!=0) {
       double etaMin = -2.5;
       double etaMax = 2.5;
       double phiMin = -3.1415926;
       double phiMax = 3.1415926;

       for (int i = par.nhits2; i < par.nhits2+addL2Bck; i++) {
          double eta = etaMin + (etaMax-etaMin)*gRandom->Rndm();
          double phi = phiMin + (phiMax-phiMin)*gRandom->Rndm();
	  double r = 7.05;
	  par.eta2[i] = eta;
	  par.phi2[i] = phi;
	  par.r2[i] = r;
       }
       par.nhits2+=addL2Bck;

    }

    
    // add trackletVertex
    if (tdata.nv == 2) tdata.nv=3;
    tdata.vz[tdata.nv] = TrackletVertexUnbin(par,0.14,0.08);
    tdata.nv++;
    
    // use trackletVertex
    if (fabs(tdata.vz[tdata.nv-1])>cuts.vzCut && makeVzCut == 1) continue;

    // Process the first layer
    vector<RecoHit> layer1 = removeDoubleHits(par, cuts,1,tdata.vz[tdata.nv-1]);
    double mult = 0;
    for(int ihit = 0; ihit< (int)layer1.size(); ++ihit) {
      int hitbin1 = (int)layer1.size();
      if (hitbin1 > 99) hitbin1 = 99;
      layer1HitEta[hitbin1]->Fill(layer1[ihit].eta);
      layer1HitPhi[hitbin1]->Fill(layer1[ihit].phi);
      layer12D[hitbin1]->Fill(layer1[ihit].eta,layer1[ihit].phi);
      nthit->Fill(layer1[ihit].phi,1);
      if(fabs(layer1[ihit].eta)<1) mult++;
    }

    // Process the second layer
    vector<RecoHit> layer2 = removeDoubleHits(par, cuts,2,tdata.vz[tdata.nv-1]);

    for(int ihit = 0; ihit< (int)layer2.size(); ++ihit) {
      int hitbin2 = (int)layer2.size();
      if (hitbin2 > 99) hitbin2 = 99;
      layer2HitEta[hitbin2]->Fill(layer2[ihit].eta);
      layer2HitPhi[hitbin2]->Fill(layer2[ihit].phi);
      layer22D[hitbin2]->Fill(layer2[ihit].eta,layer2[ihit].phi);
      nthit->Fill(layer2[ihit].phi,2);
    }

    // Form Tracklets        
    vector<Tracklet> protoTracklets = recoProtoTracklets(layer1,layer2);
    vector<Tracklet> recoTracklets = cleanTracklets(protoTracklets,0,cuts);

    // Fill Ntuple
    tdata.nTracklet = recoTracklets.size();
    tdata.nhit1 = layer1.size();
    tdata.nhit2 = layer2.size();
    
    for (int j=0;j<(int)tdata.nTracklet;j++)
    {
        tdata.eta1[j] = recoTracklets[j].eta1();	
        tdata.eta2[j] = recoTracklets[j].eta2();	
        tdata.phi1[j] = recoTracklets[j].phi1();	
        tdata.phi2[j] = recoTracklets[j].phi2();
	tdata.deta[j] = recoTracklets[j].deta();
   	tdata.dphi[j] = recoTracklets[j].dphi();
	tdata.mult = (int)mult;	
    }

    tdata.npart=0;
    for (int j=0;j<12;j++) tdata.nhad[j]=0;

    for(int j=0;j<par.npart;j++)
    {
        if (fabs(par.eta[j])>3||par.chg[j]==0) continue;
	tdata.eta[tdata.npart]=par.eta[j];
	tdata.chg[tdata.npart]=par.chg[j];
	tdata.pdg[tdata.npart]=par.pdg[j];
        tdata.npart++;
	int bin = (int)((par.eta[j]+3)*2);
	int pdg = (int)abs(par.pdg[j]);
	if (pdg== 211 || pdg == 321 || pdg == 2212 || pdg ==3122) tdata.nhad[bin]++;
    }

    nhits->Fill(mult,layer1.size(),layer2.size());
    ntmult->Fill(mult,layer1.size(),layer2.size());

    tdata.evtType = par.evtType;
    trackletTree->Fill();
    
  }

  // Close outputfile ===================================================================================
  outf->Write();
  outf->Close(); 
}
