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

static volatile uint32_t last_time = 0;
static volatile uint32_t last_time1 = 0;
static volatile uint32_t last_time2 = 0;
static volatile uint8_t contador = 0;

bool Led_G_ON = false, led_pwm = true;
const int VRX = 26, VRY = 27, ADC_CHANNEL_0 = 0, ADC_CHANNEL_1 = 1, SW = 22, LED_R = 13, LED_G = 11, LED_B = 12, Button22 = 22, ButtonA = 5, ButtonB = 6;
const float DIVIDER_PWM = 40.0;
const uint16_t PERIOD = 500;
uint slice_LED_R, slice_LED_G, slice_LED_B;
ssd1306_t ssd;


void setup_joystick();
void setup_PWM(uint led, uint *slice, uint16_t level);
void setup();
void joystick_read(uint16_t *VRX_value, uint16_t *VRY_value);
void setup_i2c();
void button_callback(uint gpio, uint32_t events);

int main()
{
    setup();
    setup_i2c();
    uint16_t VRX_value, VRY_value, SW_value;
    gpio_set_irq_enabled_with_callback(ButtonA, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(ButtonB, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(Button22, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    while (1)
    {
        int pwm_value_x=abs(VRX_value*PERIOD/4095);
        int pwm_value_y=abs(VRY_value*PERIOD/4095);
        joystick_read(&VRX_value, &VRY_value);
        
        if(led_pwm){
        pwm_set_gpio_level(LED_R, abs(pwm_value_x-250)>30||abs(pwm_value_x)<25 ? pwm_value_x:0);
        pwm_set_gpio_level(LED_B, abs(pwm_value_y-250)>30||abs(pwm_value_y)<25 ? pwm_value_y:0);
        }
        // Mapeando valores do ADC (0-4095) para resolução da tela (x: 0-127, y: 0-63)
        int x = abs((VRX_value * 119) / 4095);
        int y = abs((VRY_value * 54) / 4095);
        ssd1306_fill(&ssd,0);
        ssd1306_square(&ssd,x,y,8,1);
        switch (contador){
            case 1:ssd1306_rect(&ssd,1,1,125,60,1,0); break;
            case 2:
            ssd1306_rect(&ssd,0,1,125,4,1,1);
            ssd1306_rect(&ssd,57,1,125,4,1,1);
            break;
            case 3:
            ssd1306_line(&ssd,3,0,121,0,1);
            ssd1306_line(&ssd,3,60,121,60,1);
            break;
            case 4:
            ssd1306_line(&ssd,1,2,1,60,1);
            ssd1306_line(&ssd,125,2,125,60,1);
            break;
            default:ssd1306_rect(&ssd,0,0,127,64,0,0); break;
        }
        ssd1306_send_data(&ssd);
        
        
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
    uint16_t LEVEL_LED_R =0, LEVEL_LED_G=0, LEVEL_LED_B=0;
    setup_PWM(LED_R, &slice_LED_R, LEVEL_LED_R);
    setup_PWM(LED_B, &slice_LED_B, LEVEL_LED_B);

    gpio_init(LED_G);
    gpio_set_dir(LED_G,GPIO_OUT);
    gpio_init(Button22);
    gpio_init(ButtonA);
    
    gpio_set_dir(Button22,GPIO_IN);
    gpio_set_dir(ButtonA,GPIO_IN);

    gpio_pull_up(Button22);
    gpio_pull_up(ButtonA);
}

void joystick_read(uint16_t *VRX_value, uint16_t *VRY_value)
{
    adc_select_input(ADC_CHANNEL_0);
    sleep_ms(2);
    *VRY_value = adc_read();  // Lê VRY corretamente

    adc_select_input(ADC_CHANNEL_1);
    sleep_ms(2);
    *VRX_value = adc_read();  // Lê VRX corretamente
}

void setup_i2c(){
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
        // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    // ssd1306_send_data(&ssd);                                      // Envia os dados para o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

void button_callback(uint gpio, uint32_t events){
    
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

    if(gpio == ButtonA && tempo_atual - last_time > 200000){
        last_time=tempo_atual;
        led_pwm=!led_pwm;
    }
    if(gpio == ButtonB && tempo_atual - last_time1 > 200000){
        last_time1=tempo_atual;
        
    }
    if(gpio == Button22 && tempo_atual - last_time2 > 200000){
        last_time2=tempo_atual;
        Led_G_ON=!Led_G_ON;
        gpio_put(LED_G,Led_G_ON);
        contador++;
        if(contador>4){
            contador=0;
        }
    }
}