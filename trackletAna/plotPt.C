{
   TFile a("10TeV-flatVtx-sample-150k-20081025.root")
   TFile a("900GeV-flatVtx-sample-500k-20081025.root")
   a.cd("ana");
   TCanvas *c = new TCanvas("c","",300,300);
   TH1F *h = new TH1F("h","",600,0,30);
   PixelTree->Draw("pt>>h","abs(pdg)==221||abs(pdg)=321||abs(pdg)==2212||abs(pdg)==3122");
   int nevent = PixelTree->GetEntries("mult") ;
   h->Scale(1./nevent/30.*600.);
   h->SetXTitle("900GeV Charged Hadron Pt (GeV/c)");
   h->SetYTitle("dN/dPt per event");
   h->Draw();
   
}
