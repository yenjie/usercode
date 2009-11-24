#define myLoop_cxx
#include "myLoop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;

void myLoop::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L myLoop.C
//      Root > myLoop t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries*0+10;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      for (int j=0;j<8;j++) {
         cout <<"time size "<<j<<" "<<time[j].size()<<endl;
	 for (int k=0;k<time[j].size();k++){
	   // if (data[j][k]<pedestal[j]-1) 
	    cout <<data[j][k]<<" "<<time[j][k]<<endl;
	 }
	 cout <<"Pedestal: "<<j<<" "<<pedestal[j]<<endl;
      }
      cout <<evt<<endl;
   }
}
