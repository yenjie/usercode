#ifndef TxyCalculator_h
#define TxyCalculator_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

class TxyCalculator
{
public:  
   TxyCalculator(const edm::Event &iEvent, const edm::EventSetup &iSetup);
   double getTxy(const reco::Candidate &cluster, double x, double y);

private:
   double dRDistance(const reco::Candidate &a, const reco::Candidate &b)
   {
      double deta = a.eta() - b.eta();
      double dphi = a.phi() - b.phi();
      if(dphi > 3.1415926535)
         dphi = 3.1415926535 * 2 - dphi;
      return sqrt(deta * deta + dphi * dphi);
   }

private:
   const reco::CandidateCollection *tracks;
};

#endif

