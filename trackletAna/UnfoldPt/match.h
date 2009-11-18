//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 13 18:05:53 2009 by ROOT version 5.18/00b
// from TTree TrackletTree13/Tree of Reconstructed Tracklets
// found on file: TrackletTree-900GeV-test.root
//////////////////////////////////////////////////////////

#ifndef match_h
#define match_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class match {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   double         calcDphi(double phi1_,double phi2_);
   // Declaration of leaf types
   Int_t           nTracklets;
   Int_t           nhit1;
   Int_t           nhit2;
   Int_t           mult;
   Int_t           nv;
   Float_t         vz[4];   //[nv]
   Float_t         eta1[14300];   //[nTracklets]
   Float_t         phi1[14300];   //[nTracklets]
   Float_t         eta2[14300];   //[nTracklets]
   Float_t         phi2[14300];   //[nTracklets]
   Float_t         deta[14300];   //[nTracklets]
   Float_t         dphi[14300];   //[nTracklets]
   Int_t           npart;
   Float_t         eta[9400];   //[npart]
   Float_t         phi[9400];   //[npart]
   Int_t           pdg[9400];   //[npart]
   Int_t           chg[9400];   //[npart]
   Float_t         nhad[12];
   Float_t         pt[9400];   //[npart]
   Int_t           evtType;

   // List of branches
   TBranch        *b_nTracklets;   //!
   TBranch        *b_nhit1;   //!
   TBranch        *b_nhit2;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_nv;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_deta;   //!
   TBranch        *b_dphi;   //!
   TBranch        *b_npart;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_chg;   //!
   TBranch        *b_nhad;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_evtType;   //!

   match(TTree *tree=0);
   virtual ~match();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef match_cxx
match::match(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("TrackletTree-900GeV-test.root");
      if (!f) {
         f = new TFile("TrackletTree-900GeV-test.root");
      }
      tree = (TTree*)gDirectory->Get("TrackletTree13");

   }
   Init(tree);
}

match::~match()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t match::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t match::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void match::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nTracklets", &nTracklets, &b_nTracklets);
   fChain->SetBranchAddress("nhit1", &nhit1, &b_nhit1);
   fChain->SetBranchAddress("nhit2", &nhit2, &b_nhit2);
   fChain->SetBranchAddress("mult", &mult, &b_mult);
   fChain->SetBranchAddress("nv", &nv, &b_nv);
   fChain->SetBranchAddress("vz", vz, &b_vz);
   fChain->SetBranchAddress("eta1", eta1, &b_eta1);
   fChain->SetBranchAddress("phi1", phi1, &b_phi1);
   fChain->SetBranchAddress("eta2", eta2, &b_eta2);
   fChain->SetBranchAddress("phi2", phi2, &b_phi2);
   fChain->SetBranchAddress("deta", deta, &b_deta);
   fChain->SetBranchAddress("dphi", dphi, &b_dphi);
   fChain->SetBranchAddress("npart", &npart, &b_npart);
   fChain->SetBranchAddress("eta", eta, &b_eta);
   fChain->SetBranchAddress("phi", phi, &b_phi);
   fChain->SetBranchAddress("pdg", pdg, &b_pdg);
   fChain->SetBranchAddress("chg", chg, &b_chg);
   fChain->SetBranchAddress("nhad", nhad, &b_nhad);
   fChain->SetBranchAddress("pt", pt, &b_pt);
   fChain->SetBranchAddress("evtType", &evtType, &b_evtType);
   Notify();
}

Bool_t match::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void match::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t match::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef match_cxx
