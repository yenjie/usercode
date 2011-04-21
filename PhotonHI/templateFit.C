#include <iostream>

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
//#include "hiPhotonAna/CutAndBinCollection.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPolynomial.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCut.h"
#include "RooPlot.h"
using namespace RooFit ;


void templateFit()
{
  TFile *infSig = new TFile("ampt/mpa_amptPhotonJetMix15_mar20th.root");
  TTree *signalTree = (TTree*) infSig->Get("NTuples/Analysis");
  TFile *infBck = new TFile("ampt/mpa_amptEmDijetMix15_mar20th.root");
  TTree *backgroundTree = (TTree*) infBck->Get("NTuples/Analysis");

  TFile *infData = new TFile("data/mpaData_march17th.root");
  TTree *dataTree = (TTree*) infData->Get("NTuples/Analysis");

  double varL = -20;
  double varH = 100;
  int nBin = 20;

  char *varName = "cr4+ct4+cc4";
//   char *varName = "(-4.00626572)*eMax/e2x2+(5.07941193)*eMax/e3x3+(0.01855105)*eMax/e5x5+(-0.08046821)*eMax/energy+(-6.36218195)*e2x2/e3x3+(2.44924347)*e2x2/e5x5+(6.06287631)*e2x2/energy+(-3.01610493)*e3x3/e5x5+(-2.19817850)*e3x3/energy+(-1.81670490)*e5x5/energy+5.1982572901";

  TCut eventCut     = "( !TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && vtxNdof > 4 && abs(vtxZ) <= 15)";
  TCut removeSpikeCut = "((isEB && (seedSeverity!=3 && seedSeverity!=4 ) && (seedRecoFlag != 2) && sigmaIetaIeta > 0.002 ) || isEE)";
//  TCut dataCut = "et[0]==et&&cr4/energy<0.04&&r9>0.838716&&abs(eta)<1.47&&rawEnergy/energy>0.5&&cBin>2&&cBin<6"&&eventCut&&removeSpikeCut; 
  TCut dataCut = "et[0]==et&&r9>0.8&&isEB&&et>40&&abs(eta)<1.47&&rawEnergy/energy>0.5&&cBin<10"&&eventCut&&removeSpikeCut; 

  TCut sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0";
  
  RooRealVar var(varName,"test",varL,varH) ;
  RooRealVar bckFrac("bckFrac","test",0.1,0.8) ;
 
  
  TH1D *hSignal = new TH1D("hSignal","",nBin,varL,varH);
  TH1D *hBackground = new TH1D("hBackground","",nBin,varL,varH);
  TH1D *hData = new TH1D("hData","",nBin,varL,varH);
  signalTree->Draw(Form("%s>>hSignal",varName), dataCut&&sigCut);
//  backgroundTree->Draw(Form("%s>>hBackground",varName), dataCut);
  dataTree->Draw(Form("%s>>hBackground",varName), dataCut&&"sigmaIetaIeta>0.012");

//  signalTree->Draw(Form("%s>>hData",varName), "isEB","",100000);
  dataTree->Draw(Form("%s>>hData",varName), dataCut&&"isEB&&et==et[0]&&et>20&&sigmaIetaIeta<0.012","");
  
  
//  RooPolynomial p("p","p",x,RooArgList(RooConst(0.01),RooConst(-0.01),RooConst(0.0004)));

//  TCut hiSpikeCut       = Form("(  %s < 0.90 && abs(seedTime)<4  && sigmaIetaIeta>0.002 )  || isEE",swissCrx.Data());

  RooDataHist* signalHist = new RooDataHist("signalHist","",RooArgList(var),hSignal,1);
  RooDataHist* backgroundHist = new RooDataHist("backgroundHist","",RooArgList(var),hBackground,1);
  RooDataHist data("data","",RooArgList(var),hData,1);
  

  // C r e a t e   h i g h   s t a t s   h i s t o g r a m
  // -----------------------------------------------------

  // Sample 100000 events from p
  var.setBins(10) ;
  //RooDataSet* signalPDF = p.generate(x,100000) ;

  RooHistPdf signalPDF("signalPDF","signalPDF",var,*signalHist,3) ;
  RooHistPdf backgroundPDF("backgroundPDF","backgroundPDF",var,*backgroundHist,3) ;

  // Plot unbinned data and histogram pdf overlaid
  RooPlot* frame2 = var.frame(Title("High stats histogram pdf with interpolation"),Bins(100)) ;
  RooAddPdf model("model","sig+bck",RooArgList(backgroundPDF,signalPDF),bckFrac);
  model.fitTo(data,Minos(1));

//  RooAbsReal* nll = model.createNLL(*data);

  data.plotOn(frame2);
  model.plotOn(frame2,LineColor(kRed));
  model.plotOn(frame2,Components(RooArgSet(backgroundPDF)),DrawOption("F"),FillColor(kGreen),FillStyle(3001));
//  signalPDF.plotOn(frame2);
//  backgroundPDF.plotOn(frame2);

  TCanvas* c = new TCanvas("rf706_histpdf","rf706_histpdf",800,400) ;  
  frame2->GetYaxis()->SetTitleOffset(1.8) ; frame2->Draw() ;


}
