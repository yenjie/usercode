void draw(char *inf){
TFile *a = new TFile(inf);
a->cd("ana");
TH1F *h = new TH1F("h","",40,-10,10);
TH1F *h2= new TH1F("h2","",1000,0,10);
TCanvas *c = new TCanvas("c","",400,400);

//TCut nsdCut = "evtType!=92&&evtType!=93";
TCut nsdCut = "evtType!=103&&evtType!=104";

cout <<nsdCut<<endl;

hi->Draw("eta>>h",nsdCut&&"chg!=0&&abs(pdg)!=11&&abs(pdg)!=13");
hi->Draw("pt>>h2",nsdCut&&"chg!=0&&abs(pdg)!=11&&abs(pdg)!=13&&abs(eta)<2.4");


double nevt = hi->GetEntries(nsdCut);
h->Sumw2();
h->Scale(2./nevt);
h->SetXTitle("#eta");
h->SetYTitle("dN/d#eta");
h->Draw();
double dndeta = (h->GetBinContent(20) + h->GetBinContent(21))/2;
double dndetaError = 0.5 * sqrt(h->GetBinError(20)*h->GetBinError(20)+h->GetBinError(21)*h->GetBinError(21));

cout <<"<Pt> "<<h2->GetMean()<<" +- "<<h2->GetMeanError()<<endl;
cout <<"dN/deta "<<dndeta<<" +- "<<dndetaError<<endl;

}
