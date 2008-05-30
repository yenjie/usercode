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
   Handle<CandidateCollection> hTracks;
   iEvent.getByLabel(InputTag(""), hTracks);
   tracks = hTracks.product();
} 

double TxyCalculator::getTxy(const reco::Candidate &candidate, double x, double y)
{
   using namespace edm;
   using namespace reco;

   if(!tracks)
   {
      LogError("TxyCalculator") << "Error! The track container is not found.";
      return -100;
   }

   const Candidate &p = candidate;

   float txy = 0;

   for(int j = 0; j < (int)tracks->size(); j++)
   {
      const Candidate &q = (*tracks)[j];

      if(dRDistance(p, q) >= 0.1 * x)
         continue;

      if(q.pt() > y * 3)
         txy = txy + 1;
   }

   return txy;
}

