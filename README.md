# README - Controle de LEDs e Display OLED com Raspberry Pi Pico

## Descrição
Este projeto implementa um sistema de controle utilizando um Raspberry Pi Pico, que interage com um joystick analógico, LEDs RGB controlados por PWM e um display OLED SSD1306 via barramento I2C. O joystick é utilizado para ajustar o brilho dos LEDs e controlar a interface do display OLED.

## Funcionalidades
- Leitura dos valores do joystick analógico via ADC.
- Controle do brilho dos LEDs RGB utilizando PWM.
- Exibição de gráficos dinâmicos no display OLED com diferentes molduras.
- Interrupções de botões para alternar modos de exibição e controle dos LEDs.

## Hardware Utilizado
- Raspberry Pi Pico
- Joystick analógico
- Display OLED SSD1306 (I2C)
- LEDs RGB
- Botões de entrada

## Conexões
| Componente | Pino Raspberry Pi Pico |
|------------|------------------------|
| Joystick VRX | GP26 (ADC0) |
| Joystick VRY | GP27 (ADC1) |
| Joystick SW  | GP22 |
| LED Vermelho (PWM) | GP13 |
| LED Verde | GP11 |
| LED Azul (PWM) | GP12 |
| Botão A | GP5 |
| Display OLED SDA | GP14 |
| Display OLED SCL | GP15 |

## Dependências
Antes de compilar e rodar o código, certifique-se de que o SDK do Raspberry Pi Pico está instalado corretamente.

Além disso, o projeto faz uso das seguintes bibliotecas:
- `pico/stdlib.h` (para funções básicas do Pico)
- `hardware/adc.h` (para leitura do ADC)
- `hardware/pwm.h` (para controle de PWM)
- `hardware/i2c.h` (para comunicação com o display OLED)
- `inc/ssd1306.h` e `inc/font.h` (para controle do display OLED)

## Compilação e Upload
1. Configure o ambiente de desenvolvimento para o Raspberry Pi Pico.
2. Compile o código utilizando o CMake e o SDK do Pico.
3. Gere o arquivo `.uf2` e transfira para o Pico.

## Como Usar
1. Ao iniciar, o sistema configurará os periféricos e iniciará a leitura do joystick.
2. O joystick controlará os LEDs e desenhará elementos no display OLED.
3. Pressionar os botões alterará os modos de exibição e a iluminação dos LEDs.

## Demonstração
https://youtube.com/shorts/AdgpySh1N0I?si=rXO39NcNOD1RIOQj

