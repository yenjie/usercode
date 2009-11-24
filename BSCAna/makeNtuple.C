#include <TNtuple.h>
#include <TTree.h>
#include <TFile.h>
#include "Loop.C"

void makeNtuple(char *infile)
{
    TFile *inf = new TFile(infile);
    TTree *data = (TTree*) inf->FindObjectAny("data");
    Loop myLooper(data);
    myLooper.myLoop();
    
}