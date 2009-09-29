#include <TH1.h>
#include <TTree.h>
#include <iostream.h>
#include <TLegend.h>

class compareHist
{
   public:
       compareHist(TTree *myTree,char *var,char *cut1,char *cut2, float low=0,float high=0,int nbin=100);
       compareHist(TTree *myTree1,TTree *myTree2,char *var,char *cut1,double low=0,double high=0,int nbin=100);

       TLegend * leg1;

       TH1F *h1;
       TH1F *h2;

       char *name1;
       char *name2;
       
       void init();
       void SetXTitle(char *t) { h1->SetXTitle(t); h2->SetXTitle(t);}
       void SetYTitle(char *t) { h1->SetYTitle(t); h2->SetYTitle(t);}
       void SetHistName1(char *t) { name1 = t; };
       void SetHistName2(char *t) { name2 = t; };
       void SetMarkerSize(double t) { h1->SetMarkerSize(t); h2->SetMarkerSize(t);}
       void Draw() { h1->Draw(); h2->Draw("same"); if(leg1) leg1->Draw();}
       void Draw2() { h1->Draw(); h2->Draw("same"); if(leg1) leg1->Draw();}
       void SetLegend(double x1,double y1, double x2, double y2);
};

compareHist::compareHist(TTree *myTree,char *var,char *cut1,char *cut2, float low,float high,int nbin)
{
   if (low==high) {
       low = myTree->GetMinimum(var);
       high = myTree->GetMaximum(var);
       
   }
   
   h1 = new TH1F(Form("h%s",var),"",nbin,low,high);
   h2 = new TH1F(Form("h2%s",var),"",nbin,low,high);
   myTree->Draw(Form("%s>>h%s",var,var),cut1);
   myTree->Draw(Form("%s>>h2%s",var,var),cut2);

   h1->SetLineColor(1);
   h2->SetLineColor(2);
   h1->SetMarkerColor(1);
   h2->SetMarkerColor(2);
   
   h1->Sumw2();
   h2->Sumw2();
   
   h1->Scale(1./h1->GetEntries());
   h2->Scale(1./h2->GetEntries());
}

compareHist::compareHist(TTree *myTree1,TTree *myTree2,char *var,char *cut1,double low,double high,int nbin)
{
   if (low==high) {
       low = myTree1->GetMinimum(var);
       if (low>myTree2->GetMinimum(var)) low = myTree2->GetMinimum(var);
       high = myTree2->GetMaximum(var);
       if (high<myTree2->GetMinimum(var)) high = myTree2->GetMinimum(var);
       cout <<low<<" "<<high<<endl;
       
   }

   h1 = new TH1F(Form("h%s",var),"",nbin,low,high);
   h2 = new TH1F(Form("h2%s",var),"",nbin,low,high);
   myTree1->Draw(Form("%s>>h%s",var,var),cut1);
   myTree2->Draw(Form("%s>>h2%s",var,var),cut1);

   h1->SetLineColor(1);
   h2->SetLineColor(2);
   h1->SetMarkerColor(1);
   h2->SetMarkerColor(2);
   
   h1->Sumw2();
   h2->Sumw2();
   
   h1->Scale(1./h1->GetEntries());
   h2->Scale(1./h2->GetEntries());
}


void compareHist::init()
{
   leg1=0;
   name1=0;
   name2=0;
}


void compareHist::SetLegend(double x1,double y1, double x2, double y2)
{
   leg1 = new TLegend(x1,y1,x2,y2);
   leg1->SetFillStyle(0);  
   leg1->SetFillColor(0); 
   leg1->SetBorderSize(0);
   if (name1)  leg1->AddEntry(h1,name1,"pl");
   if (name2)  leg1->AddEntry(h2,name2,"pl");
   
}
