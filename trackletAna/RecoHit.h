#include <vector>
#include <algorithm>
#include "Math/Vector3D.h"
#include <TRandom.h>

using namespace std;
#define maxEntry 2000
#define maxEntry2 30000

class RecoHit {
   public:

   RecoHit(double _eta,double _phi,double _r) 
   { 
      eta = _eta;
      phi = _phi;
      r = _r;
   }; 
   RecoHit(double _eta,double _phi,double _r,double _l) 
   { 
      eta = _eta;
      phi = _phi;
      r = _r;
      layer = _l;
   }; 
   
   ~RecoHit(){};
   
      double eta;
      double phi;
      double r;
      double layer;
      double cs;   // cluster size
      double ch;   // cluster charge
};

class SelectionCriteria {
 public:

  double drCut   ;       // to remove double hit
  double dPhiCut ;       // to remove double hit
  double dEtaCut ;       // to remove double hit
  double vzCut   ;       // vertex cut

  bool verbose_ ;
  bool useDeltaPhi_;
  bool useDeltaRho_;
  bool checkSecondLayer_;
};

class Parameters {
 public:

  int   nRun,nEv,nLumi,nHltBit,nL1ABit,nL1TBit,nBX,nHFn,nHFp,nHits;
  bool hltBit[500];
  bool l1ABit[500];
  bool l1TBit[500];
  bool l1ABitVsBx[500][5];
  bool l1TBitVsBx[500][5];

  float vx[maxEntry];
  float vy[maxEntry];
  float vz[maxEntry];
  float eta1[maxEntry],phi1[maxEntry],r1[maxEntry],cs1[maxEntry],ch1[maxEntry];
  float eta2[maxEntry],phi2[maxEntry],r2[maxEntry],cs2[maxEntry],ch2[maxEntry];
  float eta3[maxEntry],phi3[maxEntry],r3[maxEntry],cs3[maxEntry],ch3[maxEntry];
  float etaF2[maxEntry],phiF2[maxEntry],rF2[maxEntry],csF2[maxEntry],chF2[maxEntry];
  float eta[maxEntry],phi[maxEntry],pt[maxEntry];
  int nhits1,nhits2,nhits3,nhitsF2,mult,nv,npart,evtType,chg[maxEntry],pdg[maxEntry];
};

class TrackletData {
 public:
  int   nRun,nEv,nLumi,nHltBit,nL1ABit,nL1TBit,nBX,nHFn,nHFp,nHits;
  bool hltBit[500];
  bool l1ABit[500];
  bool l1TBit[500];
  float eta1[maxEntry2],phi1[maxEntry2],eta2[maxEntry2],phi2[maxEntry2];
  float vx[maxEntry2];
  float vy[maxEntry2];
  float vz[maxEntry2];
  float r1[maxEntry2],r2[maxEntry2];
  float deta[maxEntry2],dphi[maxEntry2];
  float eta[maxEntry2],phi[maxEntry2],chg[maxEntry2],pdg[maxEntry2],nhad[12],pt[maxEntry2];
  int nTracklet,nhit1,nhit2,mult,nv,npart,evtType,trackletType;
};

bool compareEta(RecoHit a,RecoHit b) { return a.eta<b.eta;}
bool comparePhi(RecoHit a,RecoHit b) { return a.phi<b.phi;}
bool compareAbsEta(RecoHit a,RecoHit b) { return fabs(a.eta)<fabs(b.eta);}

double calcDphi(double phi1,double phi2);


void prepareHits(vector<RecoHit> &cleanedHits, Parameters par, SelectionCriteria cuts,Int_t
layer, double vx, double vy, double vz, double splitProb = 0, double dropProb = 0)
{
  vector<RecoHit> hits;

  if (layer == 1) {
    for(int ihit = 0; ihit < par.nhits1; ++ihit){
      // Reject
      if (par.phi1[ihit]>-1.395&&par.phi1[ihit]<-1.105&&par.eta1[ihit]>1.085&&par.eta1[ihit]<1.725) continue;

      if (par.phi1[ihit]>1.57&&par.phi1[ihit]<1.77&&par.eta1[ihit]>-0.27&&par.eta1[ihit]<-0.02) continue;
      
      RecoHit tmp(par.eta1[ihit],par.phi1[ihit],par.r1[ihit]);
      if (gRandom->Rndm()<dropProb) continue;
      hits.push_back(tmp);
      // put artifical split hits
      if (gRandom->Rndm()<splitProb) hits.push_back(tmp);
    }
  } else if (layer == 2){
    for(int ihit = 0; ihit < par.nhits2; ++ihit){
      RecoHit tmp(par.eta2[ihit],par.phi2[ihit],par.r2[ihit]);
      if (gRandom->Rndm()<dropProb) continue;
      hits.push_back(tmp);
      // put artifical split hits
      if (gRandom->Rndm()<splitProb) hits.push_back(tmp);
    }
  } else if (layer == 3){
    for(int ihit = 0; ihit < par.nhits3; ++ihit){
      RecoHit tmp(par.eta3[ihit],par.phi3[ihit],par.r3[ihit]);
      if (gRandom->Rndm()<dropProb) continue;
      hits.push_back(tmp);
      // put artifical split hits
      if (gRandom->Rndm()<splitProb) hits.push_back(tmp);
    }
  }
  sort (hits.begin(),hits.end(),comparePhi);
    
  for(int ihit = 0; ihit < (int)hits.size(); ++ihit) {
    double dr=0;
    double dphi=10;
    double deta=10;
    int flag=0;
    if (ihit !=0) {

      for (int k=ihit-1;k<ihit;k++) {
      dphi = fabs(calcDphi(hits[k].phi, hits[ihit].phi));
      
      deta = fabs(hits[k].eta - hits[ihit].eta);
      dr   = fabs(hits[k].r - hits[ihit].r);

// no double hit removal...
//      if (dr>cuts.drCut && dphi < cuts.dPhiCut) flag=1;
//      if (dphi > cuts.dPhiCut) k=0;
      }
    }
      
    if (flag==1) continue;  
    // recalculate eta and phi
    double x = hits[ihit].r*cos(hits[ihit].phi);
    double y = hits[ihit].r*sin(hits[ihit].phi);
    double z = hits[ihit].r/tan(atan(exp(-hits[ihit].eta))*2);

//    ROOT::Math::XYZVector tmpVector(x-vx,y-vy,z-vz);
//   ROOT::Math::XYZVector tmpVector(x-0.192598,y-0.150772,z-vz);
// Run 123596
   ROOT::Math::XYZVector tmpVector(x-0.174562,y-0.144887,z-vz);
// Run 124022
// x0 = 0.192372
// y0 = 0.162306
//   ROOT::Math::XYZVector tmpVector(x-0.192372,y-0.162306,z-vz); //refitted

// Run 124120
//   ROOT::Math::XYZVector tmpVector(x-0.11811,y-0.0244726,z-vz); //refitted
//   ROOT::Math::XYZVector tmpVector(x-0.205124,y-0.164012,z-vz);
//   ROOT::Math::XYZVector tmpVector(x,y,z-vz);
//  ROOT::Math::XYZVector tmpVector(x,y,z-vz);
    RecoHit tmpHit(tmpVector.eta(),tmpVector.phi(),tmpVector.rho());
    cleanedHits.push_back(tmpHit);      
  }
}



RecoHit RandomHit(double etaMin, double etaMax, double phiMin, double phiMax)
{
   double eta = etaMin + (etaMax-etaMin)*gRandom->Rndm();
   double phi = phiMin + (phiMax-phiMin)*gRandom->Rndm();
   RecoHit myRandomHit(eta,phi,0);
   return myRandomHit;
}

double calcDphi(double phi1_,double phi2_)
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

void combineRecHit(vector<RecoHit> &c, vector<RecoHit> a,vector<RecoHit> b)
{
   for (unsigned int i=0;i<a.size();i++) {
      c.push_back(a[i]);
   }
   for (unsigned int i=0;i<b.size();i++) {
      c.push_back(b[i]);
   }
}

void getPixelTreeBranch(TTree *t, Parameters &par)
{
  t->SetBranchAddress("nRun",&par.nRun);
  t->SetBranchAddress("nEv",&par.nEv);
  t->SetBranchAddress("nLumi",&par.nLumi);
  t->SetBranchAddress("nBX",&par.nBX);
  t->SetBranchAddress("nHFn",&par.nHFn);
  t->SetBranchAddress("nHFp",&par.nHFp);

  t->SetBranchAddress("nHltBit",&par.nHltBit);
  t->SetBranchAddress("hltBit",par.hltBit);

  t->SetBranchAddress("nL1A",&par.nL1ABit);
  t->SetBranchAddress("L1A",par.l1ABit);

  t->SetBranchAddress("nL1T",&par.nL1TBit);
  t->SetBranchAddress("L1T",par.l1TBit);

//  t->SetBranchAddress("L1AVsBX",par.l1ABitVsBX);
//  t->SetBranchAddress("L1TVsBX",par.l1TBitVsBX);


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

  // pixel froward  
  t->SetBranchAddress("etaF2",par.etaF2);
  t->SetBranchAddress("phiF2",par.phiF2);
  t->SetBranchAddress("rF2",par.rF2);
  t->SetBranchAddress("nhitsF2",&par.nhitsF2);

  t->SetBranchAddress("vz",par.vz);
  t->SetBranchAddress("nv",&par.nv);
  t->SetBranchAddress("npart",&par.npart);
  t->SetBranchAddress("eta",&par.eta);
  t->SetBranchAddress("phi",&par.phi);
  t->SetBranchAddress("pt",&par.pt);
  t->SetBranchAddress("chg",&par.chg);
  t->SetBranchAddress("pdg",&par.pdg); 
}
