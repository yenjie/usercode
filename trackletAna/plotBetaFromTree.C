#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
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

#define plotDEta true

void formatHist(TH1* h, int col = 1, double norm = 1);
void plotBetaFromTree(char* filename,char *toyMC="pseudoExp.root",int useCorrectionFile = 0,char *mycut="",bool useMC = false)
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

   TH2F *hEverything = new TH2F("hEverything","Everything",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h2 = new TH2F("h2","Reproduced Background",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h2MC = new TH2F("h2MC","Reproduced Background from ToyMC",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h3 = new TH2F("h3","Background Subtracted",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h4 = new TH2F("h3","(1-beta)*Everything",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *hSubtracted = new TH2F("hSubtracted","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *hHadron = new TH2F("hHadron","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *hCorrected = new TH2F("hCorrected","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH1F *alphaPlots[nEtaBin];
   TH1F *betaPlots[nEtaBin];
   TH1F *alphaErrPlots[nEtaBin];
   TH1F *betaErrPlots[nEtaBin];
   TH1F *hDEtaAll[nEtaBin];
   TH1F *hDEtaSig[nEtaBin];
   TH1F *hDEtaBck[nEtaBin];

   TH1F *hCorrectedEtaBin = new TH1F("hCorrectedEtaBin","Corrected",nEtaBin,-3,3);
   

   TF1 *funBeta[nEtaBin];
   TF1 *funAlpha[nEtaBin];
   TF1 *funBetaErr[nEtaBin];
   TF1 *funAlphaErr[nEtaBin];
   TF1 *fBeta[nEtaBin];
   TF1 *fBetaErr[nEtaBin];
   TF1 *fAlpha[nEtaBin];
   TF1 *fAlphaErr[nEtaBin];
      
   // Prepare histograms
   for (int i=0;i<nEtaBin;i++) {
      alphaPlots[i] = new TH1F(Form("alpha%d",i),"",nHitBin,-0.5,nHitBin-0.5);
      alphaErrPlots[i] = new TH1F(Form("alphaErr%d",i),"",nHitBin,-0.5,nHitBin-0.5);
      betaPlots[i] = new TH1F(Form("beta%d",i),"",nHitBin,-0.5,nHitBin-0.5);
      betaErrPlots[i] = new TH1F(Form("betaErr%d",i),"",nHitBin,-0.5,nHitBin-0.5);
      hDEtaAll[i] = new TH1F(Form("hDEtaAll%d",i),"",nDEtaBin,0,4);
      hDEtaBck[i] = new TH1F(Form("hDEtaBck%d",i),"",nDEtaBin,0,4);
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

/*
   TCanvas *cDelteEta = new TCanvas("cDeltaEta","",600,600);
   TrackletTree->Draw(Form("abs(deta)>>hDEtaAll%d",0),signalRegionInPhi && Form("%f<eta1&&eta1<%f",-2.5,2.5)); 
   TrackletTree->Draw(Form("abs(deta)>>hDEtaBck%d",0),sideBandRegionInPhi && Form("%f<eta1&&eta1<%f",-2.5,2.5)); 
   hDEtaSig[0] = (TH1F*)hDEtaAll[0]->Clone();
   formatHist(hDEtaAll[0],1);
   formatHist(hDEtaBck[0],4);
   formatHist(hDEtaSig[0],2);
   hDEtaAll[0]->SetXTitle("#Delta#eta");
   hDEtaAll[0]->SetYTitle(Form("# %.1f < #eta < %.1f",-2.5,2.5)); 
      
   hDEtaSig[0] -> Add(hDEtaBck[0],-1);
      
   hDEtaAll[0]->Draw("e");
   hDEtaBck[0]->Draw("e same");
   hDEtaSig[0]->Draw("e same");
  */

/*
   TCanvas *cDelteEta = new TCanvas("cDeltaEta","",800,800);
   TPad *p0 = new TPad("p0","",0,0,1,1);
   p0->Divide(3,4,0.01,0.01);
   p0->Draw();   


   for (int i=0;i<nEtaBin;i++) {
      p0->cd(i+1);
      double etaMin = -3. + i * 6.0 / (double)nEtaBin;
      double etaMax = -3. + (i+1) * 6.0 / (double)nEtaBin;
      TrackletTree->Draw(Form("abs(deta)>>hDEtaAll%d",i),signalRegionInPhi && Form("%f<eta1&&eta1<%f",etaMin,etaMax)); 
      TrackletTree->Draw(Form("abs(deta)>>hDEtaBck%d",i),sideBandRegionInPhi && Form("%f<eta1&&eta1<%f",etaMin,etaMax)); 
      hDEtaSig[i] = (TH1F*)hDEtaAll[i]->Clone();

      formatHist(hDEtaAll[i],1);
      formatHist(hDEtaBck[i],4);
      formatHist(hDEtaSig[i],2);
      hDEtaAll[i]->SetXTitle("#Delta#eta");
      hDEtaAll[i]->SetYTitle(Form("# %.1f < #eta < %.1f",etaMin,etaMax)); 
      
      hDEtaSig[i] -> Add(hDEtaBck[i],-1);
      
      hDEtaAll[i]->Draw();
      hDEtaBck[i]->Draw("same");
      hDEtaSig[i]->Draw("same");
      cout <<i<<endl;
   }
*/
   TCanvas *cBetas = new TCanvas ("cBetas","",400,400);
   TrackletTree->Draw("mult:eta1>>hEverything",signalRegion);

   h3->SetXTitle("#eta");
   h3->SetYTitle("#Delta#eta");
   if (useMC) {
     cout <<"Use ToyM"<<endl;
     ntmatchedMC->Draw("nhit1:eta1>>h2",sideBandRegionEta&&cut);
     int n = ntmatched->GetEntries(sideBandRegionEta&&cut);
     int n2 = ntmatchedMC->GetEntries(sideBandRegionEta&&cut);
     h2->Scale(1./n2*n); 
   } else {
   TrackletTree->Draw("mult:eta1>>h2",sideBandRegionEtaSignalRegion&&cut);   
   }

   hEverything->Sumw2();
   h2->Sumw2();
   h3->Sumw2();


   // Beta calculation
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         double beta = 0;
	 if (hEverything->GetBinContent(x,y)!=0&&h2->GetBinContent(x,y)!=0) 
	 {   
	       beta = h2->GetBinContent(x,y)/hEverything->GetBinContent(x,y);
	       
               double e1 = hEverything->GetBinError(x,y)/hEverything->GetBinContent(x,y);
	       double e2 = h2->GetBinError(x,y)/h2->GetBinContent(x,y);
               double betaErr = beta* sqrt(e2*e2);
               betas->Fill(x,y,beta,betaErr);
	       betaPlots[x-1]->SetBinContent(y,beta);
	       betaPlots[x-1]->SetBinError(y,betaErr);
	       betaErrPlots[x-1]->SetBinContent(y,betaErr);
               //correction1->Fill(x,y,val,hHadron->GetBinContent(x,y),valErr);

	       beta = hEverything->GetBinContent(x,y)-h2->GetBinContent(x,y);
               hSubtracted->SetBinContent(x,y, beta);
	       double e3 = hEverything->GetBinError(x,y);
	       double e4 = h2->GetBinError(x,y);
               betaErr = sqrt(e1*e1+e2*e2);
               h3->SetBinContent(x,y, beta);
	       h3->SetBinError(x,y, betaErr);
	       hSubtracted->SetBinError(x,y, sqrt(e3*e3+e4*e4));
	 }      
      }
   }
   h3->Draw("col");

   // Charged Hadrons:
   TCanvas *cHadrons = new TCanvas("cHadrons","",400,400);
   hHadron->SetXTitle("#eta");
   hHadron->SetYTitle("N_{hit}^{Layer1} |#eta|<1");
   TrackletTree->Draw("mult:eta>>hHadron","abs(eta)<3","col");
   

   TCanvas *cBetaFit = new TCanvas("cBetaFit","",800,800);
   TPad *p1 = new TPad("p1","",0,0,1,1);
   p1->Divide(3,4,0.01,0.01);
   p1->Draw();   

   
   for (int i=0;i<nEtaBin;i++) 
   {
      //c[i]= new TCanvas (Form("c%d",i),"",400,400);
      p1->cd(i+1);
      formatHist(betaPlots[i],2,1);
      funBeta[i] = new TF1(Form("funBeta%d",i),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
      funBetaErr[i] = new TF1(Form("funBetaErr%d",i),"[0]+[1]/(x+0.5)+[2]*exp([3]*x)",0,100);
      double etaMin = i*0.5 -3;
      double etaMax = i*0.5 -3+0.5;

      //betaPlots[i]->Fit(Form("funBeta%d",i),"m E");
      betaPlots[i]->SetXTitle("N_{Hits}");
      betaPlots[i]->SetYTitle(Form("#beta %.1f < #eta < %.1f",etaMin,etaMax)); 
      betaPlots[i]->SetAxisRange(0,1,"Y");
      betaPlots[i]->Draw("p");
      
      //fBeta[i] = betaPlots[i]->GetFunction(Form("funBeta%d",i));
     
   }   


   // alpha calculation
   if (useCorrectionFile == 0) {   
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         double val = 0;
	 if (hEverything->GetBinContent(x,y)!=0&&hHadron->GetBinContent(x,y)!=0) 
	 {   
	       val = hEverything->GetBinContent(x,y);
               double beta = betaPlots[x-1]->GetBinContent(y);
	       h4->SetBinContent(x,y, val*(1-beta));
	       double e1 = hEverything->GetBinError(x,y);
               double e2 = h2->GetBinError(x,y);
	       double nsig = val * (1-beta);
               double valErr = sqrt(e1*e1+e2*e2);
               h4->SetBinError(x,y, valErr);
               if (beta==1) continue;
	       double truth    = hHadron->GetBinContent(x,y);
	       double truthErr = hHadron->GetBinError(x,y);
	       double alphaErr = truth/nsig* sqrt(valErr/nsig*valErr/nsig+truthErr/truth*truthErr/truth*0);
	       cout <<"beta: "<<beta<<" "<<x<<" "<<y<<" alpha:"<<truth/val/(1-beta)<<" +-"<<alphaErr<<" from "<<truth<<" "<<truthErr<<" "<<val<<" "<<valErr<<" "<<endl;
               if (beta!=1) {
   	          alphaPlots[x-1]->SetBinContent(y,truth/val/(1-beta));
	          alphaPlots[x-1]->SetBinError(y,alphaErr);
	          alphaErrPlots[x-1]->SetBinContent(y,alphaErr);
	       }
	 }      
      }
   }
   }

   // corrected calculation
   
   if (useCorrectionFile != 0) {
      for (int i=0;i<nEtaBin;i++)
      {
         fAlpha[i] = (TF1*) fcorrection->FindObjectAny(Form("funAlpha%d",i));
	 fAlphaErr[i] = (TF1*) fcorrection->FindObjectAny(Form("funAlphaErr%d",i));
         alphaPlots[i] = (TH1F*) fcorrection->FindObjectAny(Form("alpha%d",i));
      }
   } else {

      TCanvas *cAlphaFit = new TCanvas("cAlphaFit","",800,800);
      TPad *p2 = new TPad("p2","",0,0,1,1);
      p2->Divide(3,4,0.01,0.01);
      p2->Draw();   

      for (int i=0;i<nEtaBin;i++) 
      {
         // c[i]= new TCanvas (Form("c%d",i),"",400,400);
         formatHist(alphaPlots[i],2,1);
	 p2->cd(i+1);
         funAlpha[i] = new TF1(Form("funAlpha%d",i),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
         funAlphaErr[i] = new TF1(Form("funAlphaErr%d",i),"[0]+[1]/(x+0.5)+[2]*exp([3]*x)",0,100);
         double etaMin = i*0.5 -3;
         double etaMax = i*0.5 -3+0.5;
         alphaPlots[i]->Fit(Form("funAlpha%d",i),"m E","",0,60);
         alphaPlots[i]->SetXTitle("N_{Hits}");
         alphaPlots[i]->SetYTitle(Form("#alpha %.1f < #eta < %.1f",etaMin,etaMax)); 
         alphaPlots[i]->SetAxisRange(0.9,2.0,"y");
	 if (i <2 || i >9) alphaPlots[i]->SetAxisRange(4,10,"y");
	 
	 alphaErrPlots[i]->Fit(Form("funAlphaErr%d",i),"m E","",0,30);
         alphaPlots[i]->Draw("p");
         fAlpha[i] = alphaPlots[i]->GetFunction(Form("funAlpha%d",i));  
         fAlphaErr[i] = alphaErrPlots[i]->GetFunction(Form("funAlphaErr%d",i));  

      }  
   }
   
   for (int x=1;x<=nEtaBin;x++) {
   
       double totalN=0;
       double totalNErr=0;
       
      for (int y=1;y<=nHitBin;y++) {
         double val = 0;
	 if (y<1000) {   
	       val = hEverything->GetBinContent(x,y);
               if (val==0) continue;
               double beta = betaPlots[x-1]->GetBinContent(y);
               double alpha,alphaErr;
	       alpha = alphaPlots[x-1]->GetBinContent(y);
               alphaErr = alphaPlots[x-1]->GetBinError(y);
	       
	       if (alpha!=0&&(alphaErr/(alpha+0.00001)<5000)&&(useCorrectionFile!=1||y<50)) {
//		  cout <<"read from plot!"<<endl;
	       } else {
	          alpha = fAlpha[x-1]->Eval(y);
		  alphaErr = fAlphaErr[x-1]->Eval(y);
		  if (alphaErr>alpha) alphaErr=alpha;
//		  cout <<x<<" "<<y<<" extrapolate! "<<alpha<<" "<<alphaErr<<endl;
	       }
//               if (alphaErr/alpha>2) continue;
               double nCorrected = val*(1-beta)*alpha;
	       hSubtracted->SetBinContent(x,y,nCorrected/alpha);
	       hCorrected->SetBinContent(x,y, nCorrected);
               
	       double nsig = hEverything->GetBinContent(x,y)*(1-beta);
	       
	       double valErr = alphaErr*nsig;

               hCorrected->SetBinError(x,y, valErr);
	       correction->Fill(x,y,nCorrected,hHadron->GetBinContent(x,y),valErr);
               totalN +=nCorrected;
	       totalNErr += valErr*valErr;	
	       cout <<x<<" "<<y<<" "<<nCorrected<<" "<<valErr<<endl;       
	  }      
      }
      
      hCorrectedEtaBin->SetBinContent(x,totalN);
      hCorrectedEtaBin->SetBinError(x,sqrt(totalNErr));
      
   }

   
   TCanvas *cDNdEtaC = new TCanvas("cDNdEtaC","",400,400);
   double nevent = TrackletTree->GetEntries("nhit1");
   TH1D *hTruth = hHadron->ProjectionX();
   hTruth->Sumw2();
   
   formatHist(hTruth,1,nevent/nEtaBin*6);
   formatHist(hCorrectedEtaBin,2,nevent/nEtaBin*6);
   hTruth->SetAxisRange(0,8,"y");
   hTruth->SetXTitle("#eta (Calculated by Hand)");
   hTruth->SetYTitle("dN/d#eta");
   hTruth->Draw("hist");
   hCorrectedEtaBin->Draw("e same");    

   
   TCanvas *cDNdEta = new TCanvas("cDNdEta","",400,400);
   TH1D *hMeasured = hCorrected->ProjectionX(); 
   hMeasured->Sumw2();
   
//   formatHist(hTruth,1,nevent/nEtaBin*6);
   formatHist(hMeasured,2,nevent/nEtaBin*6);
   hTruth->SetAxisRange(0,8,"y");
   hTruth->SetXTitle("#eta");
   hTruth->SetYTitle("dN/d#eta");
   hTruth->Draw("hist");
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
   TH1D *hTruthHit = hHadron->ProjectionY();
   TH1D *hMeasuredHit = hCorrected->ProjectionY(); 
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

   TH1D *hSubtractedpx = hSubtracted->ProjectionX();
   TH1D *hSubtractedpy = hSubtracted->ProjectionY();

   hCorrected->Scale(1./nevent);

   hHadron->Scale(1./nevent);
   
   for (int i=0;i<nEtaBin;i++){
//      fBeta[i]->Write();
      fAlpha[i]->Write();
//      fBetaErr[i]->Write();
      fAlphaErr[i]->Write();
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
