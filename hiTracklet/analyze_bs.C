#include <TFile.h>
#include <TNtuple.h>
#include <TH2.h>
#include <TF1.h>
#include <TTimeStamp.h>
#include <TRandom.h>
#include "Tracklet.h"


using namespace std;

void analyze_bs(char * infile, char * outfile = "output.root", long startEntry = 0, long endEntry = 1000000000,
                          double x0 = 0, double y0 = 0,
			  int addL1Bck = 0, int addL2Bck = 0, int addL3Bck = 0,
   		          bool reWeight = 0,         // reweight to Run 123596 vtx distribution
                          bool useRandomVertex= 0,
			  bool cutOnClusterSize = 1,
			  bool mimicPixelCounting = 0, 
			  int makeVzCut = 0,
			  double splitProb = 0, double dropProb = 0, 
			  double nPileUp = 0,
			  double beamHaloRatio = 0.0,
			  bool putBeamHalo = false, char * beamHaloFile =
    			             "DataSample/PixelTree-Run123151-Full.root",
                          double smearVertex = 0,
			  bool putPixelTree = 0,
			  bool useKKVertex = 1,
			  bool useNSD = 0,
			  bool checkDoubleEvent = 0,
			  bool reweightMultiplicity = 0,
			  bool reproduceBck = 0
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
  
  // Output file =======================================================================================
  TFile* outf = new TFile(outfile,"recreate");
  TNtuple *ntmult = new TNtuple("ntmult","","mult:x0:y0");
  TNtuple *nthit = new TNtuple("nthit","","phi1:layer");
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
  int nbins = zbins*hitbins;
  int nPrint = 100;
  double mult = 0;
  bool isMC = 0;
  bool isHI = 0;
  double vzShift = 0;
  
  int nEtaBin = 20;
  int nDeltaEtaBin = 1;
  int nDeltaPhiBin = 400;
  double EtaBins[nEtaBin+1];
  double DeltaEtaBins[nDeltaEtaBin+1];
  double DeltaPhiBins[nDeltaPhiBin+1];
  
  for (int i=0;i<=nEtaBin;i++)
  {
     EtaBins[i]=-2.5+(i)*5./nEtaBin;
  }

  for (int i=0;i<=nDeltaEtaBin;i++)
  {
     DeltaEtaBins[i]=-0.05+(i)*0.1/nDeltaEtaBin;
  }
  
  for (int i=0;i<=nDeltaPhiBin;i++)
  {
     DeltaPhiBins[i]=-3.2+(i)*3.2*2./nDeltaPhiBin;
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
  vector<TH1D*> layer1HitEta;
  layer1HitEta.reserve(nbins);

  vector<TH1D*> layer1HitPhi;
  layer1HitPhi.reserve(nbins);

  vector<TH2D*> layer12D;
  layer12D.reserve(nbins);

  vector<TH1D*> layer2HitEta;
  layer2HitEta.reserve(nbins);

  vector<TH1D*> layer2HitPhi;
  layer2HitPhi.reserve(nbins);

  vector<TH2D*> layer22D;
  layer22D.reserve(nbins);


  vector<TH1D*> layer3HitEta;
  layer3HitEta.reserve(nbins);

  vector<TH1D*> layer3HitPhi;
  layer3HitPhi.reserve(nbins);

  vector<TH2D*> layer32D;
  layer32D.reserve(nbins);
    


  TH3D* nhits = new TH3D("nhits","",100,0,100,100,0,100,100,0,100);

  TH3D* hLayer1Hit = new TH3D("hLayer1Hit","",75,0,15,60,-3,3,64,-3.2,3.2);
  TH3D* hLayer2Hit = new TH3D("hLayer2Hit","",75,0,15,60,-3,3,64,-3.2,3.2);
  TH3D* hLayer3Hit = new TH3D("hLayer3Hit","",75,0,15,60,-3,3,64,-3.2,3.2);

  TH3I *h3D = new TH3I("h3D","",nEtaBin,EtaBins,nDeltaEtaBin,DeltaEtaBins,nDeltaPhiBin,DeltaPhiBins);

  // Prepare hit spectra for random hit 
  cout <<"Projecting...1"<<endl;
  if (addL1Bck!=0) t->Project("hLayer1Hit","phi1:eta1:r1");
  cout <<"Projecting...2"<<endl;
  if (addL2Bck!=0) t->Project("hLayer2Hit","phi2:eta2:r2");
  cout <<"Projecting...3"<<endl;
  if (addL3Bck!=0) t->Project("hLayer3Hit","phi3:eta3:r3");
  cout <<"Projecting...done"<<endl;

  if (t->FindBranch("npart")!=0) {
     isMC=true;
     cout <<"This is a Monte Carlo study."<<endl;
     vzShift = -0.4847;
     cout <<"vzShift = "<<vzShift<<endl;
  } else {
     cout <<"This is a data analysis."<<endl;
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
    //if (par.nhits1>300000) continue;
    

    bool flagDoubleEvent = 0;

    if (checkDoubleEvent)
    {
       for (int j=0;j<events[par.nLumi].size();j++)
       {
          if (par.nEv==events[par.nLumi][j]) {
	     flagDoubleEvent = 1;
	     continue;
	  }
       }
       if (!flagDoubleEvent) events[par.nLumi].push_back(par.nEv);
    }

    if (flagDoubleEvent) continue;
//    if (par.nRun!=124023||(par.nRun==124033&&(par.nLumi<41||par.nLumi>96))) continue;
//    if (par.nRun!=132440||(par.nRun==132440&&(par.nLumi<141||par.nLumi>200))) continue;
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

    // add trackletVertex
    /*
    if (tdata12.nv == 2) tdata12.nv=3;
    if (tdata23.nv == 2) tdata23.nv=3;
    if (tdata13.nv == 2) tdata13.nv=3;
    */
    
    vector<RecoHit> layerRaw1;
    prepareHits(layerRaw1,par, cuts, 1, x0,y0,0, splitProb, dropProb,  cutOnClusterSize, par.nRun, par.nLumi, isHI, true);
    vector<RecoHit> layerRaw2;
    prepareHits(layerRaw2,par, cuts, 2, x0,y0,0, splitProb, dropProb,  cutOnClusterSize, par.nRun, par.nLumi, isHI, true);

    
    double trackletVertex = 0;

    // Choose KK Vertex if specified =============================================
    if (par.nhits1> vertexHitRegion|| useKKVertex) 
    { 
       // actually use pixel3vertex
       trackletVertex = par.vz[1]; 
    } else {
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
    prepareHits(layer1,par, cuts, 1, x0, y0, tdata12.vz[1], splitProb, dropProb, cutOnClusterSize, par.nRun, par.nLumi, isHI, true);
    
    vector<RecoHit> layer2;
    prepareHits(layer2,par, cuts, 2, x0, y0, tdata12.vz[1], splitProb, dropProb, cutOnClusterSize, par.nRun, par.nLumi, isHI, true);


    // Form Tracklets        
    vector<Tracklet> recoTracklets12;
    
    TH3I *h12 = new TH3I("h12","",nEtaBin,EtaBins,nDeltaEtaBin,DeltaEtaBins,nDeltaPhiBin,DeltaPhiBins);
    
    recoTracklets12 = recoProtoTracklets(layer1,layer2,0,h12,tdata12.nProtoTracklet,tdata12.mult);
    h3D->Add(h12);
    TH1I *h = (TH1I*) h12->Project3D("Z");
    tdata12.mult=0;
    for (int i=0;i<=h->GetNbinsX();i++)
    {
       Long64_t val = h->GetBinContent(i);
       if (val>tdata12.mult) tdata12.mult=val;
    }
    ntmult->Fill(tdata12.mult,x0,y0);
    delete h12;
    delete h;
    }
    
    outf->Write();
    outf->Close();
}


