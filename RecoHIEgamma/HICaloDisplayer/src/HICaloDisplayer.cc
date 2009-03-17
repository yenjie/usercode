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

#include "DataFormats/Math/interface/Vector3D.h"
#include "RecoHIEgamma/HIEgammaTools/interface/CxCalculator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"


#include "TNtuple.h"
#include "TFile.h"

//
// class decleration
//

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
  
   TNtuple *datatemp;
   TNtuple *datatemp1;
   TNtuple *datatemp2;
   TNtuple *datatemp3;
   TFile *f;
   int nEvent;
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
   GenCandInput = iConfig.getUntrackedParameter<std::string>("GenCandInput", "genParticleCandidates");
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
   iSetup.get<IdealGeometryRecord>().get(geometryHandle);
   geometry_ = geometryHandle.product();

   Handle<HBHERecHitCollection> hehbhandle;
   iEvent.getByLabel(InputTag("hbhereco"), hehbhandle);
   fHBHERecHits_ = hehbhandle.product();

   Handle<CandidateCollection> pGenCandInput;
   iEvent.getByLabel(InputTag(GenCandInput), pGenCandInput);
   const CandidateCollection *genParticles = pGenCandInput.product();
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



   double candeta=0;
   double candphi=0;


   for(int i = 0; i < (int) genParticles->size(); i++)
   {
     const Candidate &cand = (*genParticles)[i];
      float var[10];
//      if (cand.et()<20) continue;
      var[0]=nEvent;
      var[1]=cand.phi();
      var[2]=cand.eta();
      var[3]=cand.energy();
      var[4]=cand.pdgId();
      float mid=0;
      if (cand.mother()!=0) mid=cand.mother()->pdgId();

      var[5]=mid;
      var[6]=cand.status();
      datatemp2->Fill(var);      
   }

   nEvent++;

   for(BasicClusterCollection::const_iterator iclu = fEBclusters_->begin();
       iclu != fEBclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      math::XYZVector ClusPoint(clu->x(),clu->y(),clu->z());
      double eta = ClusPoint.eta();
      double phi = ClusPoint.phi();
      float var[10];
      var[0]=nEvent;
      var[1]=phi;
      var[2]=eta;
      var[3]=clu->energy();
      datatemp->Fill(var);

   }    

   for(BasicClusterCollection::const_iterator iclu = fEEclusters_->begin();
       iclu != fEEclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      math::XYZVector ClusPoint(clu->x(),clu->y(),clu->z());
      double eta = ClusPoint.eta();
      double phi = ClusPoint.phi();
      float var[10];
      var[0]=nEvent;
      var[1]=phi;
      var[2]=eta;
      var[3]=clu->energy();
      datatemp->Fill(var);
   }



   for(size_t index = 0; index < fEBHit_->size(); index++) {
      const EcalRecHit &rechit = (*fEBHit_)[index];
      const DetId &detid = rechit.id();
      const GlobalPoint& hitpoint = geometry_->getPosition(detid);
      double eta = hitpoint.eta();
      double phi = hitpoint.phi();
      float var[10];
      var[0]=nEvent;
      var[1]=phi;
      var[2]=eta;
      var[3]=rechit.energy();
      datatemp3->Fill(var);
   }


   for(size_t index = 0; index < fEEHit_->size(); index++) {
      const EcalRecHit &rechit = (*fEEHit_)[index];
      const DetId &detid = rechit.id();
      const GlobalPoint& hitpoint = geometry_->getPosition(detid);
      double eta = hitpoint.eta();
      double phi = hitpoint.phi();
      float var[10];
      var[0]=nEvent;
      var[1]=phi;
      var[2]=eta;
      var[3]=rechit.energy();
      datatemp3->Fill(var);
   }

   for(size_t index = 0; index < fHBHERecHits_->size(); index++) {
      const HBHERecHit &rechit = (*fHBHERecHits_)[index];
      const DetId &detid = rechit.id();
      const GlobalPoint& hitpoint = geometry_->getPosition(detid);
      double eta = hitpoint.eta();
      double phi = hitpoint.phi();
      float var[10];
      var[0]=nEvent;
      var[1]=phi;
      var[2]=eta;
      var[3]=rechit.energy();
      datatemp1->Fill(var);
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
HICaloDisplayer::beginJob(const edm::EventSetup&)
{
   TFile::TContext context(0);
   f = TFile::Open(output.c_str(), "recreate");
   datatemp = new TNtuple("bc", "ecal info",
             "id:phi:eta:e"
            );

   datatemp1 = new TNtuple("hit", "hcal info",
             "id:phi:eta:e"
            );

   datatemp2 = new TNtuple("gen", "gen info",
             "id:phi:eta:e:gid:mid:s"
            );

   datatemp3 = new TNtuple("ehit", "ecal info",
             "id:phi:eta:e"
            );

  nEvent=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HICaloDisplayer::endJob() {
   TFile::TContext context(f);

   datatemp->Write();
   datatemp1->Write();
   datatemp2->Write();
   datatemp3->Write();

   f->Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(HICaloDisplayer);
