// -*- C++ -*-
//
// Package:    HiElectron
// Class:      HiElectron
// 
/**\class HiElectron HiElectron.cc RecoHI/HiElectron/src/HiElectron.cc

 Description: Empty electron producer

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yen-Jie LEE
//         Created:  Thu Mar  5 12:16:47 CET 2009
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

#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

//
// class decleration
//
using namespace reco;



class HiElectron : public edm::EDProducer {
   public:
      explicit HiElectron(const edm::ParameterSet&);
      ~HiElectron();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
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
HiElectron::HiElectron(const edm::ParameterSet& iConfig)
{
   //register your products
   produces<GsfElectronCollection>();

   //now do what ever other initialization is needed
  
}


HiElectron::~HiElectron()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HiElectron::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/

 std::auto_ptr<GsfElectronCollection> pOutEle(new GsfElectronCollection);

 iEvent.put(pOutEle);
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
HiElectron::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HiElectron::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiElectron);
