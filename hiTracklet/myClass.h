//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jul  4 22:01:40 2010 by ROOT version 5.22/00a
// from TTree TrackletTree12/Tree of Reconstructed Tracklets
// found on file: TrackletTree-HYDJET-25k.root
//////////////////////////////////////////////////////////

#ifndef myClass_h
#define myClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
class myClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nRun;
   Int_t           nEv;
   Int_t           nLumi;
   Int_t           nBX;
   Int_t           nHFn;
   Int_t           nHFp;
   Int_t           nHits;
   Int_t           nHltBit;
   Bool_t          hltBit[1];   //[nHltBit]
   Int_t           nL1ABit;
   Bool_t          l1ABit[1];   //[nL1ABit]
   Int_t           nL1TBit;
   Bool_t          l1TBit[1];   //[nL1TBit]
   Int_t           nTracklets;
   Int_t           nhit1;
   Int_t           nhit2;
   ULong64_t       mult;
   ULong64_t       nProtoTracklet;
   Int_t           mult2;
   Int_t           nv;
   Float_t         vx[3];   //[nv]
   Float_t         vy[3];   //[nv]
   Float_t         vz[3];   //[nv]
   Float_t         eta1[7092];   //[nTracklets]
   Float_t         deta[7092];   //[nTracklets]
   Float_t         dphi[7092];   //[nTracklets]
   ULong64_t       weight[7092];   //[nTracklets]
   Int_t           npart;
   Float_t         eta[12918];   //[npart]
   Float_t         phi[12918];   //[npart]
   Int_t           pdg[12918];   //[npart]
   Int_t           chg[12918];   //[npart]
   Float_t         nhad[12];
   Float_t         pt[12918];   //[npart]
   Int_t           evtType;
   Float_t         pro2;
   Float_t         vtxqual;
   Float_t         npxhits;
   Float_t         vtxQualCut;
   Float_t         b;

   // List of branches
   TBranch        *b_nRun;   //!
   TBranch        *b_nEv;   //!
   TBranch        *b_nLumi;   //!
   TBranch        *b_nBX;   //!
   TBranch        *b_nHFn;   //!
   TBranch        *b_nHFp;   //!
   TBranch        *b_nHits;   //!
   TBranch        *b_nHltBit;   //!
   TBranch        *b_hltBit;   //!
   TBranch        *b_nL1ABit;   //!
   TBranch        *b_l1ABit;   //!
   TBranch        *b_nL1TBit;   //!
   TBranch        *b_l1TBit;   //!
   TBranch        *b_nTracklets;   //!
   TBranch        *b_nhit1;   //!
   TBranch        *b_nhit2;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_nProtoTracklet;   //!
   TBranch        *b_mult2;   //!
   TBranch        *b_nv;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_deta;   //!
   TBranch        *b_dphi;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_npart;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_chg;   //!
   TBranch        *b_nhad;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_evtType;   //!
 //  TBranch        *b_evtType;   //!
   TBranch        *b_vtxqual;   //!
   TBranch        *b_npxhits;   //!
   TBranch        *b_vtxQualCut;   //!
   TBranch        *b_b;   //!

   myClass(TTree *tree=0);
   virtual ~myClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef myClass_cxx
myClass::myClass(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("TrackletTree-HYDJET-25k.root");
      if (!f) {
         f = new TFile("TrackletTree-HYDJET-25k.root");
      }
      tree = (TTree*)gDirectory->Get("TrackletTree12");

   }
   Init(tree);
}

myClass::~myClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t myClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t myClass::LoadTree(Long64_t entry)
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

void myClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("nRun", &nRun, &b_nRun);
   fChain->SetBranchAddress("nEv", &nEv, &b_nEv);
   fChain->SetBranchAddress("nLumi", &nLumi, &b_nLumi);
   fChain->SetBranchAddress("nBX", &nBX, &b_nBX);
   fChain->SetBranchAddress("nHFn", &nHFn, &b_nHFn);
   fChain->SetBranchAddress("nHFp", &nHFp, &b_nHFp);
   fChain->SetBranchAddress("nHits", &nHits, &b_nHits);
   fChain->SetBranchAddress("nHltBit", &nHltBit, &b_nHltBit);
   fChain->SetBranchAddress("hltBit", hltBit, &b_hltBit);
   fChain->SetBranchAddress("nL1ABit", &nL1ABit, &b_nL1ABit);
   fChain->SetBranchAddress("l1ABit", &l1ABit, &b_l1ABit);
   fChain->SetBranchAddress("nL1TBit", &nL1TBit, &b_nL1TBit);
   fChain->SetBranchAddress("l1TBit", &l1TBit, &b_l1TBit);
   fChain->SetBranchAddress("nTracklets", &nTracklets, &b_nTracklets);
   fChain->SetBranchAddress("nhit1", &nhit1, &b_nhit1);
   fChain->SetBranchAddress("nhit2", &nhit2, &b_nhit2);
   fChain->SetBranchAddress("mult", &mult, &b_mult);
   fChain->SetBranchAddress("nProtoTracklet", &nProtoTracklet, &b_nProtoTracklet);
   fChain->SetBranchAddress("mult2", &mult2, &b_mult2);
   fChain->SetBranchAddress("nv", &nv, &b_nv);
   fChain->SetBranchAddress("vx", vx, &b_vx);
   fChain->SetBranchAddress("vy", vy, &b_vy);
   fChain->SetBranchAddress("vz", vz, &b_vz);
   fChain->SetBranchAddress("eta1", eta1, &b_eta1);
   fChain->SetBranchAddress("deta", deta, &b_deta);
   fChain->SetBranchAddress("dphi", dphi, &b_dphi);
   fChain->SetBranchAddress("weight", weight, &b_weight);
   fChain->SetBranchAddress("npart", &npart, &b_npart);
   fChain->SetBranchAddress("eta", eta, &b_eta);
   fChain->SetBranchAddress("phi", phi, &b_phi);
   fChain->SetBranchAddress("pdg", pdg, &b_pdg);
   fChain->SetBranchAddress("chg", chg, &b_chg);
   fChain->SetBranchAddress("nhad", nhad, &b_nhad);
   fChain->SetBranchAddress("pt", pt, &b_pt);
   fChain->SetBranchAddress("evtType", &evtType, &b_evtType);
//   fChain->SetBranchAddress("pro2", &pro2, &b_evtType);
   fChain->SetBranchAddress("vtxqual", &vtxqual, &b_vtxqual);
   fChain->SetBranchAddress("npxhits", &npxhits, &b_npxhits);
   fChain->SetBranchAddress("vtxQualCut", &vtxQualCut, &b_vtxQualCut);
   fChain->SetBranchAddress("b", &b, &b_b);
   Notify();
}

Bool_t myClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void myClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t myClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef myClass_cxx
