#include "PWM.h"

//cap xung 8MHz cho camera
void PWM_Init(void)
{
	ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_1_BIT, // resolution of PWM duty
        .freq_hz = PWM_FREQ,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = LEDC_TIMER_0,            // timer index
        .clk_cfg = LEDC_USE_APB_CLK,              // Auto select the source clock
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
            .channel    = LEDC_CHANNEL_0,
            .duty       = 1,
            .gpio_num   = PWM_GPIO_NUM,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0,
            .intr_type = LEDC_INTR_DISABLE,
    };

    // Set LED Controller with previously prepared configuration
    
        ledc_channel_config(&ledc_channel);
}