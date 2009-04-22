{
   TFile a("correction.root");
   TH1F *h = new TH1F("h","",100,-10,10);
   correction->Draw("(gen-obs)/err>>h");
   h->SetXTitle("(N_{gen}-N_{obs})/#sigma");
   h->Fit("gaus");
   TCanvas *c = new TCanvas("c","",400,400);   
   h->Draw();
}
