// -*- C++ -*-
//
// Package:    ImpParameter
// Class:      ImpParameter
// 
/**\class ImpParameter ImpParameter.cc yetkin/ImpParameter/src/ImpParameter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yetkin Yilmaz
//         Created:  Tue Dec 18 09:44:41 EST 2007
// $Id: ImpParameter.cc,v 1.1 2009/05/07 11:54:40 yjlee Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <string>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"

#include "HepMC/GenEvent.h"
#include "HepMC/HeavyIon.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

// root include file
#include "TFile.h"
#include "TNtuple.h"

using namespace std;


//
// class decleration
//

class ImpParameter : public edm::EDAnalyzer {
   public:
      explicit ImpParameter(const edm::ParameterSet&);
      ~ImpParameter();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

   std::ofstream out_b;
   std::string fBFileName;

   std::ofstream out_n;
   std::string fNFileName;

   std::ofstream out_m;
   std::string fMFileName;

  
   TNtuple *datatemp;
   TFile *f;
   std::string output;           // Output filename
 
   bool doCF_;
   edm::ESHandle < ParticleDataTable > pdt;

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
ImpParameter::ImpParameter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   fBFileName = iConfig.getUntrackedParameter<std::string>("output_b", "b_values.txt");
   fNFileName = iConfig.getUntrackedParameter<std::string>("output_n", "n_values.txt");
   fMFileName = iConfig.getUntrackedParameter<std::string>("output_m", "m_values.txt");
   doCF_ = iConfig.getUntrackedParameter<bool>("doMixed", false);
   // Output
   output = iConfig.getUntrackedParameter<std::string>("output", "imp.root");


}


ImpParameter::~ImpParameter()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
ImpParameter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace HepMC;
  
   iSetup.getData(pdt);

   const GenEvent* evt;
   int m = 0;
   int m1 = 0; // Multiplicity within |eta|<0.5
   int nc1 = 0; // Charged Multiplicity within |eta|<0.5
   int nc2 = 0; // Charged Multiplicity within |eta|<0.5

   if(doCF_){

     Handle<CrossingFrame<HepMCProduct> > cf;
     iEvent.getByLabel(InputTag("mix","source"),cf);

     MixCollection<HepMCProduct> mix(cf.product());

     int mixsize = mix.size();

     cout<<"Mix Collection Size: "<<mixsize<<endl;
     evt = mix.getObject(mixsize-1).GetEvent();

     MixCollection<HepMCProduct>::iterator begin = mix.begin();
     MixCollection<HepMCProduct>::iterator end = mix.end();
     
     for(MixCollection<HepMCProduct>::iterator mixit = begin; mixit != end; ++mixit){

       const GenEvent* subevt = (*mixit).GetEvent();
       int all = subevt->particles_size();
       HepMC::GenEvent::particle_const_iterator begin = subevt->particles_begin();
       HepMC::GenEvent::particle_const_iterator end = subevt->particles_end();
       for(HepMC::GenEvent::particle_const_iterator it = begin; it != end; ++it){
	 if((*it)->status() == 1){
	   ++m;
           float pdg_id = (*it)->pdg_id();
           float eta = (*it)->momentum().eta();
           const ParticleData * part = pdt->particle(pdg_id );
           float charge = part->charge();
           if (fabs(eta)>0.5) continue;
           ++m1;
           if (fabs(pdg_id)==211) ++nc1;
           if (fabs(pdg_id)==321) ++nc1;
           if (fabs(pdg_id)==2212) ++nc1;
           if (fabs(charge)>0) ++nc2;
	 }
       }
       
     }     
   }else{

     Handle<HepMCProduct> mc;
     iEvent.getByLabel("source",mc);
     evt = mc->GetEvent();

     int all = evt->particles_size();
     HepMC::GenEvent::particle_const_iterator begin = evt->particles_begin();
     HepMC::GenEvent::particle_const_iterator end = evt->particles_end();
     for(HepMC::GenEvent::particle_const_iterator it = begin; it != end; ++it){
       if((*it)->status() == 1){
	 ++m;
         float pdg_id = (*it)->pdg_id();
         const ParticleData * part = pdt->particle(pdg_id );
         float charge = part->charge();
         float eta = (*it)->momentum().eta();
         if (fabs(eta)>0.5) continue;
         ++m1;
         if (fabs(pdg_id)==211) ++nc1;
         if (fabs(pdg_id)==321) ++nc1;
         if (fabs(pdg_id)==2212) ++nc1;
         if (fabs(charge)>0) ++nc2;
       }
     }
     
   }

   const HeavyIon* hi = evt->heavy_ion();

   if(hi){
      double b = hi->impact_parameter();
      int n = hi->Npart_proj()+hi->Npart_targ();
      out_b<<b<<endl;
      out_n<<n<<endl;
      datatemp->Fill(m,b,n,m1,nc1,nc2);      
   }

   out_m<<m<<endl;

   

}


// ------------ method called once each job just before starting event loop  ------------
void 
ImpParameter::beginJob(const edm::EventSetup&)
{
   out_b.open(fBFileName.c_str());
   if(out_b.good() == false)
      throw cms::Exception("BadFile") << "Can\'t open file " << fBFileName;
   out_n.open(fNFileName.c_str());
   if(out_n.good() == false)
      throw cms::Exception("BadFile") << "Can\'t open file " << fNFileName;
   out_m.open(fMFileName.c_str());
   if(out_m.good() == false)
      throw cms::Exception("BadFile") << "Can\'t open file " << fMFileName;

   //TFile::TContext context(0);
   f = TFile::Open(output.c_str(), "recreate");

   datatemp = new TNtuple("event","Event Info","m:b:n:m1:nc1:nc2");
    

}

// ------------ method called once each job just after ending the event loop  ------------
void 
ImpParameter::endJob() {
   TFile::TContext context(f);

   datatemp->Write();
 
   f->Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(ImpParameter);
