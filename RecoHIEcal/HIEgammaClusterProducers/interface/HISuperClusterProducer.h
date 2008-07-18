#ifndef RecoEcal_EgammaClusterProducers_HISuperClusterProducer_h_
#define RecoEcal_EgammaClusterProducers_HISuperClusterProducer_h_

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"

#include "RecoHIEcal/HIEgammaClusterAlgos/interface/HIBremRecoveryClusterAlgo.h"

//


class HISuperClusterProducer : public edm::EDProducer 
{
  
  public:

      HISuperClusterProducer(const edm::ParameterSet& ps);

      ~HISuperClusterProducer();

      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob();

   private:

      int nMaxPrintout_; // max # of printouts
      int nEvt_;         // internal counter of events
 
      HIBremRecoveryClusterAlgo::VerbosityLevel verbosity;

      std::string endcapClusterCollection_;
      std::string barrelClusterCollection_;

      std::string endcapClusterProducer_;
      std::string barrelClusterProducer_;

      std::string endcapSuperclusterCollection_;
      std::string barrelSuperclusterCollection_;

      float barrelEtaSearchRoad_;
      float barrelPhiSearchRoad_;
      float endcapEtaSearchRoad_; 
      float endcapPhiSearchRoad_;
      float seedTransverseEnergyThreshold_;
      float barrelBasicClusterEnergyThreshold_;
      float endcapBasicClusterEnergyThreshold_;

      bool doBarrel_;
      bool doEndcaps_;

      HIBremRecoveryClusterAlgo * bremAlgo_p;

      double totalE;
      int noSuperClusters;

      
      void getClusterRefVector(edm::Event& evt, std::string clusterProducer_, std::string clusterCollection_, reco::BasicClusterRefVector *);
  
      void produceSuperclustersForECALPart(edm::Event& evt, 
					   std::string clusterProducer, 
					   std::string clusterCollection,
					   std::string superclusterColection);

      void outputValidationInfo(reco::SuperClusterCollection &superclusterCollection);
    
      bool counterExceeded() const { return ((nEvt_ > nMaxPrintout_) || (nMaxPrintout_ < 0));}
};


#endif
