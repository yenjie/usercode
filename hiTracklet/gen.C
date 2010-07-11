#include <iostream>

void gen()
{
   for (float i=-1;i<=1;i+=0.05)
   {
      for (float j=-1;j<=1;j+=0.05)
      {
         cout <<"./anaBS.sh $1 "<<i<<" "<<j<<endl;
      }
   }   
}