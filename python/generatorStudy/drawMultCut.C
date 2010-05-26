void drawMultCut(char *inf){
TFile *a = new TFile(inf);
a->cd("ana");
TH1F *h = new TH1F("h","",50,0,50);
TCanvas *c = new TCanvas("c","",400,400);
c->SetLogy();
hi->Draw("Sum$(abs(eta)<1&&pt>0.4&&chg!=0&&abs(pdg)!=11&&abs(pdg)!=13)>>h","evtType!=92&&evtType!=93","",10000);
double nevt = hi->GetEntries("evtType!=92&&evtType!=93");
h->Sumw2();
h->Scale(1./nevt);
h->SetXTitle("Charged Multiplicity with |#eta|<1.0 and |P_{T}|>0.4GeV");
h->SetYTitle("Arbitrary unit");
h->Draw();
c->SaveAs(Form("%s.gif",inf));
}
