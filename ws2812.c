


#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "hardware/timer.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define LED_PISCA 13
#define tempo 100

const uint button_0 = 5;
const uint button_1 = 6;
uint contador = 0;

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0; // Intensidade do vermelho
uint8_t led_g = 0; // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul


static volatile uint a = 1;
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Buffer para desligar leds da matriz
bool desliga_led[NUM_PIXELS] = {
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0
};

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


void set_one_led(uint8_t r, uint8_t g, uint8_t b, bool matriz[])
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (matriz[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

int main()
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

     // Inicializações
    stdio_init_all();
    gpio_init(LED_PISCA);              
    gpio_set_dir(LED_PISCA, GPIO_OUT); 

    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN); 
    gpio_pull_up(button_0); 

    gpio_init(button_1);
    gpio_set_dir(button_1, GPIO_IN); 
    gpio_pull_up(button_1); 

    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_1, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (1)
    {   gpio_put(LED_PISCA,1);
        sleep_ms(tempo);
        gpio_put(LED_PISCA,0);
        sleep_ms(tempo);
    }

    return 0;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    printf("A = %d\n", a);
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 400000) // 400 ms de debouncing
    {
        last_time = current_time; // Atualiza o tempo do último evento
        printf("Mudanca de Estado do Led. A = %d\n", a);

        if(gpio == button_0){
          contador++;
          if(contador > 10 || contador < 0)
          contador = 0;

        }

        else if (gpio == button_1){
            contador--;
            if(contador > 10 || contador < 0)
            contador = 0;
        }
          
                                                 
        a++;                                     // incrementa a variavel de verificação
    }

    switch(contador){


        case 0 :
         bool num_0[NUM_PIXELS] = {
          0, 1, 1, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_0);
        break;

        case 1 :
         bool num_1[NUM_PIXELS] = {
          0, 0, 1, 0, 0, 
          0, 0, 1, 0, 0, 
          0, 0, 1, 0, 0, 
          0, 0, 1, 0, 0, 
          0, 0, 1, 0, 0 };
          set_one_led(led_r, led_g, led_b, num_1);
        break;

        case 2 :
         bool num_2[NUM_PIXELS] = {
          0, 1, 1, 1, 0, 
          0, 1, 0, 0, 0, 
          0, 1, 1, 1, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_2);
        break;

        case 3 :
         bool num_3[NUM_PIXELS] = {
          0, 1, 1, 1, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 1, 1, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_3);
        break;

         case 4 :
         bool num_4[NUM_PIXELS] = {
          0, 1, 0, 0, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 1, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 0, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_4);
        break;

        case 5 :
         bool num_5[NUM_PIXELS] = {
          0, 1, 1, 1, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 1, 1, 0, 
          0, 1, 0, 0, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_5);
        break;

        case 6 :
         bool num_6[NUM_PIXELS] = {
          0, 1, 1, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 1, 1, 0, 
          0, 1, 0, 0, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_6);
        break;

        case 7 :
         bool num_7[NUM_PIXELS] = {
          0, 1, 0, 0, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 0, 0, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_7);
        break;

        case 8 :
         bool num_8[NUM_PIXELS] = {
          0, 1, 1, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 1, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_8);
        break;

        case 9 :
         bool num_9[NUM_PIXELS] = {
          0, 1, 1, 1, 0, 
          0, 0, 0, 1, 0, 
          0, 1, 1, 1, 0, 
          0, 1, 0, 1, 0, 
          0, 1, 1, 1, 0 };
          set_one_led(led_r, led_g, led_b, num_9);
        break;

        default :
        set_one_led(led_r, led_g, led_b, desliga_led);
        

       
    }


}
