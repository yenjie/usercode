import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from Configuration.StandardSequences.Digi_cff import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *
from CalibCalorimetry.HcalPlugins.Hcal_FakeConditions_cff import *
from RecoEgamma.EgammaPhotonProducers.photonSequence_cff import *

   # Fake running conditions
from Configuration.StandardSequences.FakeConditions_cff import *

   # Geometry
from Geometry.CaloEventSetup.CaloTopology_cfi import *
from Configuration.StandardSequences.Geometry_cff import *

   #es_module = TrackerRecoGeometryESProducer {}

   # Magnetic field

from Configuration.StandardSequences.MagneticField_cff import *
from Configuration.StandardSequences.Simulation_cff import *

   # misc. item
dump = cms.EDAnalyzer("EventContentAnalyzer")

#from PhysicsTools.HepMCCandAlgos.genParticles_cfi import *
#genParticles.saveBarCodes = cms.untracked.bool(False)
#genParticles.src = cms.InputTag("source")
#genParticles.abortOnUnknownPDGCode = cms.untracked.bool(False)

# changed genparticles_cfi for subEvent map
#from RecoHI.HiCandidateAlgos.hiGenParticles_cfi import *



from  RecoHIEgamma.HIPhotonCandidateAna.hiphotoncandidateana_cfi import *



#Hcal local reco
from RecoHIEcal.Configuration.recoHICalo_cfi import *



#  Candidate producer



 

barrelclusters = cms.EDProducer("EcalCandidateProducer",
                                src = cms.InputTag("correctedIslandBarrelSuperClusters"),
                                particleType = cms.string('gamma')
                                )

endcapclusters = cms.EDProducer("EcalCandidateProducer",
                                src = cms.InputTag("correctedIslandEndcapSuperClusters"),
                                particleType = cms.string('gamma')
                                )
hybridclusters = cms.EDProducer("EcalCandidateProducer",
                                src = cms.InputTag("correctedHybridSuperClusters"),
                                particleType = cms.string('gamma')
                                )
multi5x5clusters = cms.EDProducer("EcalCandidateProducer",
                                  src = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"),
                                  particleType = cms.string('gamma')
                                  )

barrelmulti5x5clusters = cms.EDProducer("EcalCandidateProducer",
                                        src = cms.InputTag("multi5x5SuperClusters:multi5x5BarrelSuperClusters"),
                                        particleType = cms.string('gamma')
                                        )


gammaMatch = cms.EDFilter("MCTruthDeltaRMatcher",
                          src = cms.InputTag("barrelclusters"),
                          matched = cms.InputTag("gammaFilter"),
                          distMin = cms.double(0.15),
                          matchedPDGId = cms.vint32(22),
                          )
  


from RecoLocalCalo.Configuration.ecalLocalRecoSequence_cff import *

ecalWeightUncalibRecHit.EBdigiCollection = 'simEcalDigis:ebDigis'
ecalWeightUncalibRecHit.EEdigiCollection = 'simEcalDigis:eeDigis'



#HIPhotonPP = cms.Sequence( genParticleCandidates * barrelclusters * endcapclusters * hybridclusters * multi5x5clusters * hIPhotonCandidateAna)

#HIPhoton = cms.Sequence(hiGenParticles*hIPhotonCandidateAna)
HIPhoton = cms.Sequence(barrelclusters * endcapclusters *hIPhotonCandidateAna)




