#include <TH1F.h>

TH1F* getUA5900GeV05()
{
   double x[53];
   
   for (int i=0;i<=21;i++)
   {
     x[i]=i;
   }
   x[22]=23;
   x[23]=31;
   
   TH1F *h = new TH1F("h","",52,x);
   
   h->SetBinContent(1 ,0.15);
   h->SetBinContent(2 ,0.171);
   h->SetBinContent(3 ,0.183);
   h->SetBinContent(4 ,0.124);
   h->SetBinContent(5 ,0.099);
   h->SetBinContent(6 ,0.076);
   h->SetBinContent(7 ,0.057);
   h->SetBinContent(8 ,0.043);
   h->SetBinContent(9 ,0.032);
   h->SetBinContent(10 ,0.024);
   h->SetBinContent(11 ,0.018);
   h->SetBinContent(12 ,0.013);
   h->SetBinContent(13 ,0.010);
   h->SetBinContent(14 ,0.007);
   h->SetBinContent(15 ,0.005);
   h->SetBinContent(16 ,0.004);
   h->SetBinContent(17 ,0.0027);
   h->SetBinContent(18 ,0.0021);
   h->SetBinContent(19 ,0.0015);
   h->SetBinContent(20 ,0.0011);
   h->SetBinContent(21 ,0.0008);
   h->SetBinContent(22 ,0.0013/2.);
   h->SetBinContent(23 ,0.0009/7.);

   h->SetBinError(1 ,0.01);
   h->SetBinError(2 ,0.007);
   h->SetBinError(3 ,0.007);
   h->SetBinError(4 ,0.006);
   h->SetBinError(5 ,0.005);
   h->SetBinError(6 ,0.005);
   h->SetBinError(7 ,0.004);
   h->SetBinError(8 ,0.004);
   h->SetBinError(9 ,0.003);
   h->SetBinError(10 ,0.003);
   h->SetBinError(11 ,0.002);
   h->SetBinError(12 ,0.002);
   h->SetBinError(13 ,0.002);
   h->SetBinError(14 ,0.001);
   h->SetBinError(15 ,0.001);
   h->SetBinError(16 ,0.001);
   h->SetBinError(17 ,0.0009);
   h->SetBinError(18 ,0.0008);
   h->SetBinError(19 ,0.0007);
   h->SetBinError(20 ,0.0006);
   h->SetBinError(21 ,0.0005);
   h->SetBinError(22 ,0.0006/2.);
   h->SetBinError(23 ,0.0005/7.);

  return h;
}


TH1F* getUA5900GeV15()
{
   double x[53];
   
   for (int i=0;i<=47;i++)
   {
     x[i]=i;
   }
   x[48]=49;
   x[49]=51;
   x[50]=54;
   x[51]=59;
   x[52]=76;
   
   TH1F *h = new TH1F("h","",52,x);
   
   h->SetBinContent(1 ,0.033);
   h->SetBinContent(2 ,0.035);
   h->SetBinContent(3 ,0.048);
   h->SetBinContent(4 ,0.058);
   h->SetBinContent(5 ,0.065);
   h->SetBinContent(6 ,0.066);
   h->SetBinContent(7 ,0.063);
   h->SetBinContent(8 ,0.060);
   h->SetBinContent(9 ,0.056);
   h->SetBinContent(10 ,0.051);
   h->SetBinContent(11 ,0.046);
   h->SetBinContent(12 ,0.042);
   h->SetBinContent(13 ,0.038);
   h->SetBinContent(14 ,0.034);
   h->SetBinContent(15 ,0.031);
   h->SetBinContent(16 ,0.028);
   h->SetBinContent(17 ,0.025);
   h->SetBinContent(18 ,0.023);
   h->SetBinContent(19 ,0.021);
   h->SetBinContent(20 ,0.018);
   h->SetBinContent(21 ,0.017);
   h->SetBinContent(22 ,0.015);
   h->SetBinContent(23 ,0.014);
   h->SetBinContent(24 ,0.012);
   h->SetBinContent(25 ,0.011);
   h->SetBinContent(26 ,0.010);
   h->SetBinContent(27 ,0.009);
   h->SetBinContent(28 ,0.008);
   h->SetBinContent(29 ,0.007);
   h->SetBinContent(30 ,0.007);
   h->SetBinContent(31 ,0.006);
   h->SetBinContent(32 ,0.005);
   h->SetBinContent(33 ,0.005);
   h->SetBinContent(34 ,0.004);
   h->SetBinContent(35 ,0.004);
   h->SetBinContent(36 ,0.003);
   h->SetBinContent(37 ,0.0028);
   h->SetBinContent(38 ,0.0025);
   h->SetBinContent(39 ,0.0024);
   h->SetBinContent(40 ,0.0019);
   h->SetBinContent(41 ,0.0017);
   h->SetBinContent(42 ,0.0014);
   h->SetBinContent(43 ,0.0013);
   h->SetBinContent(44 ,0.0011);
   h->SetBinContent(45 ,0.0010);
   h->SetBinContent(46 ,0.0009);
   h->SetBinContent(47 ,0.0007);
   h->SetBinContent(48 ,0.0012/2.);
   h->SetBinContent(49 ,0.0008/2.);
   h->SetBinContent(50 ,0.0008/3.);
   h->SetBinContent(51 ,0.0008/5.);
   h->SetBinContent(52 ,0.0007/17.);

   h->SetBinError(1 ,0.006);
   h->SetBinError(2 ,0.005);
   h->SetBinError(3 ,0.004);
   h->SetBinError(4 ,0.004);
   h->SetBinError(5 ,0.004);
   h->SetBinError(6 ,0.004);
   h->SetBinError(7 ,0.004);
   h->SetBinError(8 ,0.004);
   h->SetBinError(9 ,0.004);
   h->SetBinError(10 ,0.004);
   h->SetBinError(11 ,0.004);
   h->SetBinError(12 ,0.003);
   h->SetBinError(13 ,0.003);
   h->SetBinError(14 ,0.003);
   h->SetBinError(15 ,0.003);
   h->SetBinError(16 ,0.003);
   h->SetBinError(17 ,0.003);
   h->SetBinError(18 ,0.003);
   h->SetBinError(19 ,0.003);
   h->SetBinError(20 ,0.002);
   h->SetBinError(21 ,0.002);
   h->SetBinError(22 ,0.002);
   h->SetBinError(23 ,0.002);
   h->SetBinError(24 ,0.002);
   h->SetBinError(25 ,0.002);
   h->SetBinError(26 ,0.002);
   h->SetBinError(27 ,0.002);
   h->SetBinError(28 ,0.001);
   h->SetBinError(29 ,0.001);
   h->SetBinError(30 ,0.001);
   h->SetBinError(31 ,0.001);
   h->SetBinError(32 ,0.001);
   h->SetBinError(33 ,0.001);
   h->SetBinError(34 ,0.001);
   h->SetBinError(35 ,0.001);
   h->SetBinError(36 ,0.001);
   h->SetBinError(37 ,0.0009);
   h->SetBinError(38 ,0.0008);
   h->SetBinError(39 ,0.0008);
   h->SetBinError(40 ,0.0007);
   h->SetBinError(41 ,0.0007);
   h->SetBinError(42 ,0.0006);
   h->SetBinError(43 ,0.0006);
   h->SetBinError(44 ,0.0006);
   h->SetBinError(45 ,0.0005);
   h->SetBinError(46 ,0.0005);
   h->SetBinError(47 ,0.0005);
   h->SetBinError(48 ,0.0006/2.);
   h->SetBinError(49 ,0.0005/2.);
   h->SetBinError(50 ,0.0005/3.);
   h->SetBinError(51 ,0.0005/5.);
   h->SetBinError(52 ,0.0006/17.);

  return h;
}
