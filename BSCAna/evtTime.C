#include <TFile.h>
#include <TTree.h>
#include <iostream.h>

void evtTime()
{
   TFile *inf = new TFile("output.root");
   TTree *data = (TTree*)inf->FindObjectAny("data");
   data->Draw("evtTime","");
   data->SetLineColor(2);
   data->Draw("evtTime","min[0]<-10&&min[1]<-10&&min[2]<-10&&min[3]<-10&&min[4]<-10&&min[5]<-10&&min[6]<-10&&min[7]<-10","same");
   cout <<data->GetEntries("min[0]<-10&&min[1]<-10&&min[2]<-10&&min[3]<-10&&min[4]<-10&&min[5]<-10&&min[6]<-10&&min[7]<-10")<<" / "<<data->GetEntries()<<endl;
}