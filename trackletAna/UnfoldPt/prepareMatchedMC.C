#include "match.C"

// Process the MC file to produce matched MC ntuple

void prepareMatchedMC(char *infile)
{
   TFile *inf = new TFile(infile);
   TNtuple *nt = (TNtuple*)inf->FindObjectAny("TrackletTree12");
   match a(nt);
   a.Loop();
}
