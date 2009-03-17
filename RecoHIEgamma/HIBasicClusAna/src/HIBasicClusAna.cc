// -*- C++ -*-
//
// Package:    HIBasicClusAna
// Class:      HIBasicClusAna
// 
/**\class HIBasicClusAna HIBasicClusAna.cc RecoHIEgamma/HIBasicClusAna/src/HIBasicClusAna.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yen-Jie Lee
//         Created:  Mon Jul  7 02:21:19 EDT 2008
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

//#include "DataFormats/Candidate/interface/CandidateFwd.h"
//#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"

#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"


#include "TNtuple.h"
#include "TFile.h"

#include "RecoHIEgamma/HIEgammaTools/interface/CandidateSuperClusterFinder.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CandidateGenParticleFinder.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CxCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/RxCalculator.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HIPhotonMCType.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HIMCGammaJetSignalDef.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HICaloUtil.h"
#include "RecoEgamma/EgammaTools/interface/HoECalculator.h"


//
// class decleration
//

class HIBasicClusAna : public edm::EDAnalyzer {
   public:
      explicit HIBasicClusAna(const edm::ParameterSet&);
      ~HIBasicClusAna();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      std::string GenCandInput;     // GenParticle Candidates
      std::string SCCandInput;      // SuperCluster Candidates1
      std::string SCCandInput1;     // SuperCluster Candidates2
      std::string SCBInput;         // Barrel SuperClusters
      std::string SCEInput;         // Endcap SuperClusters
      std::string ShapeBInput;      // Barrel Clustershape 
      std::string ShapeEInput;      // EndCap Clustershape 
      std::string output;           // Output filename
      HoECalculator calc_;


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
HIBasicClusAna::HIBasicClusAna(const edm::ParameterSet& iConfig)

{
   // Input Tags
   GenCandInput = iConfig.getUntrackedParameter<std::string>("GenCandInput", "genParticleCandidates");
   SCCandInput  = iConfig.getUntrackedParameter<std::string>("SCCandInput", "barrelclusters");
   SCCandInput1  = iConfig.getUntrackedParameter<std::string>("SCCandInput1", "endcapclusters");
   SCBInput      = iConfig.getUntrackedParameter<std::string>("SCBInput", "correctedIslandBarrelSuperClusters");
   SCEInput      = iConfig.getUntrackedParameter<std::string>("SCEInput", "correctedIslandEndcapSuperClusters");
   ShapeBInput      = iConfig.getUntrackedParameter<std::string>("ShapeBInput", "islandBarrelShapeAssoc");
   ShapeEInput      = iConfig.getUntrackedParameter<std::string>("ShapeEInput", "islandEndcapShapeAssoc");

   // Output
   output = iConfig.getUntrackedParameter<std::string>("output", "ughuu.root");

   // Cuts
   etCut = iConfig.getUntrackedParameter<double>("etCut", 10);
   etaCut = iConfig.getUntrackedParameter<double>("etaCut", 2);

}


HIBasicClusAna::~HIBasicClusAna()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HIBasicClusAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std; 

   int trigFlag=0;
   Handle<GenParticleCollection> pGenCandInput;
   iEvent.getByLabel(InputTag(GenCandInput), pGenCandInput);
   const GenParticleCollection *genParticles = pGenCandInput.product();
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

   // Loop over superCluster Candidates, Et cut and Eta cut
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

   int maxindex = (int)sClusters->size();

   // event counter
   if (trigFlag==1) nTrigEvent++;
   nEvent++;


   // Prepare Shape variable calculators
   CandidateSuperClusterFinder SCFinder;
   CxCalculator CxC(iEvent,iSetup);
   RxCalculator RxC(iEvent,iSetup);


   cout <<"Endcap: "<<superClusters1->size()<<" Barrel: "<<superClusters->size()<<endl;


   // Loop over SuperCluster Candidates
   for(int i = 0; i < maxindex; i++)
   {

      const Candidate &SCCand = (*sClusters)[i];

      CandidateGenParticleFinder mp(genParticles,SCCand,0.15,10);

      Float_t var[300];
      for (int j = 0; j<300; j++) var[j]=0;


      vector<Float_t> vv;
      vv.push_back(SCCand.et());
      vv.push_back(SCCand.energy());
      vv.push_back(SCCand.energy()/cosh(SCCand.eta()));
      vv.push_back(SCCand.eta());
      vv.push_back(SCCand.phi());

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
         // not matched, fill 0s.
         for (int it=0;it<20;it++) vv.push_back(0);
      }

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
   
         int nc = SC->clustersSize();
         vv.push_back(SC->clustersSize());
         vv.push_back(SC->getHitsByDetId().size());
         vv.push_back(SC->etaWidth());
         vv.push_back(SC->phiWidth());
         vv.push_back(SC->rawEnergy());
         vv.push_back(calc_(SC,iEvent,iSetup));

         reco::basicCluster_iterator theEclust = SC->clustersBegin();
         
         
         for(int bcIt=0;bcIt<30;bcIt++) {
            if (bcIt<nc) {
               vv.push_back((*theEclust)->energy());
               vv.push_back((*theEclust)->eta());
               vv.push_back((*theEclust)->phi());
               const BasicCluster *bc = &(**theEclust);
               vv.push_back(calc_.getHoE(GlobalPoint((*theEclust)->x(),(*theEclust)->y(),(*theEclust)->z()),(*theEclust)->energy(),iEvent,iSetup));
            } else {
               vv.push_back(0);
               vv.push_back(0);
               vv.push_back(0);
               vv.push_back(0);
            }           
            theEclust++;
         }

      }
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

      for(int j=0;j<(int)vv.size();j++) var[j]=vv[j];
      datatemp->Fill(var);
   }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
HIBasicClusAna::beginJob(const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

   //TFile::TContext context(0);
   f = TFile::Open(output.c_str(), "recreate");

   datatemp = new TNtuple("gammas", "photon candidate info",
            "evt:e:et:eta:phi:"
            "ge:get:geta:gphi:cgeta:cgphi:vx:vy:vz:gid:gmid:id1:id:mciso:mcsig:delphi:iNear:iAway:phi1:phi2:"
            "emax:e2nd:e4:e6:e9:e16:e25:e2x5R:e2x5T:e2x5B:e2x5L:e3x2Ratio:"
            "convEtaEta:convEtaPhi:convPhiPhi:nc:nhit:etawidth:phiwidth:rawE:HoE:"
            "bcE1:bcEta1:bcPhi1:bcHoE1:"
            "bcE2:bcEta2:bcPhi2:bcHoE2:"
            "bcE3:bcEta3:bcPhi3:bcHoE3:"
            "bcE4:bcEta4:bcPhi4:bcHoE4:"
            "bcE5:bcEta5:bcPhi5:bcHoE5:"
            "bcE6:bcEta6:bcPhi6:bcHoE6:"
            "bcE7:bcEta7:bcPhi7:bcHoE7:"
            "bcE8:bcEta8:bcPhi8:bcHoE8:"
            "bcE9:bcEta9:bcPhi9:bcHoE9:"
            "bcE10:bcEta10:bcPhi10:bcHoE10:"
            "bcE11:bcEta11:bcPhi11:bcHoE11:"
            "bcE12:bcEta12:bcPhi12:bcHoE12:"
            "bcE13:bcEta13:bcPhi13:bcHoE13:"
            "bcE14:bcEta14:bcPhi14:bcHoE14:"
            "bcE15:bcEta15:bcPhi15:bcHoE15:"
            "bcE16:bcEta16:bcPhi16:bcHoE16:"
            "bcE17:bcEta17:bcPhi17:bcHoE17:"
            "bcE18:bcEta18:bcPhi18:bcHoE18:"
            "bcE19:bcEta19:bcPhi19:bcHoE19:"
            "bcE20:bcEta20:bcPhi20:bcHoE20:"
            "bcE21:bcEta21:bcPhi21:bcHoE21:"
            "bcE22:bcEta22:bcPhi22:bcHoE22:"
            "bcE23:bcEta23:bcPhi23:bcHoE23:"
            "bcE24:bcEta24:bcPhi24:bcHoE24:"
            "bcE25:bcEta25:bcPhi25:bcHoE25:"
            "bcE26:bcEta26:bcPhi26:bcHoE26:"
            "bcE27:bcEta27:bcPhi27:bcHoE27:"
            "bcE28:bcEta28:bcPhi28:bcHoE28:"
            "bcE29:bcEta29:bcPhi29:bcHoE29:"
            "bcE30:bcEta30:bcPhi30:bcHoE30:"            
            "avgBCEt1:avgBCEt2:avgBCEt3:avgBCEt4:avgBCEt5:"
            "NBC1:NBC2:NBC3:NBC4:NBC5"
            );

   nEvent = 0;
   nTrigEvent = 0;

   // Get the calo geometry
   ESHandle<CaloGeometry> theCaloGeom;
   iSetup.get<IdealGeometryRecord>().get(theCaloGeom);

   calc_=HoECalculator(theCaloGeom);


}

// ------------ method called once each job just after ending the event loop  ------------
void 
HIBasicClusAna::endJob() {
   TFile::TContext context(f);

   datatemp->Write();

   f->Close();
   cout <<"HIPhotonCandidateAna: nEvent = "<<nEvent<<endl;
   cout <<"HIPhotonCandidateAna: nTrigEvent = "<<nTrigEvent;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HIBasicClusAna);
