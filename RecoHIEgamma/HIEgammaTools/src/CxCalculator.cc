#include "RecoHIEgamma/HIEgammaTools/interface/CxCalculator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

using namespace edm;
using namespace reco;

#define PI 3.141592653589793238462643383279502884197169399375105820974945

CxCalculator::CxCalculator (const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
   Handle<BasicClusterCollection> pEBclusters;
   iEvent.getByLabel(InputTag("islandBasicClusters:islandBarrelBasicClusters"), pEBclusters);
   fEBclusters_ = pEBclusters.product(); 

   Handle<BasicClusterCollection> pEEclusters;
   iEvent.getByLabel(InputTag("islandBasicClusters:islandEndcapBasicClusters"), pEEclusters);
   fEEclusters_ = pEEclusters.product(); 

   ESHandle<CaloGeometry> geometryHandle;
   iSetup.get<IdealGeometryRecord>().get(geometryHandle);
   geometry_ = geometryHandle.product();

} 

double CxCalculator::getCx(const reco::SuperCluster* cluster, double x, double threshold)
{
   using namespace edm;
   using namespace reco;

   if(!fEBclusters_) {       
      LogError("CxCalculator") << "Error! Can't get EBclusters for event.";
      return -100;
   }

   if(!fEEclusters_) {       
      LogError("CxCalculator") << "Error! Can't get EEclusters for event.";
      return -100;
   }

   double SClusterEta = cluster->eta();
   double SClusterPhi = cluster->phi();
   double TotalEt = 0;

   TotalEt = - cluster->rawEnergy()/cosh(cluster->eta());

   for(BasicClusterCollection::const_iterator iclu = fEBclusters_->begin();
       iclu != fEBclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      const GlobalPoint clusPoint(clu->x(),clu->y(),clu->z());
      double eta = clusPoint.eta();
      double phi = clusPoint.phi();
      double dEta = eta-SClusterEta;
      double dPhi = fabs(phi-SClusterPhi);
      while (dPhi>2*PI) dPhi-=2*PI;
      if (dPhi>PI) dPhi=2*PI-dPhi;
      double dR = sqrt(dEta * dEta + dPhi * dPhi);
      if (dR<x*0.1) {
         double et = clu->energy()/cosh(eta);
         if (et<threshold) et=0;
         TotalEt += et;
      } 
   }

   for(BasicClusterCollection::const_iterator iclu = fEEclusters_->begin();
       iclu != fEEclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      const GlobalPoint clusPoint(clu->x(),clu->y(),clu->z());
      double eta = clusPoint.eta();
      double phi = clusPoint.phi();
      double dEta = eta-SClusterEta;
      double dPhi = fabs(phi-SClusterPhi);
      while (dPhi>2*PI) dPhi-=2*PI;
      if (dPhi>PI) dPhi=2*PI-dPhi;
      double dR = sqrt(dEta * dEta + dPhi * dPhi);
      if (dR<x*0.1) {
         double et = clu->energy()/cosh(eta);
         if (et<threshold) et=0;
         TotalEt += et;
      } 
   }

   return TotalEt;
}

double CxCalculator::getCCx(const reco::SuperCluster* cluster, double x, double threshold)
{
   using namespace edm;
   using namespace reco;


   if(!fEBclusters_) {       
      LogError("CxCalculator") << "Error! Can't get EBclusters for event.";
      return -100;
   }

   if(!fEEclusters_) {       
      LogError("CxCalculator") << "Error! Can't get EEclusters for event.";
      return -100;
   }

   double SClusterEta = cluster->eta();
   double SClusterPhi = cluster->phi();
   double TotalEt = 0;

   int TotalNhitE = 0;  // n hits in Endcap
   int TotalNhitB = 0;  // n hits in Barrel
   
   TotalEt = - cluster->rawEnergy()/cosh(cluster->eta());

   for(BasicClusterCollection::const_iterator iclu = fEBclusters_->begin();
       iclu != fEBclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      const GlobalPoint clusPoint(clu->x(),clu->y(),clu->z());
      double eta = clusPoint.eta();
      double phi = clusPoint.phi();
      double dEta = fabs(eta-SClusterEta);
      double dPhi = fabs(phi-SClusterPhi);
      while (dPhi>2*PI) dPhi-=2*PI;
      if (dPhi>PI) dPhi=2*PI-dPhi;
  
 
     if (dEta<x*0.1) {
         double et = clu->energy()/cosh(eta);
         if (et<threshold) et=0;
         TotalEt += et;
      } 
   }

   for(BasicClusterCollection::const_iterator iclu = fEEclusters_->begin();
       iclu != fEEclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      const GlobalPoint clusPoint(clu->x(),clu->y(),clu->z());
      double eta = clusPoint.eta();
      double phi = clusPoint.phi();
      double dEta = fabs(eta-SClusterEta);
      double dPhi = fabs(phi-SClusterPhi);
      while (dPhi>2*PI) dPhi-=2*PI;
      if (dPhi>PI) dPhi=2*PI-dPhi;

      if (dEta<x*0.1) {
         double et = clu->energy()/cosh(eta);
         if (et<threshold) et=0;
         TotalEt += et;
      } 
   }

   double Cx = getCx(cluster,x,threshold);
   double CCx = Cx - TotalEt / 40.0 * x;

   return CCx;
}


double CxCalculator::getCorrection(const reco::SuperCluster* cluster, double x, double y,double threshold)
{
   using namespace edm;
   using namespace reco;


   if(!fEBclusters_) {       
      LogError("CxCalculator") << "Error! Can't get EBclusters for event.";
      return -100;
   }

   if(!fEEclusters_) {       
      LogError("CxCalculator") << "Error! Can't get EEclusters for event.";
      return -100;
   }

   double SClusterEta = cluster->eta();
   double SClusterPhi = cluster->phi();
   double TotalEnergy = 0;
   double TotalBC = 0;

   int TotalNhitE = 0;  // n hits in Endcap
   int TotalNhitB = 0;  // n hits in Barrel
   
   TotalEnergy = 0;

   double Area = PI * (-x*x+y*y) / 100.0;
   double nCrystal = Area / 0.0174 / 0.0174; // ignore the difference between endcap and barrel for the moment....

   for(BasicClusterCollection::const_iterator iclu = fEBclusters_->begin();
       iclu != fEBclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      const GlobalPoint clusPoint(clu->x(),clu->y(),clu->z());
      double eta = clusPoint.eta();
      double phi = clusPoint.phi();
      double dEta = fabs(eta-SClusterEta);
      double dPhi = fabs(phi-SClusterPhi);
      while (dPhi>2*PI) dPhi-=2*PI;
      if (dPhi>PI) dPhi=2*PI-dPhi;
      double dR = sqrt(dEta*dEta+dPhi*dPhi);
 
     if (dR>x*0.1&&dR<y*0.1) {
         double e = clu->energy();
         if (e<threshold) e=0;
         TotalEnergy += e;
         if (e!=0) TotalBC+=clu->getHitsByDetId().size();  // number of crystals
   
      } 
   }

   for(BasicClusterCollection::const_iterator iclu = fEEclusters_->begin();
       iclu != fEEclusters_->end(); ++iclu) {
      const BasicCluster *clu = &(*iclu);
      const GlobalPoint clusPoint(clu->x(),clu->y(),clu->z());
      double eta = clusPoint.eta();
      double phi = clusPoint.phi();
      double dEta = fabs(eta-SClusterEta);
      double dPhi = fabs(phi-SClusterPhi);
      while (dPhi>2*PI) dPhi-=2*PI;
      if (dPhi>PI) dPhi=2*PI-dPhi;
      double dR = sqrt(dEta*dEta+dPhi*dPhi);
 
     if (dR>x*0.1&&dR<y*0.1) {
         double e = clu->energy();
         if (e<threshold) e=0;
         TotalEnergy += e;
         if (e!=0) TotalBC += clu->getHitsByDetId().size(); // number of crystals
      } 
   }


  if (TotalBC==0) return 0;
  return TotalEnergy/nCrystal;
}

