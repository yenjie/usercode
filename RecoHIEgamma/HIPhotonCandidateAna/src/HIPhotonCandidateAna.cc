
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
// $Id$
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

#include "TNtuple.h"
#include "TFile.h"

#include "RecoHIEgamma/HIEgammaTools/interface/CandidateSuperClusterFinder.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CandidateGenParticleFinder.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CxCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/RxCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HIPhotonMCType.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HIMCGammaJetSignalDef.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HICaloUtil.h"

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
      std::string GenCandInput;
      std::string SCCandInput;
      std::string SCCandInput1;
      std::string SCBInput;
      std::string SCEInput;
      std::string ShapeBInput;
      std::string ShapeEInput;
      std::string output;
      TNtuple *datatemp;
      TFile *f;
      double etCut;
      double etaCut;
      int GetHighestEtPart(const reco::CandidateCollection  *parts);
      Float_t CalcSuperR(Float_t *cx, Float_t *rx);
      Float_t CalcSuperCR(Float_t *ccx, Float_t *crx);
      Float_t CalcSuperE(const ClusterShape &sh,Float_t energy);
      int nEvent;
      int nTrigEvent;
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
   //now do what ever initialization is needed
   GenCandInput = iConfig.getUntrackedParameter<std::string>("GenCandInput", "genParticleCandidates");
   SCCandInput  = iConfig.getUntrackedParameter<std::string>("SCCandInput", "barrelclusters");
   SCCandInput1  = iConfig.getUntrackedParameter<std::string>("SCCandInput1", "endcapclusters");
   SCBInput      = iConfig.getUntrackedParameter<std::string>("SCBInput", "correctedIslandBarrelSuperClusters");
   SCEInput      = iConfig.getUntrackedParameter<std::string>("SCEInput", "correctedIslandEndcapSuperClusters");
   ShapeBInput      = iConfig.getUntrackedParameter<std::string>("ShapeBInput", "islandBarrelShapeAssoc");
   ShapeEInput      = iConfig.getUntrackedParameter<std::string>("ShapeEInput", "islandEndcapShapeAssoc");

   output = iConfig.getUntrackedParameter<std::string>("output", "ughuu.root");

   etCut = iConfig.getUntrackedParameter<double>("etCut", 10);
   etaCut = iConfig.getUntrackedParameter<double>("etaCut", 2);
}


HIPhotonCandidateAna::~HIPhotonCandidateAna()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

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

   int trigFlag=0;

   Handle<CandidateCollection> pGenCandInput;
   iEvent.getByLabel(InputTag(GenCandInput), pGenCandInput);
   const CandidateCollection *genParticles = pGenCandInput.product();
   if(genParticles == 0)
   {
      cout <<"***HIPhotonCandidateAna: GenParticleCandidate not found!"<<endl;
      return;
   }

   Handle<CandidateCollection> pSCCandInput;
   iEvent.getByLabel(InputTag(SCCandInput), pSCCandInput);
   const CandidateCollection *superClusters = pSCCandInput.product();
   if(superClusters == 0)
   {
      cout <<"***HIPhotonCandidateAna: SuperClusterCandidate not found!"<<endl;
      return;
   }

   Handle<CandidateCollection> pSCCandInput1;
   iEvent.getByLabel(InputTag(SCCandInput1), pSCCandInput1);
   const CandidateCollection *superClusters1 = pSCCandInput1.product();
   if(superClusters1 == 0)
   {
      cout <<"***HIPhotonCandidateAna: SuperClusterCandidate1 not found!"<<endl;
      return;
   }

   Handle<SuperClusterCollection> pSCBInput;
   iEvent.getByLabel(InputTag(SCBInput), pSCBInput);
   const SuperClusterCollection *SCBcollection = pSCBInput.product();
   if(SCBcollection == 0)
   {
      cout <<"***HIPhotonCandidateAna: SCBcollection not found!"<<endl;
      return;
   }

   Handle<SuperClusterCollection> pSCEInput;
   iEvent.getByLabel(InputTag(SCEInput), pSCEInput);
   const SuperClusterCollection *SCEcollection = pSCEInput.product();
   if(SCEcollection == 0)
   {
      cout <<"***HIPhotonCandidateAna: SCEcollection not found!"<<endl;
      return;
   }


   Handle<BasicClusterShapeAssociationCollection> hShapeB;
   iEvent.getByLabel(InputTag(ShapeBInput), hShapeB);
   const BasicClusterShapeAssociationCollection *shapeB = hShapeB.product();
   if(shapeB == 0 )
   {
      cout <<"***HIPhotonCandidateAna: ShapeB not found!"<<endl;
      return;
   }

   Handle<BasicClusterShapeAssociationCollection> hShapeE;
   iEvent.getByLabel(InputTag(ShapeEInput), hShapeE);
   const BasicClusterShapeAssociationCollection *shapeE = hShapeE.product();
   if(shapeE == 0 )
   {
      cout <<"***HIPhotonCandidateAna: ShapeE not found!"<<endl;
      return;
   }

   std::auto_ptr<CandidateCollection> sClusters(new CandidateCollection);


   
   for(int i = 0; i < (int) superClusters->size(); i++)
   {
      const Candidate &p = (*superClusters)[i];
      if ((p.energy()/cosh(p.eta()))<etCut) continue;
      if ((p.energy()/cosh(p.eta()))>70) trigFlag=1;
      sClusters->push_back(p.clone());
   }

   for(int i = 0; i < (int) superClusters1->size(); i++)
   {
      const Candidate &p = (*superClusters1)[i];
      if ((p.energy()/cosh(p.eta()))<etCut) continue;
      if ((p.energy()/cosh(p.eta()))>70) trigFlag=1;
      if (fabs(p.eta())>etaCut) continue;
      sClusters->push_back(p.clone());
   }

   // event counter
   if (trigFlag==1) nTrigEvent++;
   nEvent++;

   int maxindex = (int)sClusters->size();

   CandidateSuperClusterFinder SCFinder;
   CxCalculator CxC(iEvent,iSetup);
   RxCalculator RxC(iEvent,iSetup);

   cout <<"endcap: "<<superClusters1->size()<<" barrel: "<<superClusters->size()<<endl;


   for(int i = 0; i < maxindex; i++)
   {
      const Candidate &SCCand = (*sClusters)[i];

      CandidateGenParticleFinder mp(genParticles,SCCand,0.15,10);      
      

      Float_t var[100];
      for (int j = 0; j<100; j++) var[j]=0;

      if ((SCCand.energy()/cosh(SCCand.eta()))<etCut) continue;

      vector<Float_t> vv;
      vv.push_back(SCCand.et());      
      vv.push_back(SCCand.energy());
      vv.push_back(SCCand.energy()/cosh(SCCand.eta()));
      vv.push_back(SCCand.eta());
      vv.push_back(SCCand.phi());


      if (mp.GetTriggerGenParticle()!=0) {
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
         vv.push_back(c1.pdgId());  
         

         if (c1.mother()!=0) vv.push_back(c1.mother()->pdgId()); else vv.push_back(0);


         HIPhotonMCType::EType photontype = HIPhotonMCType::kInvalid;
         HIPhotonMCType mctype;
         photontype = mctype.getType(c1);

         int photonid1;

         if (photontype == HIPhotonMCType::kInvalid) photonid1 = 6;
         else photonid1=(Int_t)photontype;

         vv.push_back(photonid1);

         // old yenjie's id
         Int_t id=0;
         if (photonid1 == 2) id=1; //kDirect
         if (photonid1 == 3 || photonid1 == 5) id=4; //KFrag+kBrems
         if (photonid1 == 4 || photonid1 == 6) id=0; //kDecay+kInvalid

         vv.push_back(id);

         HIMCGammaJetSignalDef mcisocut(genParticles);
         bool isMCIsolated = mcisocut.IsIsolated(c1);
         bool isMCSignal   = mcisocut.IsSignal(c1,3,true);

         int iNear = mcisocut.GetNearParton();
         int iAway = mcisocut.GetAwayParton();

         double delphi = 1e12;
         double phi1= 1e12;
         double phi2= 1e12;
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
      } else {
         // not matched
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
         vv.push_back(0);
      }
      const SuperCluster *SC = SCFinder.findbest(SCBcollection,SCEcollection,SCCand);

      BasicClusterShapeAssociationCollection::const_iterator seedItr; 
      BasicClusterRef SCSeed = SC->seed();
//      std::cout <<SCSeed.size()<<std::endl;
      if (SC!=0) {
         if (fabs(SC->eta())<1.479) {
            seedItr = shapeB->find(SCSeed);
            //std::cout << "[ASDGASDGASD] " << seedItr->val.get() << endl;
         } else {
            seedItr = shapeE->find(SCSeed);
            //std::cout << "[ASDGASDGASD] " << seedItr->val.get() << endl;
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
         Float_t cx[10],ccx[10],rx[10],crx[10];

         for (int x=1;x<6;x++) {
            cx[x]=CxC.getCx(SC,x,0);
            vv.push_back(cx[x]);
         }

         for (int x=1;x<6;x++) {
            ccx[x]=CxC.getCCx(SC,x,0);
            vv.push_back(ccx[x]);
         }

         for (int x=1;x<6;x++) {
            cx[x]=CxC.getCx(SC,x,-10);
            vv.push_back(cx[x]);
         }

         for (int x=1;x<6;x++) {
            ccx[x]=CxC.getCCx(SC,x,-10);
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

      } else {
         vv.resize(vv.size()+69);
      }

      for(int j=0;j<(int)vv.size();j++) var[j]=vv[j];

       if (vv.size()!=94) cout <<"ERROR!!!!! "<<vv.size()<<endl;

      datatemp->Fill(var);
   }

/*
   for (int i=0; i<sClusters->size(); i++) {
      const Candidate *p = &(*superClusters)[i];
      delete p;
   }
*/
//   delete sClusters;

}


// ------------ method called once each job just before starting event loop  ------------
void 
HIPhotonCandidateAna::beginJob(const edm::EventSetup&)
{
   TFile::TContext context(0);
   f = TFile::Open(output.c_str(), "recreate");

   datatemp = new TNtuple("gammas", "photon candidate info", 
            "evt:e:et:eta:phi:"
            "ge:get:geta:gphi:cgeta:cgphi:vx:vy:vz:gid:gmid:id1:id:mciso:mcsig:delphi:iNear:iAway:phi1:phi2:"
            "emax:e2nd:e4:e6:e9:e16:e25:e2x5R:e2x5T:e2x5B:e2x5L:e3x2Ratio:"
            "convEtaEta:convEtaPhi:convPhiPhi:nc:nhit:etawidth:phiwidth:rawE:"
            "c1:c2:c3:c4:c5:cC1:cC2:cC3:cC4:cC5:"
            "nc1:nc2:nc3:nc4:nc5:ncC1:ncC2:ncC3:ncC4:ncC5:"
            "r1:r2:r3:r4:r5:cR1:cR2:cR3:cR4:cR5:"
            "nr1:nr2:nr3:nr4:nr5:ncR1:ncR2:ncR3:ncR4:ncR5:"
            "ECAL:HCAL:SuperE:SuperR:SuperCR:ce2:ce3:ce4:ce5"
            );

   nEvent = 0;
   nTrigEvent = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void 
HIPhotonCandidateAna::endJob() {
   TFile::TContext context(f);

   datatemp->Write();

   f->Close();
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
