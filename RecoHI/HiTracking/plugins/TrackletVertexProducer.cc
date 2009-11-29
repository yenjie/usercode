// -*- C++ -*-
//
// Package:    TrackletVertexProducer
// Class:      TrackletVertexProducer
// 
/**\class TrackletVertexProducer TrackletVertexProducer.cc RecoHI/TrackletVertexProducer/src/TrackletVertexProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yen-Jie LEE
//         Created:  Tue Sep 15 13:14:46 CEST 2009
// $Id: TrackletVertexProducer.cc,v 1.1 2009/09/17 10:08:48 yjlee Exp $
//
//


// system include files
#include <memory>

// user include files
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/Common/interface/DetSetAlgorithm.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerLayerIdAccessor.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
//#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

// my lite objects
//#include "RecoHI/HiTracking/plugins/RecoHit.h"
#include "RecoHI/HiTracking/plugins/Tracklet.h"

using namespace std;
using namespace edm;
using namespace reco;

//
// class decleration
//

class TrackletVertexProducer : public edm::EDProducer {
   public:
      explicit TrackletVertexProducer(const edm::ParameterSet&);
      ~TrackletVertexProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
     const TrackerGeometry* geo_;
//     edm::ESHandle < ParticleDataTable > pdt;
        
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
TrackletVertexProducer::TrackletVertexProducer(const edm::ParameterSet& iConfig)
{
  produces<reco::VertexCollection>();
}


TrackletVertexProducer::~TrackletVertexProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TrackletVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   std::auto_ptr<reco::VertexCollection> vertices(new reco::VertexCollection);
   const SiPixelRecHitCollection* rechits;
   Handle<SiPixelRecHitCollection> rchts;
   iEvent.getByLabel("siPixelRecHits",rchts);
   rechits = rchts.product();
   
   vector<RecoHit> layer1Hits;
   vector<RecoHit> layer2Hits;
   
   float trackletVertexPos = -100;

   for (SiPixelRecHitCollection::const_iterator it = rechits->begin(); it!=rechits->end();it++)
   {
      SiPixelRecHitCollection::DetSet hits = *it;
      DetId detId = DetId(hits.detId());
      SiPixelRecHitCollection::const_iterator recHitMatch = rechits->find(detId);
      const SiPixelRecHitCollection::DetSet recHitRange = *recHitMatch;
      unsigned int detType=detId.det();    // det type, tracker=1
      unsigned int subid=detId.subdetId(); //subdetector type, barrel=1, fpix=2
      if (detType!=1||subid!=1) continue;

      PXBDetId pdetId = PXBDetId(detId);
      unsigned int layer=0;
      layer=pdetId.layer();
      for ( SiPixelRecHitCollection::DetSet::const_iterator recHitIterator = recHitRange.begin(); 
	 recHitIterator != recHitRange.end(); ++recHitIterator) {
         const SiPixelRecHit * recHit = &(*recHitIterator);

         const PixelGeomDetUnit* pixelLayer = dynamic_cast<const PixelGeomDetUnit*> (geo_->idToDet(recHit->geographicalId()));
         GlobalPoint gpos = pixelLayer->toGlobal(recHit->localPosition());
         math::XYZVector rechitPos(gpos.x(),gpos.y(),gpos.z());

         // position
         double eta = rechitPos.eta();
         double phi = rechitPos.phi();
         double r   = rechitPos.rho();

         // put hits
	 RecoHit tmp(eta,phi,r);
	 
         if (layer==1)      layer1Hits.push_back(tmp);
	 else if (layer==2) layer2Hits.push_back(tmp);
      }
   }

   trackletVertexPos = TrackletVertexUnbin(layer1Hits,layer2Hits,0.14,0.08);
   
   reco::Vertex::Error err;
   err(2,2) = 0.1 * 0.1;
   reco::Vertex ver(reco::Vertex::Point(0,0,trackletVertexPos), err, 0, 1, 1);
   
   vertices->push_back(ver);

}

// ------------ method called once each job just before starting event loop  ------------
void 
TrackletVertexProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrackletVertexProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackletVertexProducer);
