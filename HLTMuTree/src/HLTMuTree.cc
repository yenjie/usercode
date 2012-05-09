// -*- C++ -*-
//
// Package:    HLTMuTree
// Class:      HLTMuTree
// 
/**\class HLTMuTree HLTMuTree.cc UserCode/HLTMuTree/src/HLTMuTree.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Mihee Jo,588 R-012,+41227673278,
//         Created:  Thu Jul  7 11:47:28 CEST 2011
// $Id: HLTMuTree.cc,v 1.5 2011/10/31 07:32:25 frankma Exp $
//
//

#include "MuTrig/HLTMuTree/interface/HLTMuTree.h"

using namespace std;
using namespace reco;
using namespace edm;
using namespace HepMC;

//
// constructors and destructor
//
HLTMuTree::HLTMuTree(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  tagRecoMu = iConfig.getParameter<edm::InputTag>("muons");
  tagVtx = iConfig.getParameter<edm::InputTag>("vertices");
  doReco = iConfig.getUntrackedParameter<bool>("doReco");
  doGen = iConfig.getUntrackedParameter<bool>("doGen");
  tagGenPtl = iConfig.getParameter<edm::InputTag>("genparticle");
  tagSimTrk = iConfig.getParameter<edm::InputTag>("simtrack");

}


HLTMuTree::~HLTMuTree()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HLTMuTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  
  //Initialization
  GenMu.nptl = GlbMu.nptl = StaMu.nptl = 0;
  for (int i=0; i<nmax; i++) {
    GenMu.pid[i] = 10;
    GenMu.status[i] = 0;
    GenMu.mom[i] = 10;
    GenMu.pt[i] = 0;
    GenMu.p[i] = 0;
    GenMu.eta[i] = 0;
    GenMu.phi[i] = 0;
    GlbMu.charge[i] = 0;
    GlbMu.pt[i] = 0;
    GlbMu.p[i] = 0;
    GlbMu.eta[i] = 0;
    GlbMu.phi[i] = 0;
    GlbMu.dxy[i] = 0;
    GlbMu.dz[i] = 0;
    GlbMu.nValMuHits[i] = 0;
    GlbMu.nValTrkHits[i] = 0; 
    GlbMu.nTrkFound[i] = 0;
    GlbMu.glbChi2_ndof[i] = 0;
    GlbMu.trkChi2_ndof[i] = 0;
    GlbMu.pixLayerWMeas[i] = 0;
    GlbMu.trkDxy[i] = 0;
    GlbMu.trkDz[i] = 0;
    StaMu.charge[i] = 0;
    StaMu.pt[i] = 0;
    StaMu.p[i] = 0;
    StaMu.eta[i] = 0;
    StaMu.phi[i] = 0;
    StaMu.dxy[i] = 0;
    StaMu.dz[i] = 0;
  }

  //Get run, event, centrality
  event = iEvent.id().event();
  run = iEvent.id().run();
  lumi = iEvent.id().luminosityBlock();

  //Loop over GenParticles, g4SimHits
  if (doGen) {
    int nGen = 0;

    edm::Handle<reco::GenParticleCollection> genColl;
    iEvent.getByLabel(tagGenPtl,genColl);
    if (genColl.isValid()) {
      for (reco::GenParticleCollection::size_type i=0; i+1<genColl.product()->size(); i++) {
        const GenParticleRef genPtl(genColl,i);
        if (abs(genPtl->pdgId()) == 13 && genPtl->status() == 1) {
          GenMu.pt[nGen] = genPtl->pt();
          GenMu.p[nGen] = genPtl->p();
          GenMu.eta[nGen] = genPtl->eta();
          GenMu.phi[nGen] = genPtl->phi();
          GenMu.status[nGen] = genPtl->status();
          GenMu.pid[nGen] = genPtl->pdgId();
          
          GenMu.mom[nGen] = 10; 
          if (genPtl->numberOfMothers() > 0 ) {
            vector<int> momid;
            vector<int>::iterator it_jpsi, it_ups;
            for (unsigned int mom = 0; mom < genPtl->numberOfMothers(); mom++) {
              cout << "mom pid: " << genPtl->mother(mom)->pdgId() << endl;
              momid.push_back(genPtl->mother(mom)->pdgId());
            }

            if (!momid.empty()) {
              it_jpsi = find(momid.begin(),momid.end(),443);
              it_ups = find(momid.begin(),momid.end(),553);
              if (it_jpsi != momid.end()) GenMu.mom[nGen] = 443;
              if (it_ups != momid.end()) GenMu.mom[nGen] = 553;
              
              //No J/psi, Y mother -> Should check grandmother
              if (it_jpsi == momid.end() && it_ups == momid.end()) {
                const Candidate *mother = genPtl->mother(0);
                momid.clear();
                for (unsigned int mom = 0; mom < mother->numberOfMothers(); mom++) {
                  cout << "grand mom pid: " << mother->mother(mom)->pdgId() << endl;
                  momid.push_back(mother->mother(mom)->pdgId());
                }

                if (!momid.empty()) {
                  it_jpsi = find(momid.begin(),momid.end(),443);
                  it_ups = find(momid.begin(),momid.end(),553);
                  if (it_jpsi != momid.end()) GenMu.mom[nGen] = 443;
                  if (it_ups != momid.end()) GenMu.mom[nGen] = 553;
                  if (it_jpsi == momid.end() && it_ups == momid.end()) GenMu.mom[nGen] = momid[0];
                }
              } //End of no J/psi, Y mother -> Should check grandmother
            }

          }
          nGen++;

/*          if (genPtl->numberOfMothers() > 0 ) {
            GenMu.mom[nGen] = genPtl->mother(0)->pdgId();
            cout << "mom pid: " << genPtl->mother(0)->pdgId() << endl;
          } else {
            GenMu.mom[nGen] = 10; 
          }*/

        }
      }
    } //End of gen collection
    
/*    edm::Handle<TrackingParticleCollection> simColl;
    iEvent.getByLabel(tagSimTrk,simColl);
    if (simColl.isValid()) {
      for (TrackingParticleCollection::size_type i=0; i+1<simColl.product()->size(); i++) {
        const TrackingParticleRef simTrk(simColl,i);
        if (simTrk.isNull()) continue;
        if (abs(simTrk->pdgId()) == 13 && simTrk->status() == -99) {

          GenMu.pid[nGen] = simTrk->pdgId();
          GenMu.mom[nGen] = 10; 
          for (TrackingParticle::genp_iterator it=simTrk->genParticle_begin();
              it!=simTrk->genParticle_end(); ++it) {
            if ((*it)->status() == 1) GenMu.mom[nGen] = (*it)->pdg_id();
            cout << "sim track mom pid: " << (*it)->pdg_id() <<"\t"  << (*it)->status() << endl;
          }

          GenMu.pt[nGen] = simTrk->pt();
          GenMu.p[nGen] = simTrk->p();
          GenMu.eta[nGen] = simTrk->eta();
          GenMu.phi[nGen] = simTrk->phi();
          GenMu.status[nGen] = simTrk->status();
          nGen++;
        }
      }
    } //End of sim tracks
*/
    GenMu.nptl = nGen;
    cout << "gen_nptl: " << GenMu.nptl << endl;
    if (nGen >= nmax) {
      cout << "Gen muons in a event exceeded maximum. \n";
      return ;
    }

  } //End of doGen

  //Loop over reco::muon
  if (doReco) {
    //Put centrality information
    centrality = new CentralityProvider(iSetup);
    centrality->newEvent(iEvent,iSetup);
    cbin = centrality->getBin();
//    cbin = -1;

    //Get vertex position
    edm::Handle< vector<reco::Vertex> > vertex;
    iEvent.getByLabel(tagVtx,vertex);
    if(vertex->size() > 0){
      vx = vertex->begin()->x();
      vy = vertex->begin()->y();
      vz = vertex->begin()->z();
    } else {
      vx = -1;
      vy = -1;
      vz = -1; 
    }

    edm::Handle< edm::View<reco::Muon> > muons;
    iEvent.getByLabel(tagRecoMu,muons);
    int nGlb = 0;
    int nSta = 0;
    for (unsigned int i=0; i<muons->size(); i++) {
      edm::RefToBase<reco::Muon> muCand(muons,i);
      if (muCand.isNull()) continue;
      if (muCand->globalTrack().isNonnull() && muCand->innerTrack().isNonnull()) {
        if (muCand->isGlobalMuon() && muCand->isTrackerMuon() && fabs(muCand->combinedMuon()->eta()) < 2.4) {
          edm::RefToBase<reco::Track> trk = edm::RefToBase<reco::Track>(muCand->innerTrack());
          edm::RefToBase<reco::Track> glb = edm::RefToBase<reco::Track>(muCand->combinedMuon());
          const reco::HitPattern& p = trk->hitPattern();

          GlbMu.nValMuHits[nGlb] = muCand->combinedMuon().get()->hitPattern().numberOfValidMuonHits();
          GlbMu.nValTrkHits[nGlb] = muCand->innerTrack().get()->hitPattern().numberOfValidTrackerHits();

          GlbMu.nTrkFound[nGlb] = trk->found();
          GlbMu.glbChi2_ndof[nGlb] = glb->chi2()/glb->ndof();
          GlbMu.trkChi2_ndof[nGlb] = trk->chi2()/trk->ndof();
          GlbMu.pixLayerWMeas[nGlb] = p.pixelLayersWithMeasurement();
          GlbMu.trkDxy[nGlb] = fabs(trk->dxy(vertex->begin()->position()));
          GlbMu.trkDz[nGlb] = fabs(trk->dz(vertex->begin()->position()));

          GlbMu.charge[nGlb] = glb->charge();
          GlbMu.pt[nGlb] = glb->pt();
          GlbMu.p[nGlb] = glb->p();
          GlbMu.eta[nGlb] = glb->eta();
          GlbMu.phi[nGlb] = glb->phi();
          GlbMu.dxy[nGlb] = glb->dxy(vertex->begin()->position()); 
          GlbMu.dz[nGlb] = glb->dz(vertex->begin()->position());
          nGlb++;
        }
        
      }
      if (muCand->isStandAloneMuon() && muCand->outerTrack().isNonnull()) {
        if (muCand->standAloneMuon().get()->hitPattern().numberOfValidMuonHits()>0 && fabs(muCand->standAloneMuon()->eta())<2.4) {
          edm::RefToBase<reco::Track> sta = edm::RefToBase<reco::Track>(muCand->standAloneMuon());
          StaMu.charge[nSta] = sta->charge();
          StaMu.pt[nSta] = sta->pt();
          StaMu.p[nSta] = sta->p();
          StaMu.eta[nSta] = sta->eta();
          StaMu.phi[nSta] = sta->phi();
          StaMu.dxy[nSta] = sta->dxy(vertex->begin()->position()); 
          StaMu.dz[nSta] = sta->dz(vertex->begin()->position()); 
          nSta++;
        }
      }
      if (nGlb >= nmax) {
        cout << "Global muons in a event exceeded maximum. \n";
        return ;
      }
      if (nSta >= nmax) {
        cout << "Standalone muons in a event exceeded maximum. \n";
        return ;
      }
    }
    GlbMu.nptl = nGlb;
    StaMu.nptl = nSta;
  } // End of doReco
  else {
    vx = -1;
    vy = -1;
    vz = -1; 
    cbin = -1;
  }

  // Fill a muon tree
  treeMu->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
HLTMuTree::beginJob()
{
  treeMu = foutput->make<TTree>("HLTMuTree","HLTMuTree");
  treeMu->Branch("Run",&run,"run/I");
  treeMu->Branch("Event",&event,"event/I");
  treeMu->Branch("Lumi",&lumi,"lumi/I");
  treeMu->Branch("CentBin",&cbin,"cbin/I");
  treeMu->Branch("vx",&vx,"vx/F");
  treeMu->Branch("vy",&vy,"vy/F");
  treeMu->Branch("vz",&vz,"vz/F");

  treeMu->Branch("Gen_nptl",&GenMu.nptl,"Gen_nptl/I");
  treeMu->Branch("Gen_pid",GenMu.pid,"Gen_pid[Gen_nptl]/I");
  treeMu->Branch("Gen_mom",GenMu.mom,"Gen_mom[Gen_nptl]/I");
  treeMu->Branch("Gen_status",GenMu.status,"Gen_status[Gen_nptl]/I");
  treeMu->Branch("Gen_p",GenMu.p,"Gen_p[Gen_nptl]/F");
  treeMu->Branch("Gen_pt",GenMu.pt,"Gen_pt[Gen_nptl]/F");
  treeMu->Branch("Gen_eta",GenMu.eta,"Gen_eta[Gen_nptl]/F");
  treeMu->Branch("Gen_phi",GenMu.phi,"Gen_phi[Gen_nptl]/F");

  treeMu->Branch("Glb_nptl",&GlbMu.nptl,"Glb_nptl/I");
  treeMu->Branch("Glb_charge",GlbMu.charge,"Glb_charge[Glb_nptl]/I");
  treeMu->Branch("Glb_p",GlbMu.p,"Glb_p[Glb_nptl]/F");
  treeMu->Branch("Glb_pt",GlbMu.pt,"Glb_pt[Glb_nptl]/F");
  treeMu->Branch("Glb_eta",GlbMu.eta,"Glb_eta[Glb_nptl]/F");
  treeMu->Branch("Glb_phi",GlbMu.phi,"Glb_phi[Glb_nptl]/F");
  treeMu->Branch("Glb_dxy",GlbMu.dxy,"Glb_dx[Glb_nptl]/F");
  treeMu->Branch("Glb_dz",GlbMu.dz,"Glb_dz[Glb_nptl]/F");

  treeMu->Branch("Glb_nValMuHits",GlbMu.nValMuHits,"Glb_nValMuHits[Glb_nptl]/I");
  treeMu->Branch("Glb_nValTrkHits",GlbMu.nValTrkHits,"Glb_nValTrkHits[Glb_nptl]/I");
  treeMu->Branch("Glb_nTrkFound",GlbMu.nTrkFound,"Glb_nTrkFound[Glb_nptl]/I");
  treeMu->Branch("Glb_glbChi2_ndof",GlbMu.glbChi2_ndof,"Glb_glbChi2_ndof[Glb_nptl]/F");
  treeMu->Branch("Glb_trkChi2_ndof",GlbMu.trkChi2_ndof,"Glb_trkChi2_ndof[Glb_nptl]/F");
  treeMu->Branch("Glb_pixLayerWMeas",GlbMu.pixLayerWMeas,"Glb_pixLayerWMeas[Glb_nptl]/I");
  treeMu->Branch("Glb_trkDxy",GlbMu.trkDxy,"Glb_trkDxy[Glb_nptl]/F");
  treeMu->Branch("Glb_trkDz",GlbMu.trkDz,"Glb_trkDz[Glb_nptl]/F");

  treeMu->Branch("Sta_nptl",&StaMu.nptl,"Sta_nptl/I");
  treeMu->Branch("Sta_charge",StaMu.charge,"Sta_charge[Sta_nptl]/I");
  treeMu->Branch("Sta_p",StaMu.p,"Sta_p[Sta_nptl]/F");
  treeMu->Branch("Sta_pt",StaMu.pt,"Sta_pt[Sta_nptl]/F");
  treeMu->Branch("Sta_eta",StaMu.eta,"Sta_eta[Sta_nptl]/F");
  treeMu->Branch("Sta_phi",StaMu.phi,"Sta_phi[Sta_nptl]/F");
  treeMu->Branch("Sta_dxy",StaMu.dxy,"Sta_dx[Sta_nptl]/F");
  treeMu->Branch("Sta_dz",StaMu.dz,"Sta_dz[Sta_nptl]/F");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HLTMuTree::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTMuTree);
