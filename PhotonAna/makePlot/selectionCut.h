#include <TCut.h>

class selectionCut 
{
   public:
      selectionCut();
      TCut EBCut;
      TCut EECut;
   
};
selectionCut::selectionCut()
{
   EBCut = "abs(eta[0])>1.55&&abs(eta[0])<2.5&&et[0]>15&&isEBGap[0]==0&&isEEGap[0]==0&&isEBEEGap[0]==0";
   EECut = "abs(eta[0])<1.45&&abs(eta[0])<2.5&&et[0]>15&&isEBGap[0]==0&&isEEGap[0]==0&&isEBEEGap[0]==0";
}
