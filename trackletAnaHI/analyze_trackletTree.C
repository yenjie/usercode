#include <TFile.h>
#include <TH2.h>
#include <TF1.h>
#include <TTimeStamp.h>
#include <TRandom.h>
#include "Tracklet.h"

//using namespace std;

void analyze_trackletTree(char * infile, char * outfile = "output.root", long startEntry = 0, long endEntry = 1000000000,
                          int addL1Bck = 0, int addL2Bck = 0, int addL3Bck = 0,
   		          bool reWeight = 0,         // reweight to Run 123596 vtx distribution
                          bool useRandomVertex= 0,
			  bool cutOnClusterSize = 1,
			  bool mimicPixelCounting = 0, 
			  bool reproduceBck = 0,
			  int makeVzCut = 0,
			  double splitProb = 0, double dropProb = 0, 
			  double nPileUp = 0,
			  double beamHaloRatio = 0.0,
			  bool putBeamHalo = false, char * beamHaloFile =
    			             "DataSample/PixelTree-Run123151-Full.root",
                          double smearVertex = 0,
			  bool putPixelTree = 0,
			  bool useKKVertex = 0,
			  bool useNSD = 0,
			  bool checkDoubleEvent = 0,
			  bool reweightMultiplicity = 0
			  )
{
  // Set Random Seed
  TTimeStamp myTime;
  gRandom->SetSeed(myTime.GetNanoSec());
  cout <<"Randomize "<<gRandom->Rndm()<<endl;
  
  

  // Input file =======================================================================================
  TFile* inf = new  TFile(infile);
  TTree* t = dynamic_cast<TTree*>(inf->FindObjectAny("PixelTree"));
  TFile* beamHaloInf;
  TTree* beamHaloTree;
  if (putBeamHalo) {
      cout <<"Add Beam Halo Background!!!!"<<endl;
      cout <<"Ratio = "<<beamHaloRatio<<endl;
      cout <<"File = "<<beamHaloFile<<endl;
      beamHaloInf = new TFile(beamHaloFile);
      beamHaloTree = dynamic_cast<TTree*>(beamHaloInf->FindObjectAny("PixelTree"));
  }
  if (reproduceBck) cout <<"REPRODUCE BACKGROUND MODE!!!!!!!"<<endl;
  
  // Output file =======================================================================================
  TFile* outf = new TFile(outfile,"recreate");
  outf->SetCompressionLevel(9);

  TTree *trackletTree12 = new TTree("TrackletTree12","Tree of Reconstructed Tracklets");
  TTree *trackletTree13 = new TTree("TrackletTree13","Tree of Reconstructed Tracklets");
  TTree *trackletTree23 = new TTree("TrackletTree23","Tree of Reconstructed Tracklets");
  TTree *outTree;
  if (putPixelTree) {
     outTree = t->CloneTree();    
     cout <<"Put in Pixel Tree"<<endl;
  }
  int zbins = 1;
  int hitbins = 100;
  int vertexHitRegion = 500000;
  int nPrint = 100;
  double mult = 0;
  bool isMC = 0;
  bool isHI = 0;
  double vzShift = 0;
  
  int nEtaBin = 10;
  int nDeltaEtaBin = 40;    // 1
  int nDeltaPhiBin = 40;   //64
  int nPtBin = 50;

  double EtaBins[nEtaBin+1];
  double DeltaEtaBins[nDeltaEtaBin+1];
  double DeltaPhiBins[nDeltaPhiBin+1];
  double PtBins[nPtBin+1];
  
  for (int i=0;i<=nEtaBin;i++)      { EtaBins[i]=-2.5+(i)*5./nEtaBin; }
  for (int i=0;i<=nDeltaEtaBin;i++) { DeltaEtaBins[i]=-0.25+(i)*0.5/nDeltaEtaBin;}
  for (int i=0;i<=nDeltaPhiBin;i++) { DeltaPhiBins[i]=-0.25+(i)*0.5/nDeltaPhiBin; }
  for (int i=0;i<=nPtBin;i++)       {PtBins[i]=0+(i)*5.0/nPtBin; }
  
  TH1F *hRatio;
  if (reweightMultiplicity) {
  //     hRatio = getMultRatio();
  }
  
  // Selection on Hits and events =====================================================================
  SelectionCriteria cuts;
  cuts.drCut   = 0.4;      // to remove double hit
  cuts.dPhiCut = 0.04;     // to remove double hit
  cuts.dEtaCut = 0.2;     // to remove double hit
  cuts.vzCut   = 10;       // vertex cut
 

  // Settings =========================================================================================
  cuts.verbose_ = false;
  cuts.useDeltaPhi_ = true;
  cuts.useDeltaRho_ = false;
  cuts.checkSecondLayer_ = true;
  
  // Tracklet Tree data format ========================================================================
  TrackletData tdata12;
  TrackletData tdata13;
  TrackletData tdata23;

  setTrackletTreeBranch(trackletTree12,tdata12);
  setTrackletTreeBranch(trackletTree13,tdata13);
  setTrackletTreeBranch(trackletTree23,tdata23);

  // Hit vectors & pdfs ===================================================================================
  TH3D* nhits = new TH3D("nhits","",100,0,100,100,0,100,100,0,100);

  TH3D* hLayer1Hit = new TH3D("hLayer1Hit","",75,0,15,60,-3,3,64,-3.2,3.2);
  TH3D* hLayer2Hit = new TH3D("hLayer2Hit","",75,0,15,60,-3,3,64,-3.2,3.2);
  TH3D* hLayer3Hit = new TH3D("hLayer3Hit","",75,0,15,60,-3,3,64,-3.2,3.2);

  // Prepare hit spectra for random hit 
  cout <<"Projecting...1"<<endl;
  if (addL1Bck!=0) t->Project("hLayer1Hit","phi1:eta1:r1");
  cout <<"Projecting...2"<<endl;
  if (addL2Bck!=0) t->Project("hLayer2Hit","phi2:eta2:r2");
  cout <<"Projecting...3"<<endl;
  if (addL3Bck!=0) t->Project("hLayer3Hit","phi3:eta3:r3");
  cout <<"Projecting...done"<<endl;

  if (t->FindBranch("npart")!=0&&t->GetEntries("npart!=0")!=0) {
     isMC=true;
     cout <<"This is a Monte Carlo study."<<endl;
     vzShift = -0.4847;
     cout <<"vzShift = "<<vzShift<<endl;
  } else {
     isMC=false;
     //cout <<"This is a data analysis. STILL SHIFT Vtx for the moment!!!"<<endl;
     //vzShift = -0.4847;
     smearVertex = 0;
  }
  
  if (t->GetEntries("nhits1>5000")) {
     isHI=true;
     nPrint = 10;
     cout <<"This is a Heavyion analysis"<<endl;
  } else {
     isHI=false;
     cout <<"This is a pp analysis"<<endl;
  }
                                 
  // Event record
  vector <int> events[500];
  
  // Parameters for the tree =============================================================================
  Parameters par;  
  
  //  Parameters beamHaloPar;
  if (putBeamHalo) getPixelTreeBranch(beamHaloTree,par);  
  //t->SetBranchStatus("*",false);
  getPixelTreeBranch(t,par);
  
  if(!makeVzCut) t->SetBranchAddress("evtType",&par.evtType);
  cout <<"Number of Events: "<<t->GetEntries()<<endl;

  int nBeamHalo = 0;
  int doPileUp = 0;
  if (nPileUp!=0) {
     doPileUp=1;
     cout <<"Do pileup! With probability of "<<nPileUp<<endl;
  }   
  
  if (reproduceBck) endEntry-=3;

  // Main loop ==========================================================================================
  for(int i = startEntry;  i<t->GetEntries()&&i<endEntry ; i = i + 1 + doPileUp){    
    t->GetEntry(i);
    if (i % nPrint == 000) {
       cout <<"Run "<<par.nRun<<" Event "<<i<<" "
            <<trackletTree12->GetEntries()<<" "
            <<trackletTree23->GetEntries()<<" "
            <<trackletTree13->GetEntries()<<" Add Beam Halo: "
	    <<nBeamHalo<<" "<<nBeamHalo/(double)i
	    <<" nhit1 = "<<par.nhits1
            <<endl;
       if (reWeight) cout <<"Reweighted!!!!!!!"<<endl;    
    }       

   // if (par.nhits1>300000) continue;
   // if (par.nRun<150300) continue;
   // if (par.nLumi<70) continue;

    int flag=0;
    flag=1;
/*
    if (par.hltBit[8]==1  
         &&  (   (par.nLumi>=29&&par.nLumi<237)
               ||(par.nLumi>=288&&par.nLumi<=1336)
             )
       ) flag=1;
*/
    if (flag==0&&!isMC) continue;

    bool flagDoubleEvent = 0;

    if (checkDoubleEvent)
    {
       for (unsigned int j=0;j<events[par.nLumi].size();j++)
       {
          if (par.nEv==events[par.nLumi][j]) {
	     flagDoubleEvent = 1;
	     continue;
	  }
       }
       if (!flagDoubleEvent) events[par.nLumi].push_back(par.nEv);
    }

    if (flagDoubleEvent) continue;

    bool reWeightDropFlag = 0;
    if (reWeight) {
       reWeightDropFlag = 0;
       double myVz = par.vz[1];
       if (myVz<-90) {
          TF1 *f = new TF1("f","gaus",-30,30);
          f->SetParameters(1,-0.6536,4.438);
          myVz = f->GetRandom();
	  delete f;
       }
       
       // for early data 900 GeV
       /*
       double MCPdf = TMath::Gaus(myVz,-2.709,4.551,1);
       double DataPdf = TMath::Gaus(myVz,-2.702,3.627,1);
       */
       
       // for early data 7000 GeV Run 132440
       double MCPdf = TMath::Gaus(myVz,-0.6536,4.438,1);
       double DataPdf = TMath::Gaus(myVz,0.3533-vzShift,2.161,1);

//       double DataPdf = TMath::Gaus(myVz,-0.4623,2.731,1);
       double Ratio = DataPdf / MCPdf;
       //cout <<MCPdf<<" "<<DataPdf<<" "<<Ratio<<endl;
       double x=gRandom->Rndm()*2.5;

       if (x> Ratio) reWeightDropFlag=1;
       //cout <<x<<" "<<Ratio<<" "<<reWeightDropFlag<<endl;
    }
    
    if (reWeightDropFlag) continue;
    /*
    // Filter by evt selection cut //
    if (par.l1TBit[40]==0&&par.l1TBit[41]==0) continue;
    if (par.l1TBit[0]==0) continue;
    if (par.nLumi<69||par.nLumi>144) continue;
    */

    // Filter NSD events ==========================================================
    if ((par.evtType==92||par.evtType==93)&&useNSD) continue;
    
    // Filter HF coincidence
    if ((par.nHFn==0||par.nHFp==0||par.vz[1]<-99)&&reweightMultiplicity) continue;    
    
    // Beam Halo ==================================================================
    bool beamHaloFlag = false;
    
    if ( gRandom->Rndm() < beamHaloRatio && putBeamHalo ) {
       nBeamHalo++;
       beamHaloFlag = true;
       bool selectFlag = false;
       while (!selectFlag) {
          int nEntry = beamHaloTree->GetEntries();
          beamHaloTree->GetEntry(nEntry*gRandom->Rndm());
	  if (par.hltBit[67]==1) selectFlag=true;
       }
    }
    
    // Selection on Events
    
    // Fill reco vertex information
    tdata12.nv = par.nv+1;  
    tdata23.nv = par.nv+1;  
    tdata13.nv = par.nv+1;  
    for(int j = 1; j<par.nv;j++) {
       tdata12.vz[j+1] = par.vz[j];
       tdata23.vz[j+1] = par.vz[j];
       tdata13.vz[j+1] = par.vz[j];
       tdata12.vx[j+1] = par.vx[j];
       tdata23.vx[j+1] = par.vx[j];
       tdata13.vx[j+1] = par.vx[j];
       tdata12.vy[j+1] = par.vy[j];
       tdata23.vy[j+1] = par.vy[j];
       tdata13.vy[j+1] = par.vy[j];
    }
    // Fill MC vertex
    tdata12.vx[0] = par.vx[0];
    tdata23.vx[0] = par.vx[0];
    tdata13.vx[0] = par.vx[0];
    tdata12.vy[0] = par.vy[0];
    tdata23.vy[0] = par.vy[0];
    tdata13.vy[0] = par.vy[0];
    tdata12.vz[0] = par.vz[0];
    tdata23.vz[0] = par.vz[0];
    tdata13.vz[0] = par.vz[0];
    
    // add background 
    
    int bckHits=0;
    if (addL1Bck!=0||addL2Bck!=0||addL3Bck!=0){
       TF1 *fBck = new TF1("fBck","-0.00478376+0.000435517*x",0,1000);
       double val = fBck->Eval(par.nhits1)*2;
       for (int i = 0; i<par.nhits1;i++) {
          if (gRandom->Rndm()<val) {
	     bckHits++;
	  }
       }
       delete fBck;
    }
           
    if (addL1Bck!=0) {
       
       for (int i = par.nhits1; i < par.nhits1+bckHits; i++) {
          double eta, phi, r;
	  hLayer1Hit->GetRandom3(r,eta,phi);
	  par.eta1[i] = eta;
	  par.phi1[i] = phi;
	  par.r1[i] = r;
       }
       par.nhits1+=bckHits;
    }

    if (addL2Bck!=0) {
       //int bckHits = (int) (addL2Bck*gRandom->Rndm()+0.5);
       
       for (int i = par.nhits2; i < par.nhits2+bckHits; i++) {
          double eta, phi, r;
	  hLayer2Hit->GetRandom3(r,eta,phi);
	  par.eta2[i] = eta;
	  par.phi2[i] = phi;
	  par.r2[i] = r;
       }
       par.nhits2+=bckHits;
    }
    if (addL3Bck!=0) {
       //int bckHits = (int) (addL3Bck*gRandom->Rndm()+0.5);
       for (int i = par.nhits3; i < par.nhits3+bckHits; i++) {
          double eta, phi, r;
	  hLayer3Hit->GetRandom3(r,eta,phi);
	  par.eta3[i] = eta;
	  par.phi3[i] = phi;
	  par.r3[i] = r;
       }
       par.nhits3+=bckHits;
    }

    double trackletVertex = 0;

    // Choose KK Vertex if specified =============================================
    if (par.nhits1> vertexHitRegion|| useKKVertex) 
    { 
       // actually use pixel3vertex
       trackletVertex = par.vz[1]; 
    } else {
       vector<RecoHit> layerRaw1;
       prepareHits(layerRaw1,par, cuts, 1, 0,0,0, splitProb, dropProb,  cutOnClusterSize, par.nRun, par.nLumi, isMC);
       vector<RecoHit> layerRaw2;
       prepareHits(layerRaw2,par, cuts, 2, 0,0,0, splitProb, dropProb,  cutOnClusterSize, par.nRun, par.nLumi, isMC);

       if (nPileUp!=0 ) {
          if (gRandom->Rndm()<nPileUp) {
             t->GetEntry(i+1);
             prepareHits(layerRaw1,par, cuts, 1, 0,0,0, splitProb, dropProb,  cutOnClusterSize, par.nRun, par.nLumi, isMC);
             prepareHits(layerRaw2,par, cuts, 2, 0,0,0, splitProb, dropProb,  cutOnClusterSize, par.nRun, par.nLumi, isMC);
          }
          t->GetEntry(i);
       }
       trackletVertex = TrackletVertexUnbin(layerRaw1,layerRaw2,0.14,0.08);
    }
    // ===========================================================================
    
    // For particle gun =========
    //    if (i % 1000 == 0) cout <<"!!! USE GEN VERTEX (FOR PARTICLE GUN)"<<endl;
    //    trackletVertex = par.vz[0];
    //===========================

    // vz[1] is always the selected algorithm
    
    double smear =0;
    if (smearVertex!=0) {
       if (i==1) cout <<"Vertex smeared!"<<endl;
       while (smear!=0) {
          double x = gRandom->Rndm()*2-1;
          if (gRandom->Rndm()<TMath::Gaus(x,0,smearVertex,1)) {
             smear = x;
          }
       }
       trackletVertex += smear;
    }
    
    tdata12.vz[1] = trackletVertex;
    tdata23.vz[1] = trackletVertex;
    tdata13.vz[1] = trackletVertex;
    
    if(useKKVertex) {
       if (i==1) cout <<"Use KK Vertex "<<endl;
       tdata12.vx[1] = par.vx[1];
       tdata12.vy[1] = par.vy[1];
       tdata13.vx[1] = par.vx[1];
       tdata13.vy[1] = par.vy[1];
       tdata23.vx[1] = par.vx[1];
       tdata23.vy[1] = par.vy[1];
    } else {
       if (i==1)cout <<"Use Tracklet Vertex "<<endl;
    }

    if (useRandomVertex) {
       if (i==1) cout <<"Random Vertex!!!"<<endl;
           tdata12.vz[1]=gRandom->Rndm()*40-20;
           tdata13.vz[1]=tdata12.vz[1];
           tdata23.vz[1]=tdata12.vz[1];
   }
    // use trackletVertex
    if (fabs(tdata12.vz[1])>cuts.vzCut && makeVzCut == 1) continue;
    
    
    // Process hits with Vz constraint:
    vector<RecoHit> layer1;
    prepareHits(layer1,par, cuts, 1, tdata12.vx[1], tdata12.vy[1], tdata12.vz[1], splitProb, dropProb, cutOnClusterSize, par.nRun, par.nLumi, isMC);

    double myVz=tdata12.vz[1];
    double myhits2 = par.nhits2;
    double myhits3 = par.nhits3;
    int bckFlag = 0;
    
    if (reproduceBck) {
       bckFlag=0;
       for (int j=1;j<endEntry-i;j++)
       {
          t->GetEntry(i+j);
          if (fabs(par.vz[1]-myVz)<0.2&&fabs(par.nhits2-myhits2)<500000) {
	     bckFlag=j;
	     j=endEntry;
	  }
       }
       
       if (bckFlag==0) continue;
       t->GetEntry(i+bckFlag);	  
    }   

    
    vector<RecoHit> layer2;
    prepareHits(layer2,par, cuts, 2, tdata12.vx[1], tdata12.vy[1], tdata12.vz[1], splitProb, dropProb, cutOnClusterSize, par.nRun, par.nLumi, isMC);

    if (reproduceBck) {
       int flag=0;
       for (int j=bckFlag+1;j<endEntry-i;j++)
       {
          t->GetEntry(i+j);
          if (fabs(par.vz[1]-myVz)<0.5&&fabs(par.nhits3-myhits3)<2000) {
	     flag=j;
	     j=endEntry;
	  }
       }
       
       if (flag==0) continue;
       t->GetEntry(i+flag);	  
	  
    }   

    vector<RecoHit> layer3;
    prepareHits(layer3,par, cuts, 3, tdata12.vx[1], tdata12.vy[1], tdata12.vz[1], splitProb, dropProb, cutOnClusterSize, par.nRun, par.nLumi, isMC);

    if (reproduceBck) t->GetEntry(i);
    vector<RecoHit> layer1Cut;
    prepareHits(layer1Cut,par, cuts, 1, tdata12.vx[1], tdata12.vy[1], tdata12.vz[1], splitProb, dropProb, 1, par.nRun, par.nLumi, isMC);

    if (nPileUp!=0) {
       if (gRandom->Rndm()<nPileUp) {
          t->GetEntry(i+1);
          prepareHits(layer1,par, cuts, 1,tdata12.vx[1], tdata12.vy[1],  tdata12.vz[1], splitProb, dropProb, cutOnClusterSize);
          prepareHits(layer2,par, cuts, 2,tdata12.vx[1], tdata12.vy[1],  tdata12.vz[1], splitProb, dropProb, cutOnClusterSize);
          prepareHits(layer3,par, cuts, 3,tdata12.vx[1], tdata12.vy[1],  tdata12.vz[1], splitProb, dropProb, cutOnClusterSize);
       }
       t->GetEntry(i);
    }

    // Form Tracklets        
    vector<Tracklet> protoTracklets12;
    vector<Tracklet> protoTracklets13;
    vector<Tracklet> protoTracklets23;
    vector<Tracklet> recoTracklets12;
    vector<Tracklet> recoTracklets13;
    vector<Tracklet> recoTracklets23;
/*
    if (mimicPixelCounting) {
       protoTracklets12 = recoProtoTracklets(layer1,layer1);
       protoTracklets13 = recoProtoTracklets(layer2,layer2);
       protoTracklets23 = recoProtoTracklets(layer3,layer3);
    } else {
       protoTracklets12 = recoProtoTracklets(layer1,layer2);
       protoTracklets13 = recoProtoTracklets(layer1,layer3);
       protoTracklets23 = recoProtoTracklets(layer2,layer3);
    }  

    
    recoTracklets12  = cleanTracklets(protoTracklets12,0,cuts);
    recoTracklets13  = cleanTracklets(protoTracklets13,0,cuts);
    recoTracklets23  = cleanTracklets(protoTracklets23,0,cuts);
*/
    
    TH3I *h12 = new TH3I("h12","",nEtaBin,EtaBins,nDeltaEtaBin,DeltaEtaBins,nDeltaPhiBin,DeltaPhiBins);
    TH3I *h13 = new TH3I("h13","",nEtaBin,EtaBins,nDeltaEtaBin,DeltaEtaBins,nDeltaPhiBin,DeltaPhiBins);
    TH3I *h23 = new TH3I("h23","",nEtaBin,EtaBins,nDeltaEtaBin,DeltaEtaBins,nDeltaPhiBin,DeltaPhiBins);
    
    TH2I *hPart = new TH2I("hPart","",nEtaBin,EtaBins,nPtBin,PtBins);

    for(int j=0;j<par.npart;j++)
    {
        if (fabs(par.eta[j])>3||par.chg[j]==0||abs(par.pdg[j])==11||abs(par.pdg[j])==13) continue;
	hPart->Fill(par.eta[j],par.pt[j]);
    }

    int np=0;

    for (int x=0;x<nEtaBin;x++) {
       for (int y=0;y<nPtBin;y++) {
          ULong64_t w = hPart->GetBinContent(x+1,y+1);
	  if (w==0) continue;
	  tdata12.weight2[np]= w;
	  tdata12.eta[np] = (EtaBins[x]+EtaBins[x+1])/2.;
	  tdata12.pt[np] = (PtBins[y]+PtBins[y+1])/2;
	  tdata13.weight2[np]= w;
	  tdata13.eta[np] = (EtaBins[x]+EtaBins[x+1])/2.;
	  tdata13.pt[np] = (PtBins[y]+PtBins[y+1])/2;
	  tdata23.weight2[np]= w;
	  tdata23.eta[np] = (EtaBins[x]+EtaBins[x+1])/2.;
	  tdata23.pt[np] = (PtBins[y]+PtBins[y+1])/2;
          np++;
       }
    }
    
    tdata12.npart=np;
    tdata13.npart=np;
    tdata23.npart=np;
    
    delete hPart;
    
    recoTracklets12 = recoProtoTracklets(layer1,layer2,0,h12,tdata12.nProtoTracklet,tdata12.mult);
    recoTracklets13 = recoProtoTracklets(layer1,layer3,0,h13,tdata13.nProtoTracklet,tdata13.mult);
    recoTracklets23 = recoProtoTracklets(layer2,layer3,0,h23,tdata23.nProtoTracklet,tdata23.mult);


    // Move the Vertex back
    if (smearVertex!=0)
    {
       tdata12.vz[1] = trackletVertex-smear;
       tdata23.vz[1] = trackletVertex-smear;
       tdata13.vz[1] = trackletVertex-smear;
    }

    // Vertex Compatibility information
    float vtxQualCut=0;
    if (par.pixel<150) {
       vtxQualCut = 1;
    } else {
       if (par.vtxqual>2) {
          vtxQualCut = 1;
       } else {
          if (par.vtxqual>0.0045*par.pixel) {
	     vtxQualCut = 1;
 	  } else {
 	     vtxQualCut = 0;
 	  }
       }
    }
   
    // Fill Ntuple
    tdata12.nTracklet = recoTracklets12.size();
    tdata12.nhit1   = layer1.size();
    tdata12.nhit2   = layer2.size();
    tdata12.nRun    = par.nRun;
    tdata12.nEv     = par.nEv;
    tdata12.nLumi   = par.nLumi;
    tdata12.nBX     = par.nBX;
    tdata12.nHFn    = par.nHFp;
    tdata12.nHFp    = par.nHFn;
    tdata12.nHits   = layer1.size()+layer2.size()+layer3.size();
    tdata12.nHltBit = par.nHltBit;
    tdata12.nL1ABit = par.nL1ABit;
    tdata12.nL1TBit = par.nL1TBit;
    tdata12.vtxQualCut = vtxQualCut;
    tdata12.vtxqual = par.vtxqual;
    tdata12.pixel = par.pixel;

    tdata12.beamSpotX = par.beamSpotX;
    tdata12.beamSpotY = par.beamSpotY;
    tdata12.beamSpotZ = par.beamSpotZ;

    tdata12.zdcm = par.zdcm;
    tdata12.zdcp = par.zdcp;
    tdata12.hf = par.hf;
    tdata12.hftp = par.hftp;
    tdata12.hftm = par.hftm;
    tdata12.eb = par.eb;
    tdata12.eep = par.eep;
    tdata12.eem = par.eem;
    tdata12.cBin = par.cBin;
    tdata12.nbins = par.nbins;
    tdata12.binsize = par.binsize;
    tdata12.nparti = par.nparti;
    tdata12.npartiSigma = par.npartiSigma;
    tdata12.ncoll = par.ncoll;
    tdata12.ncollSigma = par.ncollSigma;
    tdata12.nhard = par.nhard;
    tdata12.nhardSigma = par.nhardSigma;
    tdata12.b = par.b;
    tdata12.bSigma = par.bSigma;
    
    for (int j=0;j<(int)par.nHltBit;j++)
    {
       tdata12.hltBit[j]= par.hltBit[j];
    }

    for (int j=0;j<(int)par.nL1ABit;j++)
    {
       tdata12.l1ABit[j]= par.l1ABit[j];
    }

    for (int j=0;j<(int)par.nL1TBit;j++)
    {
       tdata12.l1TBit[j]= par.l1TBit[j];
    }
    
    int ntracklet12s=0;
    int ntracklet12b=0;
    int j12=0;
    
    for (int x=0;x<nEtaBin;x++) {
       for (int y=0;y<nDeltaEtaBin;y++) {
          for (int z=0;z<nDeltaPhiBin;z++) {
             ULong64_t w = h12->GetBinContent(x+1,y+1,z+1);
	     if (w==0) continue;
	     tdata12.weight[j12]= w;
	     tdata12.eta1[j12] = (EtaBins[x]+EtaBins[x+1])/2.;	
	     tdata12.deta[j12] = (DeltaEtaBins[y]+DeltaEtaBins[y+1])/2.;
   	     tdata12.dphi[j12] = (DeltaPhiBins[z]+DeltaPhiBins[z+1])/2.;
             if (fabs(tdata12.deta[j12])<0.1){
	        if (fabs(tdata12.dphi[j12])<1.0) {
	           ntracklet12s++;
	        } 
	        if (fabs(tdata12.dphi[j12])>1.0&&fabs(tdata12.dphi[j12])<2.0) {
	        ntracklet12b++;
	        }
             }
	     j12++;
	  }
       }
    }
    delete h12;
    tdata12.nTracklet = j12;
    tdata12.mult2=layer1Cut.size();
    
    bool reWeightMultDropFlag = 0;       

    if (reweightMultiplicity) {
       reWeightMultDropFlag = 0;
       double Ratio = hRatio->GetBinContent(hRatio->FindBin(tdata12.mult));
       double x=gRandom->Rndm()*1.6;

       if (x> Ratio) reWeightMultDropFlag=1;
    }
    if (reWeightMultDropFlag) continue;

    tdata12.evtType = par.evtType;

    for (int j=0;j<par.nv;j++)
    {
       tdata12.vz[j]+=vzShift;
    }
    
    trackletTree12->Fill();

    tdata13.nhit1 = layer1.size();
    tdata13.nhit2 = layer3.size();
    tdata13.nRun    = par.nRun;
    tdata13.nEv     = par.nEv;
    tdata13.nLumi   = par.nLumi;
    tdata13.nBX     = par.nBX;
    tdata13.nHFn    = par.nHFp;
    tdata13.nHFp    = par.nHFn;
    tdata13.nHits   = layer1.size()+layer2.size()+layer3.size();
    tdata13.nHltBit = par.nHltBit;
    tdata13.nL1ABit = par.nL1ABit;
    tdata13.nL1TBit = par.nL1TBit;
    tdata13.vtxQualCut = vtxQualCut;
    tdata13.vtxqual = par.vtxqual;
    tdata13.pixel = par.pixel;

    tdata13.beamSpotX = par.beamSpotX;
    tdata13.beamSpotY = par.beamSpotY;
    tdata13.beamSpotZ = par.beamSpotZ;

    tdata13.zdcm = par.zdcm;
    tdata13.zdcp = par.zdcp;

    tdata13.hf = par.hf;
    tdata13.hftp = par.hftp;
    tdata13.hftm = par.hftm;
    tdata13.eb = par.eb;
    tdata13.eep = par.eep;
    tdata13.eem = par.eem;
    tdata13.cBin = par.cBin;
    tdata13.nbins = par.nbins;
    tdata13.binsize = par.binsize;
    tdata13.nparti = par.nparti;
    tdata13.npartiSigma = par.npartiSigma;
    tdata13.ncoll = par.ncoll;
    tdata13.ncollSigma = par.ncollSigma;
    tdata13.nhard = par.nhard;
    tdata13.nhardSigma = par.nhardSigma;
    tdata13.b = par.b;
    tdata13.bSigma = par.bSigma;

    for (int j=0;j<(int)par.nHltBit;j++)
    {
       tdata13.hltBit[j]= par.hltBit[j];
    }

    for (int j=0;j<(int)par.nL1ABit;j++)
    {
       tdata13.l1ABit[j]= par.l1ABit[j];
    }

    for (int j=0;j<(int)par.nL1TBit;j++)
    {
       tdata13.l1TBit[j]= par.l1TBit[j];
    }

    int ntracklet13s=0;
    int ntracklet13b=0;
    int j13=0;
    
    for (int x=0;x<nEtaBin;x++) {
       for (int y=0;y<nDeltaEtaBin;y++) {
          for (int z=0;z<nDeltaPhiBin;z++) {
             ULong64_t w = h13->GetBinContent(x+1,y+1,z+1);
	     if (w==0) continue;
	     tdata13.weight[j13]= w;
	     tdata13.eta1[j13] = (EtaBins[x]+EtaBins[x+1])/2.;	
	     tdata13.deta[j13] = (DeltaEtaBins[y]+DeltaEtaBins[y+1])/2.;
   	     tdata13.dphi[j13] = (DeltaPhiBins[z]+DeltaPhiBins[z+1])/2.;
             if (fabs(tdata13.deta[j13])<0.1){
	        if (fabs(tdata13.dphi[j13])<1.0) {
	           ntracklet13s++;
	        } 
	        if (fabs(tdata13.dphi[j13])>1.0&&fabs(tdata13.dphi[j13])<2.0) {
	        ntracklet13b++;
	        }
             }
	     j13++;
	  }
       }
    }
    delete h13;
    tdata13.nTracklet = j13;
    tdata13.mult2=layer1Cut.size();

    tdata13.evtType = par.evtType;

    for (int j=0;j<par.nv;j++)
    {
       tdata13.vz[j]+=vzShift;
    }

    trackletTree13->Fill();

    tdata23.nTracklet = recoTracklets23.size();
    tdata23.nhit1 = layer2.size();
    tdata23.nhit2 = layer3.size();
    tdata23.nRun    = par.nRun;
    tdata23.nEv     = par.nEv;
    tdata23.nLumi   = par.nLumi;
    tdata23.nBX     = par.nBX;
    tdata23.nHFn    = par.nHFp;
    tdata23.nHFp    = par.nHFn;
    tdata23.nHits   = layer1.size()+layer2.size()+layer3.size();
    tdata23.nHltBit = par.nHltBit;
    tdata23.nL1ABit = par.nL1ABit;
    tdata23.nL1TBit = par.nL1TBit;
    tdata23.vtxQualCut = vtxQualCut;
    tdata23.vtxqual = par.vtxqual;
    tdata23.pixel = par.pixel;

    tdata23.beamSpotX = par.beamSpotX;
    tdata23.beamSpotY = par.beamSpotY;
    tdata23.beamSpotZ = par.beamSpotZ;

    tdata23.zdcm = par.zdcm;
    tdata23.zdcp = par.zdcp;

    tdata23.hf = par.hf;
    tdata23.hftp = par.hftp;
    tdata23.hftm = par.hftm;
    tdata23.eb = par.eb;
    tdata23.eep = par.eep;
    tdata23.eem = par.eem;
    tdata23.cBin = par.cBin;
    tdata23.nbins = par.nbins;
    tdata23.binsize = par.binsize;
    tdata23.nparti = par.nparti;
    tdata23.npartiSigma = par.npartiSigma;
    tdata23.ncoll = par.ncoll;
    tdata23.ncollSigma = par.ncollSigma;
    tdata23.nhard = par.nhard;
    tdata23.nhardSigma = par.nhardSigma;
    tdata23.b = par.b;
    tdata23.bSigma = par.bSigma;

    for (int j=0;j<(int)par.nHltBit;j++)
    {
       tdata23.hltBit[j]= par.hltBit[j];
    }

    for (int j=0;j<(int)par.nL1ABit;j++)
    {
       tdata23.l1ABit[j]= par.l1ABit[j];
    }

    for (int j=0;j<(int)par.nL1TBit;j++)
    {
       tdata23.l1TBit[j]= par.l1TBit[j];
    }

    int ntracklet23s=0;
    int ntracklet23b=0;
    int j23=0;
    
    for (int x=0;x<nEtaBin;x++) {
       for (int y=0;y<nDeltaEtaBin;y++) {
          for (int z=0;z<nDeltaPhiBin;z++) {
             ULong64_t w = h23->GetBinContent(x+1,y+1,z+1);
	     if (w==0) continue;
	     tdata23.weight[j23]= w;
	     tdata23.eta1[j23] = (EtaBins[x]+EtaBins[x+1])/2.;	
	     tdata23.deta[j23] = (DeltaEtaBins[y]+DeltaEtaBins[y+1])/2.;
   	     tdata23.dphi[j23] = (DeltaPhiBins[z]+DeltaPhiBins[z+1])/2.;
             if (fabs(tdata23.deta[j23])<0.1){
	        if (fabs(tdata23.dphi[j23])<1.0) {
	           ntracklet23s++;
	        } 
	        if (fabs(tdata23.dphi[j23])>1.0&&fabs(tdata23.dphi[j23])<2.0) {
	        ntracklet23b++;
	        }
             }
	     j23++;
	  }
       }
    }
    delete h23;
    tdata23.nTracklet = j23;
    tdata23.mult2=layer1Cut.size();
  
    tdata23.evtType = par.evtType;
    
    for (int j=0;j<par.nv;j++)
    {
       tdata23.vz[j]+=vzShift;
    }


    trackletTree23->Fill();
  }

  // Close outputfile ===================================================================================

  outf->Write();
  outf->Close(); 
}


