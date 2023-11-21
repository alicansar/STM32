#include "sys/sys.h"
#include "usart/usart.h"
#include "delay/delay.h"


int main(void) {
    HAL_Init();
    sys_stm32_clock_init(366, 25, 2, 4);
    delay_init(168);
    usart_init(115200);


    while (1) {

    }
}
