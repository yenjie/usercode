#ifndef CandidateSuperClusterFinder_h
#define CandidateSuperClusterFinder_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

using namespace std;
using namespace edm;
using namespace reco;

class CandidateSuperClusterFinder
{
  public:
  
   CandidateSuperClusterFinder() ;
   
   const SuperCluster *findbest(const SuperClusterCollection *clusters, const Candidate &cand);
   const SuperCluster *findbest(const SuperClusterCollection *clusters1, const SuperClusterCollection *clusters2, const Candidate &cand);
  
   
  private:
  
};

#endif
