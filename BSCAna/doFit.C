#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <iomanip>
#include <iostream>
#include <fstream>

double myfun(double *x,double *par)
{  
   double xx = x[0];
   return (TMath::Gaus(xx,par[1],par[2])*par[0])+fabs(par[3]);
   
}

double* doFitDeltaT(char *infname = "test.root",int ch1=0,int ch2=1)
{
   TFile *inf = new TFile(infname);
   TCanvas *c = new TCanvas("c","",400,400);
   
   TTree *data = (TTree*) inf->FindObjectAny("data");
  
   int nbin = 20;
   TH1F *h = new TH1F("h","",nbin,-nbin-1,nbin-1); 

   char *cut = "min[0]<-10&&min[1]<-10&&min[2]<-10&&min[3]<-10&&min[4]<-10&&min[5]<-10&&min[6]<-10&&min[7]<-10";
   data->Draw(Form("mintime[%d]-mintime[%d]>>h",ch1,ch2),Form("min[%d]>-pedestal[%d]+1&&min[%d]>-pedestal[%d]+1&&%s",ch1,ch1,ch2,ch2,cut));
   cout <<Form("min[%d]<-100&&min[%d]<-100",ch1,ch2);
   
   TF1 *fun = new TF1("fun",myfun,-100,100,4);
   fun->SetParameters(h->GetEntries()/h->GetRMS(),h->GetMean(),h->GetRMS(),0.1);
   h->Fit("fun"," M");
   h->Fit("fun","LL M");
   h->Fit("fun","LL M");
   h->Fit("fun","LL M");
   h->SetMarkerColor(2);
   h->SetLineColor(2);
   h->SetXTitle(Form("T_{%d}-T_{%d} (ns)",ch1,ch2));
   h->Draw("e");
   
   double *result = new double[4];
   result[0]=fun->GetParameter(1);
   result[1]=fun->GetParError(1);
   result[2]=fabs(fun->GetParameter(2));
   result[3]=fabs(fun->GetParError(2));
   c->SaveAs(Form("fitPlot/deltaT-%d-%d.gif",ch1,ch2));

   inf->Close();
   return result;   
}


void doFit(char *infname = "test.root")
{
   ofstream of;
   of.open("fitPlot/deltaT.txt");
   ofstream ofw;
   ofw.open("fitPlot/deltaTWidth.txt");
   
   for (int i=0;i<8;i++)
   {
      of <<" | "<<"ch"<<i;
      for (int j=0;j<8;j++)
      {
        if (i!=j) {
   	   double *result = doFitDeltaT(infname,i,j);
           of <<" | "<<setw(6)<<fixed<<setprecision(2)<<result[0]<<"+-";
	   of        <<setw(6)<<fixed<<setprecision(2)<<result[1];
           ofw <<" | "<<setw(6)<<fixed<<setprecision(2)<<result[2]<<"+-";
	   ofw        <<setw(6)<<fixed<<setprecision(2)<<result[3];
	   delete result;
	} else {
	   of <<" | "<<setw(6)<<fixed<<0<<" +- "<<setw(6)<<0;
	   ofw <<" | "<<setw(6)<<fixed<<0<<" +- "<<setw(6)<<0;
	}
      }
      of <<" | "<<endl;
      ofw <<" | "<<endl;
   }
   of.close();
   ofw.close();

}

