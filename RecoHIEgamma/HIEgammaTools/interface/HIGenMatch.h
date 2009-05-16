#ifndef HIGenMatch_h
#define HIGenMatch_h

//#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/EventSetup.h"
//#include "FWCore/Framework/interface/ESHandle.h"

//#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
//#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
//#include "DataFormats/Candidate/interface/CandidateFwd.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


class HIGenMatch
{
 public:
  
  HIGenMatch(const reco::GenParticleCollection  *bkgParticles);
  bool IsMatched(const reco::GenParticle &pp);
  
  
 private:
   const reco::GenParticleCollection    *fbkgParticles ;
   
};

#endif


