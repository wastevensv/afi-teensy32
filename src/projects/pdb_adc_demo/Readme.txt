
********************************************************************************
    Release notes for PDB-ADC demo for 50MHz kinetis family
		  Freescale Semiconductor February 2012   
********************************************************************************

Demo code runs on the TWR-K20D50M and FRDM-K50D20M modules.

This demo use the PDB to trigger the ADC, the PDB is adjusted to a frequency equal to 6.4KHZ which 
it is near to the minimum time of conversion of the two channels in sequence. 

So, the sequence error interrupt is enabled to monitor that the triggers are working properly.


For Tower board both ADC channels A and B are reading the voltage of the POT and they
are showed on the terminal.
For Freedom board the ADC channels A and B are readign the voltage of the light sensor 
and they are showed on terminal.

===========
Basic Setup
===========

TWR-K20D50M setup
----------------
Use default jumper setting for board.


FRDM-K20D50M setup
----------------
Use default jumper setting for board.

================
Demo explanation
================

1. Download the code to MCU. Make sure to disconnect the debugger by stopping debugging.

2. Disconnect and connect the USB cable of the Tower board.

2. Open the P&E Micro Terminal Utility and connect the USB COM at 115200 bps with no parity and 8 bit size.

3. Reset the board. A prompt will appear showing reset information. 

4. Two LEDs will be toggling, the blue one toggle every time the conversion of the ADC_CHA is finished, and the green one toggle every time the conversion of the ADC_CHB is finished.

5.If you're using Tower vary the POT and see the value on the terminal. If you're usign Freedom vary the light intensity.
