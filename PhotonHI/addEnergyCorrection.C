#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void addEnergyCorrection(std::string inputFile_)
{
  const int nMaxPho = 300;

   TFile *correctionFile = new TFile("correction/correction-PYTHIAData.root");
   TH2D *hEtCorrection = (TH2D*)correctionFile->FindObjectAny("hEtCorrection");
   TH2D *hEtCorrectionUnc = (TH2D*)correctionFile->FindObjectAny("hEtCorrectionUnc");

  // get the correct tree from input file
   TFile *inf = new TFile(inputFile_.data());
   TTree *fChain = (TTree*)inf->FindObjectAny("Analysis");
   TTree *fChain2 = (TTree*)inf->FindObjectAny("Generator");
   cout << "Input file is " << inputFile_ << endl;

   // rename the output file
   std::string remword=".root";
   size_t pos = inputFile_.find(remword);
   std::string forOutput = inputFile_;  
   if(pos!= std::string::npos)
     forOutput.swap(forOutput.erase(pos,remword.length()));   
   std::string endfix = "_correctedTree.root";
   std::string outputFile = forOutput + endfix;

   // now open new root file
   TFile* newfile_data = new TFile(outputFile.data(),"recreate");
   cout << "Output file " << outputFile << endl;

   // clone tree
   TTree* newtree = fChain->CloneTree(0);
   TTree* newtree2 = fChain2->CloneTree(0);
   newtree->SetMaxTreeSize(4000000000);
   newtree2->SetMaxTreeSize(4000000000);
   cout << "Saving "  << endfix << " tree" << endl;

   Long64_t nentries = fChain->GetEntries();
    cout << "nentries = " << nentries << endl;
/*
   fChain->SetBranchAddress("pt",&phoEt);
   fChain->SetBranchAddress("eta",&phoEta);
   fChain->SetBranchAddress("hadronicOverEm",&phoHoverE);
   fChain->SetBranchAddress("nPhotons",&nphotonscounter);
   fChain->SetBranchAddress("isEB",&isEB);
   fChain->SetBranchAddress("isEE",&isEE);
   fChain->SetBranchAddress("seedSeverity",&seedSeverity);
   fChain->SetBranchAddress("seedRecoFlag",&seedRecoFlag);
*/

   Float_t etCor[nMaxPho];
   Float_t eta[nMaxPho];
   Float_t et[nMaxPho];
   Float_t r9[nMaxPho];
   UChar_t isEE[nMaxPho];

   Int_t cBin;
   Int_t nPhotons;
   fChain->SetBranchAddress("cBin",&cBin);
   fChain->SetBranchAddress("nPhotons",&nPhotons);
   fChain->SetBranchAddress("et",et);
   fChain->SetBranchAddress("r9",r9);
   fChain->SetBranchAddress("isEE",isEE);

   // add var
   newtree->Branch("etCorrected",etCor,"etCorrected[nPhotons]/F");


   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
     fChain->GetEntry(jentry);
      for (int j=0;j<nPhotons;j++) {
	 int bin = hEtCorrection->FindBin(cBin,et[j]);
         if (r9[j]>0.94) etCor[j]=et[j]/(1.+hEtCorrectionUnc->GetBinContent(bin));
                    else etCor[j]=et[j]/(1.+hEtCorrection->GetBinContent(bin));
      }	

      newtree->Fill();
   }

   cout <<"Saving Generator tree..."<<endl;
   
   int nentries2 = fChain2->GetEntries();
   for (Long64_t jentry=0; jentry<nentries2;jentry++) {
     if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries2<<" "<<setprecision(2)<<(double)jentry/nentries2*100<<endl;
     fChain2->GetEntry(jentry);
       newtree2->Fill();
   }
 
   newtree2->Print();
   newtree->AutoSave();
   newtree2->AutoSave();
//   delete newfile_data;


}
