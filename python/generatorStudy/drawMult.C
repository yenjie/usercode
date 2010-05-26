void drawMult(char *inf){
TFile *a = new TFile(inf);
a->cd("ana");
TH1F *h = new TH1F("h","",100,0,200);
TCanvas *c = new TCanvas("c","",400,400);
c->SetLogy();
hi->Draw("Sum$(abs(eta)<2&&chg!=0&&abs(pdg)!=11&&abs(pdg)!=13)>>h","evtType!=92&&evtType!=93","");
double nevt = hi->GetEntries("evtType!=92&&evtType!=93");
h->Sumw2();
//h->Scale(2./nevt);
h->SetXTitle("Charged Multiplicity with |#eta|<2.0");
h->SetYTitle("Arbitrary unit");
h->Draw();
c->SaveAs(Form("%s.gif",inf));
}
