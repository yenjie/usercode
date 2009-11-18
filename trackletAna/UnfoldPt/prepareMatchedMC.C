#include "match.C"

// Process the MC file to produce matched MC ntuple

void prepareMatchEDMC(char *infile)
{
   TFile *inf = new TFile(infile);
   TNtuple *nt = (TNtuple*)inf->FindObjectAny("TrackletTree13");
   match a(nt);
   a.Loop();
}
