// -*- C++ -*-
//
// Package:    EnergyWindowConeMatcher
// Class:      EnergyWindowConeMatcher
// 
/**\class EnergyWindowConeMatcher EnergyWindowConeMatcher.cc RecoHIEgamma/EnergyWindowConeMatcher/src/EnergyWindowConeMatcher.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yi Chen, Yen-Jie Lee
//         Created:  Tue Apr  8 19:18:40 EDT 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"

using namespace std;
using namespace edm;
using namespace reco;
//
// class decleration
//

class EnergyWindowConeMatcher : public edm::EDProducer {
   public:
      explicit EnergyWindowConeMatcher(const edm::ParameterSet&);
      ~EnergyWindowConeMatcher();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      double coneSize;
      double energyWindow;
      double etMin;
      string input1;
      string input2;
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
EnergyWindowConeMatcher::EnergyWindowConeMatcher(const edm::ParameterSet& iConfig)
{
   //register your products

   //now do what ever other initialization is needed
   produces<CandidateCollection>();

   //now do what ever other initialization is needed
   coneSize = iConfig.getUntrackedParameter<double>("conesize", 0.078);
   energyWindow = iConfig.getUntrackedParameter<double>("window", 15);
   etMin = iConfig.getUntrackedParameter<double>("MinEt", 20);

   input1 = iConfig.getUntrackedParameter<string>("input", "genParticleCandidates");
   input2 = iConfig.getUntrackedParameter<string>("input2", "ctfWithMaterialTracks");
  
}


EnergyWindowConeMatcher::~EnergyWindowConeMatcher()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EnergyWindowConeMatcher::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // get candidate collections
   Handle<CandidateCollection> hCollection1;
   iEvent.getByLabel(InputTag(input1), hCollection1);
   const CandidateCollection *collection1 = hCollection1.product();
   if(collection1 == 0)
      return;

   Handle<CandidateCollection> hCollection2;
   iEvent.getByLabel(InputTag(input2), hCollection2);
   const CandidateCollection *collection2 = hCollection2.product();
   if(collection2 == 0)
      return;

   // output container
   std::auto_ptr<CandidateCollection> result(new CandidateCollection);

   double PI=3.1415926;
   // match them: brute force for now (for electron tracking performance studies)
   // it has to be changed to radix sorting method in the future
   for(int i = 0; i < (int)collection2->size(); i++)
   {
      const Candidate &input2 = (*collection2)[i];

      int index = -1;
      for(int j = 0; j < (int)collection1->size(); j++)
      {
         const Candidate &p = (*collection1)[j];
  
         if(p.status()!=1) continue;
  
         if(p.energy()/cosh(p.eta()) < etMin) continue;
         
         // filter out bad ones....
         if(fabs(p.energy() - input2.energy()) > energyWindow)
            continue;

         double dPhi = (p.phi() - input2.phi());
         if (dPhi > PI) dPhi = 2 * PI - dPhi;

         double dEta = (p.eta() - input2.eta());
         double dR = sqrt(dPhi * dPhi + dEta * dEta);

                 
         if(dR > coneSize)
            continue;

         // then pick the one that is closest to the original energy
         if(index == -1)
            index = j;
         else if(fabs(p.energy() - input2.energy()) < fabs(p.energy() - (*collection1)[index].energy()))
            index = j;
      }

      if(index != -1)   // found something!
      {
         Candidate *cand = (*collection1)[index].clone();
         result->push_back(cand);

         const Candidate &p = (*collection1)[index];
         std::cout << "[ConeMatcher] dist = " << sqrt((p.phi() - input2.phi()) * (p.phi() - input2.phi()) + (p.eta() - input2.eta()) * (p.eta() - input2.eta())) << std::endl;
      }
      else   // put a dummy candidate there
      {
         std::auto_ptr<GenParticleCandidate> answer(new GenParticleCandidate(0, Particle::LorentzVector(0, 0, 0, 0), Particle::Point(0, 0, 0), 0, 1, true));
         result->push_back(answer);
      }
   }

   std::cout << "[ConeMatcher] Sizes = " << collection2->size() << " " << result->size() << std::endl;

   // output
   iEvent.put(result);

 
}

// ------------ method called once each job just before starting event loop  ------------
void 
EnergyWindowConeMatcher::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
EnergyWindowConeMatcher::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(EnergyWindowConeMatcher);
