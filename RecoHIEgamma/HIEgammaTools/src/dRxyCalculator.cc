#include "RecoHIEgamma/HIEgammaTools/interface/dRxyCalculator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace edm;
using namespace reco;

dRxyCalculator::dRxyCalculator(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
   Handle<CandidateCollection> hTracks;
   iEvent.getByLabel(InputTag(""), hTracks);
   tracks = hTracks.product();
} 

double dRxyCalculator::getDRxy(const reco::Candidate &candidate, double x, double y)
{
   using namespace edm;
   using namespace reco;

   if(!tracks)
   {
      LogError("dRxyCalculator") << "Error! The track container is not found.";
      return -100;
   }

   const Candidate &p = candidate;

   std::vector<double> Rxy;

   for(int j = 0; j < (int)tracks->size(); j++)
   {  
      const Candidate &q = (*tracks)[j];

      if(q.pt() < x * 0.4)
         continue;
      if(q.charge() == 0)
         continue;

      if(Rxy.size() < y)
      {  
         Rxy.push_back(dRDistance(p, q));
         sort(Rxy.begin(), Rxy.end());
         continue;
      }

      double dR = dRDistance(p, q);
      if(dR < Rxy[Rxy.size()-1])
      {  
         Rxy[Rxy.size()-1] = dR;
         sort(Rxy.begin(), Rxy.end());
      }
   }

   return Rxy[(int)y-1];
}

