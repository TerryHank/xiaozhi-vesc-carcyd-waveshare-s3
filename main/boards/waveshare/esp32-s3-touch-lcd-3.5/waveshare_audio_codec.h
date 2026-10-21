#pragma once

#include "codecs/es8311_audio_codec.h"
#include "config.h"

#include <esp_io_expander.h>
#include <esp_log.h>
#include <mutex>

class WaveshareAudioCodec : public Es8311AudioCodec {
    esp_io_expander_handle_t expander_;
    std::mutex pa_mutex_;

public:
    WaveshareAudioCodec(i2c_master_bus_handle_t bus, esp_io_expander_handle_t expander)
        : Es8311AudioCodec(bus, I2C_NUM_0, AUDIO_INPUT_SAMPLE_RATE, AUDIO_OUTPUT_SAMPLE_RATE,
                           AUDIO_I2S_GPIO_MCLK, AUDIO_I2S_GPIO_BCLK, AUDIO_I2S_GPIO_WS,
                           AUDIO_I2S_GPIO_DOUT, AUDIO_I2S_GPIO_DIN, AUDIO_CODEC_PA_PIN,
                           AUDIO_CODEC_ES8311_ADDR),
          expander_(expander) {
        output_volume_ = 100;
    }

    void EnableOutput(bool enable) override {
        std::lock_guard<std::mutex> lock(pa_mutex_);
        // EXIO7 drives the active-high NS4150B speaker amplifier control.
        if (!enable) {
            ESP_ERROR_CHECK(esp_io_expander_set_level(expander_, IO_EXPANDER_PIN_NUM_7, 0));
        }
        Es8311AudioCodec::EnableOutput(enable);
        if (enable) {
            ESP_ERROR_CHECK(esp_io_expander_set_level(expander_, IO_EXPANDER_PIN_NUM_7, 1));
        }
        uint32_t level = 0;
        esp_err_t result = esp_io_expander_get_level(expander_, IO_EXPANDER_PIN_NUM_7, &level);
        ESP_LOGI("WaveshareAudio", "Speaker PA %s: EXIO7 level=%lu status=%d",
                 enable ? "on" : "off", (unsigned long)level, result);
    }
};
