void drawPt(char *inf){
TFile *a = new TFile(inf);
a->cd("ana");
TProfile *hPt = new TProfile("hPt","",25,0,200);
TCanvas *c = new TCanvas("c","",400,400);
//TCut nsdCut = "evtType!=92&&evtType!=93";  cout <<"Use PYTHIA 6 definition"<<endl;
   TCut nsdCut = "evtType!=103&&evtType!=104";  cout <<"Use PYTHIA 8! definition"<<endl;
//   TCut nsdCut = "evtType!=5&&evtType!=6";  cout <<"Use PHOJET definition"<<endl;

hi->Draw("Sum$(pt*(abs(eta)<2.4&&chg!=0&&abs(pdg)!=11&&abs(pdg)!=13))/Sum$(abs(eta)<2.4&&chg!=0&&abs(pdg)!=11&&abs(pdg)!=13):Sum$(abs(eta)<2.4&&chg!=0&&abs(pdg)!=11&&abs(pdg)!=13)>>hPt",nsdCut&&"Sum$(abs(eta)<2.4&&chg!=0&&abs(pdg)!=11&&abs(pdg)!=13)>0","");
double nevt = hi->GetEntries(nsdCut);
hPt->Sumw2();
//h->Scale(2./nevt);
hPt->SetXTitle("Charged Multiplicity with |#eta|<2.4");
hPt->SetYTitle("Average P_{T}");
hPt->Draw();
c->SaveAs(Form("%s-Pt-2.4.gif",inf));
c->SaveAs(Form("%s-Pt-2.4.C",inf));

}
