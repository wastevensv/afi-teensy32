/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1



// ISR(s) are defined in your project directory.
extern void adc0_isr(void);
extern void vfnDMACh14ISR(void);
void vfnDMA_ErrorIsr(void);
extern void pit1_isrv(void);
extern void USB_ISR (void);
extern void tsi_isr(void);
#undef  VECTOR_053
#define VECTOR_053 tsi_isr

#undef VECTOR_051
#define VECTOR_051 USB_ISR

#undef  VECTOR_038
#define VECTOR_038 adc0_isr


#undef   VECTOR_016      // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete
#define  VECTOR_016  vfnDMACh14ISR    // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete

#undef   VECTOR_021 
#define  VECTOR_021 vfnDMA_ErrorIsr   


#undef   VECTOR_047
#define  VECTOR_047 pit1_isrv


#endif  //__ISR_H

/* End of "isr.h" */
