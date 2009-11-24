//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Nov  6 11:57:53 2009 by ROOT version 5.24/00
// from TTree data/ADC readout data
// found on file: Run-10001.root
//////////////////////////////////////////////////////////

#ifndef Loop_h
#define Loop_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class Loop {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
 //adcData         *adcData;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   ULong_t         trig;
   ULong_t         trigOverFlow;
   ULong_t         evt;
   vector<int>     data[8];
   vector<int>     time[8];
   Long_t          pedestal[8];
   UInt_t          evtDate;
   UInt_t          evtTime;
   UInt_t          evtSecond;
   UInt_t          evtNanosecond;

   // List of branches
   TBranch        *b_adcData_fUniqueID;   //!
   TBranch        *b_adcData_fBits;   //!
   TBranch        *b_adcData_trig;   //!
   TBranch        *b_adcData_trigOverFlow;   //!
   TBranch        *b_adcData_evt;   //!
   TBranch        *b_adcData_data;   //!
   TBranch        *b_adcData_time;   //!
   TBranch        *b_adcData_pedestal;   //!
   TBranch        *b_adcData_evtDate;   //!
   TBranch        *b_adcData_evtTime;   //!
   TBranch        *b_adcData_evtSecond;   //!
   TBranch        *b_adcData_evtNanosecond;   //!

   Loop(TTree *tree=0);
   virtual ~Loop();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     myLoop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Loop_cxx
Loop::Loop(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Run-10001.root");
      if (!f) {
         f = new TFile("Run-10001.root");
      }
      tree = (TTree*)gDirectory->Get("data");

   }
   Init(tree);
}

Loop::~Loop()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Loop::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Loop::LoadTree(Long64_t entry)
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

void Loop::Init(TTree *tree)
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

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_adcData_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_adcData_fBits);
   fChain->SetBranchAddress("trig", &trig, &b_adcData_trig);
   fChain->SetBranchAddress("trigOverFlow", &trigOverFlow, &b_adcData_trigOverFlow);
   fChain->SetBranchAddress("evt", &evt, &b_adcData_evt);
   fChain->SetBranchAddress("data[8]", data, &b_adcData_data);
   fChain->SetBranchAddress("time[8]", time, &b_adcData_time);
   fChain->SetBranchAddress("pedestal[8]", pedestal, &b_adcData_pedestal);
   fChain->SetBranchAddress("evtDate", &evtDate, &b_adcData_evtDate);
   fChain->SetBranchAddress("evtTime", &evtTime, &b_adcData_evtTime);
   fChain->SetBranchAddress("evtSecond", &evtSecond, &b_adcData_evtSecond);
   fChain->SetBranchAddress("evtNanosecond", &evtNanosecond, &b_adcData_evtNanosecond);
   Notify();
}

Bool_t Loop::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Loop::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Loop::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Loop_cxx
