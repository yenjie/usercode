{
   // iso 5 GeV
   double bins[7]={20,25,30,40,50,80,140};
   TH1D *h = new TH1D("h","",6,bins);
   h->SetBinContent(1,2099.6);
   h->SetBinContent(2,853.508);
   h->SetBinContent(3,306.346);
   h->SetBinContent(4,91.0847);
   h->SetBinContent(5,19.6313);
   h->SetBinContent(6,1.57827);
   h->SetXTitle("Photon p_{T} GeV/c");
   h->SetYTitle("dN/d#p_{T} (nb)");
   
   double cross = 7.745;  //b
   double lumi = 7.0e6;   //b
   double ppCross = 0.064;   //b
   double ncoll = 93.1;
   double centFrac = 0.7;
   h->Scale(lumi*cross/ppCross*ncoll*centFrac*1e-12);
   h->Draw();

   
}