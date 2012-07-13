#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrections2012.h"
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TNtuple.h>
#include <iostream>

// a simple data format
class JetData
{
   public:
   int hiBin;
   float leadingJetPt;  
   float leadingJetEta;  
   float leadingJetPhi;  
   float subleadingJetPt;
   float subleadingJetEta;
   float subleadingJetPhi;
   float genleadingJetPt;  
   float genleadingJetEta;  
   float genleadingJetPhi;  
   float gensubleadingJetPt;
   float gensubleadingJetEta;
   float gensubleadingJetPhi;
   int    leadingJetIt;  
   int    subleadingJetIt;
   
   // setup output branches if setBranch
   JetData(TTree *t, bool setBranch=1) {
      if (setBranch){
         t->Branch("hiBin",&hiBin,"hiBin/I");
         t->Branch("leadingJetPt",&leadingJetPt,"leadingJetPt/F");
         t->Branch("leadingJetPhi",&leadingJetPhi,"leadingJetPhi/F");
         t->Branch("leadingJetEta",&leadingJetEta,"leadingJetEta/F");
         t->Branch("subleadingJetPt",&subleadingJetPt,"subleadingJetPt/F");
         t->Branch("subleadingJetPhi",&subleadingJetPhi,"subleadingJetPhi/F");
         t->Branch("subleadingJetEta",&subleadingJetEta,"subleadingJetEta/F");
         t->Branch("genleadingJetPt",&genleadingJetPt,"genleadingJetPt/F");
         t->Branch("genleadingJetPhi",&genleadingJetPhi,"genleadingJetPhi/F");
         t->Branch("genleadingJetEta",&genleadingJetEta,"genleadingJetEta/F");
         t->Branch("gensubleadingJetPt",&gensubleadingJetPt,"gensubleadingJetPt/F");
         t->Branch("gensubleadingJetPhi",&gensubleadingJetPhi,"gensubleadingJetPhi/F");
         t->Branch("gensubleadingJetEta",&gensubleadingJetEta,"gensubleadingJetEta/F");
         t->Branch("leadingJetIt",&leadingJetIt,"leadingJetIt/I");
         t->Branch("subleadingJetIt",&subleadingJetIt,"subleadingJetIt/I");
         t->SetAlias("Aj","(leadingJetPt-subleadingJetPt)/(leadingJetPt+subleadingJetPt)");
         t->SetAlias("genAj","(genleadingJetPt-gensubleadingJetPt)/(genleadingJetPt+gensubleadingJetPt)");
      }
   };
};


void dijetTrack(double tag=0, char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infName);

   // Turn off the non-necessary trees
   c->hasPFTree=0;
   c->hasPhotonTree=0;
   c->hasTowerTree=0;
   c->hasHbheTree=0;
   c->hasEbTree=0;
   c->hasGenpTree=0;
   c->hasGenParticleTree=0;   
   c->hasAkPu2CaloJetTree=0;
   c->hasAkPu3CaloJetTree=0;
   c->hasAkPu4CaloJetTree=0;
   c->doTrackCorrections=1;
   c->InitTree();
   
   // Output file
   TFile *output = new TFile(Form("output-%.0f.root",tag),"recreate");
   
   // Book a output tree
   TTree * t = new TTree("t","gammajet");

   // My tree data format   
   JetData data(t,1);

   // Book histograms
   TH1F *hJetTrackDphi    = new TH1F("hJetTrackDphi"   , ";#Delta #phi;Weighted Entries", 100,  0, TMath::Pi());
   TH1F *hJetTrackDeta    = new TH1F("hJetTrackDeta"   , ";#Delta #eta;Weighted Entries", 100, -2, 2);
   TH2F *hJetTrack2D      = new TH2F("hJetTrack2D"     , ";#Delta #eta; #Delta #phi"    , 100, -2, 2,100,0, TMath::Pi());
   TH1F *hGenJetTrackDphi = new TH1F("hGenJetTrackDphi", ";#Delta #phi;Weighted Entries", 100,  0, TMath::Pi());
   TH1F *hGenJetTrackDeta = new TH1F("hGenJetTrackDeta", ";#Delta #eta;Weighted Entries", 100, -2, 2);
   TH2F *hGenJetTrack2D   = new TH2F("hGenJetTrack2D"  , ";#Delta #eta; #Delta #phi"    , 100, -2, 2,100,0, TMath::Pi());
   
   // Main loop
   for (int i=0;i<c->GetEntries();i++) {
      c->GetEntry(i);
      data.hiBin = c->evt.hiBin;
      if (i % 1000 == 0) cout <<i<<" / "<<c->GetEntries()<<endl;
      data.leadingJetPt = -1;
      data.subleadingJetPt = -1;
      data.leadingJetIt = -1;
      data.subleadingJetIt = -1;
      data.genleadingJetPt = -1;
      data.gensubleadingJetPt = -1;
      
      // =================================
      // Select leading and subleading jet
      // =================================
      for (int j=0;j<c->icPu5.nref;j++) {
         if (fabs(c->icPu5.jteta[j])>2) continue;
         if (c->icPu5.jtpt[j]>data.leadingJetPt) {
	    data.leadingJetPt = c->icPu5.jtpt[j];
	    data.leadingJetEta = c->icPu5.jteta[j];
	    data.leadingJetPhi = c->icPu5.jtphi[j];
	    data.leadingJetIt = j;
	 }   
	 if (c->icPu5.jtpt[j]>data.subleadingJetPt && c->icPu5.jtpt[j] < data.leadingJetPt) {
	    data.subleadingJetPt = c->icPu5.jtpt[j];
	    data.subleadingJetEta = c->icPu5.jteta[j];
	    data.subleadingJetPhi = c->icPu5.jtphi[j];
	    data.subleadingJetIt = j;
         }
	 if (c->icPu5.jtpt[j]<data.subleadingJetPt) break;	 
      } 

      // =================================================
      // Select generator level leading and subleading jet
      // =================================================
      for (int j=0;j<c->icPu5.ngen;j++) {
         if (fabs(c->icPu5.geneta[j])>2) continue;
         if (c->icPu5.genpt[j]>data.genleadingJetPt) {
	    data.genleadingJetPt = c->icPu5.genpt[j];
	    data.genleadingJetEta = c->icPu5.geneta[j];
	    data.genleadingJetPhi = c->icPu5.genphi[j];
	 }   
	 if (c->icPu5.genpt[j]>data.gensubleadingJetPt && c->icPu5.genpt[j] < data.genleadingJetPt) {
	    data.gensubleadingJetPt  = c->icPu5.genpt[j];
	    data.gensubleadingJetEta = c->icPu5.geneta[j];
	    data.gensubleadingJetPhi = c->icPu5.genphi[j];
         }
	 if (c->icPu5.genpt[j]<data.gensubleadingJetPt) break;	 
      } 
      
      // ====================================================================
      // Now we have leading and subleading jet from generator and reco level
      // Loop over tracks in reco level
      // ====================================================================
      for (int j=0;j<c->track.nTrk;j++) {
         if (fabs(c->track.trkEta[j])>2.4) continue;
	 if (fabs(c->track.trkPt[j]) <0.5) continue;
	 double dphi1 = acos(cos(c->track.trkPhi[j]-data.leadingJetPhi));
         double deta1 = (c->track.trkEta[j]-data.leadingJetEta);

         // Fill the histogram, with track pT as the weight of the entry.
	 hJetTrackDphi->Fill(dphi1       , c->track.trkPt[j]);
	 hJetTrackDeta->Fill(deta1       , c->track.trkPt[j]);
	 hJetTrack2D  ->Fill(deta1, dphi1, c->track.trkPt[j]);
      }

      // =============================
      // Loop over tracks in gen level
      // =============================
      for (int j=0;j<c->track.nParticle;j++) {
         if (fabs(c->track.pEta[j])>2.4) continue;
	 if (fabs(c->track.pPt[j]) <0.5) continue;
	 double dphi1 = acos(cos(c->track.pPhi[j]-data.genleadingJetPhi));
         double deta1 = (c->track.pEta[j]-data.genleadingJetEta);
	 
	 double dr1 = sqrt(dphi1*dphi1+deta1*deta1);

         // Fill the histogram, with simtrack pT as the weight of the entry.
	 hGenJetTrackDphi->Fill(dphi1       , c->track.pPt[j]);
	 hGenJetTrackDeta->Fill(deta1       , c->track.pPt[j]);
	 hGenJetTrack2D  ->Fill(deta1, dphi1, c->track.pPt[j]);
      }

      t->Fill();
   }

   output->Write();
   output->Close();
}
