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

void analyze_trackletTree(char * infile, char * outfile = "output.root", int makeVzCut = 0,
                          int addL1Bck = 0, int addL2Bck = 0, double
			  splitProb = 0, double dropProb = 0, int nPileUp = 0, int putPixelTree = 0)
{

  // Input file =======================================================================================
  TFile* inf = new  TFile(infile);

  TTree* t = dynamic_cast<TTree*>(inf->FindObjectAny("PixelTree"));

  // Output file =======================================================================================
  TFile* outf = new TFile(outfile,"recreate");
  TNtuple *ntmult = new TNtuple("ntmult","","mult:nhit1:nhit2");
  TNtuple *nthit = new TNtuple("nthit","","phi1:layer");
  TTree *trackletTree12 = new TTree("TrackletTree12","Tree of Reconstructed Tracklets");
  TTree *trackletTree13 = new TTree("TrackletTree13","Tree of Reconstructed Tracklets");
  TTree *trackletTree23 = new TTree("TrackletTree23","Tree of Reconstructed Tracklets");
  TTree *outTree;
  if (putPixelTree) {
     outTree = t->CloneTree();    
     cout <<"Put in Pixel Tree"<<endl;
  }
  int zbins = 1;
  int hitbins = 100;
  int vertexHitRegion = 500000;
  int nbins = zbins*hitbins;
  double mult = 0;
  
  // Selection on Hits and events =====================================================================
  SelectionCriteria cuts;
  cuts.drCut   = 0.4;      // to remove double hit
  cuts.dPhiCut = 0.04;     // to remove double hit
  cuts.dEtaCut = 0.2;     // to remove double hit
  cuts.vzCut   = 10;       // vertex cut
  

  // Settings =========================================================================================
  cuts.verbose_ = false;
  cuts.useDeltaPhi_ = false;
  cuts.useDeltaRho_ = false;
  cuts.checkSecondLayer_ = true;
  
  // Tracklet Tree data format ========================================================================
  TrackletData tdata12;

  trackletTree12->Branch("nTracklets",&tdata12.nTracklet,"nTracklets/I");
  trackletTree12->Branch("nhit1",&tdata12.nhit1,"nhit1/I");
  trackletTree12->Branch("nhit2",&tdata12.nhit2,"nhit2/I");
  trackletTree12->Branch("mult",&tdata12.mult,"mult/I");
  trackletTree12->Branch("nv",&tdata12.nv,"nv/I");
  trackletTree12->Branch("vz",tdata12.vz,"vz[nv]/F");
  trackletTree12->Branch("eta1",tdata12.eta1,"eta1[nTracklets]/F");
  trackletTree12->Branch("phi1",tdata12.phi1,"phi1[nTracklets]/F");
  trackletTree12->Branch("r1",tdata12.r1,"r1[nTracklets]/F");
  trackletTree12->Branch("eta2",tdata12.eta2,"eta2[nTracklets]/F");
  trackletTree12->Branch("phi2",tdata12.phi2,"phi2[nTracklets]/F");
  trackletTree12->Branch("r2",tdata12.r2,"r2[nTracklets]/F");
  trackletTree12->Branch("deta",tdata12.deta,"deta[nTracklets]/F");
  trackletTree12->Branch("dphi",tdata12.dphi,"dphi[nTracklets]/F");
  trackletTree12->Branch("npart",&tdata12.npart,"npart/I");
  trackletTree12->Branch("eta",tdata12.eta,"eta[npart]/F");
  trackletTree12->Branch("phi",tdata12.phi,"phi[npart]/F");
  trackletTree12->Branch("pdg",tdata12.pdg,"pdg[npart]/I");
  trackletTree12->Branch("chg",tdata12.chg,"chg[npart]/I");
  trackletTree12->Branch("nhad",tdata12.nhad,"nhad[12]/F");
  trackletTree12->Branch("pt",tdata12.pt,"pt[npart]/F");
  trackletTree12->Branch("evtType",&tdata12.evtType,"evtType/I");

  TrackletData tdata13;

  trackletTree13->Branch("nTracklets",&tdata13.nTracklet,"nTracklets/I");
  trackletTree13->Branch("nhit1",&tdata13.nhit1,"nhit1/I");
  trackletTree13->Branch("nhit2",&tdata13.nhit2,"nhit2/I");
  trackletTree13->Branch("mult",&tdata13.mult,"mult/I");
  trackletTree13->Branch("nv",&tdata13.nv,"nv/I");
  trackletTree13->Branch("vz",tdata13.vz,"vz[nv]/F");
  trackletTree13->Branch("eta1",tdata13.eta1,"eta1[nTracklets]/F");
  trackletTree13->Branch("phi1",tdata13.phi1,"phi1[nTracklets]/F");
  trackletTree13->Branch("r1",tdata13.r1,"r1[nTracklets]/F");
  trackletTree13->Branch("eta2",tdata13.eta2,"eta2[nTracklets]/F");
  trackletTree13->Branch("phi2",tdata13.phi2,"phi2[nTracklets]/F");
  trackletTree13->Branch("r2",tdata13.r2,"r2[nTracklets]/F");
  trackletTree13->Branch("deta",tdata13.deta,"deta[nTracklets]/F");
  trackletTree13->Branch("dphi",tdata13.dphi,"dphi[nTracklets]/F");
  
  trackletTree13->Branch("npart",&tdata13.npart,"npart/I");
  trackletTree13->Branch("eta",tdata13.eta,"eta[npart]/F");
  trackletTree13->Branch("phi",tdata13.phi,"phi[npart]/F");
  trackletTree13->Branch("pdg",tdata13.pdg,"pdg[npart]/I");
  trackletTree13->Branch("chg",tdata13.chg,"chg[npart]/I");
  trackletTree13->Branch("nhad",tdata13.nhad,"nhad[12]/F");
  trackletTree13->Branch("pt",tdata13.pt,"pt[npart]/F");
  trackletTree13->Branch("evtType",&tdata13.evtType,"evtType/I");

  TrackletData tdata23;

  trackletTree23->Branch("nTracklets",&tdata23.nTracklet,"nTracklets/I");
  trackletTree23->Branch("nhit1",&tdata23.nhit1,"nhit1/I");
  trackletTree23->Branch("nhit2",&tdata23.nhit2,"nhit2/I");
  trackletTree23->Branch("mult",&tdata23.mult,"mult/I");
  trackletTree23->Branch("nv",&tdata23.nv,"nv/I");
  trackletTree23->Branch("vz",tdata23.vz,"vz[nv]/F");
  trackletTree23->Branch("eta1",tdata23.eta1,"eta1[nTracklets]/F");
  trackletTree23->Branch("phi1",tdata23.phi1,"phi1[nTracklets]/F");
  trackletTree23->Branch("r1",tdata23.r1,"r1[nTracklets]/F");
  trackletTree23->Branch("eta2",tdata23.eta2,"eta2[nTracklets]/F");
  trackletTree23->Branch("phi2",tdata23.phi2,"phi2[nTracklets]/F");
  trackletTree23->Branch("r2",tdata23.r2,"r2[nTracklets]/F");
  trackletTree23->Branch("deta",tdata23.deta,"deta[nTracklets]/F");
  trackletTree23->Branch("dphi",tdata23.dphi,"dphi[nTracklets]/F");
  
  trackletTree23->Branch("npart",&tdata23.npart,"npart/I");
  trackletTree23->Branch("eta",tdata23.eta,"eta[npart]/F");
  trackletTree23->Branch("phi",tdata23.phi,"phi[npart]/F");
  trackletTree23->Branch("pdg",tdata23.pdg,"pdg[npart]/I");
  trackletTree23->Branch("chg",tdata23.chg,"chg[npart]/I");
  trackletTree23->Branch("nhad",tdata23.nhad,"nhad[12]/F");
  trackletTree23->Branch("pt",tdata23.pt,"pt[npart]/F");
  trackletTree23->Branch("evtType",&tdata23.evtType,"evtType/I");


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


  vector<TH1D*> layer3HitEta;
  layer3HitEta.reserve(nbins);

  vector<TH1D*> layer3HitPhi;
  layer3HitPhi.reserve(nbins);

  vector<TH2D*> layer32D;
  layer32D.reserve(nbins);
    
  for(int i = 0; i< nbins; ++i){
    layer1HitEta[i] = new TH1D(Form("dNdEtaHits1_%02d",i),"dNdEta Hits Layer 1",500,-3,3);
    layer2HitEta[i] = new TH1D(Form("dNdEtaHits2_%02d",i),"dNdEta Hits Layer 2",500,-3,3);
    layer3HitEta[i] = new TH1D(Form("dNdEtaHits3_%02d",i),"dNdEta Hits Layer 3",500,-3,3);
    layer1HitPhi[i] = new TH1D(Form("dNdPhiHits1_%02d",i),"dNdPhi Hits Layer 1",500,-3.2,3.2);
    layer2HitPhi[i] = new TH1D(Form("dNdPhiHits2_%02d",i),"dNdPhi Hits Layer 2",500,-3.2,3.2);
    layer3HitPhi[i] = new TH1D(Form("dNdPhiHits3_%02d",i),"dNdPhi Hits Layer 3",500,-3,3);
    layer12D[i] = new TH2D(Form("dNdEtadPhiHits1_%02d",i),"dNdPhidEta Hits Layer 1",500,-3,3,500,-3.2,3.2);
    layer22D[i] = new TH2D(Form("dNdEtadPhiHits2_%02d",i),"dNdPhidEta Hits Layer 2",500,-3,3,500,-3.2,3.2);
    layer32D[i] = new TH2D(Form("dNdEtadPhiHits3_%02d",i),"dNdPhidEta Hits Layer 3",500,-3,3,500,-3.2,3.2);
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
  t->SetBranchAddress("eta3",par.eta3);
  t->SetBranchAddress("phi3",par.phi3);
  t->SetBranchAddress("r3",par.r3);
  t->SetBranchAddress("nhits1",&par.nhits1);
  t->SetBranchAddress("nhits2",&par.nhits2);
  t->SetBranchAddress("nhits3",&par.nhits3);
  t->SetBranchAddress("mult",&par.mult);
  t->SetBranchAddress("vz",par.vz);
  t->SetBranchAddress("nv",&par.nv);
  t->SetBranchAddress("npart",&par.npart);
  t->SetBranchAddress("eta",&par.eta);
  t->SetBranchAddress("phi",&par.phi);
  t->SetBranchAddress("pt",&par.pt);
  t->SetBranchAddress("chg",&par.chg);
  t->SetBranchAddress("pdg",&par.pdg);

  if(!makeVzCut) t->SetBranchAddress("evtType",&par.evtType);
  cout <<"Number of Events: "<<t->GetEntries()<<endl;

  // Main loop ===========================================================================================
  for(int i =0;  i<t->GetEntries()&&i<1000000000 ; i = i + 1 + nPileUp){    
    t->GetEntry(i);
    if (i % 1000 == 0) {
       cout <<"Event "<<i<<" "
            <<trackletTree12->GetEntries()<<" "
            <<trackletTree23->GetEntries()<<" "
            <<trackletTree13->GetEntries()<<" "
            <<endl;    
    }       
    // Selection on Events
    
    // Fill reco vertex information
    tdata12.nv = par.nv+1;  
    tdata23.nv = par.nv+1;  
    tdata13.nv = par.nv+1;  
    for(int j = 1; j<par.nv;j++) {
       tdata12.vz[j+1] = par.vz[j];
       tdata23.vz[j+1] = par.vz[j];
       tdata13.vz[j+1] = par.vz[j];
    }
    // Fill MC vertex
    tdata12.vz[0] = par.vz[0];
    tdata23.vz[0] = par.vz[0];
    tdata13.vz[0] = par.vz[0];
    
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
    /*
    if (tdata12.nv == 2) tdata12.nv=3;
    if (tdata23.nv == 2) tdata23.nv=3;
    if (tdata13.nv == 2) tdata13.nv=3;
    */
    
    vector<RecoHit> layerRaw1;
    prepareHits(layerRaw1,par, cuts, 1, 0, splitProb, dropProb);
    vector<RecoHit> layerRaw2;
    prepareHits(layerRaw2,par, cuts, 2, 0, splitProb, dropProb);


    if (nPileUp!=0 && par.nhits1<10 ) {
       for (int j=1;j <= nPileUp ; j++) {
          t->GetEntry(i+j);
          prepareHits(layerRaw1,par, cuts, 1, 0, splitProb, dropProb);
          prepareHits(layerRaw2,par, cuts, 2, 0, splitProb, dropProb);
       }
       t->GetEntry(i);
    }

    
    double trackletVertex = 0;
    if (par.nhits1> vertexHitRegion) trackletVertex = par.vz[1]; else trackletVertex = TrackletVertexUnbin(layerRaw1,layerRaw2,0.14,0.08);
    
    // For particle gun =========
//    if (i % 1000 == 0) cout <<"!!! USE GEN VERTEX (FOR PARTICLE GUN)"<<endl;
//    trackletVertex = par.vz[0];
    //===========================

    // vz[1] is always the selected algorithm
    tdata12.vz[1] = trackletVertex;
    tdata23.vz[1] = trackletVertex;
    tdata13.vz[1] = trackletVertex;

    // use trackletVertex
    if (fabs(tdata12.vz[1])>cuts.vzCut && makeVzCut == 1) continue;
    

    // Process hits with Vz constraint:
    vector<RecoHit> layer1;
    prepareHits(layer1,par, cuts, 1, tdata12.vz[1], splitProb, dropProb);
    vector<RecoHit> layer2;
    prepareHits(layer2,par, cuts, 2, tdata12.vz[1], splitProb, dropProb);
    vector<RecoHit> layer3;
    prepareHits(layer3,par, cuts, 3, tdata12.vz[1], splitProb, dropProb);

    if (nPileUp!=0) {
       for (int j=1;j <= nPileUp ; j++) {
          t->GetEntry(i+j);
          prepareHits(layer1,par, cuts, 1, tdata12.vz[1], splitProb, dropProb);
          prepareHits(layer2,par, cuts, 2, tdata12.vz[1], splitProb, dropProb);
          prepareHits(layer3,par, cuts, 3, tdata12.vz[1], splitProb, dropProb);
       }
       t->GetEntry(i);
    }
    

    for(int ihit = 0; ihit< (int)layer1.size(); ++ihit) {
      int hitbin1 = (int)layer1.size();
      if (hitbin1 > 99) hitbin1 = 99;
      layer1HitEta[hitbin1]->Fill(layer1[ihit].eta);
      layer1HitPhi[hitbin1]->Fill(layer1[ihit].phi);
      layer12D[hitbin1]->Fill(layer1[ihit].eta,layer1[ihit].phi);
      nthit->Fill(layer1[ihit].phi,1);
      if(fabs(layer1[ihit].eta)<1) mult++;
    }

    for(int ihit = 0; ihit< (int)layer2.size(); ++ihit) {
      int hitbin2 = (int)layer2.size();
      if (hitbin2 > 99) hitbin2 = 99;
      layer2HitEta[hitbin2]->Fill(layer2[ihit].eta);
      layer2HitPhi[hitbin2]->Fill(layer2[ihit].phi);
      layer22D[hitbin2]->Fill(layer2[ihit].eta,layer2[ihit].phi);
      nthit->Fill(layer2[ihit].phi,2);
    }

    for(int ihit = 0; ihit< (int)layer3.size(); ++ihit) {
      int hitbin3 = (int)layer3.size();
      if (hitbin3 > 99) hitbin3 = 99;
      layer3HitEta[hitbin3]->Fill(layer3[ihit].eta);
      layer3HitPhi[hitbin3]->Fill(layer3[ihit].phi);
      layer32D[hitbin3]->Fill(layer3[ihit].eta,layer3[ihit].phi);
      nthit->Fill(layer3[ihit].phi,3);
    }

    // Form Tracklets        
    vector<Tracklet> protoTracklets12 = recoProtoTracklets(layer1,layer2);
    vector<Tracklet> recoTracklets12 = cleanTracklets(protoTracklets12,0,cuts);

    vector<Tracklet> protoTracklets13 = recoProtoTracklets(layer1,layer3);
    vector<Tracklet> recoTracklets13 = cleanTracklets(protoTracklets13,0,cuts);

    vector<Tracklet> protoTracklets23 = recoProtoTracklets(layer2,layer3);
    vector<Tracklet> recoTracklets23 = cleanTracklets(protoTracklets23,0,cuts);

    // Fill Ntuple
    tdata12.nTracklet = recoTracklets12.size();
    tdata12.nhit1 = layer1.size();
    tdata12.nhit2 = layer2.size();

    
    for (int j=0;j<(int)tdata12.nTracklet;j++)
    {
        tdata12.eta1[j] = recoTracklets12[j].eta1();	
        tdata12.eta2[j] = recoTracklets12[j].eta2();	
        tdata12.r1[j] = recoTracklets12[j].r1();	
        tdata12.r2[j] = recoTracklets12[j].r2();	
        tdata12.phi1[j] = recoTracklets12[j].phi1();	
        tdata12.phi2[j] = recoTracklets12[j].phi2();
	tdata12.deta[j] = recoTracklets12[j].deta();
   	tdata12.dphi[j] = recoTracklets12[j].dphi();
	tdata12.mult = (int)mult;	
    }

    tdata12.npart=0;
    for (int j=0;j<12;j++) tdata12.nhad[j]=0;

    for(int j=0;j<par.npart;j++)
    {
        if (fabs(par.eta[j])>3||par.chg[j]==0) continue;
//        if (fabs(par.eta[j])>3) continue;
	tdata12.eta[tdata12.npart]=par.eta[j];
	tdata12.phi[tdata12.npart]=par.phi[j];
	tdata12.chg[tdata12.npart]=par.chg[j];
	tdata12.pdg[tdata12.npart]=par.pdg[j];
	tdata12.pt[tdata12.npart]=par.pt[j];
        tdata12.npart++;
	int bin = (int)((par.eta[j]+3)*2);
	int pdg = (int)abs(par.pdg[j]);
	if (pdg== 211 || pdg == 321 || pdg == 2212 || pdg ==3122) tdata12.nhad[bin]++;
    }

    nhits->Fill(mult,layer1.size(),layer2.size());
    ntmult->Fill(mult,layer1.size(),layer2.size());

    tdata12.evtType = par.evtType;
    trackletTree12->Fill();

    tdata13.nTracklet = recoTracklets13.size();
    tdata13.nhit1 = layer1.size();
    tdata13.nhit2 = layer3.size();

    
    for (int j=0;j<(int)tdata13.nTracklet;j++)
    {
        tdata13.eta1[j] = recoTracklets13[j].eta1();	
        tdata13.eta2[j] = recoTracklets13[j].eta2();	
        tdata13.phi1[j] = recoTracklets13[j].phi1();	
        tdata13.phi2[j] = recoTracklets13[j].phi2();
        tdata13.r1[j] = recoTracklets13[j].r1();	
        tdata13.r2[j] = recoTracklets13[j].r2();	
 	tdata13.deta[j] = recoTracklets13[j].deta();
   	tdata13.dphi[j] = recoTracklets13[j].dphi();
	tdata13.mult = (int)mult;	
    }

    tdata13.npart=0;
    for (int j=0;j<12;j++) tdata13.nhad[j]=0;

    for(int j=0;j<par.npart;j++)
    {
        if (fabs(par.eta[j])>3||par.chg[j]==0) continue;
//        if (fabs(par.eta[j])>3) continue;
	tdata13.eta[tdata13.npart]=par.eta[j];
	tdata13.phi[tdata13.npart]=par.phi[j];
	tdata13.chg[tdata13.npart]=par.chg[j];
	tdata13.pdg[tdata13.npart]=par.pdg[j];
	tdata13.pt[tdata13.npart]=par.pt[j];
        tdata13.npart++;
	int bin = (int)((par.eta[j]+3)*2);
	int pdg = (int)abs(par.pdg[j]);
	if (pdg== 211 || pdg == 321 || pdg == 2213 || pdg ==3132) tdata13.nhad[bin]++;
    }

    nhits->Fill(mult,layer1.size(),layer2.size());
    ntmult->Fill(mult,layer1.size(),layer2.size());

    tdata13.evtType = par.evtType;
    trackletTree13->Fill();

    tdata23.nTracklet = recoTracklets23.size();
    tdata23.nhit1 = layer2.size();
    tdata23.nhit2 = layer3.size();

    
    for (int j=0;j<(int)tdata23.nTracklet;j++)
    {
        tdata23.eta1[j] = recoTracklets23[j].eta1();	
        tdata23.eta2[j] = recoTracklets23[j].eta2();	
        tdata23.phi1[j] = recoTracklets23[j].phi1();	
        tdata23.phi2[j] = recoTracklets23[j].phi2();
        tdata23.r1[j] = recoTracklets23[j].r1();	
        tdata23.r2[j] = recoTracklets23[j].r2();	
 	tdata23.deta[j] = recoTracklets23[j].deta();
   	tdata23.dphi[j] = recoTracklets23[j].dphi();
	tdata23.mult = (int)mult;	
    }

    tdata23.npart=0;
    for (int j=0;j<12;j++) tdata23.nhad[j]=0;

    for(int j=0;j<par.npart;j++)
    {
        if (fabs(par.eta[j])>3||par.chg[j]==0) continue;
//        if (fabs(par.eta[j])>3) continue;
	tdata23.eta[tdata23.npart]=par.eta[j];
	tdata23.phi[tdata23.npart]=par.phi[j];
	tdata23.chg[tdata23.npart]=par.chg[j];
	tdata23.pdg[tdata23.npart]=par.pdg[j];
	tdata23.pt[tdata23.npart]=par.pt[j];
        tdata23.npart++;
	int bin = (int)((par.eta[j]+3)*2);
	int pdg = (int)abs(par.pdg[j]);
	if (pdg== 211 || pdg == 321 || pdg == 2223 || pdg ==3232) tdata23.nhad[bin]++;
    }

    nhits->Fill(mult,layer1.size(),layer2.size());
    ntmult->Fill(mult,layer1.size(),layer2.size());

    tdata23.evtType = par.evtType;
    trackletTree23->Fill();
  }

  // Close outputfile ===================================================================================

  outf->Write();
  outf->Close(); 
}


