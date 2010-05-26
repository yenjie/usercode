void drawPtSpectrum(char *inf){
TFile *a = new TFile(inf);
a->cd("ana");
TH1F *h = new TH1F("h","",200,0,4);
TCanvas *c = new TCanvas("c","",600,600);
hi->Draw("pt>>h","evtType!=92&&evtType!=93&&abs(eta)<2.4","",10000);
double nevt = hi->GetEntries("evtType!=92&&evtType!=93");
h->Sumw2();
h->Scale(2./nevt);
h->SetYTitle("# of particle / Event");
h->SetXTitle("P_{T} (GeV/c)");
h->Draw();
c->SaveAs(Form("%s-PtSpectrum-2.4.gif",inf));
c->SaveAs(Form("%s-PtSpectrum-2.4.C",inf));
}
