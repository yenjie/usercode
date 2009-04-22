{
   TFile a("pythia_mb_900GeV_vtxFlat_HitTree_d20081015_500k.root");
   a.cd("ana");
   TCanvas *c = new TCanvas("c","",500,500);
   TH2F *h = new TH2F("h","",1200,-3,3,1200,-3,3);
   h->SetXTitle("#phi (Layer 1)");
   h->SetYTitle("#eta (Layer 1)");
   PixelTree->Draw("eta1:phi1>>h","","col");

   TCanvas *c2 = new TCanvas("c2","",500,500);
   TH2F *h2 = new TH2F("h2","",1200,-3,3,1200,-3,3);
   h2->SetXTitle("Layer 2 #phi (Layer 2)");
   h2->SetYTitle("Layer 2 #eta (Layer 2)");
   PixelTree->Draw("eta2:phi2>>h2","","col");

}
