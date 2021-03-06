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
#process.load('Configuration/StandardSequences/Generator_cff')
#process.load('MitQcd/Production/VtxSmeared2360GeVCollisionDec1224_cfi')

process.RandomNumberGeneratorService.theSource = cms.PSet(
                initialSeed = cms.untracked.uint32(ivars.randomNumber),
                                        engineName = cms.untracked.string('HepJamesRandom')
                                        )

process.ana = cms.EDAnalyzer('HydjetAnalyzer',
                             src = cms.untracked.InputTag("source")
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


process.load('GeneratorInterface.PhojetInterface.PHOJET_MinBias_2360GeV_cff')

process.source.comEnergy = cms.untracked.double(900.0)
#process.PhojetSource.PhojetParameters.minBiasParameters = [ 'PROCESS     1  0  1  1  1  1  1  1  1  1  1' ]
process.PhojetSource.PhojetParameters.minBiasParameters = [ 'PROCESS     1  0  1  0  0  0  1  1  1  1  1' ]
process.ana_step = cms.Path(process.ana)



