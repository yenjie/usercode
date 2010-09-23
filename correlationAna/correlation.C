#include <TFile.h>
#include <TH2.h>
#include <TTree.h>
#include <TCanvas.h>
#include <iostream>
#include <TRandom.h>

#define maxEntry 30000
#define PI 3.14159
#define doubleCut 0.1
//================================================================================
// mini class
//================================================================================
class Parameter 
{
    public:
    Int_t nTrk;
    Float_t trkPt[maxEntry];
    Float_t trkEta[maxEntry];
    Float_t trkExpHit1Eta[maxEntry];
    Float_t trkExpHit2Eta[maxEntry];
    Float_t trkExpHit3Eta[maxEntry];
    Float_t trkPhi[maxEntry];
    Float_t trkVz[maxEntry];
    Float_t trkDz1[maxEntry];
    Float_t trkDz2[maxEntry];
    Float_t trkDzError[maxEntry];
    Float_t trkDxy1[maxEntry];
    Float_t trkDxy2[maxEntry];
    Float_t trkDxyError[maxEntry];
    Int_t trkQual[maxEntry];
    Float_t vz[10];
};

//================================================================================
// selection Criteria
//================================================================================
class SelectionCriteria
{
    public:
     Float_t ptMinT;  // Trigger Particle pt Cut
     Float_t ptMaxT;  // Trigger Particle pt Cut
     Float_t etaMaxT; // Trigger Particle 
     Float_t ptMinM;  // Correlated particles pt Cut
     Float_t ptMaxM;  // Correlated particle pt Cut
     Float_t etaMaxM; // Correlated Particle 

     Float_t scaleEvt;// Scale down the number of the events used.
    
     // Plotting
     Float_t maxDEta;  // MaxDeta
     Float_t projDEta;  // Projection from projDEta to maxDEta
     Int_t   nBinPhi; // # of bins in Phi 
     Int_t   nBinEta; // # of bins in Eta

     // Analysis Criteria
     bool isMC;       // Do MC analysis
     Float_t maxVz;   // Max Vz (cm)

};

//================================================================================
// Projection
//================================================================================
TH1D *getProjection(TH2D* h,SelectionCriteria cuts)
{
    TH2D *hTmp = new TH2D("hTmp","",cuts.nBinEta,-cuts.maxDEta,cuts.maxDEta,cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);

    for (int i=1;i<=cuts.nBinEta/cuts.maxDEta/2.*(cuts.maxDEta-cuts.projDEta)+0.1;i++)
    {
        for (int j=1;j<=cuts.nBinPhi;j++)
        {
           hTmp->SetBinContent(i,j,h->GetBinContent(i,j));
           hTmp->SetBinError(i,j,h->GetBinError(i,j));
        }
    }   
    TH1D *hTmpY = (TH1D*)hTmp->ProjectionY();
    hTmpY->SetName("hTmpY");
    hTmpY->Scale(1./((int)((cuts.maxDEta-cuts.projDEta)*cuts.nBinEta/cuts.maxDEta)));
    delete hTmp;
    return hTmpY;
}

//================================================================================
// Move Phi according to the convention
//================================================================================
double movePhi(double phi, SelectionCriteria cuts)
{
   if (phi<-PI/2.-PI/cuts.nBinPhi) phi += 2*PI;
   return phi;
}

//================================================================================
// calculate delta phi
//================================================================================
double calcDeltaPhi(double phi1, double phi2) {

  double deltaPhi = phi1 - phi2;

  if (deltaPhi < 0) deltaPhi = -deltaPhi;

  if (deltaPhi > 3.1415926) {
    deltaPhi = 2 * 3.1415926 - deltaPhi;
  }

  return deltaPhi;
}

//================================================================================
// shift histogram by fixed value
//================================================================================
void shiftHist(TH2D* hR, double shift)
{

   for (int x=0;x<=hR->GetNbinsX()+1;x++)
   {
      for (int y=0;y<=hR->GetNbinsY()+1;y++)
      {
         double val = hR->GetBinContent(x,y)+shift;
         hR->SetBinContent(x,y,val);
      }
   }
}

//================================================================================
// shift histogram by fixed value
//================================================================================
void shiftHist1D(TH1D* hR, double shift)
{

   for (int x=0;x<=hR->GetNbinsX()+1;x++)
   {
         double val = hR->GetBinContent(x)+shift;
         hR->SetBinContent(x,val);
   }
}

//================================================================================
// user divide function, no treatment on error yet.
//================================================================================
void divideHist(TH2D* hS, TH2D* hB)
{

   for (int x=0;x<=hS->GetNbinsX()+1;x++)
   {
      for (int y=0;y<=hS->GetNbinsY()+1;y++)
      {
         if (hS->GetBinContent(x,y)!=0&&hB->GetBinContent(x,y)!=0) {
            double val = hS->GetBinContent(x,y)/hB->GetBinContent(x,y);
            hS->SetBinContent(x,y,val);
         } else {
            if (hB->GetBinContent(x,y)==0) hS->SetBinContent(x,y,-1000);
         }
      }
   }
}

//================================================================================
// get S
//================================================================================
double getS(TH2D* h,TH1D* h1S,TH2D* hB, TH2D* hR,TTree *t,Parameter &par, SelectionCriteria cuts)
{
    int nEv=0, nEv1S=0;
    long totalTracks=0;
    double bin= 2.*cuts.maxDEta/cuts.nBinEta*2.*PI/cuts.nBinPhi;

    TH1D * hBProj = (TH1D*) getProjection(hB,cuts);
    hBProj->SetName("hBProj");
    hBProj->Scale(1./hBProj->Integral(1,cuts.nBinPhi));
    for (int i = 0; i < t->GetEntries()/cuts.scaleEvt; i++)
    {
        if (i%1000 == 0) cout <<" S "<<i<<" "<<t->GetEntries()<<endl;
        t->GetEntry(i);

        // Vertex cut
        if (fabs(par.vz[1])>cuts.maxVz) continue;
//        if (fabs(par.vz[2])cuts.maxVz) continue;
//        if (fabs(par.vz[1]-par.vz[2])<5) continue;
        long total=0;
        long totalTrack=0;
        TH2D *hS = new TH2D("hSS","",cuts.nBinEta,-cuts.maxDEta,cuts.maxDEta,cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);
        for (int j1=0;j1<par.nTrk;j1++)
        {
           if (par.trkPt[j1]<cuts.ptMinT||par.trkPt[j1]>cuts.ptMaxT) continue;
           if (  par.trkPt[j1]>cuts.ptMinT&&par.trkPt[j1]<cuts.ptMaxT
                 ) totalTrack++;
           for (int j2=0;j2<par.nTrk;j2++)
           {
              if (  par.trkPt[j1]>cuts.ptMinT&&par.trkPt[j1]<cuts.ptMaxT
                  &&par.trkPt[j2]>cuts.ptMinM&&par.trkPt[j2]<cuts.ptMaxM
/*
                  &&fabs(par.trkDz1[j1]/par.trkDzError[j1])<2
                  &&fabs(par.trkDz1[j2]/par.trkDzError[j2])<2
                  &&fabs(par.trkDxy1[j1]/par.trkDxyError[j1])<2
                  &&fabs(par.trkDxy1[j2]/par.trkDxyError[j2])<2  
*/
                  &&fabs(par.trkEta[j1])<cuts.etaMaxT
                  &&fabs(par.trkEta[j2])<cuts.etaMaxM
                  &&par.trkQual[j1]==1&&par.trkQual[j2]==1
/*
                  &&(!(fabs(par.trkExpHit1Eta[j1]-par.trkExpHit1Eta[j2])<doubleCut&&
                       fabs(par.trkPhi[j1]-par.trkPhi[j2])<doubleCut)
                    )
                  &&(!(fabs(par.trkExpHit2Eta[j1]-par.trkExpHit2Eta[j2])<doubleCut&&
                       fabs(par.trkPhi[j1]-par.trkPhi[j2])<doubleCut)
                    )
                  &&(!(fabs(par.trkExpHit3Eta[j1]-par.trkExpHit3Eta[j2])<doubleCut&&
                       fabs(par.trkPhi[j1]-par.trkPhi[j2])<doubleCut)
                    )
                  &&fabs(par.trkVz[j1])<10
                  &&fabs(par.trkVz[j2])<10
  */               ) {
              double dphi = calcDeltaPhi(par.trkPhi[j1],par.trkPhi[j2]);
              double deta = fabs(par.trkEta[j1]-par.trkEta[j2]);
              if (fabs(dphi)<0.06&&fabs(deta)<0.06) continue;
              total++;
              hS->Fill(deta,movePhi(dphi,cuts),0.5);
              hS->Fill(deta,movePhi(-dphi,cuts),0.5);
              hS->Fill(-deta,movePhi(dphi,cuts),0.5);
              hS->Fill(-deta,movePhi(-dphi,cuts),0.5);
              }
           }
        }

        // good event
        if (total!=0) {
           double weight = (double)1/total/bin;
           hS->Sumw2();
           hS->Scale(weight);
           double intergal = hS->Integral(1,cuts.nBinEta,1,cuts.nBinPhi);
           if (intergal==0) {
              delete hS;    
              cout <<"INTEGRAL=0!!!!"<<endl;
              continue;
           }
           hS->Scale(1./intergal);
           h->Add(hS); 
           TH1D *h1STmp = (TH1D*)getProjection(hS,cuts);
           hS->Divide(hB);//divideHist(hS,hB);
           double intS = h1STmp->Integral(1,cuts.nBinPhi);
           //cout <<intS<<endl;
           if (intS!=0) {
              h1STmp->Scale(1./intS); 
              nEv1S++;
           } else { 
//              cout <<"IntS=0!"<<endl;
           }
           h1STmp->Divide(hBProj);
           shiftHist(hS,-1.);
           shiftHist1D(h1STmp,-1.);
           h1STmp->Scale(totalTrack);
           hS->Scale(totalTrack);
           totalTracks+=totalTrack;
           if (intS!=0)h1S->Add(h1STmp);
           delete h1STmp;
           hR->Add(hS);
           nEv++;
        }
        delete hS;    
    }

    // normalize according to the number of events used
    h->Scale(1./nEv);
    hR->Scale((double)1./nEv);
    h1S->Scale(1./nEv1S);
    cout <<nEv<<" "<<nEv1S<<endl;
    return totalTracks/nEv;
}


//================================================================================
// get B template
//================================================================================
void getB(TH2D* h,TTree *t,Parameter &par, TTree *t2,Parameter &par2,SelectionCriteria cuts,int diff)
{
    double bin= 2.*cuts.maxDEta/cuts.nBinEta*2.*PI/cuts.nBinPhi;
    int nEv=0;
    int nevt = t->GetEntries();
    for (int i = 0; i < t->GetEntries()/cuts.scaleEvt-diff; i++)
    {
        // Display
        if (i%1000 == 0) cout <<" B "<<i<<" / "<<t->GetEntries()<<endl;
        t->GetEntry(i);

        // Vertex cut
        if (fabs(par.vz[1])>cuts.maxVz) continue;
//        if (fabs(par.vz[2])>cuts.maxVz) continue;
     
        int nEntries = t->GetEntries();
        for (int j=i+1+diff*1000;j<t->GetEntries()+i+diff*1000;j++) {
           int ent=j % nEntries;
           t2->GetEntry(ent);
           if (fabs(par.vz[1]-par2.vz[1])<1&&fabs(par.nTrk-par2.nTrk)<1000&&ent!=i) j=t->GetEntries()+i+diff*1000;
        }
        if (fabs(par.vz[1]-par2.vz[1])>1) continue;
//        if (fabs(par2.vz[1])>cuts.maxVz) continue;
        long total=0;


        TH2D *hS = new TH2D("hSS","",cuts.nBinEta,-cuts.maxDEta,cuts.maxDEta,cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);
        for (int j1=0;j1<par.nTrk;j1++)
        {
           if (par.trkPt[j1]<cuts.ptMinT) continue;
           for (int j2=0;j2<par2.nTrk;j2++)
           {
              if (  par.trkPt[j1]>cuts.ptMinT&&par.trkPt[j1]<cuts.ptMaxT
                  &&par2.trkPt[j2]>cuts.ptMinM&&par2.trkPt[j2]<cuts.ptMaxM
/*                  &&fabs(par.trkDz1[j1]/par.trkDzError[j1])<3
                  &&fabs(par2.trkDz1[j2]/par2.trkDzError[j2])<3
                  &&fabs(par.trkDxy1[j1]/par.trkDxyError[j1])<3
                  &&fabs(par2.trkDxy1[j2]/par2.trkDxyError[j2])<3
  */                &&fabs(par.trkEta[j1])<cuts.etaMaxT
                  &&fabs(par2.trkEta[j2])<cuts.etaMaxM
                  &&par.trkQual[j1]==1&&par2.trkQual[j2]==1
/*                &&(!(fabs(par.trkExpHit1Eta[j1]-par2.trkExpHit1Eta[j2])<doubleCut&&
                       fabs(par.trkPhi[j1]-par2.trkPhi[j2])<doubleCut)
                  )
                &&(!(fabs(par.trkExpHit2Eta[j1]-par2.trkExpHit2Eta[j2])<doubleCut&&
                       fabs(par.trkPhi[j1]-par2.trkPhi[j2])<doubleCut)
                  )
                &&(!(fabs(par.trkExpHit3Eta[j1]-par2.trkExpHit3Eta[j2])<doubleCut&&
                       fabs(par.trkPhi[j1]-par2.trkPhi[j2])<doubleCut)
                  )
                  //&&par.trkQual[j1]==1&&par2.trkQual[j2]==1
                  &&fabs(par.trkVz[j1])<10
                  &&fabs(par2.trkVz[j2])<10
  */               ) {
              double dphi = calcDeltaPhi(par.trkPhi[j1],par2.trkPhi[j2]);
              double deta = fabs(par.trkEta[j1]-par2.trkEta[j2]);
              if (fabs(dphi)<0.06&&fabs(deta)<0.06) continue;
              total++;
              hS->Fill(deta,movePhi(dphi,cuts),1);
              hS->Fill(deta,movePhi(-dphi,cuts),1);
              hS->Fill(-deta,movePhi(dphi,cuts),1);
              hS->Fill(-deta,movePhi(-dphi,cuts),1);
              }
           }
        }
        if (total!=0) {
           double weight = 1./(double)total/bin;
           hS->Sumw2();
           hS->Scale(weight);
           nEv++;
           h->Add(hS); 
        }
        delete hS;
    }

    // normalize according to the number of events used
    h->Scale(1./nEv/2.);
    h->Scale(1./h->Integral(1,cuts.nBinEta,1,cuts.nBinPhi));
}


//================================================================================
// main routine
//================================================================================
void correlation(char *infname, 
                 char* name = "test",
                 double scale = 1, 
                 double ptMinT = 1,
                 double ptMaxT = 3,
                 double ptMinM = 1,
                 double ptMaxM = 3,
                 double maxVz = 10,
                 bool isMC = false
                )
{
    // Define the selection criteria and analysis settings
    SelectionCriteria cuts;

    cuts.nBinEta = 48;
    cuts.nBinPhi = 32;
    cuts.maxDEta = 4.8;
    cuts.ptMinT = ptMinT;
    cuts.ptMaxT = ptMaxT;
    cuts.ptMinM = ptMinM;
    cuts.ptMaxM = ptMaxM;
    cuts.maxVz = maxVz;

    cuts.etaMaxM = 1.5;
    cuts.etaMaxT = 1.5;
    cuts.scaleEvt = scale;
    cuts.projDEta = 2;
    cuts.isMC = isMC;

    TFile* inf = new TFile(infname);
    TTree *t = (TTree*)inf->FindObjectAny("PixelTree");
    TTree *t2 = (TTree*)t->Clone();
    Parameter par;
    Parameter par2;

    t->SetBranchAddress("nTrk",&par.nTrk);
    t->SetBranchAddress("trkVz",par.trkVz);
    t->SetBranchAddress("trkDz1",par.trkDz1);
    t->SetBranchAddress("trkDz2",par.trkDz2);
    t->SetBranchAddress("trkDzError",par.trkDzError);
    t->SetBranchAddress("trkDxy1",par.trkDxy1);
    t->SetBranchAddress("trkDxy2",par.trkDxy2);
    t->SetBranchAddress("trkDxyError",par.trkDxyError);
    t->SetBranchAddress("trkPt",par.trkPt);
    t->SetBranchAddress("trkEta",par.trkEta);
    t->SetBranchAddress("trkExpHit1Eta",par.trkExpHit1Eta);
    t->SetBranchAddress("trkExpHit2Eta",par.trkExpHit2Eta);
    t->SetBranchAddress("trkExpHit3Eta",par.trkExpHit3Eta);
    t->SetBranchAddress("trkPhi",par.trkPhi);
    t->SetBranchAddress("trkQual",par.trkQual);
    t->SetBranchAddress("vz",par.vz);

    t2->SetBranchAddress("nTrk",&par2.nTrk);
    t2->SetBranchAddress("trkVz",par2.trkVz);
    t2->SetBranchAddress("trkDz1",par2.trkDz1);
    t2->SetBranchAddress("trkDz2",par2.trkDz2);
    t2->SetBranchAddress("trkDzError",par2.trkDzError);
    t2->SetBranchAddress("trkDxy1",par2.trkDxy1);
    t2->SetBranchAddress("trkDxy2",par2.trkDxy2);
    t2->SetBranchAddress("trkDxyError",par2.trkDxyError);
    t2->SetBranchAddress("trkPt",par2.trkPt);
    t2->SetBranchAddress("trkEta",par2.trkEta);
    t2->SetBranchAddress("trkExpHit1Eta",par2.trkExpHit1Eta);
    t2->SetBranchAddress("trkExpHit2Eta",par2.trkExpHit2Eta);
    t2->SetBranchAddress("trkExpHit3Eta",par2.trkExpHit3Eta);
    t2->SetBranchAddress("trkPhi",par2.trkPhi);
    t2->SetBranchAddress("trkQual",par2.trkQual);
    t2->SetBranchAddress("vz",par2.vz);
 
    TH1F *h = new TH1F("h","",1000,0,1000);
    t->Draw("nTrk>>h");
    double N = h->GetMean();

    TFile *outf = new TFile(Form("%s-%.0f-%.0f-%.0f-%.0f-vzpm%.1fcm.root",name,ptMinT,ptMaxT,ptMinM,ptMaxM,maxVz),"recreate");
    cout <<Form("%s-%.0f-%.0f-%.0f-%.0f-vzpm%.1fcm.root",name,ptMinT,ptMaxT,ptMinM,ptMaxM,maxVz)<<endl;

    TH2D *hS = new TH2D("hS","",cuts.nBinEta,-cuts.maxDEta,cuts.maxDEta,cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);
    TH2D *hB = new TH2D("hB","",cuts.nBinEta,-cuts.maxDEta,cuts.maxDEta,cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);
    TH2D *hR = new TH2D("hR","",cuts.nBinEta,-cuts.maxDEta,cuts.maxDEta,cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);

    TH1D* h1S = new TH1D("h1S","",cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);

    int nDiff=1;
    for (int d=3;d<3+nDiff;d++) {    
        TH2D *hB1 = new TH2D("hB1","",cuts.nBinEta,-cuts.maxDEta,cuts.maxDEta,cuts.nBinPhi,-PI/2.-PI/cuts.nBinPhi,PI/2.*3.-PI/cuts.nBinPhi);
        getB(hB1,t,par,t2,par2,cuts,d);
        hB->Add(hB1);
        delete hB1;
    }
    hB->Scale(1./nDiff);
    N=getS(hS,h1S,hB,hR,t,par,cuts);
    hS->Sumw2();
    hB->Sumw2();

    hR->SetXTitle("#Delta#eta");
    hR->SetYTitle("#Delta#phi");
    hS->SetXTitle("#Delta#eta");
    hS->SetYTitle("#Delta#phi");
    hB->SetXTitle("#Delta#eta");
    hB->SetYTitle("#Delta#phi");

    hR->SetAxisRange(-2,2,"Z");
    TCanvas *c = new TCanvas("c","hS",600,600);
    c->SetTheta(60);
    c->SetPhi(30);
    hS->Draw("surf1");
    TCanvas *c2 = new TCanvas("c2","hB",600,600);
    c2->SetTheta(60);
    c2->SetPhi(30);
    hB->Draw("surf1");
    TCanvas *c3 = new TCanvas("c3","hR",600,600);
    c3->SetTheta(60);
    c3->SetPhi(30);
    hR->Draw("surf1");
    TCanvas *c4 = new TCanvas("c4","hProj",600,600);
    TH1D *hProj = getProjection(hS,cuts);
    hProj->SetName("hProj");
    TH1D *hProjB = getProjection(hB,cuts);
    hProjB->SetName("hProjB");
    hProj->Scale(1/hProj->Integral(1,cuts.nBinPhi));
    hProjB->Scale(1./hProjB->Integral(1,cuts.nBinPhi));
    hProj->Divide(hProjB);
    for (int i=1;i<=cuts.nBinPhi;i++){
       hProj->SetBinContent(i,hProj->GetBinContent(i)-1);
    }
    hProj->Scale(N);
    hProj->SetXTitle("#Delta#phi");
    hProj->SetAxisRange(0,3.2,"X");
    hProj->SetMarkerColor(2);
    hProj->SetLineColor(2);
    hProj->Draw("pe");
    h1S->SetAxisRange(0,3.2,"X");

    h1S->Draw("same");
    outf->Write();
}
