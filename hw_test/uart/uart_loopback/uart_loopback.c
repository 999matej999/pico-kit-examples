#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();
    
    printf("UART loopback test\n");

    while (true)
    {
        int c = getchar_timeout_us(0);
        if (c >= 0) putchar_raw(c);
    }
}
