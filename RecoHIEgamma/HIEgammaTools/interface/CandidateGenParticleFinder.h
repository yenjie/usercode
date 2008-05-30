#ifndef CandidateGenParticleFinder_h
#define CandidateGenParticleFinder_h

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

class CandidateGenParticleFinder
{
  public:
  
   CandidateGenParticleFinder(const CandidateCollection *genParticles,double eta,double phi,double dR,double etcut) ;
   CandidateGenParticleFinder(const CandidateCollection *genParticles,const reco::Candidate &cand,double dR,double etcut) ;  

   void Match(const reco::CandidateCollection *genParticles);
   const Candidate *GetTriggerGenParticle(); 

   double                        GetDeltaR()             const { return fDeltaR; }
   double                        GetEta()                const { return fEta; }
   double                        GetEtCut()              const { return fEtCut; }
   const CandidateCollection    *GetParticles()          const { return fParticles; }
   double                        GetPhi()                const { return fPhi; }
   const Candidate              *GetTriggerGenParticle() const;

   
  private:
   double                        fEtCut;     //minimum et for the matched particles
   double                        fDeltaR;    //delta in radius to be used for matching
   double                        fEta;       //eta of the cone center
   double                        fPhi;       //phi of the cone center
   bool                          fDoEcalEta; //if true shift eta of particles
   CandidateCollection          *fParticles; //!matched particles

   double DeltaR2(double phi1,double phi2,double eta1,double eta2);
  
};

#endif
