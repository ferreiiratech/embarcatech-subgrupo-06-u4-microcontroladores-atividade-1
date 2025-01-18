#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

#define linhas 4
#define colunas 4

#define green 11
#define blue 12
#define red 13

#define buzzer 21
#define buzzf 10000

unsigned int gpio_linhas[linhas] = {16, 17, 19, 20};
unsigned int gpio_colunas[colunas] = {4, 8, 9, 28};
const char keymap[linhas][colunas] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Inicializa os GPIOs e PWM
void initialize_hardware() {
    gpio_init(buzzer);
    gpio_set_dir(buzzer, GPIO_OUT);
    gpio_set_function(buzzer, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(buzzer);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (buzzf * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(buzzer, 0);

    gpio_init(green);
    gpio_set_dir(green, GPIO_OUT);

    gpio_init(blue);
    gpio_set_dir(blue, GPIO_OUT);

    gpio_init(red);
    gpio_set_dir(red, GPIO_OUT);

    for (int i = 0; i < linhas; i++) {
        gpio_init(gpio_linhas[i]);
        gpio_set_dir(gpio_linhas[i], GPIO_OUT);
        gpio_put(gpio_linhas[i], 1);
    }

    for (int j = 0; j < colunas; j++) {
        gpio_init(gpio_colunas[j]);
        gpio_set_dir(gpio_colunas[j], GPIO_IN);
        gpio_pull_up(gpio_colunas[j]);
    }
}

// Menu de instruções
void display_menu() {
    printf("Keypad iniciado. Pressione uma tecla...\n");
    printf("Pressione 2 para ligar o LED verde\n");
    printf("Pressione 3 para ligar o LED azul\n");
    printf("Pressione 5 para ligar o LED vermelho\n");
    printf("Pressione 6 para ligar todos os LEDs\n");
    printf("Pressione 0 para desligar todos os LEDs\n");
    printf("Pressione # para ativar o buzzer\n");
}

// Lê a tecla pressionada no keypad
char keypad_get_key() {
    for (int i = 0; i < linhas; i++) {
        gpio_put(gpio_linhas[i], 0);
        for (int j = 0; j < colunas; j++) {
            if (gpio_get(gpio_colunas[j]) == 0) {
                while (gpio_get(gpio_colunas[j]) == 0);
                gpio_put(gpio_linhas[i], 1);
                return keymap[i][j];
            }
        }
        gpio_put(gpio_linhas[i], 1);
    }
    return '\0';
}

// Controle dos LEDs
void control_leds(int green_state, int blue_state, int red_state) {
    gpio_put(green, green_state);
    gpio_put(blue, blue_state);
    gpio_put(red, red_state);
}

// Pisca todos os LEDs (Função secreta 1)
void blink_all_leds() {
    for (int i = 0; i < 5; i++) {
        control_leds(1, 1, 1);
        sleep_ms(200);
        control_leds(0, 0, 0);
        sleep_ms(200);
    }
}