#include "RecoHIEgamma/HIEgammaTools/interface/TxyCalculator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace edm;
using namespace reco;

TxyCalculator::TxyCalculator(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{ 
   // Get reconstructed tracks
   iEvent.getByLabel("globalPrimTracks", recCollection); // !!
} 

double TxyCalculator::getTxy(const reco::Candidate &candidate, double x, double y)
{
   using namespace edm;
   using namespace reco;

   /*
   if(!recCollection)
   {
      LogError("TxyCalculator") << "Error! The track container is not found.";
      return -100;
   }
   */
   
   const Candidate &p = candidate;

   double eta1 = p.eta();
   double phi1 = p.phi();
   
   float txy = 0;

   for(reco::TrackCollection::const_iterator
   	  recTrack = recCollection->begin(); recTrack!= recCollection->end(); recTrack++)
   {
<<<<<<< TxyCalculator.cc
      double pt = recTrack->pt();
      double eta2 = recTrack->phi();
      double phi2 = recTrack->phi();
      
      if(dRDistance(eta1,phi1,eta2,phi2) >= 0.1 * x)
=======
      double pt = recTrack->pt();
      double eta2 = recTrack->eta();
      double phi2 = recTrack->phi();
      
      if(dRDistance(eta1,phi1,eta2,phi2) >= 0.1 * x)
>>>>>>> 1.6
         continue;

<<<<<<< TxyCalculator.cc
      if(pt > y * 3)
=======
      if(pt > y * 0.4)
>>>>>>> 1.6
         txy = txy + 1;
   }

   return txy;
}

