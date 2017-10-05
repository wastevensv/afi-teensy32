/****************************************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       tsi_test.c
\brief      configure tsi to read TSI CH6 and TSI CH0,
\author     FSL
\version    1.0
\date       Sep 29 2011
*/

#include "tsi.h"


#define NO_TOUCH                 0

#define SLIDER_LENGTH           40 //LENGTH in mm

#define TOTAL_ELECTRODE               2

#define TSI0         0
#define TSI1         1
#define TSI2         2
#define TSI3         3
#define TSI4         4
#define TSI5         5
#define TSI6         6
#define TSI7         7
#define TSI8         8
#define TSI9         9
#define TSI10        10
#define TSI11        11
#define TSI12        12
#define TSI13        13
#define TSI14        14
#define TSI15        15

#define THRESHOLD0   100
#define THRESHOLD1   100
#define THRESHOLD2   100
#define THRESHOLD3   100
#define THRESHOLD4   100
#define THRESHOLD5   100
#define THRESHOLD6   100
#define THRESHOLD7   100
#define THRESHOLD8   100
#define THRESHOLD9   100
#define THRESHOLD10   100
#define THRESHOLD11   100
#define THRESHOLD12   100
#define THRESHOLD13   100
#define THRESHOLD14   100
#define THRESHOLD15   100



/*Chose the correct TSI channel for the electrode number*/


uint8 SliderPercentegePosition[2] = {NO_TOUCH,NO_TOUCH};
uint8 SliderDistancePosition[2] = {NO_TOUCH,NO_TOUCH};


uint8 AbsolutePercentegePosition = NO_TOUCH;
uint8 AbsoluteDistancePosition = NO_TOUCH;
int16 gu16Delta[16];
uint8 ongoing_elec;
uint16 gu16Threshold[16]={THRESHOLD0,THRESHOLD1,THRESHOLD2,THRESHOLD3,THRESHOLD4,THRESHOLD5,
                                THRESHOLD6,THRESHOLD7,THRESHOLD8,THRESHOLD9,THRESHOLD10,THRESHOLD11,
                                THRESHOLD12,THRESHOLD13,THRESHOLD14,THRESHOLD15};
int16 gu16Delta[16];
uint8 ongoing_elec;

uint8 end_flag = TRUE;

/********************************************************************/
void tsi_test_init(void)
{
    printf("****Kinetis TSI Demo****\r\n");
    TSI_Init();
    TSI_SelfCalibration();
    
    START_SCANNING;
    ENABLE_EOS_INT;
    ENABLE_TSI;
}   
      
void TSI_SliderRead(void )
{
     if(end_flag)
      {    
       // printf(" Percentage = %d %           Distance = %d mm          \r", AbsolutePercentegePosition, AbsoluteDistancePosition );   
        end_flag = FALSE;
        
      
        

        if((gu16Delta[0] > gu16Threshold[0])||(gu16Delta[1] > gu16Threshold[1]))
         {
           SliderPercentegePosition[0] = (gu16Delta[0]*100)/(gu16Delta[0]+gu16Delta[1]);
           
           SliderPercentegePosition[1] = (gu16Delta[1]*100)/(gu16Delta[0]+gu16Delta[1]);
           
           SliderDistancePosition[0] = (SliderPercentegePosition[0]* SLIDER_LENGTH)/100;
             
           SliderDistancePosition[1] = (SliderPercentegePosition[1]* SLIDER_LENGTH)/100;
           
          AbsolutePercentegePosition = ((100 - SliderPercentegePosition[0]) + SliderPercentegePosition[1])/2;
          AbsoluteDistancePosition = ((SLIDER_LENGTH - SliderDistancePosition[0]) + SliderDistancePosition[1])/2;
           
         }else
         {
            SliderPercentegePosition[0] = NO_TOUCH;  
            SliderPercentegePosition[1] = NO_TOUCH; 
            SliderDistancePosition[0] = NO_TOUCH; 
            SliderDistancePosition[1] = NO_TOUCH; 
            AbsolutePercentegePosition = NO_TOUCH; 
            AbsoluteDistancePosition = NO_TOUCH; 
         }
      }
 
}



  /***
param lValidTouch   per electrode bit


********************/
void  tsi_test_service(void)
{
  TSI_SliderRead();
  if(lValidTouch&((1<<ELECTRODE0)))
  {
      lValidTouch &= ~((1<<ELECTRODE0));
      printf("\n\r Electrode 1  \n\r");     
    
  }
  if(lValidTouch&((1<<ELECTRODE1)))
  {
      printf("\n\r Electrode 2 \n\r");
      lValidTouch &= ~((1<<ELECTRODE1));
     
  }
  if(lValidTouch&((1<<ELECTRODE2)))
  {
    printf("\n\r Electrode 3  \n\r");
    lValidTouch &= ~((1<<ELECTRODE2));
    
    
  }
  if(lValidTouch&((1<<ELECTRODE3)))
  {
    printf("Electrode 4  \n\r");
    lValidTouch &= ~((1<<ELECTRODE3));
  }
}