#ifndef HIPhotonMCType_h
#define HIPhotonMCType_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"


class HIPhotonMCType
{
  public:
   enum EType {
      kUnknown = 0,  // complement of sets below
      kLO      = 1,  // leading order direct gamma
      kDirect  = 2,  // direct gamma
      kFrag    = 3,  // gamma from gluon, quark
      kDecay   = 4,  // gamma from hadron decay
      kBrems   = 5,  // gamma from bremsstrahlung effect
      kInvalid = 100 // given particle is not a photon 
   };                // or any other problem occured 

   enum EPid {
      kUq=1, kDq=2, kSq=3, kCq=4, kBq=5, kTq=6,
      kElectron=11, kMuon=13, kTau=15,
      kEneutrino=12, kMneutrino=14, kTneutrino=16,
      kGluon=21, kGamma=22,
      kJpsi=443, kUpsilon=553
   }; 
  
   HIPhotonMCType();
   static EType getType(const reco::Candidate &part);

  private:
   
};

#endif
