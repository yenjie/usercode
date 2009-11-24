{
   makeNtuple("../Run-10001.root");
   TFile a("output.root");
   data->SetMarkerStyle(20);
   data->Draw("(data7-256)/256/0.86:time7","","",2,3);
}
