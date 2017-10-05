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

/**   PIT_init
 * \brief    Initialize Periodic interrupt timer,
 * \brief    PIT1 is used for tone/buzzer time control
 * \author   b01252
 * \param    none
 * \return   none
 */  
extern  void pit_init(void);

extern void pit1_isrv(void);
extern uint16  ti_test_leds;
extern uint16  base_time125m;
