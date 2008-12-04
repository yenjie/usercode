import FWCore.ParameterSet.Config as cms



from RecoLocalCalo.EcalRecProducers.ecalWeightUncalibRecHit_cfi import *
from RecoLocalCalo.EcalRecProducers.ecalRecHit_cfi import *
from RecoLocalCalo.EcalRecProducers.ecalPreshowerRecHit_cfi import *

ecalloc = cms.Sequence(ecalWeightUncalibRecHit*ecalRecHit*ecalPreshowerRecHit )

from RecoEcal.EgammaClusterProducers.islandClusteringSequence_cff import *
from RecoEcal.EgammaClusterProducers.hybridClusteringSequence_cff import *
from RecoEcal.EgammaClusterProducers.preshowerClusteringSequence_cff import *
ecalcst = cms.Sequence( islandClusteringSequence * hybridClusteringSequence )

from Configuration.StandardSequences.Reconstruction_cff import *

hbhereco.digiLabel = 'simHcalDigis'
horeco.digiLabel = 'simHcalDigis'
hfreco.digiLabel = 'simHcalDigis'

ecalWeightUncalibRecHit.EBdigiCollection =  'simEcalDigis:ebDigis'
ecalWeightUncalibRecHit.EEdigiCollection =  'simEcalDigis:eeDigis'
ecalPreshowerRecHit.ESdigiCollection = 'simEcalPreshowerDigis'
    

from RecoLocalCalo.Configuration.hcalLocalReco_cff import *

HIcaloReco = cms.Sequence(ecalloc*islandClusteringSequence*hcalLocalRecoSequence)
 
