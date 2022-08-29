//*********************************************************************************************************************
//  Raspberry Pi arcade console power sequencer and lights controller.
//   
//  
//    
//*********************************************************************************************************************
//

#include <htc.h>
#include <xc.h>
#include <stdlib.h>
#include <pic18f4550.h>
#include <arcade_lights.h>
 
#include <arcade_config.h>

//*********************************************************************************************************************
//

unsigned long ulI2;

unsigned long ulLcounter;                          // Delay counter
unsigned char cLstate;                             // Main state  0:lights off 1:starting 2:running 3:stopping 4:light on
unsigned char cLoldstate;                          // Previous state
unsigned char cLseq;                               // Current sequence. 0: between sequences
unsigned char cLstep;                              // Current step in sequence
unsigned char cStartStop;                          // True when starting up or shutting down, ignore 'action'


//*********************************************************************************************************************
// Interrupt handlers
// 
//*********************************************************************************************************************
//
 
void interrupt vHighIsr(void)                      // High priority interrupt
{
   INTCONbits.GIE_GIEH = 0;
  
   if (INTCONbits.T0IF)
   {   
      INTCONbits.T0IE = 0;     
     
      if (ulLcounter < 2800)
         ulLcounter++;
            
      if (!(PORTB & 0x04))
      {         
         if (!cStartStop)
         {   
            BOARD_LED = 1;
            Delay10TCYx(20);  
            ulLcounter = 0;         // Reset counter timer
         
            // Main state  0:lights off 1:starting 2:running 3:stopping 4:lights on
            if (cLstate == 2)
               cLstate = 1;         // Change to starting               
         }   
      }   
            
      // Start lightshow after 2 minutes
      if ((ulLcounter > 2760) && (cLstate != 2) && (cLstate != 3))     
         cLstate = 2;            // Lights to running
      
      // Call state machine
      vPatterns();
     
      INTCONbits.T0IF = 0;
      INTCONbits.T0IE = 1;
      BOARD_LED = 0;
   }      
   else if (PIR1bits.TMR2IF)                       // In a tone sequence
   {
      PIR1bits.TMR2IF = 0;
      ulI2 = CCPR1L;
      ulI2-=1;
      CCPR1L = ulI2;
      if (ulI2 < 3)                                // Done with sequence, return port to stdout
      {
         CCP1CONbits.CCP1M3 = 0;
         CCP1CONbits.CCP1M2 = 0;
         T2CONbits.TMR2ON = 0;
      }
   }
   
   INTCONbits.GIE_GIEH = 1;
  
}
  
//*********************************************************************************************************************
// Delay
// 
//*********************************************************************************************************************
//

void vDelay(unsigned long ulMs)
{
   unsigned long     ulI      = 0;
   
   while (ulI++ < ulMs)      
   {   
      Delay1KTCYx(12);   // 1ms
      ClrWdt();
   }   
}

//*********************************************************************************************************************
// Sound routines
// 
//*********************************************************************************************************************
//

void vSound(unsigned char ucPitch, unsigned char ucLength)
{
   unsigned int uiTemp     = 0;
   unsigned int uiTemp3    = 0;
   
   uiTemp3 = ucLength;

   for (uiTemp = 0; uiTemp < uiTemp3; uiTemp++)
   {
      SPEAKER = 0;
      Delay100TCYx(ucPitch);
      SPEAKER = 1;
      Delay100TCYx(ucPitch);
   }
}

/*
void vTone(unsigned char ucTone)
{
   PIE1bits.TMR2IE = 1;
   ECCP1CON = 0x0c;                                   // Single compatible
   PR2 = 0xff;                                        // Period
   ECCP1CONbits.ECCP1M2 = 1;
   ECCP1CONbits.ECCP1M3 = 1;
   ECCP1CONbits.EPWM1M0 = 0;
   ECCP1CONbits.EPWM1M1 = 0;
   ECCP1CONbits.EDC1B0 = 0;                           // Duty cycle LSB 0
   ECCP1CONbits.EDC1B1 = 0;                           // Duty cycle LSB 1
   ECCPR1L = 0x4f;                                    // Duty cycle MSBs
   PIR1bits.TMR2IF = 0;                               // Clear interrupt
   T2CON = 0x56;
   
}
*/

void vBeep(unsigned char ucBeep) 
{
   unsigned char ucI       = 0;
   
   switch(ucBeep)
   {        
      case 0:  // 3 tones
         vSound(20, 100);        
         vSound(30, 70);       
         vSound(25, 85);
      break;
      case 1:  // Sweep 
         for (ucI = 0; ucI < 40; ucI++)            
            vSound(ucI * 3, 10);         
      break;   
      case 2:  // Tiny bipp
         vSound(10, 50);       
      break;
      case 3:  // Low tone 
         vSound(170, 30);         
      break;       
      case 4:  // Tick
         vSound(100, 5);         
      break; 
      case 5:  // Ding
         CCP1CONbits.CCP1M3 = 1;
         CCP1CONbits.CCP1M2 = 1;
         CCP1CON = 0x0c;
         
         CCPR1L = 0x85;                      // Pulse width
         PR2 = 0xFE;                         // Frequency
         PIR1bits.TMR2IF = 0;
         PIE1bits.TMR2IE = 1;
         T2CONbits.TMR2ON = 1;               // Timer2 on                            
      break;   
       
   }
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

void vInit_device(void) 
{
   // PWM setup
   CCP1CONbits.CCP1M3 = 0;
   CCP1CONbits.CCP1M2 = 0;
   CCP1CONbits.CCP1M1 = 0;
   CCP1CONbits.CCP1M0 = 0;
   
   CCPR1L = 0x55;                      // Initial pulse width
   PR2 = 0xFF;                         // Initial frequency
   T2CON = 0x7E;                       // Timer2 on, prescaler postscaler 16
   T2CONbits.TMR2ON = 0;               // Timer2 off         
   
   UCFGbits.UTRDIS = 1;                // USB tranceiver off 
   UCONbits.USBEN = 0;                 // USB off
   
   // Disable AD
   ADCON0bits.ADON = 0;
   
   
   // Interrupt registers
   INTCON   = 0x00;
   INTCON2  = 0x80;
   INTCON3  = 0x00;
   PIE1     = 0x00;
   PIE2     = 0x00;
   IPR1     = 0x00;
   IPR2     = 0x00; 
   RCON     = 0x00;                    // Disable interrupt priorities  
}   
   
//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

void vInit_ports(void) 
{
   TRISA = 0x00;  LATA = 0xFF;
   TRISB = 0x04;  LATB = 0xF1;         // In on RB2   
   TRISC = 0x18;  LATC = 0xFE;         // RPI Status IN on pin 4, 5
   TRISD = 0x00;  LATD = 0xFC;         // Buffered lines: high is off   
   TRISE = 0x08;  LATE = 0xFF;
   
   TRISBbits.TRISB2 = 1;               // Input for ACTION
   INTCON2bits.nRBPU = 0;              // Pull-up on RB
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

void vInit_timers(void) 
{ 
   // Timer 0 light pace
   T0CONbits.TMR0ON = 0;               // Disable TMR0
   T0CONbits.T08BIT = 0;               // Timer0 is 16bit timer/counter
   T0CONbits.T0CS = 0;                 // Internal clock source  
   T0CONbits.PSA = 0;                  // Prescaler enabled
   T0CONbits.T0PS2 = 0;                // Prescaler ratio
   T0CONbits.T0PS1 = 1;
   T0CONbits.T0PS0 = 1;

   INTCON2bits.TMR0IP = 0;             // Make Timer0 interrupt low priority
   INTCONbits.TMR0IF = 0;              // Clear timer interrupt flag
   T0CONbits.TMR0ON = 1;               // Start the timer...
   INTCONbits.TMR0IE = 1;              // Enable TMR0 interrupt
   
}

//*********************************************************************************************************************
// Main 
// 
//*********************************************************************************************************************
//

//#define TEST
//#define IR         // Use infrared signaling to TV

void main() 
{
   unsigned char ucI       = 0;
   unsigned char ucBusy    = 1;
      
   ClrWdt();
    
   // Initialize device
   vInit_device();
   // Configure ports
   vInit_ports();
   
   ClrWdt();
   
   // Configure timers and interrupts
   vInit_timers();
   
   // Button lights off
   vPortOff();
   
   // Enable interrupts
   INTCONbits.GIE_GIEH = 1;
   srand((unsigned)ulLcounter);
      
//*********************************************************************************************************************
//  
    
   ClrWdt();
   
   // Initialize light sequence variables
   ulLcounter = 0;
   cLstate = 0;
   cLoldstate = 250;
   cLseq = 0;
   cLstep = 0;
   cStartStop = 1;                  // Starting                          

   // We are up and running
   // Beep on
   vBeep(0);

   // PWR relay on
   PWR_RELAY = 1;
     
   // Turn button LED on
   SWITCH_LED = LON;

   ClrWdt();
 
   for (ucI = 0; ucI < 15; ucI++)  // 15 seconds
   {   
      vDelay(700);
      SWITCH_LED = LOFF;
      vDelay(300);
      SWITCH_LED = LON;
   }   
   vBeep(2);

   // Turn on TV and accessories
   ACC_RELAY = 1;

   // Start button lights
   cLstate = 1;  

   cStartStop = 0;                  // Done starting                          
      
   // Turn TV on, switch to video (if IR enabled)
#ifdef IR
   vPwrOn();
   vDelay(200);
   vVideo1();
   vDelay(200);
   vVideo2();
   vDelay(200);
#endif

   // Console is up
   // Main loop to poll power switch for shutdown
   while(ucBusy) 
   { 
      ClrWdt();
      SWITCH_LED = LON;
      for (ucI = 0; ucI < 200; ucI++)
      {   
         if (PWR_BUTTON)
         {   
            ucBusy = 0;
            break;
         }   
         vDelay(25);
      }   

      for (ucI = 0; ucI < 4; ucI++)
      {   
         if (PWR_BUTTON)
         {   
            ucBusy = 0;
            break;
         }   
         SWITCH_LED = LOFF;
         vDelay(20);
         if (PWR_BUTTON)
         {   
            ucBusy = 0;
            break;
         }   
         SWITCH_LED = LON;
         vDelay(20);         
      }      
   }   

   cStartStop = 1;                  // Stopping                          
   
   // TV off
   vBeep(2);
   vDelay(300);

#ifdef IR
   vPwrOff();  // (IR)
#endif
   // Turn accessories off
   ACC_RELAY = 0;
 
   // Shut down RPI
   RPI_SHUTDOWN = 1;
   vBeep(1);
   vDelay(3000);
   RPI_SHUTDOWN = 0;
  
   // Wait for system to be shut down 
   for (ucI = 0; ucI < 40; ucI++)  // 
   {   
      vDelay(250);
      SWITCH_LED = 0;
  
      // Stop button lights
      if (ucI == 10)
      {
         ulLcounter = 0;
         cLstate = 3;  
      }   

      vDelay(250);
      SWITCH_LED = 1;
   } 
   vBeep(3);
   // Main power off
   PWR_RELAY = 0;

   // Halt
   while(1)
      vDelay(250);
 
}

//*********************************************************************************************************************
//
















