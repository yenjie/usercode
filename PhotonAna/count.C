#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

void count(char *infname)
{
   TFile *inf = new TFile(infname);
   TH1F *h = (TH1F*)inf->FindObjectAny("MET");
   cout <<infname<<" # "<<h->GetEntries()<<endl;
   
}
