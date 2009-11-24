#define Loop_cxx
#include "Loop.h"
#include "adcCorrectedDataFormat.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
void Loop::myLoop()
{
  // Correction factors:
  double cA[8] = {0.8567,0.8567,0.8700,0.8610,0.8567,0.8550,0.8630,0.8567};
  double cOff[8] = {0.034,0.049,0.049,0.037,0.045,0.049,0.057,0.057};
  
  // Dummy correction factors:
  // double cA[8] = {1,1,1,1,1,1,1,1};
  // double cOff[8] = {0,0,0,0,0,0,0,0};


  TFile *outfile = new TFile("output.root","recreate"); 

  TTree *nt = new TTree("data","ADC readout data");  

  adcCorrectedData myData;
  myData.nch=8;
  nt->Branch("nch",&myData.nch,"nch/I");
  nt->Branch("run",&myData.run,"run/I");
  nt->Branch("evt",&myData.evt,"evt/I");
  nt->Branch("evtSecond",&myData.evtSecond,"evtSecond/I");
  nt->Branch("evtTime",&myData.evtTime,"evtTime/I");
  nt->Branch("pedestal",myData.pedestal,"nch/I");
  nt->SetMarkerStyle(20);

  for (int i=0;i<8;i++){
    nt->Branch(Form("dSize%d",i),&myData.dataSize[i],Form("dSize%d/I",i));
    nt->Branch(Form("data%d",i),myData.data[i],Form("data%d[dSize%d]/F",i,i));
    nt->Branch(Form("time%d",i),myData.time[i],Form("time%d[dSize%d]/F",i,i)); 
  }

  nt->Branch("min",myData.min,"min[8]/F");
  nt->Branch("mintime",myData.mintime,"mintime[8]/F");
  nt->Branch("mintime08",myData.mintime08,"mintime08[8]/F");
  nt->Branch("mintime02",myData.mintime02,"mintime02[8]/F");
  nt->Branch("mintime04",myData.mintime04,"mintime04[8]/F");
  nt->Branch("mintime0802",myData.mintime0802,"mintime0802[8]/F");
  nt->Branch("mintime0804",myData.mintime0804,"mintime0804[8]/F");
  //nt->Branch("minSB",myData.minSB,"minSB[8]/F");
  //nt->Branch("minSBtime",myData.minSBtime,"minSBtime[8]/F");

  char ch;
  unsigned long dSize, dummy, trgTime;
  unsigned long nbr=0, evt=0;

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      if (jentry % 1000 == 0 ) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<"% \r";

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      Float_t min[8];
      Float_t mintime[8];
      Float_t minSB[8];
      Float_t minSBtime[8];
       
      for (int i=0;i<8;i++) {
         myData.min[i]=10e-10;
//         myData.minSB[i]=10e-10;
         myData.mintime[i]=-100;
//         myData.minSBtime[i]=-100;
         myData.dataSize[i]=0;  
      }  

      nbr=0;
      int flag=0;
      int minIt[8];
    
      for (int i=0;i<8;i++) {
         myData.dataSize[i] = 0;
	 myData.pedestal[i] = pedestal[i];
         minIt[i]=-1;
	 
         for (int j=0;j<data[i].size();j++)
         {
            double pulseHeight = data[i][j]-pedestal[i]; //(((double)data[i][j])/255.-0.5 - cOff[i])/cA[i];
            double mytime = time[i][j];

	    myData.data[i][myData.dataSize[i]] = pulseHeight;
	    myData.time[i][myData.dataSize[i]] = mytime;
	    myData.dataSize[i]++;

  	    // fill signal region
  
            if (pulseHeight<myData.min[i]) { 
   	           myData.min[i]=pulseHeight;
	           myData.mintime[i]=mytime;
		   minIt[i]=myData.dataSize[i]-1;
	    }

            /*
            // fill sideband region
            if (pulseHeight<myData.minSB[i]) { 
               if (i!=0) {   
	          if (fabs(mytime-myData.minSBtime[0])>400&&fabs(mytime-myData.minSBtime[0])<500){ 
   	             myData.minSB[i]=pulseHeight;
  	             myData.minSBtime[i]=mytime;
	          }
	       } else {
   	          // channel 0
   	          myData.minSB[i]=pulseHeight;
	          myData.minSBtime[i]=mytime;
	       }
  	    }
	    */

         }
         if (myData.min[i]<-1) {flag+=1;}
	 
//	 cout <<endl<<"minIt = "<<minIt[i]<<" "<<myData.dataSize[i]<<endl;
         int flag08=0;
         int flag04=0;
         int flag02=0;
	 myData.mintime08[i]=-1;
	 myData.mintime04[i]=-1;
	 myData.mintime02[i]=-1;
	 myData.mintime0802[i]=-1;
	 myData.mintime0804[i]=-1;
	 
	 for (int j=minIt[i];j>=1;j--){
	    if (myData.data[i][j]<0.8*myData.min[i]&&myData.data[i][j-1]>0.8*myData.min[i]){
	       
	       if ((myData.time[i][j]-myData.time[i][j-1])==2) {
  	          myData.mintime08[i]=myData.time[i][j-1]+2*(0.8*myData.min[i]-myData.data[i][j-1])/(myData.data[i][j]-myData.data[i][j-1]);
   	          flag08=1;
	       } else {
	          myData.mintime08[i]=myData.time[i][j]-2+2*(0.8*myData.min[i])/(myData.data[i][j]);
	       }
	    }
	    if (myData.data[i][j]<0.2*myData.min[i]&&myData.data[i][j-1]>0.2*myData.min[i]){
	       if ((myData.time[i][j]-myData.time[i][j-1])==2) {
	          myData.mintime02[i]=myData.time[i][j-1]+2*(0.2*myData.min[i]-myData.data[i][j-1])/(myData.data[i][j]-myData.data[i][j-1]);
	          flag02=1;
	       } else {
       	          myData.mintime02[i]=myData.time[i][j]-2+2*(0.2*myData.min[i])/(myData.data[i][j]);
               }
	    }
	    if (myData.data[i][j]<0.4*myData.min[i]&&myData.data[i][j-1]>0.4*myData.min[i]){
	       if ((myData.time[i][j]-myData.time[i][j-1])==2) {
	          myData.mintime04[i]=myData.time[i][j-1]+2*(0.4*myData.min[i]-myData.data[i][j-1])/(myData.data[i][j]-myData.data[i][j-1]);
	          flag04=1;
	       } else {
       	          myData.mintime04[i]=myData.time[i][j]-2+2*(0.2*myData.min[i])/(myData.data[i][j]);
               }
	    }
	 } 
	 
	 if (flag08==0&&minIt[i]!=-1) {
//	    myData.mintime08[i]=myData.time[i][0]-2+2*(1-0.8)*myData.min[i]/(myData.data[i][0]);
	 }

	 if (flag02==0&&minIt[i]!=-1) {
	    myData.mintime02[i]=myData.time[i][0]-2+2*(0.2)*myData.min[i]/(myData.data[i][0]);
	 }
	 
         if (flag08!=0&&flag04!=0) {
	    myData.mintime0804[i]=myData.mintime04[i]-0.4*(myData.mintime08[i]-myData.mintime04[i])/0.4;
	 }

         if (flag08!=0&&flag02!=0) {
	    myData.mintime0802[i]=myData.mintime02[i]-0.2*(myData.mintime08[i]-myData.mintime02[i])/0.6;
	 }
	 
      }


      myData.evt = evt;
      myData.evtSecond = evtSecond;
      myData.evtTime = evtTime;
      
      if (flag>=1) nt->Fill();    
  }
  outfile->Write();
  outfile->Close();

}
