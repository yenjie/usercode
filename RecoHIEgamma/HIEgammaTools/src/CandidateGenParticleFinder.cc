#include "RecoHIEgamma/HIEgammaTools/interface/CandidateGenParticleFinder.h"

#include "DataFormats/Common/interface/Handle.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollection.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "RecoHIEgamma/HIEgammaTools/interface/HICaloUtil.h"



using namespace reco;
using namespace std;

CandidateGenParticleFinder::CandidateGenParticleFinder (const GenParticleCollection *genParticles,double eta,double phi,double dR,double etcut)  
   : fEtCut(etcut), fDeltaR(dR), fEta(eta), fPhi(phi), fDoEcalEta(false), fParticles(new CandidateCollection)
{
   Match(genParticles);
} 

CandidateGenParticleFinder::CandidateGenParticleFinder (const GenParticleCollection *genParticles,const reco::Candidate &cand,double dR,double etcut)  
   : fEtCut(etcut), fDeltaR(dR), fEta(cand.eta()), fPhi(cand.phi()), fDoEcalEta(true), fParticles(new CandidateCollection)
{
   Match(genParticles);
} 

void CandidateGenParticleFinder::Match(const reco::GenParticleCollection *genParticles)
{
   int nparts = (int)genParticles->size();

   double dRcut = fDeltaR;

   for (int i=0;i<nparts;++i) {
      Candidate *p = (*genParticles)[i].clone();
      if (p->et()<fEtCut) continue;
      if (p->status()==3) continue;;
      
      double eta = 0;
      
      if (fDoEcalEta) {
         const Candidate &par = (*genParticles)[i];
         eta = HICaloUtil::EcalEta(par);  //should be replaced by corrected Ecal Eta.
      } else {
         eta = p->eta();  
      }

      double dR = sqrt(DeltaR2(fPhi,p->phi(),fEta,eta));

      if(dR<dRcut) {
         fParticles->push_back(p);
      }

   }
}

const Candidate *CandidateGenParticleFinder::GetTriggerGenParticle() 
{
   const int nump = fParticles->size();
   double etCand = 0;
   Candidate *cand=0;

   for(int i=0; i<nump; i++) {
      Candidate *genPart = (*fParticles)[i].clone();
      if (genPart->status()==3) continue;
      
      //Only check candidate particles 
      const int pid=abs(genPart->pdgId());
//      if (pid==22 || pid==111 || pid==211 || pid==221 || pid==223 || pid==331 || pid==11) {
        if (genPart->status()==1) {
         if(genPart->et()<etCand) continue;
         etCand = genPart->et();
         cand = genPart;
        }
   }

   return cand;
}

double CandidateGenParticleFinder::DeltaR2(double phi1,double phi2,double eta1,double eta2)
{
    double dEta = eta1-eta2;
    double dPhi = phi1-phi2;
    double PI=3.141592653589793238463; 
   
    while(dPhi >= PI)       dPhi -= (2.0*PI);
    while(dPhi < (-1.0*PI)) dPhi += (2.0*PI);
    
    return (dEta*dEta+dPhi*dPhi);
}
