#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile.h>

#include "/home/yjlee/rootTool/histFunction2.C"

void scaleBin(TH1F *h,int i,double width)
{
   double val = h->GetBinContent(i)/width;
   double valErr = h->GetBinError(i)/width;
   h->SetBinContent(i,val);
   h->SetBinError(i,valErr);
}

double doFit(TTree *tData,TTree* tSig,TTree *tBck,TCut myCut, double minEt, double maxEt,int useMethod, double &result, double &resultErr)
{

   // useMethod = 1;  
   // 1 = iso, 
   // 2 = shape
   // Binning
   int nBin = 20;
   double binL = -10;
   double binH = 10;
   char *var = "((0.00154394)*ct1PtCut+(-0.01855323)*ct2PtCut+(0.00915530)*ct3PtCut+(-0.00897927)*ct4PtCut+(0.00155425)*ct5PtCut+(0.01150634)*cr1+(-0.01122315)*cr2+(-0.00019952)*cr3+(-0.01024246)*cr4+(-0.00749438)*cr5+(0.01906002)*cc1+(-0.01589967)*cc2+(-0.01249385)*cc3+(-0.00664819)*cc4+(-0.00252866)*cc5+0.320703717043-0.324143-2.29533e-7*hf)/(0.0247854+(1.17691e-6)*hf)";

   TCut etCut = Form("abs(eta)<1.45&&et>%f&&et<%f",minEt,maxEt);
   TCut sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0";

  /*
   TCanvas *c2 = new TCanvas("c2","",600,600);
   
   TProfile *p = new TProfile("p","",30,0,0.03);
   TF1 *l1 = new TF1("l1","[0]+[1]*x");
   double coeff0 = l1->GetParameter(0);
   double coeff = l1->GetParameter(1);

   c2->Update();
   c2->SaveAs(Form("plot/fitSlope-%.0f-%.0f-%d.C",minEt,maxEt,useMethod));
   c2->SaveAs(Form("plot/fitSlope-%.0f-%.0f-%d.gif",minEt,maxEt,useMethod));
*/
   char *title = Form("%.0f<E_{T}<%.0f GeV   S_{R} (GeV)",minEt,maxEt);
   TCut dataCut = "sigmaIetaIeta<0.012";//"sieie43<0.012";
   TCut dataSidebandCut = "sigmaIetaIeta>0.012&&sigmaIetaIeta<0.013";//"sigmaIetaIeta>0.014&&sigmaIetaIeta<0.016";
//   dataCut = "ecalIso-compEcalIso+hcalIso-compHcalIso+trackIso-compTrackIso<5";

   if (useMethod==2) {
     binL = 0.002;
     binH = 0.026;
     var = "sieie46";
     title = Form("%.0f<E_{T}<%.0f GeV   #sigma_{i#eta i#eta} ",minEt,maxEt);
//     dataCut = "ecalIso-compEcalIso+hcalIso-compHcalIso+trackIso-compTrackIso<5";
     dataCut = "";//"(trackIso-compTrackIso+cr4+cc4)<30";
     dataSidebandCut="((0.00154394)*ct1PtCut+(-0.01855323)*ct2PtCut+(0.00915530)*ct3PtCut+(-0.00897927)*ct4PtCut+(0.00155425)*ct5PtCut+(0.01150634)*cr1+(-0.01122315)*cr2+(-0.00019952)*cr3+(-0.01024246)*cr4+(-0.00749438)*cr5+(0.01906002)*cc1+(-0.01589967)*cc2+(-0.01249385)*cc3+(-0.00664819)*cc4+(-0.00252866)*cc5+0.320703717043-0.324143-2.29533e-7*hf)/(0.0247854+(1.17691e-6)*hf)<-5";
     
   }

   // Histograms
   TH1F *hData = new TH1F("hData","",nBin,binL,binH);
   TH1F *hSig = new TH1F("hSig","",nBin,binL,binH);
   TH1F *hBck = new TH1F("hBck","",nBin,binL,binH);

   tData->Draw(Form("%s>>hData",var),etCut&&myCut&&dataCut);
   tSig->Draw(Form("%s+0.00025>>hSig",var),   etCut&&myCut&&sigCut&&dataCut*"(49.7014*(exp((-0.943492e-3)*cBin*cBin*cBin+0.09106*cBin*cBin-0.44891*cBin+2.2789))+31.5117)");
//   tBck->Draw(Form("%s>>hBck",var), etCut&&myCut&&dataCut&&(!sigCut));
   //if (useMethod==2) 
 tData->Draw(Form("%s>>hBck",var), etCut&&myCut&&dataSidebandCut);


   TH1F *hFinal = (TH1F*)hData->Clone();
   hFinal->SetName("hFinal");

   histFunction2 *myfun = new histFunction2(hSig,hBck);
   TF1 *f = new TF1("f",myfun,&histFunction2::evaluate,binL,binH,2);
   f->SetParameters(hData->GetEntries(),0.9);
   f->SetParLimits(1,0,1);
//   f->FixParameter(0,hData->GetEntries());
   hData->Fit("f","LL m");
   hData->Draw("pe");
   
   result=0;
   resultErr=0;
   if (hData->GetEntries()==0) return 0;
   
   double nev = f->GetParameter(0);
   double ratio = f->GetParameter(1);
   double ratioErr = f->GetParError(1);


   TH1F *hSigPdf = (TH1F*)hSig->Clone();
   hSigPdf->SetName("hSigPdf");
   hSigPdf->Scale(nev*ratio/hSigPdf->GetEntries());

   TH1F *hBckPdf = (TH1F*)hBck->Clone();
   hBckPdf->SetName("hBckPdf");
   hBckPdf->Scale(nev*(1-ratio)/hBckPdf->GetEntries());
   hSigPdf->SetLineColor(2);
   hSigPdf->Add(hBckPdf);

   TCanvas *c = new TCanvas("c","",600,600);
   hSigPdf->SetXTitle(title);
   hSigPdf->SetNdivisions(505);
   hSigPdf->Draw("");
   hFinal->Draw("e same");
//   hSigPdf->Draw("same");
   hBckPdf->SetFillColor(1);
   hBckPdf->SetFillStyle(3004);
   hBckPdf->Draw("same");
   hSigPdf->Draw("same");
   
   TLegend *leg ;
   if (useMethod==2) {
      leg = new TLegend(0.4,0.7,0.9,0.9);
   } else {
      leg = new TLegend(0.2,0.7,0.7,0.9);
   }
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->AddEntry(hFinal,"Data","pl");
   leg->AddEntry(hSigPdf,"Signal+Background","l");
   leg->AddEntry(hBckPdf,"Background","l");
   leg->Draw();
   c->Update();
   c->SaveAs(Form("plot/fit-%.0f-%.0f-%d.C",minEt,maxEt,useMethod));
   c->SaveAs(Form("plot/fit-%.0f-%.0f-%d.gif",minEt,maxEt,useMethod));
   if (ratio>0) {
     result = nev*ratio;
     resultErr = nev*ratio*sqrt(1./nev+(ratioErr/ratio)*(ratioErr/ratio));
     cout <<minEt<<" "<<maxEt<<"Nsig : "<<result<<" +- "<<resultErr<<endl;
   }
   
   
   return 1;
}

void myFit(char* infname = "data/mpaData_march17th.root")
{

   TFile *infData = new TFile(infname);

   TTree *tData = (TTree*)infData->FindObjectAny("Analysis");
   TTree *tDataGen = (TTree*)infData->FindObjectAny("Generator");

   TFile *infSig = new TFile("PythiaData/mpaPhotonJet15_mix_hiData_april04.root");
   TTree *tSig = (TTree*)infSig->FindObjectAny("Analysis");
   TTree *tSigGen = (TTree*)infSig->FindObjectAny("Generator");

   TFile *infBck = new TFile("ampt/mpa_amptEmDijetMix15_mar20th.root");
   TTree *tBck = (TTree*)infBck->FindObjectAny("Analysis");

   const int nEtBin = 5;
   double myBin[nEtBin+1] = {20,25,30,40,50,80};

   TH1F *hEtAll = new TH1F("hEtAll","",nEtBin,myBin);
   TH1F *hEt = new TH1F("hEt","",nEtBin,myBin);
   TH1F *hEtIso = new TH1F("hEtIso","",nEtBin,myBin);
   TCut eventCut     = "( !TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && abs(vtxZ) <= 15)";
   TCut removeSpikeCut = "((isEB && (seedSeverity!=3 && seedSeverity!=4 ) && (seedRecoFlag != 2) && sigmaIetaIeta > 0.002 ) || isEE)";

   TCut myCut = "et[0]==et&&cBin<8&&cr4/energy<0.4&&abs(eta)<1.47&&rawEnergy/energy>0.5"&&eventCut&&removeSpikeCut; 

   TCut sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0";

   tData->Draw("et>>hEtAll","abs(eta)<1.47"&&myCut);

   TH1F *hEtGen = new TH1F("hEtGen","",nEtBin,myBin);
   if (tDataGen!=0) tDataGen->Draw("pt>>hEtGen","abs(eta)<1.47 && calIsoDR04<5");

   TH1F *hEtTruth = new TH1F("hEtTruth","",nEtBin,myBin);
   tSigGen->Draw("pt>>hEtTruth","abs(eta)<1.47 && calIsoDR04<5");

   TH1F *hEtEff = new TH1F("hEtEff","",nEtBin,myBin);
   tSig->Draw("et>>hEtEff",sigCut&&myCut);
   hEtEff->Sumw2();
   hEtTruth->Sumw2();
   hEtEff->Divide(hEtTruth);
   
   for (int i=0;i<nEtBin;i++)
   {
      double result,resultErr;
      doFit(tData,tSig,tBck,myCut,myBin[i],myBin[i+1],2,result,resultErr);
      hEt->SetBinContent(i+1,result);
      hEt->SetBinError(i+1,sqrt(resultErr*resultErr+0.1*result*0.1*result));

      doFit(tData,tSig,tBck,myCut,myBin[i],myBin[i+1],1,result,resultErr);
      hEtIso->SetBinContent(i+1,result);
      hEtIso->SetBinError(i+1,sqrt(resultErr*resultErr+0.1*result*0.1*result));
   }

   for (int i=0;i<nEtBin;i++)
   {
      double binWidth = myBin[i+1]-myBin[i];
      scaleBin(hEt,i+1,binWidth);
      scaleBin(hEtIso,i+1,binWidth);
      scaleBin(hEtAll,i+1,binWidth);
      scaleBin(hEtGen,i+1,binWidth);
   }

   TCanvas *c = new TCanvas ("c","",600,600);
   c->SetLogy();
   hEt->SetXTitle("E_{T} (GeV)");
   hEt->SetYTitle("dN/dE_{T} ");
   hEtAll->SetXTitle("E_{T} (GeV)");
   hEtAll->SetYTitle("dN/dE_{T} ");
   hEtAll->SetAxisRange(0.1,1000,"Y");
   hEtAll->Draw("hist");
   TH1F *hEtBck = (TH1F*)hEtAll->Clone();
   hEtBck->SetName("hEtBck");
   hEtBck->Add(hEt,-1);
   hEt->Draw("same");
   hEt->SetLineColor(2);
   hEt->SetMarkerColor(2);
   hEtBck->SetFillStyle(3004);
   hEtBck->Draw("hist same");
   hEtIso->SetLineColor(4);
   hEtIso->SetMarkerColor(4);
   hEtIso->SetMarkerStyle(4);
   hEtIso->Draw("same");
   hEtGen->SetLineColor(6);
   hEtGen->SetMarkerColor(6);
   hEtGen->Draw("hist same");

   TLegend *leg ;
   leg = new TLegend(0.2,0.7,0.9,0.9);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->AddEntry(hEtAll,"All ","pl");
   leg->AddEntry(hEtIso,"Sigmal from Isolation method","pl");
   leg->AddEntry(hEt,"Signal from Shower shape method","pl");
   leg->AddEntry(hEtBck,"Background from Shower shape method","l");
   leg->Draw();


   TCanvas *c3 = new TCanvas ("c3","",600,600);
   hEtEff->SetXTitle("E_{T} (GeV)");
   hEtEff->SetYTitle("Efficiency");
   hEtEff->Draw();
   
   
   TCanvas *c2 = new TCanvas ("c2","",600,600);

   TH1F *hEtIsoCorrected = (TH1F*) hEtIso->Clone();
   hEtIsoCorrected->SetName("hEtIsoCorrected");
   TH1F *hEtCorrected = (TH1F*) hEt->Clone();
   hEtCorrected->SetName("hEtCorrected");
   hEtIsoCorrected->Divide(hEtEff);
   hEtCorrected->Divide(hEtEff);
   
   hEtCorrected->Draw();
   hEtIsoCorrected->Draw("same");
   hEtGen->Draw("hist same");

   TLegend *leg2 ;
   leg2 = new TLegend(0.2,0.7,0.9,0.9);
   leg2->SetFillStyle(0);
   leg2->SetBorderSize(0);
   leg2->AddEntry(hEtAll,"All ","pl");
   leg2->AddEntry(hEtIso,"Isolation method","pl");
   leg2->AddEntry(hEt,"Shower shape method","pl");
   leg2->AddEntry(hEtGen,"MC Truth","l");
   leg2->Draw();

   c->SaveAs("plot/result.gif");   
   c->SaveAs("plot/result.C");   
   c2->SaveAs("plot/resultCorrected.gif");   
   c2->SaveAs("plot/resultCorrected.C");    
   c3->SaveAs("plot/eff.gif");   
   c3->SaveAs("plot/eff.C");   

   
//   infData->Close();
//   infSig->Close();
//   infBck->Close();
   
   
   
}

