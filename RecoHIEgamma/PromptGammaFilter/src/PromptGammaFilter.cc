// -*- C++ -*-
//
// Package:    PromptGammaFilter
// Class:      PromptGammaFilter
// 
/**\class PromptGammaFilter PromptGammaFilter.cc RecoHIEgamma/PromptGammaFilter/src/PromptGammaFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yen-Jie Lee
//         Created:  Sat Apr 12 20:05:56 EDT 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
//
// class declaration
//

class PromptGammaFilter : public edm::EDFilter {
   public:
      explicit PromptGammaFilter(const edm::ParameterSet&);
      ~PromptGammaFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      double fETCut;
      double fEtaRange;
      std::string fParticleSource;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PromptGammaFilter::PromptGammaFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   fETCut = iConfig.getUntrackedParameter<double>("etCut", 70);
   fEtaRange = iConfig.getUntrackedParameter<double>("etaRange", 2);
   fParticleSource = iConfig.getUntrackedParameter<std::string>("input", "genParticleCandidates");
}


PromptGammaFilter::~PromptGammaFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
PromptGammaFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;
   using namespace reco;
   using namespace std;

   // select events with photon whose mother is a photon, no grandmother,
   // et > 70 GeV, |eta| < 2 (Yenjie's definition of prompt photon)

   // forget about mixing~
   // it "may be" supported in the future

   Handle<CandidateCollection> hParticles;
   iEvent.getByLabel(InputTag(fParticleSource), hParticles);
   const CandidateCollection *particles = hParticles.product();
   if(particles == 0) {
      cout <<"Prompt Gamma Filter: NO COLLECTION FOUND!"<<endl;
      return false;
   }

   cout <<"FilterOHOH!"<<endl;
   for(CandidateCollection::const_iterator iter = particles->begin(); iter != particles->end(); iter++)
   {
      const Candidate *cand = &(*iter);
      
      if(cand->pdgId() != 22)   // we need a photon to begin with
         continue;
      if(cand->numberOfMothers() == 0)   // we need a mother
         continue;
//      if(cand->mother(0)->numberOfMothers() != 0)   // grandmother found!
//         continue;
      if(cand->mother(0)->pdgId() != 22)   // mother also has to be a photon
         continue;
      if(fabs(cand->eta()) > fEtaRange)   // out of range...
         continue;
      if(cand->et() < fETCut)   // too small...
         continue;
      if(cand->numberOfDaughters() != 0)   // this also has to be a final particle
         continue;
      cout <<"Found Candidate!!"<<endl;
      return true;
      break;
   }

   return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
PromptGammaFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PromptGammaFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PromptGammaFilter);
