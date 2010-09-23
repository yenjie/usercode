void compare(char *infname){
   TFile *inf1 = new TFile("1D_highmult.root");   
   TH1D *hWei0 = (TH1D*)inf1->FindObjectAny("correlation_phi");

   TFile *inf2 = new TFile(infname);   
   TH1D *h = (TH1D*)inf2->FindObjectAny("h1S");
   h->SetMarkerColor(2);
   h->SetLineColor(2);
   h->SetYTitle("R");
   //h->Scale(1.11);

   TH1D *hWei = (TH1D*)h->Clone();
   hWei->SetName("hWei");
   for (int i=0;i<=h->GetNbinsX();i++)
   {
      hWei->SetBinContent(i+1,hWei0->GetBinContent(i));
      hWei->SetBinError(i+1,hWei0->GetBinError(i));
   }
   hWei->SetMarkerColor(1);
   hWei->SetLineColor(1);
   TLegend *t = new TLegend(0.2,0.7,0.8,0.9);
   t->SetBorderSize(0);
   t->SetFillStyle(0);
   t->AddEntry(hWei,"Wei's raw result","pl");
   t->AddEntry(h,"Raw result","pl");

   TCanvas *c2 = new TCanvas("c2","",600,600);
   TH1D *hDiff =(TH1D*)h->Clone();
   hDiff->SetName("hDiff");
   hDiff->Add(hWei,-1);
   hDiff->SetYTitle("Yen-Jie - Wei");
   hDiff->Draw();
   TCanvas *c = new TCanvas("c","",600,600);
   h->SetXTitle("#Delta#phi");
   h->SetYTitle("R");
   h->Draw();
   hWei->Draw("same");
   t->Draw();
  
}
