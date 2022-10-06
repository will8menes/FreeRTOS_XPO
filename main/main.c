#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>


#define BLINK_GPIO 26

void app_main(void)
{
    while (true) {
        printf("Hello from app_main!\n");
        sleep(1);
    }
}
