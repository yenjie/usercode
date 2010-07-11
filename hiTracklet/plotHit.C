#include <TFile.h>
#include <TH2.h>
#include <TTree.h>
#include <TCanvas.h>

void plotHit(char *infname)
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->FindObjectAny("PixelTree");
   
   TH2D *h1 = new TH2D("h1","",140,-2.8,2.8,160,-3.2,3.2);
   TCanvas *c1 = new TCanvas("c1","",600,600);
   t->Draw("phi1:eta1>>h1","","col");
   h1->SetXTitle("#eta (Layer1)");
   h1->SetYTitle("#phi ");

   TH2D *h2 = new TH2D("h2","",140,-2.8,2.8,160,-3.2,3.2);
   TCanvas *c2 = new TCanvas("c2","",600,600);
   t->Draw("phi2:eta2>>h2","","col");
   h2->SetXTitle("#eta (Layer2)");
   h2->SetYTitle("#phi ");

   TH2D *h3 = new TH2D("h3","",140,-2.8,2.8,160,-3.2,3.2);
   TCanvas *c3 = new TCanvas("c3","",600,600);
   t->Draw("phi3:eta3>>h3","","col");
   h3->SetXTitle("#eta (Layer3)");
   h3->SetYTitle("#phi ");

}
