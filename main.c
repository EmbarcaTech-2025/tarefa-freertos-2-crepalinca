#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h" 

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// --- Definições de Pinos ---
#define BLUE_LED_PIN   12
#define BUZZER_PIN     21

// --- Definições de Tom e Tempo ---
#define DOT_MS 60
#define BUZZER_TONE_HZ 175

// --- Estrutura e Tabela Morse ---
typedef struct {
    char character;
    const char* morse_code;
} MorseMapping;

const MorseMapping morse_table[] = {
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},  {'D', "-.."},
    {'E', "."},     {'F', "..-."},  {'G', "--."},   {'H', "...."},
    {'I', ".."},    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."},
    {'M', "--"},    {'N', "-."},    {'O', "---"},   {'P', ".--."},
    {'Q', "--.-"},  {'R', ".-."},   {'S', "..."},   {'T', "-"},
    {'U', "..-"},   {'V', "...-"},  {'W', ".--"},   {'X', "-..-"},
    {'Y', "-.--"},  {'Z', "--.."},
    {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
    {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
    {'9', "----."}, {'0', "-----"},
    {' ', " "}
};
const int morse_table_size = sizeof(morse_table) / sizeof(morse_table[0]);

// --- Handle da Fila ---
QueueHandle_t morse_queue = NULL;

// --- Funções Auxiliares para o Sinal Morse ---
void turn_signal_on() {
    gpio_put(BLUE_LED_PIN, 1);
    // Para ligar o som, definimos o duty cycle para 50% de 255 (nosso wrap fixo)
    pwm_set_gpio_level(BUZZER_PIN, 127); 
}

void turn_signal_off() {
    gpio_put(BLUE_LED_PIN, 0);
    // Para desligar, duty cycle 0
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

void play_dot() {
    turn_signal_on();
    vTaskDelay(pdMS_TO_TICKS(DOT_MS));
    turn_signal_off();
}

void play_dash() {
    turn_signal_on();
    vTaskDelay(pdMS_TO_TICKS(3 * DOT_MS));
    turn_signal_off();
}

const char* get_morse_code(char c) {
    for (int i = 0; i < morse_table_size; i++) {
        if (morse_table[i].character == c) {
            return morse_table[i].morse_code;
        }
    }
    return NULL;
}
void morse_output_task(void *pvParameters) {
    char char_to_play;
    const char *morse_str;
    for (;;) {
        if (xQueueReceive(morse_queue, &char_to_play, portMAX_DELAY)) {
            morse_str = get_morse_code(char_to_play);
            if (morse_str) {
                if (char_to_play == ' ') {
                    vTaskDelay(pdMS_TO_TICKS(4 * DOT_MS));
                } else {
                    for (int i = 0; morse_str[i] != '\0'; i++) {
                        if (morse_str[i] == '.') {
                            play_dot();
                        } else if (morse_str[i] == '-') {
                            play_dash();
                        }
                        if (morse_str[i+1] != '\0') {
                            vTaskDelay(pdMS_TO_TICKS(DOT_MS));
                        }
                    }
                    vTaskDelay(pdMS_TO_TICKS(3 * DOT_MS));
                }
            }
        }
    }
}
void console_reader_task(void *pvParameters) {
    int c;
    for (;;) {
        c = getchar();
        if (c != PICO_ERROR_TIMEOUT) {
            char uppercase_c = toupper(c);
            xQueueSend(morse_queue, &uppercase_c, portMAX_DELAY);
        }
    }
}


int main() {
    stdio_init_all();

    // --- Configuração do LED ---
    gpio_init(BLUE_LED_PIN); 
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);

    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    uint32_t sys_clock_hz = clock_get_hz(clk_sys);
    
    // Valor de wrap fixo (255)
    uint16_t wrap_value = 255; 
    pwm_set_wrap(slice_num, wrap_value);

    // Calcula o divisor de clock para atingir a frequência desejada
    float div = (float)sys_clock_hz / ( (float)BUZZER_TONE_HZ * ((float)wrap_value + 1.0f) );
    pwm_set_clkdiv(slice_num, div);

    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(BUZZER_PIN, 0); 

    // --- Cria a Fila e as Tarefas ---
    morse_queue = xQueueCreate(20, sizeof(char));
    xTaskCreate(console_reader_task, "Reader_Task", 256, NULL, 1, NULL);
    xTaskCreate(morse_output_task, "Morse_Task", 256, NULL, 1, NULL);
    
    vTaskStartScheduler();

    while (true) {}
    return 0;
}