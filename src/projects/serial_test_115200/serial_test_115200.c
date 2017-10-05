/**
  \mainpage 
  \n Copyright (c) 2011 Freescale
  \n Freescale Confidential Proprietary 
  \brief 
  \author   	Freescale Semiconductor
  \author    
  \version      m.n
  \date         Sep 14, 2011
  
  Put here all the information needed of the Project. Basic configuration as well as information on
  the project definition will be very useful 
*/
/****************************************************************************************************/
/*                                                                                                  */
/* All software, source code, included documentation, and any implied know-how are property of      */
/* Freescale Semiconductor and therefore considered CONFIDENTIAL INFORMATION.                       */
/* This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.                      */
/*                                                                                                  */
/* All Confidential Information remains the property of Freescale Semiconductor and will not be     */
/* copied or reproduced without the express written permission of the Discloser, except for copies  */
/* that are absolutely necessary in order to fulfill the Purpose.                                   */
/*                                                                                                  */
/* Services performed by FREESCALE in this matter are performed AS IS and without any warranty.     */
/* CUSTOMER retains the final decision relative to the total design and functionality of the end    */
/* product.                                                                                         */
/* FREESCALE neither guarantees nor will be held liable by CUSTOMER for the success of this project.*/
/*                                                                                                  */
/* FREESCALE disclaims all warranties, express, implied or statutory including, but not limited to, */
/* implied warranty of merchantability or fitness for a particular purpose on any hardware,         */
/* software ore advise supplied to the project by FREESCALE, and or any product resulting from      */
/* FREESCALE services.                                                                              */
/* In no event shall FREESCALE be liable for incidental or consequential damages arising out of     */
/* this agreement. CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or */
/* actions by anyone on account of any damage,or injury, whether commercial, contractual, or        */
/* tortuous, rising directly or indirectly as a result of the advise or assistance supplied CUSTOMER*/ 
/* in connectionwith product, services or goods supplied under this Agreement.                      */
/*                                                                                                  */
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include "common.h"

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/


#include "common.h"
#include "frdm-k20-def.h"


/**
 * \brief   main
 * \author  
 * \param  none
 * \return none
 * \todo
 * \warning
 */  
int main (void)
{
	char ch;

#ifdef KEIL
	start();
#endif
        frdm_GPIO_init();
        LED1_OFF;
        LED2_OFF;
        LED3_OFF;
          
  	printf("\n\r Running the serial_test_115200 project in K2050MHz family \n");
        printf("\n\r        FRDM-K20D50M     ");
        printf("\n\r   press a key to power on/off  leds ");
        printf("\n\r  1=Green; 2= Red; 3= Blue;  4=off    ");

	while(1)
	{
		ch = getchar();
		putchar(ch);
                switch (ch&0x3)
                {
                 case 1: LED1_TOGGLE; break; 
                 case 2: LED2_TOGGLE; break;
                 case 3: LED3_TOGGLE; break;
                 case 0:
                   LED1_OFF;
                   LED2_OFF;
                   LED3_OFF;
                }
                
	} 
}
/********************************************************************/
