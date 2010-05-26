#
# \version $Id: MPA_Apr27_V00-00-06_MC35X_START3X_V26.py,v 1.1 2010/04/27 16:15:21 musella Exp $
#

import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

## Source
process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_7_0_pre2/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/MC_37Y_V1-v1/0017/0ACB5AB8-B152-DF11-9BC6-002618943969.root'
#    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_6_0/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_36Y_V4-v1/0014/303FDF17-AB49-DF11-93D9-0018F3D09670.root'
#    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_5_7/RelValHydjetQ_MinBias_4TeV/GEN-SIM-RECO/MC_3XY_V26-v1/0012/0057B2D7-6749-DF11-BD93-0018F3D09612.root'
#     'file:edmfile_1.root'
    )
                            )

## # EGamma sequence
process.load("RecoHI.HiEgammaAlgos.HiEgamma_cff")

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START3X_V26::All')
# process.GlobalTag.globaltag = cms.string('GR10_P_V4::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

## Standard PAT Configuration File
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.patHeavyIonSequences_cff")
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

#from PhysicsTools.PatAlgos.tools.coreTools import *
# turn off MC matching for the process -- ONLY needed if running over data -- for MC comment out
#removeMCMatching(process, ['All'])

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

# configure HLT
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('40 OR 41')

# EGAMMA customization 
process.photons.minSCEtBarrel = 5.
process.photons.minSCEtEndcap = 5.
process.photonCore.minSCEt = 2.
#process.conversionTrackCandidates.minSCEt =1.
#process.conversions.minSCEt =1.
#process.trackerOnlyConversions.AllowTrackBC = cms.bool(False)
#process.trackerOnlyConversions.AllowRightBC = cms.bool(False)
#process.trackerOnlyConversions.MinApproach = cms.double(-.25)
#process.trackerOnlyConversions.DeltaCotTheta = cms.double(.07)
#process.trackerOnlyConversions.DeltaPhi = cms.double(.2)

# Remove impact parameter cuts
process.photons.isolationSumsCalculatorSet.transImpactParameterA_Barrel=9999.0
process.photons.isolationSumsCalculatorSet.transImpactParameterB_Barrel=9999.0
process.photons.isolationSumsCalculatorSet.transImpactParameterA_Endcap=9999.0
process.photons.isolationSumsCalculatorSet.transImpactParameterB_Endcap=9999.0
process.photons.isolationSumsCalculatorSet.longImpactParameterA_Barrel=9999.0
process.photons.isolationSumsCalculatorSet.longImpactParameterB_Barrel=9999.0
process.photons.isolationSumsCalculatorSet.longImpactParameterA_Endcap=9999.0
process.photons.isolationSumsCalculatorSet.longImpactParameterB_Endcap=9999.0

# Modification for HI
process.load("QCDPhotonAnalysis.DataAnalyzers.MultiPhotonAnalyzer_cfi")
process.multiPhotonAnalyzer.GenParticleProducer = cms.InputTag("hiGenParticles")
process.multiPhotonAnalyzer.PhotonProducer = cms.InputTag("selectedPatPhotons")
process.multiPhotonAnalyzer.VertexProducer = cms.InputTag("hiPixelAdaptiveVertex")
process.multiPhotonAnalyzer.doStoreMET = cms.untracked.bool(False)
process.multiPhotonAnalyzer.doStoreJets = cms.untracked.bool(False)



#process.photonMatch.matched = cms.InputTag("hiGenParticles")
process.selectDigi.barrelSuperClusterProducer = cms.InputTag("correctedIslandBarrelSuperClusters")
process.selectDigi.endcapSuperClusterProducer = cms.InputTag("correctedIslandEndcapSuperClusters")

# timing
process.Timing = cms.Service("Timing")                                         
# let it run
process.p = cms.Path(
            process.photonCore*process.photons*process.photonIDSequence*
#            process.patHeavyIonDefaultSequence 
            process.heavyIon *
            process.makeHeavyIonPhotons *
            process.selectedPatPhotons *
            process.multiPhotonAnalyzer
            )


