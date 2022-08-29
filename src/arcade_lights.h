//*********************************************************************************************************************
//  Raspberry Pi arcade console lights
//   
//  
//    
//*********************************************************************************************************************
//


//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

#include       <ctype.h>
#include       <delays.h>
#include       <string.h>
#include       <stdio.h>
#include       <stdarg.h>
#include       <math.h>
#include       <float.h>
#include       <plib.h>
 


//*********************************************************************************************************************
// Function prototypes 
// 
//*********************************************************************************************************************
//
 
// Arcade_patterns
void vPortOn(void);
void vPortOff(void);
void vPatterns(void); 

// IR
void vPwrOn(void);
void vPwrOff(void);
void vVideo1(void);
void vVideo2(void);

// Misc
void vBeep(unsigned char); 


//*********************************************************************************************************************
// PIN defines 
// 
//*********************************************************************************************************************
//
 
#define PWR_RELAY       LATCbits.LATC0          // Out: Relay 1 Power
#define ACC_RELAY       LATDbits.LATD0          // Out: Relay 2 Accessories
#define DIV_RELAY       LATDbits.LATD1          // Out: Relay 3
#define PWR_BUTTON      PORTEbits.RE3           // In:  Power button (MCLR)
#define ACTION          PORTBbits.RB2           // In: Low when button action (stop running lights)
#define IR_LED          LATBbits.LATB2          // Out: IR LED  (not used)

#define RPI_STATUS      PORTCbits.RC4           // In:  RPI status
#define _ACTION          PORTCbits.RC5          // Not used In:  Low when button action (stop running lights)
#define SWITCH_LED      LATBbits.LATB5          // Out: Switch button LED
#define BOARD_LED       LATBbits.LATB4          // Out: Board status LED
#define RPI_SHUTDOWN    LATBbits.LATB3          // Out: Set high to signal RPI shutdown

// PIN mappings 16 lights
#define MLT1            LATCbits.LATC7          // Out: Left Top 1
#define MLT2            LATBbits.LATB0          // Out: Left Top 2
#define MLT3            LATDbits.LATD7          // Out: Left Top 3
#define MLT4            LATDbits.LATD5          // Out: Left Top 4

#define MLB1            LATBbits.LATB1          // Out: Left Bottom 1
#define MLB2            LATDbits.LATD6          // Out: Left Bottom 2
#define MLB3            LATDbits.LATD4          // Out: Left Bottom 3
#define MLB4            LATDbits.LATD3          // Out: Left Bottom 4

#define MRT1            LATAbits.LATA5          // Out: Right Top 1
#define MRT2            LATAbits.LATA3          // Out: Right Top 2
#define MRT3            LATAbits.LATA1          // Out: Right Top 3
#define MRT4            LATEbits.LATE2          // Out: Right Top 4

#define MRB1            LATAbits.LATA2          // Out: Left Bottom 1
#define MRB2            LATAbits.LATA0          // Out: Left Bottom 2
#define MRB3            LATEbits.LATE1          // Out: Left Bottom 3
#define MRB4            LATEbits.LATE0          // Out: Left Bottom 4

#define MPLAYER1        LATCbits.LATC2          // Out: PWM1 Player1
#define MPLAYER2        LATCbits.LATC1          // Out: PWM2 Player2

// Extra unused
#define EXT1            LATCbits.LATC6          // Out: 
#define EXT2            LATBbits.LATB7          // Out: 
#define SPEAKER         LATBbits.LATB6          // Out: Speaker

// State defines of driven lights
#define LON             0                       // Low is light on
#define LOFF            1                       // High is light off

//                       41   42
//    01   02   03   04           21   22   23   24
//
//    11   12   13   14           31   32   33   34
//

#define LT1              1
#define LT2              2
#define LT3              3
#define LT4              4

#define LB1              11
#define LB2              12
#define LB3              13
#define LB4              14

#define RT1              21
#define RT2              22
#define RT3              23
#define RT4              24

#define RB1              31
#define RB2              32
#define RB3              33
#define RB4              34

#define PLAYER1          41
#define PLAYER2          42



//*********************************************************************************************************************
// Test section
#ifdef TEST 
   do
   {       
      
      vDelay(2000);      
      
      LT1 = 0;
      vDelay(200);
      LT1 = 1;
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 2; ucI++)
      {
         LT2 = 0;
         vDelay(200);
         LT2 = 1;
         vDelay(200);         
      }   
      
      vDelay(1000);
      
      for (ucI = 0; ucI < 3; ucI++)
      {
         LT3 = 0;
         vDelay(200);
         LT3 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 4; ucI++)
      {
         LT4 = 0;
         vDelay(200);
         LT4 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      //---------------
      
      for (ucI = 0; ucI < 5; ucI++)
      {
         LB1 = 0;
         vDelay(200);
         LB1 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 6; ucI++)
      {
         LB2 = 0;
         vDelay(200);
         LB2 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 7; ucI++)
      {
         LB3 = 0;
         vDelay(200);
         LB3 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
 
      for (ucI = 0; ucI < 8; ucI++)
      {
         LB4 = 0;
         vDelay(200);
         LB4 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      //-----------
      
         RT1 = 0;
         vDelay(200);
         RT1 = 1;
         vDelay(200);         
    
         vDelay(1000);
 
      
      for (ucI = 0; ucI < 2; ucI++)
      {
         RT2 = 0;
         vDelay(200);
         RT2 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 3; ucI++)
      {
         RT3 = 0;
         vDelay(200);
         RT3 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
      
      for (ucI = 0; ucI < 4; ucI++)
      {
         RT4 = 0;
         vDelay(200);
         RT4 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      //---------------
      
      for (ucI = 0; ucI < 5; ucI++)
      {
         RB1 = 0;
         vDelay(200);
         RB1 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 6; ucI++)
      {
         RB2 = 0;
         vDelay(200);
         RB2 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 7; ucI++)
      {
         RB3 = 0;
         vDelay(200);
         RB3 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
 
      for (ucI = 0; ucI < 8; ucI++)
      {
         RB4 = 0;
         vDelay(200);
         RB4 = 1;
         vDelay(200);         
      }   
      vDelay(1000);
 
 
      for (ucI = 0; ucI < 10; ucI++)
      {
         PLAYER1 = 0;
         vDelay(100);
         PLAYER1 = 1;
         vDelay(100);         
      }   
      vDelay(1000);
 
      
      for (ucI = 0; ucI < 10; ucI++)
      {
         PLAYER2 = 0;
         vDelay(100);
         PLAYER2 = 1;
         vDelay(100);         
      }   
      vDelay(1000);
 
       
      vBeep(0);
        
   }
   while(1);
#endif  
   
