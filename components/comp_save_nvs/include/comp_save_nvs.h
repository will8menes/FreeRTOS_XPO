#include <pzem_service.h>

esp_err_t nvs_save_pzem(struct pzem_values *valores);
esp_err_t nvs_read_pzem(struct pzem_values *valores);
