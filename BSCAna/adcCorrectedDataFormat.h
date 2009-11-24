class adcCorrectedData
{
   public:
       adcCorrectedData(){};
       ~adcCorrectedData(){};
       Int_t dataSize[8];
       Int_t nch;
       Int_t run;
       Int_t evt;
       Int_t evtSecond;
       Int_t evtTime;
       Int_t pedestal[8];
       Float_t data[8][10000];
       Float_t time[8][10000];
       Float_t min[8];
       Float_t mintime[8];
       Float_t mintime08[8];
       Float_t mintime04[8];
       Float_t mintime02[8];
       Float_t mintime0802[8];
       Float_t mintime0804[8];
       
};

