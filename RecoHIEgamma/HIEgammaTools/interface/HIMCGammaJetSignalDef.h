#ifndef HIMCGammaJetSignalDef_h
#define HIMCGammaJetSignalDef_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"


class HIMCGammaJetSignalDef
{
  public:
  
   HIMCGammaJetSignalDef(const reco::CandidateCollection  *sigParticles);
   bool IsIsolated(const reco::Candidate &pp);
   bool IsSignal(const reco::Candidate &pp, double dPhi, bool isIso);

   double getDeltaR (const reco::Candidate &track1, const reco::Candidate &track2);
   double getDeltaPhi(const reco::Candidate &track1, const reco::Candidate &track2);

   int GetAwayParton() const { return fiAway;}
   int GetNearParton() const { return fiNear;}

  private:
   const reco::CandidateCollection        *fSigParticles;
   int                               fiNear;
   int                               fiAway;
};

#endif

