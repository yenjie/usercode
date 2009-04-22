#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TTree.h>
#include <iostream.h>
#include <TLine.h>
#include <math.h>
#include <TF1.h>
#include <TCut.h>
#include <TPad.h>

#define nEtaBin 12
#define nHitBin 100
#define nDEtaBin 80
#define nVzBin 10

#define plotDEta true

void formatHist(TH1* h, int col = 1, double norm = 1);
void plotBetaFromTreeVz(char* filename,char *toyMC="pseudoExp.root",int useCorrectionFile = 0,char *mycut="",bool useMC = false)
{
   TFile *f= new TFile(filename);
   
   TNtuple * ntmatched= dynamic_cast<TNtuple *>(f->Get("ntmatched"));
   TTree * TrackletTree= dynamic_cast<TTree *>(f->Get("TrackletTree"));
   TFile *fmc= new TFile(toyMC);
   TNtuple * ntmatchedMC= dynamic_cast<TNtuple *>(fmc->Get("ntmatched"));

   TFile *fcorrection;
   if (useCorrectionFile == 1) {
      fcorrection = new TFile("correction-900GeV.root");
   } else if (useCorrectionFile == 2) {
      fcorrection = new TFile("correction-10TeV.root");
   } else if (useCorrectionFile == 3) {
      fcorrection = new TFile("correction-900GeV-half.root");
   } else if (useCorrectionFile == 4) {
      fcorrection = new TFile("correction-10TeV-half.root");
   }

   TFile *outf = new TFile ("correction.root","recreate");

   TNtuple * betas = new TNtuple("betas","","bin:nhit:beta:betaErr");
   TNtuple * correction = new TNtuple("correction","","bin:nhit:obs:gen:err");
   TNtuple * correction1 = new TNtuple("correction1","","e1:e2");

   TH3F *hEverything = new
   TH3F("hEverything","Everything",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *h2 = new TH3F("h2","Reproduced Background",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *h2MC = new TH3F("h2MC","Reproduced Background from ToyMC",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *h3 = new TH3F("h3","Background Subtracted",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *h4 = new TH3F("h3","(1-beta)*Everything",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *hSubtracted = new TH3F("hSubtracted","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *hHadron = new TH3F("hHadron","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *hHadronAccepted = new TH3F("hHadronAccepted","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH3F *hCorrected = new TH3F("hCorrected","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5,nVzBin,-10,10);
   TH1F *alphaPlots[nEtaBin][nVzBin];
   TH1F *betaPlots[nEtaBin][nVzBin];
   TH1F *alphaErrPlots[nEtaBin][nVzBin];
   TH1F *betaErrPlots[nEtaBin][nVzBin];
   TH1F *hDEtaAll[nEtaBin][nVzBin];
   TH1F *hDEtaSig[nEtaBin][nVzBin];
   TH1F *hDEtaBck[nEtaBin][nVzBin];

   TH1F *hCorrectedEtaBin = new TH1F("hCorrectedEtaBin","Corrected",nEtaBin,-3,3);
   

   TF1 *funBeta[nEtaBin][nVzBin];
   TF1 *funAlpha[nEtaBin][nVzBin];
   TF1 *funBetaErr[nEtaBin][nVzBin];
   TF1 *funAlphaErr[nEtaBin][nVzBin];
   TF1 *fBeta[nEtaBin][nVzBin];
   TF1 *fBetaErr[nEtaBin][nVzBin];
   TF1 *fAlpha[nEtaBin][nVzBin];
   TF1 *fAlphaErr[nEtaBin][nVzBin];
   TH1D *hTruthRatio;
      
   // Prepare histograms
   for (int i=0;i<nEtaBin;i++) {
      for (int j=0;j<nVzBin;j++) {
         alphaPlots[i][j] = new TH1F(Form("alpha%dVz%d",i,j),"",nHitBin,-0.5,nHitBin-0.5);
         alphaErrPlots[i][j] = new TH1F(Form("alphaErr%dVz%d",i,j),"",nHitBin,-0.5,nHitBin-0.5);
         betaPlots[i][j] = new TH1F(Form("beta%dVz%d",i,j),"",nHitBin,-0.5,nHitBin-0.5);
         betaErrPlots[i][j] = new TH1F(Form("betaErr%dVz%d",i,j),"",nHitBin,-0.5,nHitBin-0.5);
         hDEtaAll[i][j] = new TH1F(Form("hDEtaAll%dVz%d",i,j),"",nDEtaBin,0,4);
         hDEtaBck[i][j] = new TH1F(Form("hDEtaBck%dVz%d",i,j),"",nDEtaBin,0,4);
      } 
   }
   
   // Signal and Sideband regions  
   double signalRegionCut = 1;
   double sideBandRegionCut = 2;
   TCut cut = mycut;
   TCut signalRegion = Form("abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut);
   TCut signalRegionEta = Form("abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut);
   TCut signalRegionInPhi = Form("abs(dphi)<%f",signalRegionCut);
   TCut sideBandRegionInPhi = Form("abs(dphi)>%f&&abs(dphi)<%f",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegion =  Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegionEta = Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)>1",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegionEtaSignalRegion = Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut,sideBandRegionCut);

   TCanvas *cBetas = new TCanvas ("cBetas","",400,400);
   TrackletTree->Draw("vz[1]:mult:eta1>>hEverything",signalRegion);

   h3->SetXTitle("#eta");
   h3->SetYTitle("#Delta#eta");
   if (useMC) {
     cout <<"Use ToyM"<<endl;
     ntmatchedMC->Draw("vz[1]:nhit1:eta1>>h2",sideBandRegionEta&&cut);
     int n = ntmatched->GetEntries(sideBandRegionEta&&cut);
     int n2 = ntmatchedMC->GetEntries(sideBandRegionEta&&cut);
     h2->Scale(1./n2*n); 
   } else {
   TrackletTree->Draw("vz[1]:mult:eta1>>h2",sideBandRegionEtaSignalRegion&&cut);   
   }

   hEverything->Sumw2();
   h2->Sumw2();
   h3->Sumw2();


   // Beta calculation
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
            double beta = 0;
  	    if (hEverything->GetBinContent(x,y,z)!=0&&h2->GetBinContent(x,y,z)!=0) 
	    {   
	       beta = h2->GetBinContent(x,y,z)/hEverything->GetBinContent(x,y,z);
	       
               double e1 = hEverything->GetBinError(x,y,z)/hEverything->GetBinContent(x,y,z);
	       double e2 = h2->GetBinError(x,y,z)/h2->GetBinContent(x,y,z);
               double betaErr = beta* sqrt(e2*e2);
               betas->Fill(x,y,beta,betaErr);
	       betaPlots[x-1][z-1]->SetBinContent(y,beta);
	       betaPlots[x-1][z-1]->SetBinError(y,betaErr);
	       betaErrPlots[x-1][z-1]->SetBinContent(y,betaErr);
               //correction1->Fill(x,y,val,hHadron->GetBinContent(x,y),valErr);

	       beta = hEverything->GetBinContent(x,y,z)-h2->GetBinContent(x,y,z);
               hSubtracted->SetBinContent(x,y,z, beta);
	       double e3 = hEverything->GetBinError(x,y,z);
	       double e4 = h2->GetBinError(x,y,z);
               betaErr = sqrt(e1*e1+e2*e2);
               h3->SetBinContent(x,y,z, beta);
	       h3->SetBinError(x,y,z, betaErr);
	       hSubtracted->SetBinError(x,y,z, sqrt(e3*e3+e4*e4));
	    }
	 }      
      }
   }
   h3->Draw("col");

   // Charged Hadrons:
   TCanvas *cHadrons = new TCanvas("cHadrons","",400,400);
   hHadron->SetXTitle("#eta");
   hHadron->SetYTitle("N_{hit}^{Layer1} |#eta|<1");
   TrackletTree->Draw("vz[1]:mult:eta>>hHadron","abs(eta)<3");
   TrackletTree->Draw("vz[1]:mult:eta>>hHadronAccepted","abs(eta)<3");
   

   TCanvas *cBetaFit = new TCanvas("cBetaFit","",800,800);
   TPad *p1 = new TPad("p1","",0,0,1,1);
   p1->Divide(3,4,0.01,0.01);
   p1->Draw();   

   
   for (int i=0;i<nEtaBin;i++) 
   {
      for (int j=0;j<nVzBin;j++) 
      {
      //c[i]= new TCanvas (Form("c%d",i),"",400,400);
      p1->cd(i+1);
      formatHist(betaPlots[i][j],2,1);
      funBeta[i][j] = new TF1(Form("funBeta%dVz%d",i,j),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
      funBetaErr[i][j] = new TF1(Form("funBetaErr%d%-d",i,j),"[0]+[1]/(x+0.5)+[2]*exp([3]*x)",0,100);
      double etaMin = i*0.5 -3;
      double etaMax = i*0.5 -3+0.5;

      //betaPlots[i]->Fit(Form("funBeta%d",i),"m E");
      betaPlots[i][j]->SetXTitle("N_{Hits}");
      betaPlots[i][j]->SetYTitle(Form("#beta %.1f < #eta < %.1f",etaMin,etaMax)); 
      betaPlots[i][j]->SetAxisRange(0,1,"Y");
      betaPlots[i][j]->Draw("p");
      
      //fBeta[i] = betaPlots[i]->GetFunction(Form("funBeta%d",i));
      }
   }   


   // alpha calculation
   if (useCorrectionFile == 0) {   
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
         double val = 0;
	 if (hEverything->GetBinContent(x,y,z)!=0&&hHadron->GetBinContent(x,y,z)!=0) 
	 {   
	       val = hEverything->GetBinContent(x,y,z);
               double beta = betaPlots[x-1][z-1]->GetBinContent(y);
	       h4->SetBinContent(x,y,z, val*(1-beta));
	       double e1 = hEverything->GetBinError(x,y,z);
               double e2 = h2->GetBinError(x,y,z);
	       double nsig = val * (1-beta);
               double valErr = sqrt(e1*e1+e2*e2);
               h4->SetBinError(x,y,z, valErr);
               if (beta>=1) continue;
	       double truth    = hHadron->GetBinContent(x,y,z);
	       double truthErr = hHadron->GetBinError(x,y,z);
	       double alphaErr = truth/nsig* sqrt(valErr/nsig*valErr/nsig+truthErr/truth*truthErr/truth*0);
	       cout <<"beta: "<<beta<<" "<<x<<" "<<y<<" alpha:"<<truth/val/(1-beta)<<" +-"<<alphaErr<<" from "<<truth<<" "<<truthErr<<" "<<val<<" "<<valErr<<" "<<endl;
               if (beta!=1) {
   	          alphaPlots[x-1][z-1]->SetBinContent(y,truth/val/(1-beta));
	          alphaPlots[x-1][z-1]->SetBinError(y,alphaErr);
	          alphaErrPlots[x-1][z-1]->SetBinContent(y,alphaErr);
	       }
	 }
	 }      
      }
   }
   }

   // corrected calculation
   
   if (useCorrectionFile != 0) {
      for (int i=0;i<nEtaBin;i++)
      {
         for (int j=0;j<nVzBin;j++)
         {
         fAlpha[i][j] = (TF1*)
	 fcorrection->FindObjectAny(Form("funAlpha%dVz%d",i,j));
	 fAlphaErr[i][j] = (TF1*)
	 fcorrection->FindObjectAny(Form("funAlphaErr%dVz%d",i,j));
         alphaPlots[i][j] = (TH1F*)
	 fcorrection->FindObjectAny(Form("alpha%dVz%d",i,j));
         }
      }
      hTruthRatio = (TH1D*) fcorrection->FindObjectAny("hTruthRatio");
   } else {

      TCanvas *cAlphaFit = new TCanvas("cAlphaFit","",800,800);
      TPad *p2 = new TPad("p2","",0,0,1,1);
      p2->Divide(3,4,0.01,0.01);
      p2->Draw();   

      for (int i=0;i<nEtaBin;i++) 
      {
         for (int j=0;j<nVzBin;j++) 
         {
         // c[i]= new TCanvas (Form("c%d",i),"",400,400);
         formatHist(alphaPlots[i][j],2,1);
	 p2->cd(i+1);
         funAlpha[i][j] = new TF1(Form("funAlpha%dVz%d",i,j),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
         funAlphaErr[i][j] = new TF1(Form("funAlphaErr%dVz%d",i,j),"[0]+[1]/(x+0.5)+[2]*exp([3]*x)",0,100);
         double etaMin = i*0.5 -3;
         double etaMax = i*0.5 -3+0.5;
         alphaPlots[i][j]->Fit(Form("funAlpha%dVz%d",i,j),"m E","",0,60);
         alphaPlots[i][j]->SetXTitle("N_{Hits}");
         alphaPlots[i][j]->SetYTitle(Form("#alpha %.1f < #eta < %.1f",etaMin,etaMax)); 
         alphaPlots[i][j]->SetAxisRange(0.9,2.0,"y");
	 if (i <2 || i >9) alphaPlots[i][j]->SetAxisRange(4,10,"y");
	 
	 alphaErrPlots[i][j]->Fit(Form("funAlphaErr%dVz%d",i,j),"m E","",0,30);
         alphaPlots[i][j]->Draw("p");
         fAlpha[i][j] = alphaPlots[i][j]->GetFunction(Form("funAlpha%dVz%d",i,j));  
         fAlphaErr[i][j] = alphaErrPlots[i][j]->GetFunction(Form("funAlphaErr%dVz%d",i,j));  
         }
      }  
   }
   
   for (int x=1;x<=nEtaBin;x++) {
   
       double totalN=0;
       double totalNErr=0;
          
      for (int y=1;y<=nHitBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
         double val = 0;
	 if (y<1000) {   
	       val = hEverything->GetBinContent(x,y,z);
	       if (val==0) hHadronAccepted->SetBinContent(x,y,z,0);
               if (val==0) continue;
               double beta = betaPlots[x-1][z-1]->GetBinContent(y);
               double alpha,alphaErr;
	       alpha = alphaPlots[x-1][z-1]->GetBinContent(y);
               alphaErr = alphaPlots[x-1][z-1]->GetBinError(y);
	       
	       if (alpha!=0&&(alphaErr/(alpha+0.00001)<5000)&&(useCorrectionFile!=1&&y<40)) {
//		  cout <<"read from plot!"<<endl;
	       } else {
	          alpha = fAlpha[x-1][z-1]->Eval(y);
		  alphaErr = fAlphaErr[x-1][z-1]->Eval(y);
		  if (alphaErr>alpha) alphaErr=alpha;
		  cout <<x<<" "<<y<<" extrapolate! "<<alpha<<" "<<alphaErr<<endl;
	       }
//               if (alphaErr/alpha>2) continue;
               double nCorrected = val*(1-beta)*alpha;
	       hSubtracted->SetBinContent(x,y,z,nCorrected/alpha);
	       hCorrected->SetBinContent(x,y,z, nCorrected);
               double nsig = hEverything->GetBinContent(x,y,z)*(1-beta);
	       
	       double valErr = alphaErr*nsig;

               hCorrected->SetBinError(x,y,z, valErr);
	       correction->Fill(x,y,z,nCorrected,hHadron->GetBinContent(x,y,z),valErr);
               totalN +=nCorrected;
	       totalNErr += valErr*valErr;	
	       cout <<x<<" "<<y<<" "<<z<<" "<<nCorrected<<" "<<valErr<<endl;       
	  }
	  }      
      }
      
      hCorrectedEtaBin->SetBinContent(x,totalN);
      hCorrectedEtaBin->SetBinError(x,sqrt(totalNErr));
      
   }

   

   
   TCanvas *cDNdEtaC = new TCanvas("cDNdEtaC","",400,400);
   double nevent = TrackletTree->GetEntries("nhit1");
   TH1D *hTruth = (TH1D*)hHadron->Project3D("x");
   hTruth->SetName("hTruth");
   TH1D *hTruthAccepted = (TH1D*)hHadronAccepted->Project3D("x");
   
   hTruth->Sumw2();
   hTruthAccepted->Sumw2();
   if (useCorrectionFile == 0) {
      // Calculate Acceptance
      hTruthRatio = (TH1D*)hHadron->Project3D("x");
      hTruthRatio->SetName("hTruthRatio");
      hTruthRatio->Sumw2();
      hTruthRatio->Divide(hTruthAccepted);
      hTruthRatio->SetName("hTruthRatio");
   }
   
   formatHist(hTruth,1,nevent/nEtaBin*6);
   formatHist(hCorrectedEtaBin,2,nevent/nEtaBin*6);
   hTruth->SetAxisRange(0,8,"y");
   hTruth->SetXTitle("#eta (Calculated by Hand)");
   hTruth->SetYTitle("dN/d#eta");
   hTruth->Draw("hist");
   hCorrectedEtaBin->Multiply(hTruthRatio);
   hCorrectedEtaBin->Draw("e same");    

   
   TCanvas *cDNdEta = new TCanvas("cDNdEta","",400,400);
   TH1D *hMeasured = (TH1D*)hCorrected->Project3D("x"); 
   hMeasured->Sumw2();
   
//   formatHist(hTruth,1,nevent/nEtaBin*6);
   formatHist(hMeasured,2,nevent/nEtaBin*6);
   hTruth->SetAxisRange(0,8,"y");
   hTruth->SetXTitle("#eta");
   hTruth->SetYTitle("dN/d#eta");
   hTruth->Draw("hist");
   hMeasured->Multiply(hTruthRatio);
   hMeasured->Draw("e same");    
   
   TCanvas *cRatio = new TCanvas("cRatio","",400,400);
   TH1D *hRatio = (TH1D*)hMeasured->Clone();
   hRatio->Divide(hTruth);
   hRatio->SetXTitle("#eta");
   hRatio->SetYTitle("Ratio");
   hRatio->SetAxisRange(0.9,1.1,"y");
   hRatio->Draw();
   
   TLine *l1 = new TLine(-3,1,3,1);
   l1->Draw();

   TCanvas *cDNdNhit = new TCanvas("cDNdNhit","",400,400);
   TH1D *hTruthHit = (TH1D*)hHadron->Project3D("y");
   TH1D *hMeasuredHit = (TH1D*)hCorrected->Project3D("y"); 
   hTruthHit->Sumw2();
   hMeasuredHit->Sumw2();
   
   formatHist(hTruthHit,1,nevent);
   formatHist(hMeasuredHit,2,nevent);
   hTruthHit->SetAxisRange(0,8,"y");
   hTruthHit->SetXTitle("N_{Hit1} |#eta|<1");
   hTruthHit->SetYTitle("dN/dN_{Hit1}");
   hTruthHit->Draw("hist");
   hMeasuredHit->Draw("e same");    
   
   TCanvas *cRatioNhit = new TCanvas("cRatioNHit","",400,400);
   TH1D *hRatioHit = (TH1D*)hMeasuredHit->Clone();
   hRatioHit->Divide(hTruthHit);
   hRatioHit->SetXTitle("#eta");
   hRatioHit->SetYTitle("Ratio");
   hRatioHit->SetAxisRange(0.7,1.3,"y");
   hRatioHit->Draw();
   
   TLine *l2 = new TLine(0,1,nHitBin,1);
   l2->Draw();

   TCanvas *diff = new TCanvas ("diff","",400,400);
   hSubtracted->Scale(1./nevent); 
   hSubtracted->Draw("col");

   TH1D *hSubtractedpx = (TH1D*)hSubtracted->Project3D("x");
   TH1D *hSubtractedpy = (TH1D*)hSubtracted->Project3D("y");

   hCorrected->Scale(1./nevent);

   hHadron->Scale(1./nevent);
   
   for (int i=0;i<nEtaBin;i++){
      for (int j=0;j<nVzBin;j++){
//      fBeta[i]->Write();
      fAlpha[i][j]->Write();
//      fBetaErr[i]->Write();
      fAlphaErr[i][j]->Write();
      }
   }
   cDNdEta->Write();
   outf->Write();
}

void formatHist(TH1* h, int col, double norm){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.5);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

}
