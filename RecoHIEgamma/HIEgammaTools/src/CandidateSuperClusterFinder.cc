#include "RecoHIEgamma/HIEgammaTools/interface/CandidateSuperClusterFinder.h"

#include "DataFormats/Common/interface/Handle.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollection.h"

CandidateSuperClusterFinder::CandidateSuperClusterFinder () 
{
} 

const reco::SuperCluster *CandidateSuperClusterFinder::findbest(const reco::SuperClusterCollection *clusters, const reco::Candidate &cand)
{
   if(clusters->size() == 0)
      return 0;

   int index = 0;
   double distance2 = ((*clusters)[0].phi() - cand.phi()) * ((*clusters)[0].phi() - cand.phi())
      + ((*clusters)[0].eta() - cand.eta()) * ((*clusters)[0].eta() - cand.eta())
      + ((*clusters)[0].energy() - cand.energy()) * ((*clusters)[0].energy() - cand.energy());
   const reco::SuperCluster *best = &(*clusters)[0];

   for(int i = 1; i < (int)clusters->size(); i++)
   {
      double newdistance2 = ((*clusters)[i].phi() - cand.phi()) * ((*clusters)[i].phi() - cand.phi())
         + ((*clusters)[i].eta() - cand.eta()) * ((*clusters)[i].eta() - cand.eta())
         + ((*clusters)[i].energy() - cand.energy()) * ((*clusters)[i].energy() - cand.energy());
      if(newdistance2 < distance2)
      {
         best = &(*clusters)[i];
         index = i;
         distance2 = newdistance2;
      }
   }

   return best;
}

const reco::SuperCluster *CandidateSuperClusterFinder::findbest(const reco::SuperClusterCollection *clusters1, const reco::SuperClusterCollection *clusters2, const reco::Candidate &cand)
{
   const reco::SuperCluster *s1 = findbest(clusters1,cand);
   const reco::SuperCluster *s2 = findbest(clusters2,cand);

   if (s1==0&&s2==0) return 0;

   if (s1==0) return s2;
   if (s2==0) return s1;

   double newdistance1 = (s1->phi() - cand.phi()) * (s1->phi() - cand.phi())
          + (s1->eta() - cand.eta()) * (s1->eta() - cand.eta())
          + (s1->energy() - cand.energy()) * (s1->energy() - cand.energy());
   double newdistance2 = (s2->phi() - cand.phi()) * (s2->phi() - cand.phi())
          + (s2->eta() - cand.eta()) * (s2->eta() - cand.eta())
          + (s2->energy() - cand.energy()) * (s2->energy() - cand.energy());

   if (newdistance1<newdistance2) return s1;

   return s2;

}
