#include <vector>
#include <algorithm>
#include <Math/Vector3D.h>
#include <TRandom.h>
#include <TTree.h>
#include <TMath.h>

using namespace std;
#define maxEntry 30000
#define maxEntry2 30000

class RecoHit {
   public:

   RecoHit(float _eta,float _phi,float _r,float _cs) 
   { 
      eta = _eta;
      phi = _phi;
      r = _r;
      cs = _cs;
   }; 
   RecoHit(float _eta,float _phi,float _r,float _cs, float _l) 
   { 
      eta = _eta;
      phi = _phi;
      r = _r;
      cs = _cs;
      layer = _l;
   }; 
   
   ~RecoHit(){};
   
      float eta;
      float phi;
      float r;
      float layer;
      float cs;   // cluster size
      float ch;   // cluster charge
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
  bool fl1[maxEntry],fl2[maxEntry],fl3[maxEntry];

  float vx[10];
  float vy[10];
  float vz[10];
  float eta1[maxEntry],phi1[maxEntry],r1[maxEntry],cs1[maxEntry];//,ch1[maxEntry];
  float eta2[maxEntry],phi2[maxEntry],r2[maxEntry],cs2[maxEntry];//,ch2[maxEntry];
  float eta3[maxEntry],phi3[maxEntry],r3[maxEntry],cs3[maxEntry];//,ch3[maxEntry];
  //float etaF2[maxEntry],phiF2[maxEntry],rF2[maxEntry],csF2[maxEntry],chF2[maxEntry];
  float eta[maxEntry],phi[maxEntry],pt[maxEntry];
  int nhits1,nhits2,nhits3,nhitsF2,mult,nv,npart,evtType,chg[maxEntry],pdg[maxEntry];
  float npxhits,vtxqual;
  float hf,hftp,hftm,eb,eep,eem,nparti,npartiSigma,ncoll,ncollSigma,nhard,nhardSigma,b,bSigma;
  int cBin,nbins,binsize;
};

class TrackletData {
 public:
  int   nRun,nEv,nLumi,nHltBit,nL1ABit,nL1TBit,nBX,nHFn,nHFp,nHits;
  bool hltBit[500];
  bool l1ABit[500];
  bool l1TBit[500];
  float eta1[maxEntry],phi1[maxEntry],r1[maxEntry],cs1[maxEntry];//,ch1[maxEntry];
  float eta2[maxEntry],phi2[maxEntry],r2[maxEntry],cs2[maxEntry];//,ch2[maxEntry];
  ULong64_t weight[maxEntry];
  float vx[10];
  float vy[10];
  float vz[10];
  float deta[maxEntry2],dphi[maxEntry2];
  float eta[maxEntry2],phi[maxEntry2],nhad[12],pt[maxEntry2];
  int chg[maxEntry2],pdg[maxEntry2];
  ULong64_t weight2[maxEntry2];
  float npxhits,vtxqual,vtxQualCut;
  int nTracklet,nhit1,nhit2,mult2,nv,npart,evtType,trackletType;
  ULong64_t mult,nProtoTracklet;
  float hf,hftp,hftm,eb,eep,eem,nparti,npartiSigma,ncoll,ncollSigma,nhard,nhardSigma,b,bSigma;
  int cBin,nbins,binsize;
};

bool compareEta(RecoHit a,RecoHit b) { return a.eta<b.eta;}
bool comparePhi(RecoHit a,RecoHit b) { return a.phi<b.phi;}
bool compareAbsEta(RecoHit a,RecoHit b) { return fabs(a.eta)<fabs(b.eta);}

double calcDphi(double phi1,double phi2);


void prepareHits(vector<RecoHit> &cleanedHits, Parameters par, SelectionCriteria cuts,Int_t
layer, double vx, double vy, double vz, double splitProb = 0, double dropProb = 0, bool
cutOnClusterSize = 0, double runNum = 0,double nLumi = 0, bool isMC = 0, bool
useV = false)
{
  vector<RecoHit> hits;
  static Bool_t firstCall = 0;
  
  double smearX=0,smearY=0;

  double x0,y0;

  // The beamspot for each run

  if (useV) {
     x0 = vx;
     y0 = vy;
  } else {
    if (runNum == 1) {
     
     if (isMC==0) {
        x0 = 0.2436;
        y0 = 0.3840;
     } else {
        x0 = 0.03;
        y0 = 0;
     }
        x0 = 0.2436;
        y0 = 0.3840;
//        x0 = 0.03;
//        y0 = 0.0;
    }
  }

  if (layer == 1) {
    for(int ihit = 0; ihit < par.nhits1; ++ihit){
      // Reject
      if (par.phi1[ihit]>-1.395&&par.phi1[ihit]<-1.105&&par.eta1[ihit]>1.085&&par.eta1[ihit]<1.725) continue;

      if (par.phi1[ihit]>1.57&&par.phi1[ihit]<1.77&&par.eta1[ihit]>-0.27&&par.eta1[ihit]<-0.02) continue;
//      if (par.fl1[ihit]) continue;
      RecoHit tmp(par.eta1[ihit],par.phi1[ihit],par.r1[ihit],par.cs1[ihit]);
      if (gRandom->Rndm()<dropProb) continue;
      hits.push_back(tmp);
      // put artifical split hits
      if (gRandom->Rndm()<splitProb) hits.push_back(tmp);
    }
  } else if (layer == 2){
    for(int ihit = 0; ihit < par.nhits2; ++ihit){

      // for Run 124120 && 12402x
      if (par.phi2[ihit]>2.98&&par.phi2[ihit]<3.2&&par.eta2[ihit]>-2.1&&par.eta2[ihit]<-1.71) continue;

//      if (par.fl2[ihit]) continue;
      RecoHit tmp(par.eta2[ihit],par.phi2[ihit],par.r2[ihit],par.cs2[ihit]);
      if (gRandom->Rndm()<dropProb) continue;
      hits.push_back(tmp);
      // put artifical split hits
      if (gRandom->Rndm()<splitProb) hits.push_back(tmp);
    }
  } else if (layer == 3){
    for(int ihit = 0; ihit < par.nhits3; ++ihit){
//      if (par.fl3[ihit]) continue;
      RecoHit tmp(par.eta3[ihit],par.phi3[ihit],par.r3[ihit],par.cs3[ihit]);

      if (gRandom->Rndm()<dropProb) continue;
      hits.push_back(tmp);
      // put artifical split hits
      if (gRandom->Rndm()<splitProb) hits.push_back(tmp);
    }
  }
    
  for(int ihit = 0; ihit < (int)hits.size(); ++ihit) {
    // recalculate eta and phi
    float x = hits[ihit].r*cos(hits[ihit].phi);
    float y = hits[ihit].r*sin(hits[ihit].phi);
    float z = hits[ihit].r/tan(atan(exp(-hits[ihit].eta))*2);

    if (vz!=0&&firstCall==0) {
       cout << "Beamspot X0 = "<<x0<<" Y0 = "<<y0<<endl;
       firstCall=1;
    }

    ROOT::Math::XYZVector tmpVector(x-x0,y-y0,z-vz); //vtx fit ?? (temporarily)
    RecoHit tmpHit(tmpVector.eta(),tmpVector.phi(),tmpVector.rho(),hits[ihit].cs);
    float eta = tmpVector.eta();

    if (cutOnClusterSize && fabs(eta)< 0.5 &&                  hits[ihit].cs < 1) continue;
    if (cutOnClusterSize && fabs(eta)<=1.0 && fabs(eta)>0.5 && hits[ihit].cs < 2) continue;
    if (cutOnClusterSize && fabs(eta)<=1.5 && fabs(eta)>1.0 && hits[ihit].cs < 3) continue;
    if (cutOnClusterSize && fabs(eta)<=2.0 && fabs(eta)>1.5 && hits[ihit].cs < 4) continue;
    if (cutOnClusterSize && fabs(eta)<=2.5 && fabs(eta)>2.0 && hits[ihit].cs < 6) continue;
    if (cutOnClusterSize && fabs(eta)<=5.0 && fabs(eta)>2.5 && hits[ihit].cs < 9) continue;


  cleanedHits.push_back(tmpHit);      
  }
}



RecoHit RandomHit(double etaMin, double etaMax, double phiMin, double phiMax)
{
   double eta = etaMin + (etaMax-etaMin)*gRandom->Rndm();
   double phi = phiMin + (phiMax-phiMin)*gRandom->Rndm();
   RecoHit myRandomHit(eta,phi,0,100);
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
  t->SetBranchAddress("cs1",par.cs1);
  t->SetBranchAddress("fl1",par.fl1);
  t->SetBranchAddress("eta2",par.eta2);
  t->SetBranchAddress("phi2",par.phi2);
  t->SetBranchAddress("r2",par.r2);
  t->SetBranchAddress("cs2",par.cs2);
  t->SetBranchAddress("fl2",par.fl2);
  t->SetBranchAddress("eta3",par.eta3);
  t->SetBranchAddress("phi3",par.phi3);
  t->SetBranchAddress("r3",par.r3);
  t->SetBranchAddress("cs3",par.cs3);
  t->SetBranchAddress("fl3",par.fl3);
  t->SetBranchAddress("nhits1",&par.nhits1);
  t->SetBranchAddress("nhits2",&par.nhits2);
  t->SetBranchAddress("nhits3",&par.nhits3);

  // pixel froward  
  /*
  t->SetBranchAddress("etaF2",par.etaF2);
  t->SetBranchAddress("phiF2",par.phiF2);
  t->SetBranchAddress("rF2",par.rF2);
  t->SetBranchAddress("nhitsF2",&par.nhitsF2);
  */
  
  t->SetBranchAddress("vz",par.vz);
  t->SetBranchAddress("nv",&par.nv);
  t->SetBranchAddress("npart",&par.npart);
  t->SetBranchAddress("eta",&par.eta);
  t->SetBranchAddress("phi",&par.phi);
  t->SetBranchAddress("pt",&par.pt);
  t->SetBranchAddress("chg",&par.chg);
  t->SetBranchAddress("pdg",&par.pdg);
  t->SetBranchAddress("hf",&par.hf);
  t->SetBranchAddress("hftp",&par.hftp);
  t->SetBranchAddress("hftm",&par.hftm);
  t->SetBranchAddress("eb",&par.eb);
  t->SetBranchAddress("eep",&par.eep);
  t->SetBranchAddress("eem",&par.eem);
  t->SetBranchAddress("cBin",&par.cBin);
  t->SetBranchAddress("nbins",&par.nbins);
  t->SetBranchAddress("binsize",&par.binsize);
  t->SetBranchAddress("nparti",&par.nparti);
  t->SetBranchAddress("npartiSigma",&par.npartiSigma);
  t->SetBranchAddress("ncoll",&par.ncoll);
  t->SetBranchAddress("ncollSigma",&par.ncollSigma);
  t->SetBranchAddress("nhard",&par.nhard);
  t->SetBranchAddress("nhardSigma",&par.nhardSigma);
  t->SetBranchAddress("b",&par.b);
  t->SetBranchAddress("bSigma",&par.bSigma);
  
  t->SetBranchAddress("vtxqual",&par.vtxqual);
  t->SetBranchAddress("npxhits",&par.npxhits);
  t->SetBranchAddress("npxhits",&par.npxhits);
  t->SetBranchAddress("b",&par.b);
}
