#ifndef CxCalculator_h
#define CxCalculator_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"


class CxCalculator
{
  public:
  
   CxCalculator(const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  
   double getCx(const reco::SuperCluster* clus, double i,double threshold);
   double getCCx(const reco::SuperCluster* clus, double i,double threshold); // background subtracted Cx
   double getCorrection(const reco::SuperCluster* clus, double i,double j,double threshold); // background subtracted Cx

  private:
   
   const reco::BasicClusterCollection *fEBclusters_;
   const reco::BasicClusterCollection *fEEclusters_;
   const CaloGeometry                 *geometry_;
};

#endif
