/////////////////////////////////////////////////////////////////////////
//   This class has been automatically generated 
//   (at Tue Nov 10 16:16:46 2009 by ROOT version 5.22/00a)
//   from TTree data/ADC readout data
//   found on file: outputCosmicTest.root
/////////////////////////////////////////////////////////////////////////


#ifndef generatedSel_h
#define generatedSel_h

// System Headers needed by the proxy
#if defined(__CINT__) && !defined(__MAKECINT__)
   #define ROOT_Rtypes
   #define ROOT_TError
#endif
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelectorDraw.h>
#include <TPad.h>
#include <TH1.h>
#include <TBranchProxy.h>
#include <TBranchProxyDirector.h>
#include <TBranchProxyTemplate.h>
#include <TFriendProxy.h>
#include <TMethodCall.h>

using namespace ROOT;

// forward declarations needed by this particular proxy


// Header needed by this particular proxy


class generatedSel : public TSelector {
   public :
   TTree          *fChain;    //!pointer to the analyzed TTree or TChain
   TSelectorDraw  *fHelper;   //!helper class to create the default histogram
   TList          *fInput;    //!input list of the helper
   TH1            *htemp;     //!pointer to the histogram
   TBranchProxyDirector  fDirector; //!Manages the proxys

   // Optional User methods
   TClass         *fClass;    // Pointer to this class's description
   TMethodCall     fBeginMethod;
   TMethodCall     fSlaveBeginMethod;
   TMethodCall     fNotifyMethod;
   TMethodCall     fProcessMethod;
   TMethodCall     fSlaveTerminateMethod;
   TMethodCall     fTerminateMethod;

   // Proxy for each of the branches, leaves and friends of the tree
   TIntProxy          nch;
   TIntProxy          run;
   TIntProxy          trig;
   TIntProxy          evt;
   TIntProxy          evtSecond;
   TIntProxy          evtTime;
   TIntProxy          dSize0;
   TArrayFloatProxy   data0;
   TArrayFloatProxy   time0;
   TIntProxy          dSize1;
   TArrayFloatProxy   data1;
   TArrayFloatProxy   time1;
   TIntProxy          dSize2;
   TArrayFloatProxy   data2;
   TArrayFloatProxy   time2;
   TIntProxy          dSize3;
   TArrayFloatProxy   data3;
   TArrayFloatProxy   time3;
   TIntProxy          dSize4;
   TArrayFloatProxy   data4;
   TArrayFloatProxy   time4;
   TIntProxy          dSize5;
   TArrayFloatProxy   data5;
   TArrayFloatProxy   time5;
   TIntProxy          dSize6;
   TArrayFloatProxy   data6;
   TArrayFloatProxy   time6;
   TIntProxy          dSize7;
   TArrayFloatProxy   data7;
   TArrayFloatProxy   time7;
   TArrayFloatProxy   min;
   TArrayFloatProxy   mintime;
   TArrayFloatProxy   minSB;
   TArrayFloatProxy   minSBtime;


   generatedSel(TTree *tree=0) : 
      fChain(0),
      fHelper(0),
      fInput(0),
      htemp(0),
      fDirector(tree,-1),
      fClass                (TClass::GetClass("generatedSel")),
      fBeginMethod          (fClass,"outputCosmicTest_Begin","(TTree*)0"),
      fSlaveBeginMethod     (fClass,"outputCosmicTest_SlaveBegin","(TTree*)0"),
      fNotifyMethod         (fClass,"outputCosmicTest_Notify",""),
      fProcessMethod        (fClass,"outputCosmicTest_Process","0"),
      fSlaveTerminateMethod (fClass,"outputCosmicTest_SlaveTerminate",""),
      fTerminateMethod      (fClass,"outputCosmicTest_Terminate",""),
      nch               (&fDirector,"nch"),
      run               (&fDirector,"run"),
      trig              (&fDirector,"trig"),
      evt               (&fDirector,"evt"),
      evtSecond         (&fDirector,"evtSecond"),
      evtTime           (&fDirector,"evtTime"),
      dSize0            (&fDirector,"dSize0"),
      data0             (&fDirector,"data0"),
      time0             (&fDirector,"time0"),
      dSize1            (&fDirector,"dSize1"),
      data1             (&fDirector,"data1"),
      time1             (&fDirector,"time1"),
      dSize2            (&fDirector,"dSize2"),
      data2             (&fDirector,"data2"),
      time2             (&fDirector,"time2"),
      dSize3            (&fDirector,"dSize3"),
      data3             (&fDirector,"data3"),
      time3             (&fDirector,"time3"),
      dSize4            (&fDirector,"dSize4"),
      data4             (&fDirector,"data4"),
      time4             (&fDirector,"time4"),
      dSize5            (&fDirector,"dSize5"),
      data5             (&fDirector,"data5"),
      time5             (&fDirector,"time5"),
      dSize6            (&fDirector,"dSize6"),
      data6             (&fDirector,"data6"),
      time6             (&fDirector,"time6"),
      dSize7            (&fDirector,"dSize7"),
      data7             (&fDirector,"data7"),
      time7             (&fDirector,"time7"),
      min               (&fDirector,"min"),
      mintime           (&fDirector,"mintime"),
      minSB             (&fDirector,"minSB"),
      minSBtime         (&fDirector,"minSBtime")
      { }
   ~generatedSel();
   Int_t   Version() const {return 1;}
   void    Begin(::TTree *tree);
   void    SlaveBegin(::TTree *tree);
   void    Init(::TTree *tree);
   Bool_t  Notify();
   Bool_t  Process(Long64_t entry);
   void    SetOption(const char *option) { fOption = option; }
   void    SetObject(TObject *obj) { fObject = obj; }
   void    SetInputList(TList *input) {fInput = input;}
   TList  *GetOutputList() const { return fOutput; }
   void    SlaveTerminate();
   void    Terminate();

   ClassDef(generatedSel,0);


//inject the user's code
#include "outputCosmicTest.root"
};

#endif


#ifdef __MAKECINT__
#pragma link C++ class generatedSel;
#endif


inline generatedSel::~generatedSel() {
   // destructor. Clean up helpers.

   delete fHelper;
   delete fInput;
}

inline void generatedSel::Init(TTree *tree)
{
//   Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fDirector.SetTree(fChain);
   delete fHelper;
   fHelper = new TSelectorDraw();
   delete fInput;
   fInput  = new TList();
   fInput->SetOwner();
   fInput->Add(new TNamed("varexp","0.0")); // Fake a double size histogram
   fInput->Add(new TNamed("selection",""));
   fHelper->SetInputList(fInput);
}

Bool_t generatedSel::Notify()
{
   // Called when loading a new file.
   // Get branch pointers.
   fDirector.SetTree(fChain);
   if (fNotifyMethod.IsValid()) fNotifyMethod.Execute(this);
   
   return kTRUE;
}
   

inline void generatedSel::Begin(TTree *tree)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   if (fBeginMethod.IsValid()) fBeginMethod.Execute(this,Form("0x%lx",tree));

}

inline void generatedSel::SlaveBegin(TTree *tree)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   Init(tree);

   TString option = GetOption();
   fHelper->SetOption(option);
   fHelper->Begin(tree);
   htemp = (TH1*)fHelper->GetObject();
   htemp->SetTitle("outputCosmicTest.root");
   fObject = htemp;
   if (fSlaveBeginMethod.IsValid()) {
      fSlaveBeginMethod.Execute(this,Form("0x%lx",tree));
   }

}

inline Bool_t generatedSel::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either TTree::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.

   // WARNING when a selector is used with a TChain, you must use
   //  the pointer to the current TTree to call GetEntry(entry).
   //  The entry is always the local entry number in the current tree.
   //  Assuming that fChain is the pointer to the TChain being processed,
   //  use fChain->GetTree()->GetEntry(entry).


   fDirector.SetReadEntry(entry);
   htemp->Fill(outputCosmicTest());
   if (fProcessMethod.IsValid()) fProcessMethod.Execute(this,Form("%d",entry));
   return kTRUE;

}

inline void generatedSel::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   if (fSlaveTerminateMethod.IsValid()) fSlaveTerminateMethod.Execute(this);
}

inline void generatedSel::Terminate()
{
   // Function called at the end of the event loop.
   Int_t drawflag = (htemp && htemp->GetEntries()>0);
   
   if (!drawflag && !fOption.Contains("goff") && !fOption.Contains("same")) {
      gPad->Clear();
   } else {
      if (fOption.Contains("goff")) drawflag = false;
      if (drawflag) htemp->Draw(fOption);
   }
   if (fTerminateMethod.IsValid()) fTerminateMethod.Execute(this);
   delete fHelper; fHelper = 0;
}
