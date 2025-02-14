#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/i2c.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C // EndereÃ§o do display SSD1306

const int VRX = 26;
const int VRY = 27;
const int ADC_CHANNEL_0 = 0;
const int ADC_CHANNEL_1 = 1;
const int SW = 22;
const int LED_R = 13;
const int LED_G = 12;
const int LED_B = 11;
const float DIVIDER_PWM = 488.0;
const uint16_t PERIOD = 500;
uint16_t LEVEL_LED_R, LEVEL_LED_G, LEVEL_LED_B;
uint slice_LED_R, slice_LED_G, slice_LED_B;

void setup_joystick();
void setup_PWM(uint led, uint *slice, uint16_t level);
void setup();
void joystick_read(uint16_t *VRX_value, uint16_t *VRY_value);
void setup_i2c();

int main()
{
    uint16_t VRX_value, VRY_value, SW_value;
    setup();
    setup_i2c();
    

    while (1)
    {
        joystick_read(&VRX_value, &VRY_value);
        if (VRX_value == 1918 || VRY_value == 2187)
        {
            pwm_set_gpio_level(LED_R, 0);
            pwm_set_gpio_level(LED_B, 0);
            
        }
        else
        {
            pwm_set_gpio_level(LED_B, VRX_value);
            pwm_set_gpio_level(LED_R, VRY_value);
            printf("X: %d\nY: %d\n", VRX_value, VRY_value);
        }
        sleep_ms(10);
    }
}

void setup_joystick()
{
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);

    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

void setup_PWM(uint led, uint *slice, uint16_t level)
{
    gpio_set_function(led, GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(led);
    pwm_set_clkdiv(*slice, DIVIDER_PWM);
    pwm_set_wrap(*slice, PERIOD);
    pwm_set_gpio_level(led, level);
    pwm_set_enabled(*slice, true);
}

void setup()
{
    stdio_init_all();
    setup_joystick();
    setup_PWM(LED_R, &slice_LED_R, LEVEL_LED_R);
    setup_PWM(LED_G, &slice_LED_G, LEVEL_LED_G);
    setup_PWM(LED_B, &slice_LED_B, LEVEL_LED_B);
}

void joystick_read(uint16_t *VRX_value, uint16_t *VRY_value)
{
    adc_select_input(ADC_CHANNEL_1);
    sleep_ms(2);
    *VRX_value = adc_read();
    adc_select_input(ADC_CHANNEL_0);
    sleep_ms(2);
    *VRY_value = adc_read();
}

void setup_i2c(){
    ssd1306_t ssd;
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
        // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}