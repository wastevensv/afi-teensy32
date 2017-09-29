.text
.global getc
getc:
        PUSH {LR}
        BL usb_serial_getchar
        POP  {PC}

.global putc
putc:
        PUSH {LR}
        BL usb_serial_putchar
        POP  {PC}
