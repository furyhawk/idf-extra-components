#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <esp_log.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>

static const char *TAG = "nvs_writer";

/* Reuse the example dev-mode salt/verifier from the example app so you can
 * quickly provision devices for testing. In production, replace with
 * device-specific values. */
static const char sec2_salt[] = {
    0x03, 0x6e, 0xe0, 0xc7, 0xbc, 0xb9, 0xed, 0xa8, 0x4c, 0x9e, 0xac, 0x97, 0xd9, 0x3d, 0xec, 0xf4
};

static const char sec2_verifier[] = {
    0x7c, 0x7c, 0x85, 0x47, 0x65, 0x08, 0x94, 0x6d, 0xd6, 0x36, 0xaf, 0x37, 0xd7, 0xe8, 0x91, 0x43,
    0x78, 0xcf, 0xfd, 0x61, 0x6c, 0x59, 0xd2, 0xf8, 0x39, 0x08, 0x12, 0x72, 0x38, 0xde, 0x9e, 0x24
};

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init NVS: %s", esp_err_to_name(err));
        return;
    }

    nvs_handle_t handle;
    ESP_LOGI(TAG, "Opening NVS namespace 'sec2' for write");
    err = nvs_open("sec2", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_open failed: %s", esp_err_to_name(err));
        return;
    }

    err = nvs_set_blob(handle, "salt", sec2_salt, sizeof(sec2_salt));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_set_blob(salt) failed: %s", esp_err_to_name(err));
        nvs_close(handle);
        return;
    }

    err = nvs_set_blob(handle, "verifier", sec2_verifier, sizeof(sec2_verifier));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_set_blob(verifier) failed: %s", esp_err_to_name(err));
        nvs_close(handle);
        return;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_commit failed: %s", esp_err_to_name(err));
        nvs_close(handle);
        return;
    }

    nvs_close(handle);
    ESP_LOGI(TAG, "Successfully wrote salt (%d bytes) and verifier (%d bytes) to NVS namespace 'sec2'", (int)sizeof(sec2_salt), (int)sizeof(sec2_verifier));

    ESP_LOGI(TAG, "Done. Restarting in 2s...");
    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart();
}
