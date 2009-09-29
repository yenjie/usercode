#include "compareHist.h"
#include <TFile.h>
#include <TCanvas.h>

void compare(char *var,char *cut="",char *inf1="herwig.root",char *inf2="pythia.root")
{
   TFile *infile1 = new TFile(inf1,"");
   TTree *tree1 = (TTree*)infile1->FindObjectAny("PixelTree");
   
   TFile *infile2 = new TFile(inf2,"");
   TTree *tree2 = (TTree*)infile2->FindObjectAny("PixelTree");
   
   TCanvas *c = new TCanvas("c","",600,600);
   compareHist comp(tree1,tree2,var,cut,0,10);
   comp.SetHistName1("HERWIG 7TeV");
   comp.SetHistName2("PYTHIA 7TeV");
   comp.SetXTitle("P_{T} (GeV)");
   comp.SetYTitle("Arbitrary normalization");
   comp.SetMarkerSize(0.7);
   comp.SetLegend(0.5,0.7,0.9,0.9);
   comp.Draw();
   
}
