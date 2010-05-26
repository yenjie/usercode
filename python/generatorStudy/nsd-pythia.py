
import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.files = ''
ivars.output = 'HIPAT_output_full3.root'
ivars.maxEvents = 50000
ivars.register ('randomNumber',
                mult=ivars.multiplicity.singleton,
                info="for testing")

ivars.register ('initialEvent',
                mult=ivars.multiplicity.singleton,
                info="for testing")

ivars.randomNumber=5
ivars.initialEvent=1
ivars.parseArguments()

import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/Generator_cff')
#process.load('MitQcd/Production/VtxSmeared2360GeVCollisionDec1224_cfi')

process.RandomNumberGeneratorService.theSource = cms.PSet(
                initialSeed = cms.untracked.uint32(ivars.randomNumber),
                                        engineName = cms.untracked.string('HepJamesRandom')
                                        )

process.ana = cms.EDAnalyzer('HydjetAnalyzer',
                             src = cms.untracked.InputTag("generator")
                             )

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(ivars.output)
                                   )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(ivars.maxEvents)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.load('Configuration.GenProduction.PYTHIA6_MinBias_2360GeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBias_Default_7TeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBias_YJL_7TeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBias_ATLAS2_7TeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBias_ATLAS_7TeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBiasProQ20_7TeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBiasStdQ20_7TeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBiasDW_2360GeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBiasP0_7TeV_cff')
#process.load('Configuration.GenProduction.PYTHIA6_MinBiasProPT0_7TeV_cff')
process.generator.comEnergy = cms.double(7000.0)
process.gen_step = cms.Path(process.generator)
process.ana_step = cms.Path(process.ana)
#process.Timing = cms.Service("Timing")


