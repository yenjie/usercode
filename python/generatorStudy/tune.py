import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')

process.load('Configuration.StandardSequences.VtxSmearedEarly7TeVCollision_cff')

process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.RandomNumberGeneratorService.generator0 = cms.PSet(
    initialSeed = cms.untracked.uint32(1),
    engineName = cms.untracked.string('HepJamesRandom')
    )

from IOMC.EventVertexGenerators.VtxSmearedParameters_cfi import *
process.VtxSmeared = cms.EDFilter("BetafuncEvtVtxGenerator",
                                       VtxSmearedCommon,
                                       Phi = cms.double(0.0),
                                       BetaStar = cms.double(1100.0),
                                       Emittance = cms.double(1.0e-07),
                                       Alpha = cms.double(0.0),
                                       SigmaZ = cms.double(2.22),
                                       TimeOffset = cms.double(0.0),
                                  #X0 = cms.double(0.2542),
                                  #Y0 = cms.double(0.4082),
                                  #Z0 = cms.double(-0.6569)

                                  X0 = cms.double(0.2417), # 0.09419   + 0.14750
                                  Y0 = cms.double(0.3855), # 0.007286  + 0.3782
                                  Z0 = cms.double(0.8685) # 0.3838    + 0.4847
                                  )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.5 $'),
    annotation = cms.untracked.string('PYTHIA6-MinBias at 900GeV'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/GenProduction/python/PYTHIA6_MinBias_900GeV_cff.py,v $')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("EmptySource")

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string("edmfile.root"),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RAW'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

#process.GlobalTag.globaltag = 'START3X_V25B::All'

process.generator0 = cms.EDFilter("Pythia6GeneratorFilter",
                                     pythiaPylistVerbosity = cms.untracked.int32(1),
                                     filterEfficiency = cms.untracked.double(1.0),
                                     pythiaHepMCVerbosity = cms.untracked.bool(False),
                                     comEnergy = cms.double(7000.0),
                                     crossSection = cms.untracked.double(52410000000.0),
                                     maxEventsToPrint = cms.untracked.int32(0),

                                     PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring(
        # ATLAS
        'MSTU(21)=1     ! Check on possible errors during program execution',
            'MSTJ(22)=2     ! Decay those unstable particles',
            'PARJ(71)=10.   ! for which ctau  10 mm',
            'MSTP(2)=1      ! which order running alphaS',
            'MSTP(33)=0     ! no K factors in hard cross sections',
            'MSTP(51)=7     ! structure function chosen (internal PDF CTEQ5L)',
            'MSTP(52)=1     ! work with internal PDF',
            'MSTP(81)=1    ! multiple parton interactions 1 is Pythia default',
            'MSTP(82)=4    ! Defines the multi-parton model',
            'PARP(82)=1.8  ! pt cutoff for multiparton interactions',
            'PARP(83)=0.5  ! Multiple interactions: matter distrbn parameter',
            'PARP(84)=0.5  ! Multiple interactions: matter distribution parameter',
            'PARP(85)=0.33 ! gluon prod. mechanism in MI',
            'PARP(86)=0.66 ! gluon prod. mechanism in MI',
            'PARP(89)=1000 ! sqrts for which PARP82 is set',
            'PARP(90)=0.16 ! Multiple interactions: rescaling power',
            'PARP(62)=1.0  !',
            'PARP(64)=1.0  !',
            'PARP(67)=1.0  ! amount of initial-state radiation',
            'MSTP(91)=1    !',
            'PARP(91)=1    ! kt distribution',
            'PARP(93)=5.0  !'
        ),
            processParameters = cms.vstring('MSEL=0         ! User defined processes',
                                                                                'MSUB(11)=1     ! Min bias process',
                                                                                'MSUB(12)=1     ! Min bias process',
                                                                                'MSUB(13)=1     ! Min bias process',
                                                                                'MSUB(28)=1     ! Min bias process',
                                                                                'MSUB(53)=1     ! Min bias process',
                                                                                'MSUB(68)=1     ! Min bias process',
                                                                                'MSUB(92)=1     ! Min bias process, single diffractive',
                                                                                'MSUB(93)=1     ! Min bias process, single diffractive',
                                                                                'MSUB(94)=1     ! Min bias process, double diffractive',
                                                                                'MSUB(95)=1     ! Min bias process'),
            parameterSets = cms.vstring('pythiaUESettings',
                                                                        'processParameters')
            )
                                                                  )

process.generator = cms.EDProducer('GenPtTuner',
                                   useFunction = cms.untracked.bool(True),
                                   doScale = cms.bool(True),
                                   scaleFactor = cms.double(0),
                                   src = cms.InputTag("generator0")
                                   )

process.ana = cms.EDAnalyzer('HydjetAnalyzer',
                             src = cms.untracked.InputTag("generator")
                                                          )
process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string("test.root")
                                  )
                                                                                                                                
process.ProductionFilterSequence = cms.Sequence(process.generator0 * process.generator * process.ana)

# Path and EndPath definitions
process.generation_step = cms.Path(process.ProductionFilterSequence)
process.end = cms.EndPath(process.output)