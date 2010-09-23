#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <iostream>
#include <TGraph.h>
#include <TF1.h>


Double_t poissonFunc(Double_t meanN,Double_t k, Double_t n)
{
    Double_t a = exp(-meanN)*pow(meanN,n*k);
    Double_t b = TMath::Gamma(n*k+1);
    return a/b;
}

Double_t nbdFunc(Double_t meanN, Double_t k, Double_t n)
{
    Double_t a = TMath::Gamma(n+k)/TMath::Gamma(n+1)/TMath::Gamma(k);
    Double_t b = pow((meanN/k)/(1+(meanN/k)),n);
    Double_t c = 1./pow((1+meanN/k),k);
    return a*b*c;
}
Double_t nbdz(Double_t *x, Double_t *par)
{
    Double_t norm = par[0];
    Double_t meanN = par[1];
    Double_t k = par[2];
    Double_t n = x[0]*meanN;
    Double_t res = nbdFunc(meanN,k,n);
    return norm*res;
}

/*
Double_t doublenbdz(Double_t *x, Double_t *par)
{
    Double_t norm = par[0];
    Double_t norm2 = par[1];
    Double_t meanN = par[2];
    Double_t meanN2 = par[3];
    Double_t k = par[4];
    Double_t n = x[0]*meanN;
    Double_t res = nbdFunc(meanN,k,n);
    Double_t res2 = nbdFunc(meanN2,k,n);
    return norm*res+norm2*res2;
}


Double_t doublenbd(Double_t *x, Double_t *par)
{
    Double_t norm = par[0];
    Double_t norm2 = par[1];
    Double_t meanN = par[2];
    Double_t meanN2 = par[3];
    Double_t k = par[4];
    Double_t n = x[0];
    Double_t res = nbdFunc(meanN,k,n);
    Double_t res2 = nbdFunc(meanN2,k,n);
    return norm*res+norm2*res2;
}
*/


Double_t nbd(Double_t *x, Double_t *par)
{
    Double_t norm = par[0];
    Double_t meanN = par[1];
    Double_t k = par[2];
    Double_t n = x[0];
    Double_t res = nbdFunc(meanN,k,n);
    return norm*res;
}

Double_t twonbd(Double_t *x, Double_t *par)
{
    Double_t norm = par[0];
    Double_t meanN = par[1];
    Double_t k = par[2];
    Double_t n = x[0];
    Double_t res = nbdFunc(meanN,k,n);
    Double_t norm2 = par[3];
    Double_t meanN2 = par[4];
    Double_t k2 = par[5];
    Double_t n2 = x[0];
    Double_t res2 = nbdFunc(meanN2,k2,n2);
    return norm*res+norm2*res2;
}


Double_t poisson(Double_t *x, Double_t *par)
{
    Double_t norm = par[0];
    Double_t meanN = par[1];
    Double_t k = par[2];
    Double_t n = x[0];
    Double_t res = poissonFunc(meanN,k,n);
    return norm*res;
}


Float_t getScaleFactor(Float_t scaleTo, Float_t energy)
{
   return log(scaleTo)/log(energy)*log(scaleTo)/log(energy);
     Float_t a = 2.716 - 0.307*2*log(scaleTo)+0.0267*log(scaleTo)*log(scaleTo)*4;
     Float_t b = 2.716 - 0.307*2*log(energy)+0.0267*log(energy)*log(energy)*4;


//     Float_t a = 1.54 - 0.096*2*log(scaleTo)+0.0155*log(scaleTo)*log(scaleTo)*4;
//     Float_t b = 1.54 - 0.096*2*log(energy)+0.0155*log(energy)*log(energy)*4;
     return a/b;
}

Float_t getY0(Float_t energy)
{
   double p = sqrt(energy*energy-0.938*0.938);
   double y = 0.5 * log((energy+p)/(energy-p));
   cout <<y<<endl;
   return y;
} 

Float_t getMean(int nData, Float_t *data,Float_t *dataeta)
{
   double sumCh=0;
   double sum=0;
   double max=0;
   double maxCh=0;

   for(Int_t i=0;i<nData;i++)
   {
     if (data[i]>max) {
        max=data[i];
	maxCh=dataeta[i];
     }
     sumCh+=data[i]*dataeta[i];
     sum+=data[i];
   }
//   return maxCh;
   return sumCh/sum;

   TGraph *tmp = new TGraph(nData,dataeta,data);
   TF1 *f = new TF1("f",nbd,0,dataeta[nData]+10,3);
   f->SetParameters(1,sumCh/sum,3);
   f->FixParameter(1,sumCh/sum);
   tmp->Fit("f","LL","",0.1,sumCh/sum*1.);
   tmp->Fit("f","LL","",0.1,sumCh/sum*1.);
   f->ReleaseParameter(1);
   tmp->Fit("f","LL","",0.1,sumCh/sum*1.);
   tmp->Fit("f","LL","",0.1,sumCh/sum*1.);
   
   double result=f->GetMaximumX(0,sumCh/sum*2);
   delete f;
   delete tmp;
   cout <<"Result : "<<sumCh/sum<<" "<<result<<endl;
   return result;

}

void normalizeTheSpectrum(int nData,Float_t *data, Float_t *dataeta,Float_t *dataErrP,Float_t *dataErrN,Float_t sDNDeta,Float_t sEta)
{
   double sumCh=0;
   double sum=0;
   double max=0;
   double maxCh=0;
   for(Int_t i=0;i<nData;i++)
   {
     if (data[i]>max) {
        max=data[i];
	maxCh=dataeta[i];
     }
     sumCh+=data[i]*dataeta[i];
     sum+=data[i];
   }
   double mean = sumCh/sum;

   for(Int_t i=0;i<nData;i++)
   {
     data[i] = data[i]/sum;
     dataErrP[i] = dataErrP[i]/sum;
     dataErrN[i] = dataErrN[i]/sum;
     dataeta[i] = dataeta[i];
   }
}

void scaleSpectrum(int nData,Float_t *data, Float_t *dataeta,Float_t *dataErrP,Float_t *dataErrN,Float_t scale)
{
   for(Int_t i=0;i<nData;i++)
   {
     data[i] = data[i]*scale;
     dataErrP[i] = dataErrP[i]*scale;
     dataErrN[i] = dataErrN[i]*scale;
     dataeta[i] = dataeta[i];
//     if ((dataErrP[i]/data[i])>0.05) data[i]=1000;
   }
}

void scaleTheSpectrum(int nData,Float_t *data, Float_t *dataeta,Float_t *dataErrP,Float_t *dataErrN,Float_t sDNDeta,Float_t sEta)
{
   double sumCh=0;
   double sum=0;
   double max=0;
   double maxCh=0;
   for(Int_t i=0;i<nData;i++)
   {
     if (data[i]>max) {
        max=data[i];
	maxCh=dataeta[i];
     }
     sumCh+=data[i]*dataeta[i];
     sum+=data[i];
   }
   double mean = sumCh/sum;
   cout <<sum<<endl;
   cout <<mean<<endl;
   double scale = sum/mean;
   for(Int_t i=0;i<nData;i++)
   {// scale = dataeta[i];
     data[i] = data[i]/scale;
     dataErrP[i] = dataErrP[i]/scale;
     dataErrN[i] = dataErrN[i]/scale;
     dataeta[i] = sEta*dataeta[i];
//     if ((dataErrP[i]/data[i])>0.05) data[i]=1000;
   }
}

TGraphAsymmErrors *makeGraph(int nData, Float_t *data, Float_t *dataeta,Float_t *dataErrP,Float_t *dataErrN, Float_t moveIt = 0, bool symmetrize = 1 )
{
   TGraphAsymmErrors *graph;
   int nData2=0;
   for (int i=0;i<nData;i++)
   {
      if (dataeta[i]<0) symmetrize = 0;
      if (dataeta[i]<=0) nData2++;
   }
   if (symmetrize) {
      if (moveIt==0) graph = new TGraphAsymmErrors(2*nData); else graph = new TGraphAsymmErrors(nData);
      for(Int_t i=0;i<nData;i++) 
      {
        graph->SetPoint(i,-1*dataeta[nData-i-1]+moveIt,data[nData-i-1]);
        graph->SetPointError(i,0,0,fabs(dataErrN[nData-i-1]),fabs(dataErrP[nData-i-1]));  
	
	if (moveIt==0) {
   	   graph->SetPoint(nData+i,dataeta[i]+moveIt,data[i]);
	   graph->SetPointError(nData+i,0,0,fabs(dataErrN[i]),fabs(dataErrP[i]));  
        }
      }
   } else {
      graph = new TGraphAsymmErrors(nData2);
      for(Int_t i=0;i<nData;i++) 
      {
        if (moveIt!=0&&dataeta[i]>0) continue;
	graph->SetPoint(i,dataeta[i]+moveIt,data[i]);
	graph->SetPointError(i,0,0,fabs(dataErrN[i]),fabs(dataErrP[i]));  
      }
   } 
   graph->SetMarkerSize(1);
   graph->SetLineStyle(3);
   return graph; 
}
