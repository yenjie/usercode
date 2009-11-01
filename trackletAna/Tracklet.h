#include <vector>
#include <iostream>

#include "RecoHit.h"
using namespace std;

#define PI 3.141592653589

class Tracklet
{
   public:
      Tracklet(double eta1,double eta2,double phi1,double phi2,double r1,double
      r2);
      ~Tracklet(){};

      double eta1() { return eta1_; }
      double eta2() { return eta2_; }
      double phi1() { return phi1_; }
      double phi2() { return phi2_; }
      double r1()   { return r1_; }
      double r2()   { return r2_; }
      

      double deta() { return eta1_-eta2_; }
      double dphi();
      double dR();
      double dR2();
      
 
      void   setIt1(int i) { it1_ = i; }
      void   setIt2(int i) { it2_ = i; }
      int    getIt1() { return it1_; }
      int    getIt2() { return it2_; }

      void   setId(int i) { id_ = i; }
      void   setSId(int i) { sid_ = i; }
      void   setType(int i) { ptype_ = i; }
      void   setId1(int i) { id1_ = i; }
      void   setId2(int i) { id2_ = i; }
      int    getId() { return id_; }
      int    getSId() { return sid_; }
      int    getType() { return ptype_; }
      int    getId1() { return id1_; }
      int    getId2() { return id2_; }


   private:

      double eta1_;
      double eta2_;

      double phi1_;
      double phi2_;

      double r1_;
      double r2_;

      int it1_;   // first iterator
      int it2_;   // second iterator
      
      int sid_;   // signal exist?
      int ptype_;  // process type
      int id_;
      int id1_;
      int id2_;
      
};

Tracklet::Tracklet(double eta1, double eta2, double phi1, double phi2, double r1, double r2) {

   eta1_ = eta1;
   eta2_ = eta2;

   phi1_ = phi1;
   while (phi1_>2*PI) phi1_-=2*PI;

   phi2_ = phi2;
   while (phi2_>2*PI) phi2_-=2*PI;

   r1_ = r1;
   r2_ = r2;
}

double Tracklet::dphi() 
{
   double dphi=phi1_-phi2_;

   if (dphi>0){
      while (dphi>2*PI) dphi-=2*PI;
      if (dphi>PI) dphi=2*PI-dphi;
   } else {
      while (dphi<-2*PI) dphi+=2*PI;
      if (dphi<-PI) dphi=-2*PI-dphi;
   }


   return dphi; 
}

double Tracklet::dR() 
{
   return sqrt(dR2());
}

double Tracklet::dR2() 
{
   double dPhi=dphi();
   double dEta=deta();

   return dPhi*dPhi+dEta*dEta;
}

bool compareDeltaR(Tracklet a,Tracklet b) { return fabs(a.dR2())<fabs(b.dR2());}
bool compareDeltaEta(Tracklet a,Tracklet b) {return fabs(a.deta())<fabs(b.deta());}

vector<Tracklet> cleanTracklets(vector<Tracklet> input, int matchNumber, SelectionCriteria cuts);
vector<Tracklet> recoProtoTracklets(vector<RecoHit> firstLayerHits, vector<RecoHit> secondLayerHits);

vector<Tracklet> recoProtoTracklets(vector<RecoHit> hits1, vector<RecoHit> hits2)
{
  vector<Tracklet> protoTracklets;

  for (int i = 0; i < (int) hits1.size(); i++)
    {
      for (int j = 0; j < (int) hits2.size(); j++)
	{
	  Tracklet mytracklet(hits1[i].eta,hits2[j].eta,hits1[i].phi,hits2[j].phi,hits1[i].r,hits2[j].r);
	  mytracklet.setIt1(i);
	  mytracklet.setIt2(j);
	  protoTracklets.push_back(mytracklet);
	}
    }

  return protoTracklets;
}

vector<Tracklet> recoFastTracklets(vector<RecoHit> hits,int verbose_ = 0)
{
  vector<Tracklet> fastTracklets;
  sort( hits.begin() , hits.end() , compareEta);

  int i=0;
  int i1=1,i2=2;
  int n[3]={0,0,0};
  int flag=0;
  for (int i=0;i<(int)hits.size();i++) {
     n[(int)hits[i].layer]++;
  }

  while (n[1]!=0&&n[2]!=0)
  {
     double left=-10,center=10e10,right=10e10;

     if (i!=0&&hits[i].layer!=hits[i-1].layer) {
        left = fabs(hits[i].eta-hits[i-1].eta);       
     } else {
        left = 10e10;
     }
	
     i1=i+1;
     while (center==10e10&&i1<(int)hits.size()) {
        if (i1!=(int)hits.size()&&hits[i1-1].layer!=hits[i1].layer) {
           center = fabs(hits[i1-1].eta-hits[i1].eta);
        } else {
           center = 10e10;
	   i1++;
        } 
     }

     i2=i1+1;
     while (right==10e10&&i2<(int)hits.size()) {
        if (i2!=(int)hits.size()&&hits[i2-1].layer!=hits[i2].layer) {
           right = fabs(hits[i2-1].eta-hits[i2].eta);
        } else {
           right = 10e10;
	   i2++;
        }
     }

     if (verbose_) {       
        for (int j=0;j<(int) hits.size();j++) 
        {
           if (j==i1-1) cout <<"[";
           cout <<" "<<Form("%.2f",hits[j].eta)<<";"<<hits[j].layer<<" ";
           if (j==i1) cout <<"]";
        }
         
     cout <<" ("<<i<<" "<<i1<<" "<<i2<<")"<<endl;
     cout <<"LCR = "<<left<<" "<<center<<" "<<right<<endl;
     }
     
     if (center<left&&center<right) {
          if (flag==0&&i2>=(int)hits.size()) {
	      flag++;
	      i=0;
	      continue;
	  }
	  flag=0;
	  Tracklet mytracklet(hits[i1-1].eta,hits[i1].eta,hits[i1-1].phi,hits[i1].phi,hits[i1-1].r,hits[i1].r);
          fastTracklets.push_back(mytracklet);
	  n[(int)hits[i1-1].layer]--;
	  n[(int)hits[i1].layer]--;
	  hits.erase(hits.begin()+i1-1,hits.begin()+i1+1);
	  if (verbose_) {
	     cout <<"Form Tracklet! "<<i1-1<<" "<<i1<<" d= "<<mytracklet.deta()<<endl;
             cout <<endl;
	     cout <<n[1]<<" "<<n[2]<<endl;
          }
	  i=0;
     } else {
          i++;
     }
     if (i>=(int)hits.size()||i<0) i=0;
  }
    sort( fastTracklets.begin() , fastTracklets.end() , compareDeltaEta);

  return fastTracklets;
}


vector<Tracklet> cleanTracklets(vector<Tracklet> input, int matchNumber,SelectionCriteria cuts)
{
  vector<Tracklet> output;

  if(cuts.useDeltaPhi_)
    sort( input.begin() , input.end() , compareDeltaR);
  else
    sort( input.begin() , input.end() , compareDeltaEta);

  if (cuts.verbose_) {
    for (unsigned int i = 0; i < input.size(); i++)
      {
	cout <<input[i].deta()<<" "<<input[i].getIt1()<<" "<<input[i].getIt2()<<endl;
      }
  }

  int used1[50000];
  int used2[50000];

  for (int i=0;i<50000;i++) {
    used1[i]=0;
    used2[i]=0;
  }
  if (cuts.verbose_) cout<<"Printing Hits"<<endl;

  for (unsigned int i = 0; i < input.size(); i++){


    if(cuts.useDeltaPhi_) {
       if (cuts.verbose_) cout<<"Eta 1 : "<<input[i].eta1()<<"  ; Eta 2 : "<<input[i].eta2()<<" ;  Delta R : "<<input[i].dR()<<endl;
    } else {
       if (cuts.verbose_) cout<<"Eta 1 : "<<input[i].eta1()<<"  ; Eta 2 : "<<input[i].eta2()<<" ;  Delta Eta : "<<input[i].deta()<<endl;
    }
    int i1=input[i].getIt1();
    int i2=input[i].getIt2();

    if (used1[i1]==0&&used2[i2]==matchNumber) {
      Tracklet tmp = input[i];
      output.push_back(tmp);
      used1[i1]++;
      if (cuts.checkSecondLayer_) used2[i2]++;
    }
    if (used1[i1]==0&&used2[i2]<matchNumber) {
      if (cuts.checkSecondLayer_) used2[i2]++;
    }
  }
  if (cuts.verbose_) {
    cout <<"Output:"<<endl;
    for (unsigned int i = 0; i < output.size(); i++)
      {
	cout <<output[i].deta()<<" "<<output[i].getIt1()<<" "<<output[i].getIt2()<<endl;
      }
  }

  return output;
}

void printTrackletVector(vector<Tracklet> x)
{
   
   for (int i=0;i<(int)x.size();i++)
   {
      cout <<fabs(x[i].deta())<<" "<<"L1: "<<x[i].eta1()<<" "<<x[i].phi1()<<" L2: "<<x[i].eta2()<<" "<<x[i].phi2()<<endl;
       
   }
}

double sumTrackletVector(vector<Tracklet> x)
{
   double total=0;
   for (int i=0;i<(int)x.size();i++)
   {
      total+=fabs(x[i].deta());
   }
   return total;
}

double TrackletVertexUnbin(vector<RecoHit> layer1, vector<RecoHit> layer2,double histDeltaZ, double dPhiCut, bool redoAvg = true, bool fillZ=true)
{

    double maxNz=0;
    double maxTotalZ=0;
    double maxRMS=10e10;
    double nRecoZ=0;
 
    vector<double> vectorZ;
    
    for(int ihit = 0; ihit< (int)layer1.size(); ++ihit) {
       double r1 = layer1[ihit].r;
       double phi1 = layer1[ihit].phi;
       double z1 = r1/tan(atan(exp(-layer1[ihit].eta))*2);
       
       for(int ihit2 = 0; ihit2< (int)layer2.size(); ++ihit2) {
          double r2 = layer2[ihit2].r;
          double phi2 = layer2[ihit2].phi;
	  if (fabs(calcDphi(phi1,phi2))>dPhiCut) continue;
          double z2 = r2/tan(atan(exp(-layer2[ihit2].eta))*2);
	  
	  double z = z1-(z2-z1)/(r2-r1)*r1;
	   if (fabs(z)<20) {
             if (fillZ){
	        nRecoZ++;
		vectorZ.push_back(z);
	     }
	   }
       }
       
    } 


   sort(vectorZ.begin(),vectorZ.end());

   for (int i=0;i<(int)vectorZ.size();i++)
   {
      double nz=0;
      double totalZ=0;
      double rms=0;
      nz++;

      TH1F *h = new TH1F("h","",100,vectorZ[i]-histDeltaZ,vectorZ[i]+histDeltaZ);
      totalZ+=vectorZ[i];
      h->Fill(vectorZ[i]);
      int flag=0;
      for (int j=0;j<(int)vectorZ.size();j++) 
      {
         if ((fabs(vectorZ[j]-vectorZ[i]))<histDeltaZ&&i!=j) {
	    nz++;
	    totalZ+=vectorZ[j];
	    flag=1;
	    h->Fill(vectorZ[j]);
	 } else {
	    if (flag==1) continue; 
	 }
      }
      
      rms=h->GetRMS();
      delete h;
      if (nz>maxNz||(nz==maxNz&&rms<maxRMS)) {
         maxNz=nz;
         maxTotalZ=totalZ;
	 maxRMS=rms;
      }
   }
//    cout <<maxTotalZ<<" "<<maxNz<<endl;
   if (maxNz==0||nRecoZ==0) return -99; else return maxTotalZ/maxNz;
}



