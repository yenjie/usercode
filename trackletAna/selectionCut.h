#include <TCut.h>

class selectionCut
{  
    public:
    selectionCut();
    ~selectionCut(){}
    
    TCut Cut;  
    int nEtaBin;
    int nHitBin;
    int nVzBin;
};

selectionCut::selectionCut()
{
   Cut      = "abs(vz[1])<10"; 
   nEtaBin  = 12;
   nHitBin  = 14;
   nVzBin   = 10;
}
