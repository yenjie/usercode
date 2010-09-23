void compare1(char *infname){
   TFile *inf2 = new TFile(infname);   
   TH1D *h = (TH1D*)inf2->FindObjectAny("h1S");
   TH1D *h2 = (TH1D*)inf2->FindObjectAny("hProj");
   h2->SetYTitle("R");

   TLegend *l = new TLegend(0.3,0.6,0.6,0.9);
   l->SetBorderSize(0);
   l->SetFillStyle(0);
   l->AddEntry(h,"<(N-1)(#frac{S_{N}}{B_{N}}-1)>","pl");
   l->AddEntry(h2,"<(N-1)><(#frac{S_{N}}{B_{N}}-1)>","pl");
   l->Draw();

   TCanvas *c2 = new TCanvas("c2","",600,600);
   h2->Draw();
   h->Draw("same");
   l->Draw();
  
}
