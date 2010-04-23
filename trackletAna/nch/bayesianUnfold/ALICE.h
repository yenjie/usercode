#include <TH1F.h>

TH1F* getALICE7TeV10()
{
   double x[65];
   
   for (int i=0;i<=59;i++)
   {
     x[i]=i+1;
   }
   x[60]=62;
   x[61]=64;
   x[62]=66;
   x[63]=68;
   x[64]=70;
   
   TH1F *h = new TH1F("h","",64,x);
   h->SetBinContent(1,0.06270);
   h->SetBinContent(2,0.07360);
   h->SetBinContent(3,0.07070);
   h->SetBinContent(4,0.06790);
   h->SetBinContent(5,0.06440);
   h->SetBinContent(6,0.05860);
   h->SetBinContent(7,0.05200);
   h->SetBinContent(8,0.04670);
   h->SetBinContent(9,0.04150);
   h->SetBinContent(10,0.03730);
   h->SetBinContent(11,0.03440);
   h->SetBinContent(12,0.03130);
   h->SetBinContent(13,0.02890);
   h->SetBinContent(14,0.02630);
   h->SetBinContent(15,0.02460);
   h->SetBinContent(16,0.02340);
   h->SetBinContent(17,0.02160);
   h->SetBinContent(18,0.01990);
   h->SetBinContent(19,0.01890);
   h->SetBinContent(20,0.01740);
   h->SetBinContent(21,0.01610);
   h->SetBinContent(22,0.01480);
   h->SetBinContent(23,0.01390);
   h->SetBinContent(24,0.01290);
   h->SetBinContent(25,0.01170);
   h->SetBinContent(26,0.01050);
   h->SetBinContent(27,0.00955);
   h->SetBinContent(28,0.00881);
   h->SetBinContent(29,0.00803);
   h->SetBinContent(30,0.00751);
   h->SetBinContent(31,0.00693);
   h->SetBinContent(32,0.00648);
   h->SetBinContent(33,0.00598);
   h->SetBinContent(34,0.00560);
   h->SetBinContent(35,0.00517);
   h->SetBinContent(36,0.00471);
   h->SetBinContent(37,0.00417);
   h->SetBinContent(38,0.00385);
   h->SetBinContent(39,0.00342);
   h->SetBinContent(40,0.00311);
   h->SetBinContent(41,0.00269);
   h->SetBinContent(42,0.00242);
   h->SetBinContent(43,0.00217);
   h->SetBinContent(44,0.00198);
   h->SetBinContent(45,0.00185);
   h->SetBinContent(46,0.00164);
   h->SetBinContent(47,0.00158);
   h->SetBinContent(48,0.00140);
   h->SetBinContent(49,0.00126);
   h->SetBinContent(50,0.00113);
   h->SetBinContent(51,0.00104);
   h->SetBinContent(52,0.00095);
   h->SetBinContent(53,0.00082);
   h->SetBinContent(54,0.00074);
   h->SetBinContent(55,0.00065);
   h->SetBinContent(56,0.00057);
   h->SetBinContent(57,0.00050);
   h->SetBinContent(58,0.00049);
   h->SetBinContent(59,0.00044);
   h->SetBinContent(60,0.00036);
   h->SetBinContent(61,0.00029);
   h->SetBinContent(62,0.00026);
   h->SetBinContent(63,0.00019);
   h->SetBinContent(64,0.00016);
   h->SetBinContent(65,0.00013);
  
  return h;
}


