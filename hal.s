.equ UART_ADDR,    0x4006A000
.equ UART_D_OFFSET,     0x07
.equ UART_S1_OFFSET,    0x04
.equ UART_S1_RDRF_MASK, 0x20
.equ UART_S1_TDRE_MASK, 0x80

.text
.global getc
getc:
/*  Reads the latest byte from the UART and stores it in R0.
 *  Preconditions:
 *  Modifies:
 *    R0 will contain the value in the UART buffer.
 */
        PUSH {R1-R3,LR}                 // Preserve used registers in stack.
        LDR   R1, =UART_ADDR            // Load R1 with UART_ADDR.
        MOV   R3, #UART_S1_RDRF_MASK
1:      LDR   R2, [R1,#UART_S1_OFFSET]  // Check [R1:UART_FR_OFFSET] & UART_TXFF_MASK == 0.
        TST   R2, R3
        BNE   1b                        // Loop till UART0 is ready to TX.
        LDR   R0, [R1,#UART_D_OFFSET]  // Store value R0 in [R1:UART_DR_OFFSET].
        POP  {R1-R3,PC}                 // Return used registers from stack.

.global putc
putc:
/*  Prints the contents of R0 out of the UART pointed to by R1.
 *  Preconditions:
 *    R0 contains the character to send over the UART.
 *  Modifies: None
 */
        PUSH {R1-R3,LR}                 // Preserve used registers in stack.
        LDR   R1, =UART_ADDR            // Load R1 with UART_ADDR.
        MOV   R3, #UART_S1_TDRE_MASK
1:      LDR   R2, [R1,#UART_S1_OFFSET]  // Check [R1:UART_FR_OFFSET] & UART_TXFF_MASK == 0.
        TST   R2, R3
        BNE   1b                        // Loop till UART0 is ready to TX.
        STR   R0, [R1,#UART_D_OFFSET]  // Store value R0 in [R1:UART_DR_OFFSET].
        POP  {R1-R3,PC}                 // Return used registers from stack.
