#include "RecoHIEgamma/HIEgammaTools/interface/HIMCGammaJetSignalDef.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace edm;
using namespace reco;

#define PI 3.141592653589793238462643383279502884197169399375105820974945

HIMCGammaJetSignalDef::HIMCGammaJetSignalDef (const reco::CandidateCollection  *sigParticles) :
fSigParticles(0),fiNear(-1),fiAway(-1)
{
   fSigParticles =  sigParticles;
} 

bool HIMCGammaJetSignalDef::IsIsolated(const reco::Candidate &pp)
{
   using namespace edm;
   using namespace reco;
   // Check if a given particle is isolated.

   double  EtCone = 0;
   double  EtPhoton = 0;
   double  PtMaxHadron = 0;
   double  cone = 0.5;

   double etNear = 0;
   int iNear = -1;

   double etAway = 0;
   int iAway = -1;
 
   fiNear = -1;
   fiAway = -1;

   const int num = fSigParticles->size();
   for(int i=0; i < num; ++i) {

      const Candidate &p=(*fSigParticles)[i];

      if(p.status()==3){
	 if(p.pdgId()!=2212 &&
	    p.et()>40) { 
            double dphi = fabs(p.phi()-pp.phi());
            if (dphi>PI) dphi = 2*PI-dphi;
	    if(dphi < 0.5*PI) {
	       if(p.et()>etNear){
		  etNear = p.et();
		  iNear = (int) i;
	       }
	    }
	    if(dphi > 0.5*PI) {
	       if(p.et()>etAway){
		  etAway = p.et();
		  iAway = (int) i;
	       }
	    }
	 }
      }

      if(p.status()!=1) 
         continue; 

      int apid= abs(p.pdgId());
      if(apid>11 &&  apid<20) 
	 continue; //get rid of muons and neutrinos

      if(getDeltaR(p,pp)>cone) 
	 continue;

      if (iNear!=-1&&iAway!=-1) {
         fiNear = iNear;
         fiAway = iAway;
      }

      double pt=p.pt();
      EtCone+=pt;

      if(apid>100 && apid!=310 && pt>PtMaxHadron) 
	 PtMaxHadron=pt;
   }

   EtPhoton = pp.et();

   // isolation cuts
   if(EtCone-EtPhoton > 5+EtPhoton/20) 
      return kFALSE;
   if(PtMaxHadron > 4.5+EtPhoton/40) 
      return kFALSE;
     
   return kTRUE; 

}

bool HIMCGammaJetSignalDef::IsSignal(const reco::Candidate &pp,
                                      double dPhi, bool isIso)
{
   // Check if a given particle is considered to be signal. If isIso=true it is
   // already assumed that it is known that the given particle is isolated.

   if(pp.pdgId()!=22)
      return false;

   if(!isIso && !IsIsolated(pp))
      return false;

   if(fiNear==-1||fiAway==-1)
      return false;

   const reco::Candidate &AwayParton=(*fSigParticles)[fiAway];

   if(fabs(getDeltaPhi(pp,AwayParton))<dPhi)
      return false;

   return true;
}


double HIMCGammaJetSignalDef::getDeltaR(const reco::Candidate &track1, const reco::Candidate &track2)
{
    double dEta = track1.eta() - track2.eta();
    double dPhi = track1.phi() - track2.phi();
    
    while(dPhi >= PI)       dPhi -= (2.0*PI);
    while(dPhi < (-1.0*PI)) dPhi += (2.0*PI);
    
    return sqrt(dEta*dEta+dPhi*dPhi);
}

double HIMCGammaJetSignalDef::getDeltaPhi(const reco::Candidate &track1, const reco::Candidate &track2)
{
    double dPhi = track1.phi() - track2.phi();
    
    while(dPhi >= PI)       dPhi -= (2.0*PI);
    while(dPhi < (-1.0*PI)) dPhi += (2.0*PI);
    
    return dPhi;
}
