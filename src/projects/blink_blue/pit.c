/****************************************************************************************************/
/**
Copyright (c) 2011 Freescale Semiconductor
Freescale Confidential Proprietary
\file      pit.c
\brief     Buzzer is based on a FTM and one channel,  also it uses and PIT for timer control  
\author     
\author     
\version    1.0
\date       Sep 26, 2011
*/

#include "common.h"
#include "pit.h"



uint16  ti_test_leds;
uint16  base_time125m;

/**   PIT_init
 * \brief    Initialize Periodic interrupt timer,
 * \brief    PIT1 is used for tone/buzzer time control
 * \author   b01252
 * \param    none
 * \return   none
 */  
void pit_init(void)
{
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; // enable PIT module
    enable_irq(INT_PIT1 - 16);
       
    PIT_MCR = 0x00;  // MDIS = 0  enables timer
    PIT_TCTRL1 = 0x00; // disable PIT0
    PIT_LDVAL1 = 48000; // 
    PIT_TCTRL1 = PIT_TCTRL_TIE_MASK; // enable PIT0 and interrupt
    PIT_TFLG1 = 0x01; // clear flag
    PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK;
   
}



/* Pit ISRT*/

void pit1_isrv(void)
{  
    static char count_mseg=125;
    PIT_TFLG1 = 0x01; // clear flag
    PIT_TCTRL1; // dummy read to ensure the interrupt, 
                // must be executed here, or interrupt will never happen.
   
    if (ti_test_leds) ti_test_leds--;
    
    if (count_mseg)count_mseg--;
    else
    {
     count_mseg=125;
    // base_time125m++; 
    }

}

