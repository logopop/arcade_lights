//*********************************************************************************************************************
//  Arcade lights IR routines
//   
//  
//    
//*********************************************************************************************************************
//

//#define TEST

#include <htc.h>
#include <arcade_lights.h>
#include <xc.h>
 

//*********************************************************************************************************************
// 

void vSig(unsigned char ucL)
{
   unsigned char j         = 0;
   
   for (j = 0; j < ucL; j++)
   {
      IR_LED = 1;
      Delay1TCYx(7);    
      IR_LED = 0;
      Delay1TCYx(7);
      
   }      
}

//*********************************************************************************************************************
// 

void vPulse(unsigned char ucT)
{
   switch(ucT)
   {
      case 0:           // short-short     
         vSig(32);         
         IR_LED = 0;
         Delay1TCYx(120);
      break;
      case 1:           // short-long
         vSig(32);
         IR_LED = 0;
         Delay1TCYx(240);
      break;
      case 2:           // long-short
         vSig(64);
         IR_LED = 0;
         Delay1TCYx(120);
      break;
      case 3:           // long-long
         vSig(64);
         IR_LED = 0;
         Delay1TCYx(240);
      break;
   }        
}

//*********************************************************************************************************************
// 

void vPwrOn(void)
{
   unsigned char i         = 0;
   
   for (i = 0; i < 5; i++)
   {   
      vPulse(0);
      vPulse(0);
      vPulse(2);
      vPulse(0);
      vPulse(0);
      vPulse(1);

      vPulse(0);
      vPulse(2);
      vPulse(0);
      vPulse(0);
      vPulse(0);
      vPulse(0);

      Delay1KTCYx(108);     
   }   
}

//*********************************************************************************************************************
// 

void vPwrOff(void)
{
   unsigned char i         = 0;
   
   for (i = 0; i < 5; i++)
   {   
      vPulse(0);
      vPulse(2);
      vPulse(0);
      vPulse(0);
      vPulse(0);
      vPulse(1);

      vPulse(2);
      vPulse(1);
      vPulse(0);
      vPulse(2);
      vPulse(0);
      
      Delay1KTCYx(108);     
   }   
}

//*********************************************************************************************************************
// 

void vVideo1(void)
{
   unsigned char i         = 0;
   
   for (i = 0; i < 5; i++)
   {   
      vPulse(0);
      vPulse(0);
      vPulse(2);
      vPulse(0);
      vPulse(0);
      vPulse(1);

      vPulse(0);
      vPulse(0);
      vPulse(0);
      vPulse(2);
      vPulse(0);
      vPulse(0);

      Delay1KTCYx(108);     
   }   
}

//*********************************************************************************************************************
// 

void vVideo2(void)
{
   unsigned char i         = 0;
   
   for (i = 0; i < 5; i++)
   {   
      vPulse(0);
      vPulse(2);
      vPulse(0);
      vPulse(0);
      vPulse(0);
      vPulse(1);

      vPulse(0);
      vPulse(0);
      vPulse(0);
      vPulse(2);
      vPulse(0);
      vPulse(0);

      Delay1KTCYx(108);     
   }   
}







