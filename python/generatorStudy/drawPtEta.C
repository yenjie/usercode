void drawPtEta(char *inf){
TFile a(inf);
a.cd("ana");
TProfile *h = new TProfile("h","",100,-3,3);
TCanvas *c = new TCanvas("c","",400,400);
hi->Draw("(pt):eta>>h","evtType!=92&&evtType!=93&&(abs(eta)<2.4&&chg!=0&&abs(pdg)!=11&&abs(pdg)!=13)","prof",10000);
double nevt = hi->GetEntries("evtType!=92&&evtType!=93");
h->Sumw2();
//h->Scale(2./nevt);
h->SetXTitle("#eta");
h->SetYTitle("Average P_{T}");
h->Draw();
c->SaveAs(Form("%s-PtEta-2.4.gif",inf));
c->SaveAs(Form("%s-PtEta-2.4.C",inf));
}
