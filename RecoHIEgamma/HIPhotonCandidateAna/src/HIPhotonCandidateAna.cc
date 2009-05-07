
//
// Package:    HIPhotonCandidateAna
// Class:      HIPhotonCandidateAna
// 
/**\class HIPhotonCandidateAna HIPhotonCandidateAna.cc RecoHIEgamma/HIPhotonCandidateAna/src/HIPhotonCandidateAna.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yen-Jie Lee
//         Created:  Sun Apr 13 13:53:50 EDT 2008
// $Id: HIPhotonCandidateAna.cc,v 1.6 2009/05/06 18:56:44 yjlee Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"

#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"


#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"


#include "TNtuple.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include "HepMC/GenEvent.h"
#include "HepMC/HeavyIon.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"




#include "RecoHIEgamma/HIEgammaTools/interface/CandidateSuperClusterFinder.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CandidateGenParticleFinder.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CxCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/RxCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/TxyCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/dRxyCalculator.h"



#include "RecoHIEgamma/HIEgammaTools/interface/ShapeCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HIPhotonMCType.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HIMCGammaJetSignalDef.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HIGenMatch.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HICaloUtil.h"
#include "RecoEgamma/EgammaTools/interface/HoECalculator.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//
// class decleration
//

class HIPhotonCandidateAna : public edm::EDAnalyzer {
   public:
      explicit HIPhotonCandidateAna(const edm::ParameterSet&);
      ~HIPhotonCandidateAna();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  std::string GenCandInput;     // GenParticle Candidates
  std::string hiGenParticles;
  std::string SCCandInput;      // SuperCluster Candidates1
  std::string SCCandInput1;     // SuperCluster Candidates2
  std::string SCBInput;         // Barrel SuperClusters
  std::string SCEInput;         // Endcap SuperClusters
  std::string ShapeBInput;      // Barrel Clustershape
  std::string ShapeEInput;      // EndCap Clustershape
  //  std::string output;           // Output filename
  HoECalculator calc_;

  TNtuple *datatemp;
  TFile *f;
  double etCut;
  double etaCut;
  double dRCut;
  int GetHighestEtPart(const reco::CandidateCollection  *parts);
  Float_t CalcSuperR(Float_t *cx, Float_t *rx);
  Float_t CalcSuperCR(Float_t *ccx, Float_t *crx);
  Float_t CalcSuperE(const ClusterShape &sh,Float_t energy);
  int nEvent;
  int nTrigEvent;
  bool DoBarrel;
  bool DoEndcap;
  bool SignalOnly;

  edm::Service<TFileService> fs;           


};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HIPhotonCandidateAna::HIPhotonCandidateAna(const edm::ParameterSet& iConfig)

{
   // Input Tags
  GenCandInput = iConfig.getUntrackedParameter<std::string>("GenCandInput", "genParticles");
  hiGenParticles = iConfig.getUntrackedParameter<std::string>("src", "hiGenParticles");
  SCCandInput  = iConfig.getUntrackedParameter<std::string>("SCCandInput", "barrelclusters");
  SCCandInput1  = iConfig.getUntrackedParameter<std::string>( "SCCandInput1", "endcapclusters");
  SCBInput      = iConfig.getUntrackedParameter<std::string>("SCBInput", "correctedIslandBarrelSuperClusters");
  SCEInput      = iConfig.getUntrackedParameter<std::string>("SCEInput", "correctedIslandEndcapSuperClusters");
  ShapeBInput      = iConfig.getUntrackedParameter<std::string>("ShapeBInput", "islandBarrelShapeAssoc");
  ShapeEInput      = iConfig.getUntrackedParameter<std::string>("ShapeEInput", "islandEndcapShapeAssoc");
  DoBarrel      = iConfig.getUntrackedParameter<bool>("DoBarrel", true);
  DoEndcap      = iConfig.getUntrackedParameter<bool>("DoEndcap", true);
  SignalOnly      = iConfig.getUntrackedParameter<bool>("SignalOnly", false);
  
   // Output
   //   output = iConfig.getUntrackedParameter<std::string>("output", "ughuu.root");

   // Cuts
   etCut = iConfig.getUntrackedParameter<double>("etCut", 10);
   etaCut = iConfig.getUntrackedParameter<double>("etaCut", 3);
   dRCut = iConfig.getUntrackedParameter<double>("dRCut", 0.2);


}


HIPhotonCandidateAna::~HIPhotonCandidateAna()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HIPhotonCandidateAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;
   using namespace HepMC;

   int trigFlag=0;

   int jj = 0;
   
 
 
   //  const GenEvent* evt;
   //  Handle<CrossingFrame<HepMCProduct> > cf;
   //  iEvent.getByLabel(InputTag("mix","source"),cf);
   //  cout << " line " << jj << endl ; jj++;
   
   //  MixCollection<HepMCProduct> mix(cf.product());
   // int mixsize = mix.size();
   // evt = mix.getObject(mixsize-1).GetEvent();
   //  const HeavyIon* hi = evt->heavy_ion();
   // int nPart = hi->Npart_proj()+hi->Npart_targ();
   // double impactP = hi->impact_parameter();
   int nPart = 0;
   double impactP = 0;
 




   
   //   Handle<CandidateCollection> pGenCandInput;    This has only Background
   Handle<GenParticleCollection> pGenCandInput;
   iEvent.getByLabel(InputTag(GenCandInput), pGenCandInput);


  
  
   const GenParticleCollection *genParticlesBack = pGenCandInput.product();
   if(genParticlesBack == 0)
   {
      cout <<"***HIPhotonCandidateAna: GenParticleCandidate not found!"<<endl;
      return;
   }


   //  These are the GenCand Collection from HIGenParticles.  which has all the particles ( Background + signal)
   edm::Handle<GenParticleCollection> pGenCandInputHI;
   iEvent.getByLabel(InputTag(hiGenParticles),pGenCandInputHI);
   const GenParticleCollection *genParticlesHI = pGenCandInputHI.product();
   if(genParticlesHI == 0)
     {
       cout <<"***genparcilesHI is empty  !!"<<endl;
       return;
     }

   

   
    //   vector<reco::GenParticle>

   GenParticleCollection genParticlesP;
   GenParticleCollection* genParticles = &genParticlesP;
   HIGenMatch gm(genParticlesBack);
   
   int maxindexHI = (int)genParticlesHI->size();
   cout << "Number of Total Particles = " << maxindexHI << endl;
   cout << "Number of Background Particles = " << genParticlesBack->size() << endl;
   
   for ( int i = 0 ; i < maxindexHI ; i ++)
     {       
       const reco::GenParticle &cc = (*genParticlesHI)[i];
       if ( gm.IsMatched(cc) == true)  // which means this is background
	 { //cout << "matched!  it is background " << endl; 
	 continue;}
       else
	 { 
	 cout <<i<<endl;
	 //cout << "Not matched!  it is Signal " << endl;
	 //cout << "pdgId of this = " << cc.pdgId() << endl;
	 genParticles->push_back(cc);
	 }
     }   
   
   cout << "Number of signals = " << genParticles->size() << endl;
   
   
//   cout << " line jj " << endl; jj++;
   Handle<CandidateCollection> pSCCandInput;
   iEvent.getByLabel(InputTag(SCCandInput), pSCCandInput);
   const CandidateCollection *superClusters = pSCCandInput.product();
   if(superClusters == 0)
   {
      cout <<"***HIPhotonCandidateAna: SuperClusterCandidate not found!"<<endl;
      return;
   }
//   cout << " line jj " << endl; jj++;

   Handle<CandidateCollection> pSCCandInput1;
   iEvent.getByLabel(InputTag(SCCandInput1), pSCCandInput1);
   const CandidateCollection *superClusters1 = pSCCandInput1.product();
   if(superClusters1 == 0)
   {
      cout <<"***HIPhotonCandidateAna: SuperClusterCandidate1 not found!"<<endl;
      return;
   }
//   cout << " line jj " << endl; jj++;

   Handle<SuperClusterCollection> pSCBInput;
   iEvent.getByLabel(InputTag(SCBInput), pSCBInput);
   const SuperClusterCollection *SCBcollection = pSCBInput.product();
   if(SCBcollection == 0)
   {
      cout <<"***HIPhotonCandidateAna: SCBcollection not found!"<<endl;
      return;
   }
//   cout << " line jj " << endl; jj++;

   Handle<SuperClusterCollection> pSCEInput;
   iEvent.getByLabel(InputTag(SCEInput), pSCEInput);
   const SuperClusterCollection *SCEcollection = pSCEInput.product();
   if(SCEcollection == 0)
   {
      cout <<"***HIPhotonCandidateAna: SCEcollection not found!"<<endl;
      return;
   }

//   cout << " line jj " << endl; jj++;

   Handle<BasicClusterShapeAssociationCollection> hShapeB;
   iEvent.getByLabel(InputTag(ShapeBInput), hShapeB);
   const BasicClusterShapeAssociationCollection *shapeB = hShapeB.product();
   if(shapeB == 0 )
   {
      cout <<"***HIPhotonCandidateAna: ShapeB not found!"<<endl;
      return;
   }

//   cout << " line jj " << endl; jj++;

   Handle<BasicClusterShapeAssociationCollection> hShapeE;
   iEvent.getByLabel(InputTag(ShapeEInput), hShapeE);
   const BasicClusterShapeAssociationCollection *shapeE = hShapeE.product();
   if(shapeE == 0 )
   {
      cout <<"***HIPhotonCandidateAna: ShapeE not found!"<<endl;
      return;
   }

   std::auto_ptr<CandidateCollection> sClusters(new CandidateCollection);

   double maxEt=0;

//   cout << " line jj " << endl; jj++;


   if (DoBarrel) {
      // Loop over superCluster Candidates, Et cut and Eta cut   
     for(int i = 0; i < (int) superClusters->size(); i++)
       {
         const Candidate &p = (*superClusters)[i];
         if ((p.energy()/cosh(p.eta()))<etCut) continue;
         if ((p.energy()/cosh(p.eta()))>70) trigFlag=1;
         if (fabs(p.eta())>etaCut) continue;
         if ((p.energy()/cosh(p.eta()))>maxEt) maxEt = p.energy()/cosh(p.eta());
         sClusters->push_back(p.clone());

       }
   }

   if (DoEndcap) {   
      for(int i = 0; i < (int) superClusters1->size(); i++)
      {
         const Candidate &p = (*superClusters1)[i];
         if ((p.energy()/cosh(p.eta()))<etCut) continue;
         if ((p.energy()/cosh(p.eta()))>70) trigFlag=1;
         if (fabs(p.eta())>etaCut) continue;
         if ((p.energy()/cosh(p.eta()))>maxEt) maxEt = p.energy()/cosh(p.eta());
         sClusters->push_back(p.clone());
      }
   }

   int maxindex = (int)sClusters->size();

   // event counter
   if (trigFlag==1) nTrigEvent++;
   nEvent++;

   
   // Prepare Shape variable calculators
   CandidateSuperClusterFinder SCFinder;
   CxCalculator CxC(iEvent,iSetup);
   RxCalculator RxC(iEvent,iSetup);
   TxyCalculator Txy(iEvent,iSetup);
   dRxyCalculator dRxy(iEvent,iSetup);
   ShapeCalculator ShapeC(iEvent,iSetup);

   
//   cout <<"Endcap: "<<superClusters1->size()<<" Barrel: "<<superClusters->size()<<endl;


   HIMCGammaJetSignalDef mcisocut(genParticles);
   // Loop over SuperCluster Candidates
   for(int i = 0; i < maxindex; i++)
   {
      const Candidate &SCCand = (*sClusters)[i];

      CandidateGenParticleFinder mp(genParticles,SCCand,dRCut,1);

      
      Float_t var[300];
      for (int j = 0; j<100; j++) var[j]=0;

      if ((SCCand.energy()/cosh(SCCand.eta()))<etCut) continue;

      vector<Float_t> vv;
      int maxFlag=0;
      if ((SCCand.energy()/cosh(SCCand.eta()))==maxEt) maxFlag=1;
      vv.push_back(maxFlag);      
      vv.push_back(SCCand.energy());
      vv.push_back(SCCand.energy()/cosh(SCCand.eta()));
      vv.push_back(SCCand.eta());
      vv.push_back(SCCand.phi());
     
      Int_t id=0,photonid1=0;

      if (mp.GetTriggerGenParticle()!=0) {
	// Matched with Trigger GenParticle
	const Candidate &c1 = *(mp.GetTriggerGenParticle());
	
	vv.push_back(c1.energy());
	vv.push_back(c1.energy()/cosh(c1.eta()));
	vv.push_back(c1.eta());
	vv.push_back(c1.phi());
	vv.push_back(HICaloUtil::EcalEta(c1));
	vv.push_back(HICaloUtil::EcalPhi(c1));
	vv.push_back(c1.vertex().X());
	vv.push_back(c1.vertex().Y());
	vv.push_back(c1.vertex().Z());
	vv.push_back(float(c1.status()));
	
	vv.push_back(c1.pdgId()); 
	if (c1.mother()!=0) {
	  vv.push_back(c1.mother()->pdgId()); 
	  if (c1.mother()->mother()!=0) {
	    vv.push_back(c1.mother()->mother()->pdgId());
	  } else {
	    vv.push_back(0);
	  }
	} else {
	  vv.push_back(0);
	  vv.push_back(0);
	}
	
	HIPhotonMCType::EType photontype = HIPhotonMCType::kInvalid;
	HIPhotonMCType mctype;
	photontype = mctype.getType(c1);
	
	if (photontype == HIPhotonMCType::kInvalid) photonid1 = 6;
	else photonid1=(Int_t)photontype;
	
	vv.push_back(photonid1);
	
	// old yenjie's id
	if (photonid1 == 2) id=1; //kDirect
	if (photonid1 == 3 || photonid1 == 5) id=4; //KFrag+kBrems
	if (photonid1 == 4 || photonid1 == 6) id=0; //kDecay+kInvalid
	
	
//	cout <<"id="<<id<<" "<<SignalOnly<<endl;
	if (SignalOnly && id !=1 ) continue;
	
//	cout <<"oh id="<<id<<" "<<SignalOnly<<endl;
	
	vv.push_back(id);
	
	bool isMCIsolated = mcisocut.IsIsolated(c1);
	bool isMCSignal   = mcisocut.IsSignal(c1,3,true);
	
	int iNear = mcisocut.GetNearParton();
	int iAway = mcisocut.GetAwayParton();
	
	double delphi = -100;
	double phi1= -100;
	double phi2= -100;
	if (iNear!=-1&&iAway!=-1) {
	  const Candidate &p1 = (*genParticles)[iNear];
	  const Candidate &p2 = (*genParticles)[iAway];
	  delphi = mcisocut.getDeltaPhi(p1,p2);
	  phi1 = p1.phi();
	  phi2 = p2.phi();
	}
	
	vv.push_back(isMCIsolated);
	vv.push_back(isMCSignal);
	vv.push_back(delphi);
	vv.push_back(iNear);
	vv.push_back(iAway);
	vv.push_back(phi1);
	vv.push_back(phi2);

	//here 27//                                                                                                                                                                                                                 
	Float_t txy[4][5], drxy[4][5];
	
	for ( int x=1 ; x <=4 ; x++) {
	  for (  int y = 0 ; y <= 4 ; y++) {
	    txy[x][y] = Txy.getTxy(c1,x,y);
	    vv.push_back(txy[x][y]);
          }
	} //20 variables
	
	for ( int x=1 ; x <=4 ; x++) {
	  for (  int y = 1 ; y <= 5 ; y++) {
	    drxy[x][y] = dRxy.getDRxy(c1,x,y);
	    vv.push_back(drxy[x][y]);
	  }
	} // 20 variables
	

	
      } 
      else {
	// not matched, fill 0s.
	for (int it=0;it<62;it++) vv.push_back(0);
      }
      if (SignalOnly && id !=1 ) continue;
      const SuperCluster *SC = SCFinder.findbest(SCBcollection,SCEcollection,SCCand);

      BasicClusterShapeAssociationCollection::const_iterator seedItr; 
      BasicClusterRef SCSeed = SC->seed();

      if (SC!=0) {
         if (fabs(SC->eta())<1.479) {
            seedItr = shapeB->find(SCSeed);
         } else {
            seedItr = shapeE->find(SCSeed);
         }
                  
         const ClusterShape &sh = *(seedItr->val.get());

	 vv.push_back(sh.eMax());
	 vv.push_back(sh.e2nd());
	 vv.push_back(sh.e2x2());
	 vv.push_back(sh.e3x2());
	 vv.push_back(sh.e3x3());
	 vv.push_back(sh.e4x4());
	 vv.push_back(sh.e5x5());
	 vv.push_back(sh.e2x5Right());
	 vv.push_back(sh.e2x5Top());
	 vv.push_back(sh.e2x5Bottom());
	 vv.push_back(sh.e2x5Left());
	 vv.push_back(sh.e3x2Ratio());
	 vv.push_back(sh.covEtaEta());
	 vv.push_back(sh.covEtaPhi());
	 vv.push_back(sh.covPhiPhi());  
         vv.push_back(SC->clustersSize());
         vv.push_back(SC->getHitsByDetId().size());
         vv.push_back(SC->etaWidth());
         vv.push_back(SC->phiWidth());
         vv.push_back(SC->rawEnergy());
         vv.push_back(calc_(SC,iEvent,iSetup));

         if (1==2) {
         ShapeC.calculate(SC);
         TLorentzVector  thrust = ShapeC.thrust();
         double Sper = ShapeC.Sper(thrust);
         vv.push_back(Sper);
         double Moment = ShapeC.Moment();
         vv.push_back(Moment);
         vv.push_back(thrust.X());
         vv.push_back(thrust.Y());
 
         std::vector<TLorentzVector>& hitCollection = ShapeC.getVector();
         std::vector<TLorentzVector>& hitCollection2 = ShapeC.getVector2();

         SuperFoxWolfram sfw;
         sfw.fill(hitCollection,hitCollection);
         vv.push_back(sfw.R(0));
         vv.push_back(sfw.R(1));
         vv.push_back(sfw.R(2));
         vv.push_back(sfw.R(3));
         vv.push_back(sfw.R(4));
         vv.push_back(sfw.R(5));
         vv.push_back(sfw.R(6));

         SuperFoxWolfram sfw2;
         sfw2.fill(hitCollection2,hitCollection2);
         vv.push_back(sfw2.R(0));
         vv.push_back(sfw2.R(1));
         vv.push_back(sfw2.R(2));
         vv.push_back(sfw2.R(3));
         vv.push_back(sfw2.R(4));

         SuperFoxWolfram sfw3;
         sfw3.fill(hitCollection,hitCollection2);
         vv.push_back(sfw3.R(0));
         vv.push_back(sfw3.R(1));
         vv.push_back(sfw3.R(2));
         vv.push_back(sfw3.R(3));
         vv.push_back(sfw3.R(4));

         } else {
	          vv.resize(vv.size()+21);
         }

         Float_t cx[10],ccx[10],rx[10],crx[10],sccx[10],scrx[10];


	 
         for (int x=1;x<6;x++) {
            cx[x]=CxC.getCx(SC,x,0);
            vv.push_back(cx[x]);
         }

         for (int x=1;x<6;x++) {
            ccx[x]=CxC.getCCx(SC,x,0);
            vv.push_back(ccx[x]);
         }
	   
	 //    for (int x=1;x<10;x++) {
	 //    sccx[x]=CxC.getCCx(SC,(double)x/10.0,0);
	 //    vv.push_back(sccx[x]);
	 //  }

         for (int x=1;x<6;x++) {
            cx[x]=CxC.getCxRemoveSC(SC,x,0);
            vv.push_back(cx[x]);
         }

         for (int x=1;x<6;x++) {
            ccx[x]=CxC.getCCxRemoveSC(SC,x,0);
            vv.push_back(ccx[x]);
         }

         for (int x=1;x<6;x++) {
            cx[x]=CxC.getCx(SC,x,2);
            vv.push_back(cx[x]);
         }

         for (int x=1;x<6;x++) {
            ccx[x]=CxC.getCCx(SC,x,2);
            vv.push_back(ccx[x]);
         }

         for (int x=1;x<6;x++) {
            rx[x]=RxC.getRx(SC,x,0);
            vv.push_back(rx[x]);
         }

         for (int x=1;x<6;x++) {
            crx[x]=RxC.getCRx(SC,x,0);
            vv.push_back(crx[x]);
         }

	 //  for (int x=1;x<10;x++) {
	 //    scrx[x]=RxC.getCRx(SC,(double)x/10.0,0);
	 //    vv.push_back(scrx[x]);
	 //   }

         for (int x=1;x<6;x++) {
            rx[x]=RxC.getRx(SC,x,-10);
            vv.push_back(rx[x]);
         }

         for (int x=1;x<6;x++) {
            crx[x]=RxC.getCRx(SC,x,-10);
            vv.push_back(crx[x]);
         }

         vv.push_back(CxC.getCx(SC,10000,0));
         vv.push_back(RxC.getRx(SC,10000,0));
 
         vv.push_back(CalcSuperE(sh,SCCand.energy()));
         vv.push_back(CalcSuperR(cx,rx));
         vv.push_back(CalcSuperCR(ccx,crx));
         vv.push_back(CxC.getCorrection(SC,1,2,0));
         vv.push_back(CxC.getCorrection(SC,1,3,0));
         vv.push_back(CxC.getCorrection(SC,1,4,0));
         vv.push_back(CxC.getCorrection(SC,1,5,0));
         vv.push_back(CxC.getBCMax(SC,1));
         vv.push_back(CxC.getBCMax(SC,2));
         vv.push_back(CxC.getAvgBCEt(SC,1,0,1000,0));
         vv.push_back(CxC.getAvgBCEt(SC,2,0,1000,0));
         vv.push_back(CxC.getAvgBCEt(SC,3,0,1000,0));
         vv.push_back(CxC.getAvgBCEt(SC,4,0,1000,0));
         vv.push_back(CxC.getAvgBCEt(SC,5,0,1000,0));
         vv.push_back(CxC.getNBC(SC,1,0,1000,0));
         vv.push_back(CxC.getNBC(SC,2,0,1000,0));
         vv.push_back(CxC.getNBC(SC,3,0,1000,0));
         vv.push_back(CxC.getNBC(SC,4,0,1000,0));
         vv.push_back(CxC.getNBC(SC,5,0,1000,0));
	 
	 vv.push_back(float(nPart));
	 vv.push_back(impactP);
      } else {
         vv.resize(vv.size()+113+2);
      }

      for(int j=0;j<(int)vv.size();j++) var[j]=vv[j];

       if (vv.size()!=180) cout <<"ERROR!!!!! "<<vv.size()<<endl;

      if (SignalOnly && id !=1 ) continue; else      datatemp->Fill(var);
   }


   

}


// ------------ method called once each job just before starting event loop  ------------
void 
HIPhotonCandidateAna::beginJob(const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

   datatemp = fs->make<TNtuple>("gammas", "photon candidate info", 
			  "evt:e:et:eta:phi:"  //5
			  "ge:get:geta:gphi:cgeta:cgphi:vx:vy:vz:status:gid:gmid:ggmid:id1:id:mciso:mcsig:delphi:iNear:iAway:phi1:phi2:"  //22
			  "T10:T11:T12:T13:T14:T20:T21:T22:T23:T24:T30:T31:T32:T33:T34:T40:T41:T42:T43:T44:"  //20
                          "dR10:dR11:dR12:dR13:dR14:dR20:dR21:dR22:dR23:dR24:dR30:dR31:dR32:dR33:dR34:dR40:dR41:dR42:dR43:dR44:" //20
			  "emax:e2nd:e4:e6:e9:e16:e25:e2x5R:e2x5T:e2x5B:e2x5L:e3x2Ratio:" //12
			  "convEtaEta:convEtaPhi:convPhiPhi:nc:nhit:etawidth:phiwidth:rawE:HoE:" //9
			  "Spher:Moment:tx:ty:sfw0:sfw1:sfw2:sfw3:sfw4:sfw5:sfw6:msfw0:msfw1:msfw2:msfw3:msfw4:xsfw0:xsfw1:xsfw2:xsfw3:xsfw4:"  //21
			  "c1:c2:c3:c4:c5:cC1:cC2:cC3:cC4:cC5:" //10
			  //        "cC01:cC02:cC03:cC04:cC05:cC06:cC07:cC08:cC09:"
			  "rmc1:rmc2:rmc3:rmc4:rmc5:rmcC1:rmcC2:rmcC3:rmcC4:rmcC5:" //10
			  "nc1:nc2:nc3:nc4:nc5:ncC1:ncC2:ncC3:ncC4:ncC5:" //10
			  "r1:r2:r3:r4:r5:cR1:cR2:cR3:cR4:cR5:"  //10
			  //      "cR01:cR02:cR03:cR04:cR05:cR06:cR07:cR08:cR09:"
			  "nr1:nr2:nr3:nr4:nr5:ncR1:ncR2:ncR3:ncR4:ncR5:" //10
			  "ECAL:HCAL:SuperE:SuperR:SuperCR:ce2:ce3:ce4:ce5:bcMax:bc2nd:" //11
			  "avgBCEt1:avgBCEt2:avgBCEt3:avgBCEt4:avgBCEt5:" //5
				"NBC1:NBC2:NBC3:NBC4:NBC5:" //5
				"npart:b" //2
				
				);
   
   nEvent = 0;
   nTrigEvent = 0;

   // Get the calo geometry
   ESHandle<CaloGeometry> theCaloGeom;
   iSetup.get<CaloGeometryRecord>().get(theCaloGeom);


   calc_=HoECalculator(theCaloGeom);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
HIPhotonCandidateAna::endJob() {
   cout <<"HIPhotonCandidateAna: nEvent = "<<nEvent<<endl;
   cout <<"HIPhotonCandidateAna: nTrigEvent = "<<nTrigEvent;
}


int HIPhotonCandidateAna::GetHighestEtPart(const reco::CandidateCollection  *parts)
{
   if (parts==0) return -1;

   int highestEtspart = -1;
   double highestEt   = -1;

   int maxindex = (int)parts->size();

   for(int i = 0; i < maxindex; i++) {

      const Candidate &p = (*parts)[i];

      if (p.status()==3) continue;
      int pid = abs(p.pdgId());
      //no partons
      if(pid <= 9 || pid  == 21) continue;
      //no quarks, muons, neutrinos, gluons, W, H, Z, gravitons.
      //only leptons (e,mu,tau) gammas are allowed:
      if(pid<100 && pid!=11 && pid!=13 && pid!=15 && pid!=22) continue;
      if(highestEt<p.et()) {
         highestEt = p.et();
         highestEtspart = i;
      }
   }

   return highestEtspart;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HIPhotonCandidateAna);

Float_t HIPhotonCandidateAna::CalcSuperE(const ClusterShape &sh,Float_t energy)
{
   Float_t v[10];
   Float_t c[10];
   Float_t shift;

   shift=6.8050;
   c[0]=-3.1654;
   c[1]= 3.0321;
   c[2]=-0.5532;
   c[3]= 1.5758;
   c[4]=-5.5818;
   c[5]= 0.8712;
   c[6]= 5.4842;
   c[7]=-3.4702;
   c[8]=-1.2147;
   c[9]=-2.9002;
   
   v[0]=sh.eMax()/sh.e2x2();
   v[1]=sh.eMax()/sh.e3x3();
   v[2]=sh.eMax()/sh.e5x5();
   v[3]=sh.eMax()/energy;
   v[4]=sh.e2x2()/sh.e3x3();
   v[5]=sh.e2x2()/sh.e5x5();
   v[6]=sh.e2x2()/energy;
   v[7]=sh.e3x3()/sh.e5x5();
   v[8]=sh.e3x3()/energy;
   v[9]=sh.e5x5()/energy;

   Float_t superE=shift;
  
   for (int i=0;i<10;i++)
   {
      superE+=c[i]*v[i];
   }

   return superE;
}

Float_t HIPhotonCandidateAna::CalcSuperR(Float_t *cx, Float_t *rx)
{
   Float_t v[10];
   Float_t c[10];
   Float_t shift;

   shift=0.1863;
   c[0]= 0.0061;
   c[1]=-0.0004;
   c[2]=-0.0041;
   c[3]= 0.0086;
   c[4]=-0.0122;
   c[5]= 0.0145;
   c[6]=-0.0078;
   c[7]=-0.0039;
   c[8]=-0.0069;
   c[9]= 0.0019;
   
   Float_t superR=shift;
  
   for (int i=1;i<6;i++)
   {
      superR+=rx[i]*v[i-1];
      superR+=cx[i]*v[i+4];
   }

   return superR;
}

Float_t HIPhotonCandidateAna::CalcSuperCR(Float_t *ccx, Float_t *crx)
{
   Float_t v[10];
   Float_t c[10];
   Float_t shift;

   shift=0.1077;
   c[0]= 0.0049;
   c[1]=-0.0014;
   c[2]=-0.0029;
   c[3]=-0.0026;
   c[4]=-0.0015;
   c[5]= 0.0086;
   c[6]=-0.0044;
   c[7]=-0.0036;
   c[8]=-0.0019;
   c[9]=-0.0022;
   
   Float_t superCR=shift;
  
   for (int i=1;i<6;i++)
   {
      superCR+=crx[i]*v[i-1];
      superCR+=ccx[i]*v[i+4];
   }

   return superCR;
}
