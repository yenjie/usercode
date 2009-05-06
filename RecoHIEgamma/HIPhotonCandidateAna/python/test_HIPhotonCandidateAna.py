import FWCore.ParameterSet.Config as cms

process = cms.Process("SIMMIX")

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring('cout','cerr'),
                                    fwkJobReports= cms.untracked.vstring('FrameworkJobReport.xml'),
                                    cerr = cms.untracked.PSet(    threshold = cms.untracked.string('WARNING')    ),
                                    cout = cms.untracked.PSet(    threshold = cms.untracked.string('WARNING')   ) 
                                    )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:/net/hisrv0001/home/jazzitup/CMSSW_2_2_8/src/test/testGammaJet2.root')
                            )


process.maxEvents = cms.untracked.PSet(    input = cms.untracked.int32(1000)   )



#process.load("SimGeneral.MixingModule.mixNoPU_cfi")
#process.load('RecoHIEgamma.Sample.mixNew_cfi')

#process.mix.input.seed = cms.untracked.uint32(1123)   


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   sourceSeed = cms.untracked.uint32(1123),
                                                   moduleSeeds = cms.PSet(VtxSmeared  = cms.untracked.uint32(1123),
                                                                          g4SimHits   = cms.untracked.uint32(1123),
                                                                          mix         = cms.untracked.uint32(1123),
                                                                          simHcalDigis= cms.untracked.uint32(1123),       
                                                                          muonCSCDigis= cms.untracked.uint32(1123),       
                                                                          muonDTDigis = cms.untracked.uint32(1123),       
                                                                          muonRPCDigis= cms.untracked.uint32(1123),       
                                                                          siPixelDigis= cms.untracked.uint32(1123),       
                                                                          siStripDigis= cms.untracked.uint32(1123),       
                                                                          simEcalUnsuppressedDigis =cms.untracked.uint32(1123), 
                                                                          simHcalUnsuppressedDigis =cms.untracked.uint32(1123)
                                                                          )
                                                   )

process.Timing = cms.Service("Timing")


process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.Digi_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("RecoEgamma.EgammaPhotonProducers.photonSequence_cff")


# Fake running conditions
process.load("SimTracker.SiStripDigitizer.SiStripDigi_cfi")
process.load("RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")   
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

# Magnetic field

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.Simulation_cff")
process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")

process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoLocalCalo.Configuration.RecoLocalCalo_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("RecoHIEgamma.Configuration.Analysis_cfi")



process.hIPhotonCandidateAna.output = cms.untracked.string('___OUTPUT_HI___.root')
process.hIPhotonCandidateAna.etCut = cms.untracked.double(10)
#process.p = cms.Path(process.calDigi*process.HIcaloReco*process.HIPhoton)
process.p = cms.Path(process.HIPhoton)





process.output = cms.OutputModule("PoolOutputModule",
                                  compressionLevel = cms.untracked.int32(2),
                                  fileName = cms.untracked.string('___OUTPUT_EDM___'),
                                  commitInterval = cms.untracked.uint32(1),
                                  fastCloning = cms.untracked.bool(False),
                                  splitLevel = cms.untracked.int32(1),
                                  outputCommands = cms.untracked.vstring('keep reco*_*_*_*',
                                                                         'keep *_hbhereco_*_*',
                                                                         'keep *_horeco_*_*',
                                                                         'keep *_hfreco_*_*',
                                                                         'keep *_hcalcalibreco_*_*',
                                                                         'keep *_ecalRecHit_*_*',
                                                                         'keep *_source_*_*',
                                                                         'drop *CrossingFrame_*_*_*'       
                                                                         )
                     )

#process.q = cms.EndPath(process.output)


