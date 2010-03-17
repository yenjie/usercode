#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>

TH1F* getPlot(TH1F *hTemplate, char *var, char *cut, char *filename, char* treeName,double scale)
{
   
   TFile *inf = new TFile(filename);
   TTree *t = (TTree*)inf->FindObjectAny(treeName);
   TH1F * tmp = (TH1F*)hTemplate->Clone();
   tmp->SetName(Form("tmp%s",filename));
   t->Draw(Form("%s>>tmp%s",var,filename),cut);
   tmp->Sumw2();
   cout <<scale<<endl;
   tmp->Scale(scale);
   cout <<tmp->GetEntries()<<endl;
   //inf->Close();
      
   return tmp;
}

void makePlot()
{

   char *var = "ecalIso";
   char *cut = "et>15";
   
   FILE *fTable = fopen("table","r");
   
   int flag=1;
   TH1F *hTemplate = new TH1F("hTemplate","",100,0,100);
   TH2F *hh = new TH2F("h","",100,0,1000,100,0,10e10);
   TH1F *h = (TH1F*) hTemplate->Clone();
   
   TH1F *hs[50];
   h->SetName("h");
   hh->Draw();
   int nfile=0;
   while (flag!=-1){
      char filename[100];
      flag=fscanf(fTable,"%s",filename);
      char tmp[100];
      
      flag=fscanf(fTable,"%s",tmp);
      double cross=atof(tmp);
      
      flag=fscanf(fTable,"%s",tmp);
      double nevt=atof(tmp);
      cout <<flag<<endl;
      if (flag!=-1) {
         cout <<filename<<" "<<cross<<" "<<nevt<<endl;
      
         hs[nfile] = getPlot(h,var,cut,filename,"Analysis",cross/nevt);
         hs[nfile]->SetName(Form("h%d",nfile));
         h->Add(hs[nfile]);
         nfile++; 
      }	 
	
   }
   
   h->Draw();
      
   for (int i=0;i<nfile;i++)
   {
      hs[i]->SetMarkerColor(i%6+2);
      if (i>=6) hs[i]->SetMarkerStyle(4);
      if (i>=12) hs[i]->SetMarkerStyle(21);
      hs[i]->Draw("same");
   }
//   h->Draw("same");
}
