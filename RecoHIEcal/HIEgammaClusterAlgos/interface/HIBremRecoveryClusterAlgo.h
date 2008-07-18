#ifndef RecoEcal_EgammaClusterAlgos_HIBremRecoveryClusterAlgo_h_
#define RecoEcal_EgammaClusterAlgos_HIBremRecoveryClusterAlgo_h_

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include <vector>


/*
  The HIBremRecoveryClusterAlgo class encapsulates the functionality needed
  to perform the SuperClustering.
  
  WARNING: This code assumes that the BasicClusters 
  from the event are sorted by energy
*/

class HIBremRecoveryClusterAlgo
{
 public:
  
  enum VerbosityLevel { pDEBUG = 0, pWARNING = 1, pINFO = 2, pERROR = 3 }; 

  HIBremRecoveryClusterAlgo(double eb_sc_road_etasize = 0.06, // Search window in eta - Barrel
			  double eb_sc_road_phisize = 0.80, // Search window in phi - Barrel
			  double ec_sc_road_etasize = 0.14, // Search window in eta - Endcap
			  double ec_sc_road_phisize = 0.40, // Search window in eta - Endcap
			  double theSeedTransverseEnergyThreshold = 0.40,
			  double theBarrelBasicClusterEnergyThreshold = 0.00,
			  double theEndcapBasicClusterEnergyThreshold = 0.00,
			  VerbosityLevel the_verbosity = pERROR
			  )
    {
      // e*_rdeta_ and e*_rdphi_ are half the total window 
      // because they correspond to one direction (positive or negative)
      eb_rdeta_ = eb_sc_road_etasize / 2;
      eb_rdphi_ = eb_sc_road_phisize / 2;
      ec_rdeta_ = ec_sc_road_etasize / 2;
      ec_rdphi_ = ec_sc_road_phisize / 2;

      seedTransverseEnergyThreshold = theSeedTransverseEnergyThreshold;
      barrelBasicClusterEnergyThreshold = theBarrelBasicClusterEnergyThreshold;
      endcapBasicClusterEnergyThreshold = theEndcapBasicClusterEnergyThreshold;
      verbosity = the_verbosity;
    }

  void setVerbosity(VerbosityLevel the_verbosity)
    {
      verbosity = the_verbosity;
    }
  
  // the method called from outside to do the SuperClustering - returns a vector of SCs:
  reco::SuperClusterCollection makeSuperClusters(reco::BasicClusterRefVector & clusters);
  
 private:
  
  // make superclusters out of clusters produced by the Island algorithm:
  void makeIslandSuperClusters(reco::BasicClusterRefVector &clusters_v, 
			       double etaRoad, double phiRoad);
  
  // return true if the cluster is within the search phi-eta window of the seed
  bool match(reco::BasicClusterRef seed_p, 
	     reco::BasicClusterRef cluster_p,
	     double etaRoad, double phiRoad);
  
  //

  VerbosityLevel verbosity;

  double eb_rdeta_;
  double eb_rdphi_;
  double ec_rdeta_;
  double ec_rdphi_;
  
  double seedTransverseEnergyThreshold;
  double barrelBasicClusterEnergyThreshold;
  double endcapBasicClusterEnergyThreshold;
  
  reco::SuperClusterCollection superclusters_v;
  
};

#endif
