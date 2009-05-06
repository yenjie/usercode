import FWCore.ParameterSet.Config as cms


hIPhotonCandidateAna = cms.EDAnalyzer("HIPhotonCandidateAna",
    SCBInput = cms.untracked.string('correctedIslandBarrelSuperClusters'),
    SCCandInput = cms.untracked.string('barrelclusters'),
    DoEndcap = cms.untracked.bool(True),
    GenCandInput = cms.untracked.string('hiGenParticles'),
    SignalOnly = cms.untracked.bool(False),
    ShapeBInput = cms.untracked.string('islandBasicClusters:islandBarrelShapeAssoc'),
    SCEInput = cms.untracked.string('correctedIslandEndcapSuperClusters'),
    etaCut = cms.untracked.double(3.0),
    ShapeEInput = cms.untracked.string('islandBasicClusters:islandEndcapShapeAssoc'),
    etCut = cms.untracked.double(10.0),
    output = cms.untracked.string('HIPhotonCand.root'),
    SCCandInput1 = cms.untracked.string('endcapclusters'),
    DoBarrel = cms.untracked.bool(True)
)



