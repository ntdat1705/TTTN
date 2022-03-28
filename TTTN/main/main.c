#include "main.h"

extern volatile uint16_t Frame_Buffer[IMG_ROWS * IMG_COLUMNS];

void app_main(void)
{
    PWM_Init();
    SCCB_Init();
    OV7670_Pin_Config();
    OV7670_XCLK_Init();
    OV7670_QQVGA_RGB565_Init();
    Capture_Image(IMG_COLUMNS, IMG_ROWS);
    httpd_handle_t server = NULL;
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();
    for(uint32_t i = 0; i < (IMG_COLUMNS*IMG_ROWS); i++)
    {
      printf("%d\t", Frame_Buffer[i]);
    }
    /* Start the server for the first time */
    server = start_webserver();
}
