// -*- C++ -*-
//
// Package:    HICaloDisplayer
// Class:      HICaloDisplayer
// 
/**\class HICaloDisplayer HICaloDisplayer.cc RecoHIEgamma/HICaloDisplayer/src/HICaloDisplayer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yen-Jie Lee
//         Created:  Wed Jun 18 20:21:55 EDT 2008
// $Id: HICaloDisplayer.cc,v 1.3 2009/03/17 17:25:25 yjlee Exp $
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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Math/interface/Vector3D.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CxCalculator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"

//
// class decleration
//


struct MyEventContent{
   int nbc;
   int nhhit;
   int nehit;
   int ngen;
   float bcEta[10000];
   float bcPhi[10000];
   float bcE[10000];
   float ehitEta[10000];
   float ehitPhi[10000];
   float ehitE[10000];
   float hhitEta[10000];
   float hhitPhi[10000];
   float hhitE[10000];
   float gPhi[10000];
   float gEta[10000];
   float gE[10000];
   float gId[10000];
   float gMid[10000];
   float gS[10000];
};

class HICaloDisplayer : public edm::EDAnalyzer {
   public:
      explicit HICaloDisplayer(const edm::ParameterSet&);
      ~HICaloDisplayer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
   const reco::BasicClusterCollection *fEBclusters_;
   const reco::BasicClusterCollection *fEEclusters_;
   const CaloGeometry                 *geometry_;
   const HBHERecHitCollection         *fHBHERecHits_;
   const EcalRecHitCollection   *fEBHit_;
   const EcalRecHitCollection   *fEEHit_;
  
   edm::Service<TFileService> fs;           
   TTree* EventTree_;

   int nEvent;
   MyEventContent pev_;
   
   std::string GenCandInput;
   std::string output;

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
HICaloDisplayer::HICaloDisplayer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   GenCandInput = iConfig.getUntrackedParameter<std::string>("GenCandInput","genParticles");
   output = iConfig.getUntrackedParameter<std::string>("output", "ughuu.root");

}


HICaloDisplayer::~HICaloDisplayer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HICaloDisplayer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

   Handle<BasicClusterCollection> pEBclusters;
   iEvent.getByLabel(InputTag("islandBasicClusters:islandBarrelBasicClusters"), pEBclusters);
   fEBclusters_ = pEBclusters.product(); 

   Handle<BasicClusterCollection> pEEclusters;
   iEvent.getByLabel(InputTag("islandBasicClusters:islandEndcapBasicClusters"), pEEclusters);
   fEEclusters_ = pEEclusters.product(); 

   ESHandle<CaloGeometry> geometryHandle;
   iSetup.get<CaloGeometryRecord>().get(geometryHandle);
   geometry_ = geometryHandle.product();

   Handle<HBHERecHitCollection> hehbhandle;
   iEvent.getByLabel(InputTag("hbhereco"), hehbhandle);
   fHBHERecHits_ = hehbhandle.product();

   Handle<GenParticleCollection> pGenCandInput;
   iEvent.getByLabel(InputTag(GenCandInput), pGenCandInput);
   const GenParticleCollection *genParticles = pGenCandInput.product();
   if(genParticles == 0)
   {
      cout <<"***HIPhotonCandidateAna: GenParticleCandidate not found!"<<endl;
      return;
   }

    
   Handle<EcalRecHitCollection> pEBHit;
   iEvent.getByLabel(InputTag("ecalRecHit:EcalRecHitsEB"), pEBHit);
   fEBHit_ = pEBHit.product(); 
 
   Handle<EcalRecHitCollection> pEEHit;
   iEvent.getByLabel(InputTag("ecalRecHit:EcalRecHitsEE"), pEEHit);
   fEEHit_ = pEEHit.product(); 




   pev_.ngen=0;
   for(int i = 0; i < (int) genParticles->size(); i++)
   {
     const Candidate &cand = (*genParticles)[i];
      pev_.gPhi[pev_.ngen] = cand.phi();
      pev_.gEta[pev_.ngen] = cand.eta();
      pev_.gE  [pev_.ngen] = cand.energy();
      pev_.gId [pev_.ngen] = cand.pdgId();

      float mid=0;
      if (cand.mother()!=0) mid=cand.mother()->pdgId();
      pev_.gMid[pev_.ngen] = mid;
      pev_.gS  [pev_.ngen] = cand.status();
      pev_.ngen++;
   }

   nEvent++;

   pev_.nbc = 0;
   for(BasicClusterCollection::const_iterator iclu = fEBclusters_->begin();
       iclu != fEBclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      math::XYZVector ClusPoint(clu->x(),clu->y(),clu->z());
      double eta = ClusPoint.eta();
      double phi = ClusPoint.phi();
      pev_.bcPhi[pev_.nbc] = phi;
      pev_.bcEta[pev_.nbc] = eta;
      pev_.bcE  [pev_.nbc] = clu->energy();
      pev_.nbc++;
   }    

   for(BasicClusterCollection::const_iterator iclu = fEEclusters_->begin();
       iclu != fEEclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      math::XYZVector ClusPoint(clu->x(),clu->y(),clu->z());
      double eta = ClusPoint.eta();
      double phi = ClusPoint.phi();
      pev_.bcPhi[pev_.nbc] = phi;
      pev_.bcEta[pev_.nbc] = eta;
      pev_.bcE  [pev_.nbc] = clu->energy();
      pev_.nbc++;
   }

   
   pev_.nehit=0;
   for(size_t index = 0; index < fEBHit_->size(); index++) {
      const EcalRecHit &rechit = (*fEBHit_)[index];
      const DetId &detid = rechit.id();
      const GlobalPoint& hitpoint = geometry_->getPosition(detid);
      double eta = hitpoint.eta();
      double phi = hitpoint.phi();

      pev_.ehitPhi[pev_.nehit] = phi;
      pev_.ehitEta[pev_.nehit] = eta;
      pev_.ehitE  [pev_.nehit] = rechit.energy();
      pev_.nehit++;
   }


   for(size_t index = 0; index < fEEHit_->size(); index++) {
      const EcalRecHit &rechit = (*fEEHit_)[index];
      const DetId &detid = rechit.id();
      const GlobalPoint& hitpoint = geometry_->getPosition(detid);
      double eta = hitpoint.eta();
      double phi = hitpoint.phi();

      pev_.ehitPhi[pev_.nehit] = phi;
      pev_.ehitEta[pev_.nehit] = eta;
      pev_.ehitE  [pev_.nehit] = rechit.energy();
      pev_.nehit++;
   }

   pev_.nhhit=0;
   for(size_t index = 0; index < fHBHERecHits_->size(); index++) {
      const HBHERecHit &rechit = (*fHBHERecHits_)[index];
      const DetId &detid = rechit.id();
      const GlobalPoint& hitpoint = geometry_->getPosition(detid);
      double eta = hitpoint.eta();
      double phi = hitpoint.phi();

      pev_.hhitPhi[pev_.nhhit] = phi;
      pev_.hhitEta[pev_.nhhit] = eta;
      pev_.hhitE  [pev_.nhhit] = rechit.energy();
      pev_.nhhit++;
   }

   EventTree_->Fill();
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
HICaloDisplayer::beginJob(const edm::EventSetup&)
{
   TFile::TContext context(0);

   EventTree_ = fs->make<TTree>("EventTree","Tree");

   EventTree_->Branch("nbc",&pev_.nbc,"nbc/I");
   EventTree_->Branch("nhhit",&pev_.nhhit,"nhhit/I");
   EventTree_->Branch("nehit",&pev_.nehit,"nehit/I");
   EventTree_->Branch("ngen",&pev_.ngen,"ngen/I");
   EventTree_->Branch("bcEta",pev_.bcEta,"bcEta[nbc]/F");
   EventTree_->Branch("bcPhi",pev_.bcPhi,"bcPhi[nbc]/F");
   EventTree_->Branch("bcE",pev_.bcE,"bcE[nbc]/F");
   EventTree_->Branch("hhitEta",pev_.hhitEta,"hhitEta[nhhit]/F");
   EventTree_->Branch("hhitPhi",pev_.hhitPhi,"hhitPhi[nhhit]/F");
   EventTree_->Branch("hhitE",pev_.hhitE,"hhitE[nhhit]/F");
   EventTree_->Branch("ehitEta",pev_.ehitEta,"ehitEta[nehit]/F");
   EventTree_->Branch("ehitPhi",pev_.ehitPhi,"ehitPhi[nehit]/F");
   EventTree_->Branch("ehitE",pev_.ehitE,"ehitE[nehit]/F");
   EventTree_->Branch("gPhi",pev_.gPhi,"gPhi[ngen]/F");
   EventTree_->Branch("gEta",pev_.gEta,"gEta[ngen]/F");
   EventTree_->Branch("gE",pev_.gE,"gE[ngen]/F");
   EventTree_->Branch("gId",pev_.gId,"gId[ngen]/F");
   EventTree_->Branch("gMid",pev_.gMid,"gMid[ngen]/F");
   EventTree_->Branch("gS",pev_.gS,"gS[ngen]/F");
   
   nEvent=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HICaloDisplayer::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(HICaloDisplayer);
