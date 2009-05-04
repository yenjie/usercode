#include <vector>
#include "Math/Vector3D.h"
#include <TRandom.h>

using namespace std;

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
};

class SelectionCriteria {
 public:

  double drCut   ;       // to remove double hit
  double dPhiCut ;       // to remove double hit
  double dEtaCut ;       // to remove double hit
  double vzCut   ;       // vertex cut

  bool verbose_ ;
  bool useDeltaPhi_;
  bool checkSecondLayer_;
};

class Parameters {
 public:

  float eta1[50000],phi1[50000],r1[50000],eta2[50000],phi2[50000],r2[50000],vz[5000];
  float eta[50000],phi[50000],chg[50000],pdg[50000];
  int nhits1,nhits2,mult,nv,npart,evtType;
};

class TrackletData {
 public:

  float eta1[50000],phi1[50000],eta2[50000],phi2[50000],vz[5000];
  float deta[50000],dphi[50000];
  float eta[50000],phi[50000],chg[50000],pdg[50000],nhad[12];
  int nTracklet,nhit1,nhit2,mult,nv,npart,evtType;
};

bool compareEta(RecoHit a,RecoHit b) { return a.eta<b.eta;}
bool comparePhi(RecoHit a,RecoHit b) { return a.phi<b.phi;}
bool compareAbsEta(RecoHit a,RecoHit b) { return fabs(a.eta)<fabs(b.eta);}

double calcDphi(double phi1,double phi2);


vector<RecoHit> removeDoubleHits(Parameters par, SelectionCriteria cuts,Int_t
layer, double vz)
{
  vector<RecoHit> hits;
  vector<RecoHit> cleanedHits;

  if (layer == 1) {
    for(int ihit = 0; ihit < par.nhits1; ++ihit){
      RecoHit tmp(par.eta1[ihit],par.phi1[ihit],par.r1[ihit]);
      hits.push_back(tmp);
    }
  } else {
    for(int ihit = 0; ihit < par.nhits2; ++ihit){
      RecoHit tmp(par.eta2[ihit],par.phi2[ihit],par.r2[ihit]);
      hits.push_back(tmp);
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
      if (dr>cuts.drCut && dphi < cuts.dPhiCut) flag=1;
//      if (dphi > cuts.dPhiCut) k=0;
      }
    }
      
    if (flag==1) continue;  
    // recalculate eta and phi
    double x = hits[ihit].r*cos(hits[ihit].phi);
    double y = hits[ihit].r*sin(hits[ihit].phi);
    double z = hits[ihit].r/tan(atan(exp(-hits[ihit].eta))*2);

    ROOT::Math::XYZVector tmpVector(x,y,z-vz);
    RecoHit tmpHit(tmpVector.eta(),tmpVector.phi(),tmpVector.rho());
    cleanedHits.push_back(tmpHit);      
  }
  return cleanedHits;
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
