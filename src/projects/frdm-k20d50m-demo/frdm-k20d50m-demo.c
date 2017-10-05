/**
 * File:		frdm_k20d50m_demo.c
 * Purpose:		shows features of the KL25 and the frdm_board.
 *    Accelerometer  - change color of RGB led depending on the tilt angle.
 *    USB            - connects as communication device and echoes character and number of received characters
 *    FTM            - PWM to control RGB colors
 *    TSI            - Detect finger position on Electrode
 *    UART           - 115200,n,8) Display light sensor, temperature, accelerometer,
                       Electrodes (delta, percentage,position), Electrode touch.
 *    Feb 27,2013    
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
#include "adc_test.h"
#include "adc16.h"
#include "freemaster.h"
#include "frdm-k20-def.h"
#include "usb_cdc.h"
#include "usb_reg.h"
#include "FSLTypes.h"
#include "TSI.h"






/*
 * Test structure types - demonstrates the "TSA" feature thanks to which the 
 * FreeMASTER is able to load a variable and type information directly from 
 * the embedded application. 
 *
 */

typedef struct {
  
  uint16_t  aa;
  uint32_t bb[2];
  uint16_t  cc;
  uint32_t dd[3];
  uint8_t  ee;
  uint8_t  ff[5];
} INNER_STRUCT;

typedef struct {
  
  uint16_t  a;
  uint32_t b;
  INNER_STRUCT inA[4];
  INNER_STRUCT inB;
} OUTER_STRUCT;

/* Structure type information will be available in the FreeMASTER application (TSA) */
OUTER_STRUCT so1, so2;
INNER_STRUCT si1, si2;


/////////////////////////////////////////////////////////////////////////

/*
 * With TSA enabled, the user describes the global and static variables using 
 * so-called TSA tables. There can be any number of tables defined in 
 * the project files. Each table does have the identifier which should be
 * unique across the project. 
 *
 * Note that you can declare variables as Read-Only or Read-Write.
 * The FreeMASTER driver denies any write access to the Read-Only variables
 * when TSA_SAFETY is enabled.
 */



/////////////////////////////////////////////////////////////////////////

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/

void tsi_test_init(void);
void tsi_test_service(void);
void accel_read(void);


/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

#define FTM_Cn_MODE  (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK)
#define FTM_MODULE    1000
#define FTM_INIT_VAL    25
#define SET_LED_GREEN(x)   FTM0_C4V = (x)
#define SET_LED_RED(x)     FTM0_C2V = (x)
#define SET_LED_BLUE(x)    FTM0_C7V = (x)


#define T_DELAY_LED  10

#define RED_MASK   0x01
#define GREEN_MASK 0x02
#define BLUE_MASK  0x04

/* Testing Suite */

#undef  FREEMASTER_ENABLE
#define USB_ENABLE

#define USB_DEVICE_XCVR_TEST

#define ADC_LIGHT_CH       19 //ADC0_DM0
#define ADC_TEMPERATURE_CH 21 //ADC0_DM3
/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/


char string0[] = "FRDM-K20D50M Demo Application\n";
char string1[] = "Tilt the board to change the color mix\n";
char string2[] = "Touch the slider to change white LED brightness\n";
char string3[] = "Visit www.freescale.com/frdm-k20d50m for more software samples\n";
char string4[] = "\n";
char *strings[] = {string0, string1, string2, string3, string4};
const int NumStrings = 5;



extern uint8 AbsolutePercentegePosition;
extern uint8 AbsoluteDistancePosition;
extern uint8 gu8USB_State;
extern uint8 gu8USB_Flags;
extern uint8 gu8EP3_OUT_ODD_Buffer[];


extern tBDT tBDTtable[];
char gu8USB_Buffer[64];

volatile uint8  gu8ISR_Flags;

/* Test variables, will be displayed in the FreeMASTER application */
volatile uint8_t   var8;
volatile uint16_t  var16;
volatile uint16_t  var16inc = 1;
volatile uint32_t  var32;
volatile uint32_t  var32inc = 100;
volatile uint8_t   nAppCmdCounter;

uint32_t ti_print;

signed short accel_x, accel_y, accel_z;
signed short resultx, resulty, resultz;
uint8  count_initial_led_cycles=10;

uint16_t light_sensor;
uint16_t temp_sensor;
uint16 ti_accel_sampling;

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/


#include "common.h"

#include "adc_test.h"
#include "frdm-k20-def.h"
#include "pit.h"
#include "hal_i2c.h"
#include "hal_dev_mma8451.h"
#include "angle_cal.h"
#include "task_mgr.h"

extern int16 gu16Delta[16];

extern uint16 ti_accel_sampling;


void accel_init(void)
{
    unsigned char tmp;
    printf("****Accelerometor init ****\n");
    //Initialize I2C modules
    hal_i2c_init();
   //Configure MMA8451 sensor //MMA8451Q
    tmp = hal_dev_mma8451_read_reg(0x2a);
    hal_dev_mma8451_write_reg(0x2a,tmp|0x01);
}

void vfn_test_accel(void)
{
               if (ti_accel_sampling==0)
             {
               accel_read();              
               ti_accel_sampling = 10;            
               } 

}


/*if read available update  accel_x,accel_y and accel_z */
void accel_read(void)
{
        if((hal_dev_mma8451_read_reg(0x00)&0xf) != 0)
        {
            resultx   = hal_dev_mma8451_read_reg(0x01)<<8;
            resultx  |= hal_dev_mma8451_read_reg(0x02);
            resultx >>= 8;

            resulty   = hal_dev_mma8451_read_reg(0x03)<<8;
            resulty  |= hal_dev_mma8451_read_reg(0x04);
            resulty >>= 8;

            resultz   = hal_dev_mma8451_read_reg(0x05)<<8;
            resultz  |= hal_dev_mma8451_read_reg(0x06);
            resultz >>= 8;
            
            
            
            angle_calculation(); //-900  to  900            
            detect_fall_detection();
            
                             
        }
            
}


/******************************************************************************/
void accel_demo(void)
{
    if (ti_accel_sampling==0)
    {
        accel_read();
        ti_accel_sampling = 10;
    }

    if (xy_mag > 20)
    {
        SET_LED_RED((abs(xz_angle) > 450) ? 0 : (450 - abs(xz_angle)));
        SET_LED_GREEN((abs(yz_angle) > 450) ? 0 : (450 - abs(yz_angle)));
        SET_LED_BLUE((abs(xy_angle) > 450) ? 0 : (450 - abs(xy_angle)));
    }
    if (xy_mag < 20 &&  AbsolutePercentegePosition > 0 )
    {
        SET_LED_BLUE(AbsolutePercentegePosition*8);
        SET_LED_RED(AbsolutePercentegePosition*7);
        SET_LED_GREEN(AbsolutePercentegePosition*8);
    }

}


/******************************************************************************/
/**   FTM_init
 * \brief    Initialize LPTPM for RGB led control
 * \brief    FTM2 and FTM  in PWM edge aligned mode
 * \author   b01252
 * \param    none
 * \return   none
 */
void FTM_init(void)
{
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK;
    SIM_SCGC6|= SIM_SCGC6_FTM0_MASK;
  
    PORTA_PCR2 = (0|PORT_PCR_MUX(3)); /* FTM0_CH2 enable on PTA2 */
    PORTC_PCR3 = (0|PORT_PCR_MUX(4)); /* FTM0_CH4 enable on PTC3 */
    PORTD_PCR4 = (0|PORT_PCR_MUX(4)); /* FTM0_CH7 enable on PTD4 */

    FTM0_MOD  = FTM_MODULE;  /* 0x0063 / 25MHz = 4uS PWM period */
    FTM0_C2SC = FTM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
    FTM0_C2V  = FTM_INIT_VAL;  /* 90% pulse width */

    FTM0_C4SC = FTM_Cn_MODE;   /* No Interrupts; Low True pulses on Edge Aligned PWM */
    FTM0_C4V  = FTM_INIT_VAL;  /* 90% pulse width */

    FTM0_C7SC = FTM_Cn_MODE;   /* No Interrupts; Low True pulses on Edge Aligned PWM */
    FTM0_C7V  = FTM_INIT_VAL;  /* 90% pulse width */

    FTM0_SC   = FTM_SC_CLKS(1) | FTM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
}
/******************************************************************************/
/**   vfn_rgb_test
 * \brief    test RGB leds, by turning them sequencially, at the end switch to accel_demo task.
 * \brief
 * \param    none
 * \return   none
 */
void vfn_rgb_test(void)
{
    static char seq_count=0;

    if (!timer_task)
    {
        seq_count++;
        timer_task = 500;
        switch (seq_count)
        {
            case 1:
                SET_LED_RED(300);
                SET_LED_GREEN(0);
                SET_LED_BLUE(0);
                break;
            case 2:
                SET_LED_RED(0);
                SET_LED_GREEN(300);
                SET_LED_BLUE(0);
                break;
            case 3:
                SET_LED_RED(0);
                SET_LED_GREEN(0);
                SET_LED_BLUE(300);
                break;
            case 4:
                SET_LED_RED(300);
                SET_LED_GREEN(300);
                SET_LED_BLUE(0);
                break;
            case 5:
                SET_LED_RED(0);
                SET_LED_GREEN(300);
                SET_LED_BLUE(300);
                break;
            case 6:
                SET_LED_RED(300);
                SET_LED_GREEN(0);
                SET_LED_BLUE(300);
                break;
            case 7:
                SET_LED_RED(200);
                SET_LED_GREEN(250);
                SET_LED_BLUE(250);
                break;
            default:
                seq_count=0;
                next_task(accel_demo);
        };
    }
}

/******************************************************************************/
/**   usb_init
 * \brief    initialize usb module.
 * \brief
 * \param    none
 * \return   none
 */
void usb_init(void)
{
      printf("\n********** USB XCVR Module Testing **********\n");     
      USB_REG_SET_ENABLE;
      CDC_Init();
      // Pull up enable
      FLAG_SET(USB_CONTROL_DPPULLUPNONOTG_SHIFT,USB0_CONTROL);
}

/******************************************************************************/
/**   usb_sesrvice
 * \brief    
 * \brief
 * \param    none
 * \return   none
 */
void usb_service(void)
{
    static char char_count;
    // If data transfer arrives
    if(FLAG_CHK(EP_OUT,gu8USB_Flags))
    {
        (void)USB_EP_OUT_SizeCheck(EP_OUT);
        usbEP_Reset(EP_OUT);
        usbSIE_CONTROL(EP_OUT);
        FLAG_CLR(EP_OUT,gu8USB_Flags);

        // Send it back to the PC
        EP_IN_Transfer(EP2,CDC_OUTPointer,1);
        char_count++;
    }
    if (!ti_print)
    {
        ti_print = 300;
        sprintf(gu8USB_Buffer,"\n\r ch count %%= %03i  ", char_count);
        EP_IN_Transfer(EP2,gu8USB_Buffer,20);
    }
}


     


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
	char ch, i;
 int UsbDetected = FALSE;
#ifdef KEIL
	start();
#endif
       // frdm_GPIO_init();
        adc_init();
        pit_init();
        accel_init();
        tsi_test_init();
     
        FTM_init();
#ifdef USB_ENABLE        
        usb_init();
#endif
        
        next_task(vfn_rgb_test);
        
  	printf("\nRunning the frdm-k20d50m-demo project in K2050MHz family\n");
        
        for (i=0;i<NumStrings;i++)  printf(strings[i]); 
#ifdef  FREEMASTER_ENABLE
        FMSTR_Init();
#endif

	while(1)
	{
          
         tsi_test_service(); 
         
         CDC_Engine();
#ifdef USB_ENABLE                  
         usb_service();
        if (gu8USB_State == uENUMERATED && !UsbDetected) {
         //   next_task(vfn_rgb_test);
            UsbDetected = TRUE;
        }
#endif
        
        
#ifdef  FREEMASTER_ENABLE
          FMSTR_Poll();
#else
         if (uart_getchar_present(TERM_PORT))
          {
           ch = getchar();
	   putchar(ch);
           printf("\n\r light sensor              =%4i",light_sensor);
           printf("\n\r temperature sensor        =%4i",temp_sensor);
           printf("\n\r accelerometer xy_angle*10 =%3i Degrees",xy_angle);
           printf("\n\r accelerometer xz_angle*10 =%3i Degrees",xz_angle);
           printf("\n\r accelerometer yz_angle*10 =%3i Degrees",yz_angle);
           printf("\n\r Electrodes delta0  =%i, delta0 = %i",gu16Delta[0],gu16Delta[1]);
           printf("\n\r slider percentage =%i %%, Distance = %i mm",AbsolutePercentegePosition,AbsoluteDistancePosition);
           }
#endif
         
          ptr_next_task();  // do the actual function 
         
          adc_read(ADC_LIGHT_CH);
          light_sensor = adc_sample1;
         
          
          adc_read(ADC_TEMPERATURE_CH);
          temp_sensor = adc_sample1;
         
	} 
}
/********************************************************************/
