#include <iostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

void testPhi()
{
   TFile *inf = new TFile("../sample/10TeV/pixel-1.root");
   TTree * tree = (TTree*)inf->FindObjectAny("PixelTree");
   TH1F *h1 = new TH1F("h1","",100000,-3.2,3.2);
   TH1F *h2 = new TH1F("h2","",100000,-3.2,3.2);
   TH1F *hdphi1 = new TH1F("hdphi","",10000,-3.2,3.2);
   TH1F *hdphiMC = new TH1F("hdphiMC","",10000,-3.2,3.2);

   tree->Draw("phi1>>h1","abs(vz[0])<10");
   tree->Draw("phi2>>h2","abs(vz[0])<10");
//   tree->Draw("dphi>>h3","abs(vz[3])<10");
   
   for (long i=0; i<1000000;i++)
   {
      if (i % 100000 == 0 ) cout <<i<<endl;
      double phi1 = h1->GetRandom();
      double phi2 = h2->GetRandom();
      hdphiMC->Fill(phi1-phi2);
   }   
   hdphiMC->Draw();
}
