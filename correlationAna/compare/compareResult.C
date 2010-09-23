void compareResult(char *infname, char *infname2, char *title1="Baseline", char *title2="Test"){
   TFile *inf1 = new TFile(infname);   
   TH1D *h1 = (TH1D*)inf1->FindObjectAny("h1S");
   TFile *inf2 = new TFile(infname2);   
   TH1D *h2 = (TH1D*)inf2->FindObjectAny("hProj");
   h2->SetLineColor(2);
   h2->SetMarkerColor(2);
   TLegend *l = new TLegend(0.2,0.7,0.7,0.9);
   l->SetBorderSize(0);
   l->SetFillStyle(0);
   l->AddEntry(h1,title1,"pl");
   l->AddEntry(h2,title2,"pl");
   l->Draw();

   TCanvas *c2 = new TCanvas("c2","",600,600);
   h1->SetXTitle("#Delta#phi");
   h1->SetYTitle("R");
   h1->Draw();
   h2->Draw("same");
   l->Draw();
  
}
