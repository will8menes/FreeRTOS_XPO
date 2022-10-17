#include <stdio.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "comp_save_nvs.h"
#include <pzem_service.h>

static const char *tag_nvs = "NVS";
static const char *tag_error_nvs ="ERROR_NVS";

esp_err_t  nvs_read_pzem(struct pzem_values *valores){

	struct pzem_values *pzem_data_flash = valores;
	esp_err_t err;

	nvs_handle_t my_handle;
	int32_t valor=0;
	nvs_open("storage", NVS_READWRITE, &my_handle);

	//LECTURA DE VOLTAJE
	err=nvs_get_i32(my_handle, "VOLTAJE", &valor);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al leer el dato en memoria: (%s)",esp_err_to_name(err));
	}
	pzem_data_flash->voltage=(float)valor/100;
	printf("**********VOLTAJE ES: %f \n",pzem_data_flash->voltage);

	//LECTURA DE CORRIENTE
	err=nvs_get_i32(my_handle, "CORRIENTE", &valor);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al leer el dato en memoria: (%s)",esp_err_to_name(err));
	}
	pzem_data_flash->current=(float)valor/100;
	printf("**********CORRIENTE ES: %f \n",pzem_data_flash->current);

	//LECTURA DE POTENCIA
	err=nvs_get_i32(my_handle, "POTENCIA", &valor);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al leer el dato en memoria: (%s)",esp_err_to_name(err));
	}
	pzem_data_flash->power=(float)valor/100;
	printf("**********POtencia ES: %f \n",pzem_data_flash->power);

	//LECTURA DE FRECUENCIA
	err=nvs_get_i32(my_handle, "FRECUENCIA", &valor);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al leer el dato en memoria: (%s)",esp_err_to_name(err));
	}
	pzem_data_flash->frequency=(float)valor/100;
	printf("**********Frecuencia ES: %f \n",pzem_data_flash->frequency);

	//LECTURA DE FACTOR DE POTENCIA
	err=nvs_get_i32(my_handle, "FACTORP", &valor);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al leer el dato en memoria: (%s)",esp_err_to_name(err));
	}
	pzem_data_flash->pf=(float)valor/100;

	//LECTURA DE ENERGIA
	err=nvs_get_i32(my_handle, "ENERGIA", &valor);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al leer el dato en memoria: (%s)",esp_err_to_name(err));
	}
	pzem_data_flash->energy=(float)valor/100;
	printf("**********ENERGIA ES: %f \n",pzem_data_flash->energy);

	nvs_close(my_handle);
	return ESP_OK;
}


esp_err_t nvs_save_pzem(struct pzem_values *valores){

	struct pzem_values *data_pzem = valores;
	esp_err_t err;
	nvs_handle_t my_handle;

	nvs_open("storage", NVS_READWRITE, &my_handle);
	err=nvs_set_i32(my_handle, "VOLTAJE", (data_pzem->voltage)*100);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al guardar el dato en memoria: (%s)",esp_err_to_name(err));
	}
	err=nvs_set_i32(my_handle, "CORRIENTE", (data_pzem->current)*100);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al guardar el dato en memoria: (%s)",esp_err_to_name(err));
	}
	err=nvs_set_i32(my_handle, "POTENCIA", (data_pzem->power)*100);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al guardar el dato en memoria: (%s)",esp_err_to_name(err));
	}
	err=nvs_set_i32(my_handle, "FRECUENCIA", (data_pzem->frequency)*100);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al guardar el dato en memoria: (%s)",esp_err_to_name(err));
	}
	err=nvs_set_i32(my_handle, "FACTORP", (data_pzem->pf)*100);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al guardar el dato en memoria: (%s)",esp_err_to_name(err));
	}
	err=nvs_set_i32(my_handle, "ENERGIA", (data_pzem->energy)*100);
	if (err!=ESP_OK){
		ESP_LOGE(tag_error_nvs,"Error al guardar el dato en memoria: (%s)",esp_err_to_name(err));
	}
	//**************************************************//
	ESP_LOGE(tag_nvs,"**********Se han guardado los datos en la memoria************");
	printf("***dato_voltaje: %f  *** \n", data_pzem->voltage);
	printf("***dato_corriente: %f  *** \n", data_pzem->current);
	printf("***dato_potencia: %f  *** \n", data_pzem->power);
	printf("***dato_energia: %f  *** \n", data_pzem->energy);
	printf("***dato_frecuencia: %f  *** \n", data_pzem->frequency);

	nvs_commit(my_handle);
	nvs_close(my_handle);

	return ESP_OK;
}
