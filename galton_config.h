#ifndef GALTON_CONFIG_H
#define GALTON_CONFIG_H

#include "oled_driver.h" 

// === Configurações Gerais da Simulação ===
#define SIM_FRAME_INTERVAL_MS   20   // Intervalo entre quadros da simulação (ms)
#define MAX_SIM_PARTICLES       50   // Número máximo de partículas simultâneas

// === Configurações dos Botões de Interação ===
#define PIN_ADD_PARTICLE        6    // Pino GPIO para adicionar partícula
#define PIN_CYCLE_VIEW          5    // Pino GPIO para alternar visualização
#define BUTTON_DEBOUNCE_INTERVAL_US (200 * 1000) // Intervalo de debounce para botões (µs)

// === Configurações da Estrutura de Pinos (Pirâmide) ===
#define BOARD_PIN_ROWS          7    // Número de linhas de pinos
#define BOARD_PIN_Y_ORIGIN      8    // Coordenada Y inicial da primeira linha de pinos
#define BOARD_PIN_ROW_GAP       8    // Espaçamento vertical entre linhas de pinos
#define BOARD_PIN_HORIZ_GAP     8    // Espaçamento horizontal entre pinos na mesma linha

// === Configurações das Partículas (Bolinhas) ===
// Posição inicial X centralizada
#define PARTICLE_START_X_POS    (OLED_WIDTH / 2.0f)
// Posição inicial Y um pouco acima da primeira linha de pinos
#define PARTICLE_START_Y_POS    (BOARD_PIN_Y_ORIGIN - (BOARD_PIN_ROW_GAP / 2.0f))
#define PARTICLE_FALL_VELOCITY  0.8f // Velocidade de queda da partícula (pixels por quadro)
// Desvio horizontal ao colidir com um pino (metade do espaço entre pinos)
#define PARTICLE_X_DEFLECTION_AMOUNT (BOARD_PIN_HORIZ_GAP / 2.0f)

// Probabilidade em % da partícula desviar para a DIREITA ao colidir com um pino.
#define PROBABILITY_RIGHT_DEFLECTION_PERCENT 50

#endif // GALTON_CONFIG_H