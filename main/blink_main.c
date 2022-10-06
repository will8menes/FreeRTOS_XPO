#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


#define BLINK_GPIO 26

const char *TAG = "MAIN";

void vTaskBlink(void *pvParameters)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
	gpio_reset_pin(BLINK_GPIO);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1){
    	ESP_LOGI(TAG,"encendiendo el led");
    	//printf("encendiendo el led \n");
    	gpio_set_level(BLINK_GPIO, 1);
    	vTaskDelay(500 / portTICK_PERIOD_MS);
    	ESP_LOGW(TAG,"apagando el led");
    	//printf("Apagando el led \n");
    	gpio_set_level(BLINK_GPIO, 0);
    	vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    xTaskCreatePinnedToCore(vTaskBlink, "task1",2024,NULL,1,NULL,1);
    							// Use xTaskCreate() in vanilla FreeRTOS
    			  	    		// Function to be called
    							// Name of task
    							// Stack size (bytes in ESP32, words in FreeRTOS)
    							// Parameter to pass to function
    							// Task priority (0 to configMAX_PRIORITIES - 1)
    							// Task handle
    							// Run on one core for demo purposes (ESP32 only)
}
