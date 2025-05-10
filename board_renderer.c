#include "board_renderer.h" // Renderizador do tabuleiro
#include "galton_config.h"  // Configurações da simulação
#include "oled_driver.h"    // Driver para o display OLED
#include <stdio.h>          // Para snprintf
#include <string.h>         // Para strlen

// Desenha a disposição dos pinos no tabuleiro
static void _draw_pin_layout() {
    float board_center_x = (float)OLED_WIDTH / 2.0f; // Centro horizontal do tabuleiro

    // Itera sobre cada linha de pinos
    for (int row_idx = 0; row_idx < BOARD_PIN_ROWS; ++row_idx) {
        int pins_this_row = row_idx + 1; // Número de pinos na linha atual
        int y_coord = BOARD_PIN_Y_ORIGIN + (row_idx * BOARD_PIN_ROW_GAP); // Coordenada Y da linha

        // Calcula a largura visual da linha de pinos e a posição X do primeiro pino
        float visual_row_width = (pins_this_row - 1) * BOARD_PIN_HORIZ_GAP;
        float first_pin_x_coord = board_center_x - (visual_row_width / 2.0f);

        // Itera sobre cada pino na linha atual
        for (int pin_idx_in_row = 0; pin_idx_in_row < pins_this_row; ++pin_idx_in_row) {
            // Arredonda para o inteiro mais próximo para posicionamento no pixel
            int x_coord = (int)(first_pin_x_coord + (pin_idx_in_row * BOARD_PIN_HORIZ_GAP) + 0.5f);

            // Verifica se o pino está dentro dos limites do display antes de desenhar
            if (x_coord >= 0 && x_coord < OLED_WIDTH && y_coord >= 0 && y_coord < OLED_HEIGHT) {
                oled_set_pixel(x_coord, y_coord, true); // Desenha o pino
            }
        }
    }
}

// Desenha as partículas ativas na simulação
static void _draw_active_particles(const ParticleManager* pm) {
    if (!pm) return; // Verifica se o gerenciador de partículas é válido

    // Itera sobre todas as partículas possíveis
    for (int i = 0; i < MAX_SIM_PARTICLES; ++i) {
        if (pm->particles[i].is_in_play) { // Se a partícula está ativa
            // Arredonda para o inteiro mais próximo para posicionamento no pixel
            int x_display = (int)(pm->particles[i].x + 0.5f);
            int y_display = (int)(pm->particles[i].y + 0.5f);

            // Verifica se a partícula está dentro dos limites do display antes de desenhar
            if (x_display >= 0 && x_display < OLED_WIDTH && y_display >= 0 && y_display < OLED_HEIGHT) {
                oled_set_pixel(x_display, y_display, true); // Desenha a partícula
            }
        }
    }
}

// Desenha um quadro da simulação (pinos e partículas)
void renderer_draw_simulation_frame(const ParticleManager* pm) {
    oled_clear_buffer();          // Limpa o buffer do display
    _draw_pin_layout();           // Desenha os pinos
    _draw_active_particles(pm);   // Desenha as partículas ativas
    oled_render();                // Atualiza o display com o conteúdo do buffer
}

// Desenha o histograma da distribuição de partículas
void renderer_draw_pixel_distribution(const uint8_t* column_stacks) {
    oled_clear_buffer(); // Limpa o buffer do display
    if (!column_stacks) return; // Verifica se os dados do histograma são válidos

    // Itera sobre cada coluna do display
    for (int x_col = 0; x_col < OLED_WIDTH; ++x_col) {
        uint8_t stack_h = column_stacks[x_col]; // Altura da pilha de partículas na coluna
        if (stack_h > 0) {
            // Desenha a pilha de partículas de baixo para cima
            for (int y_offset_from_bottom = 0; y_offset_from_bottom < stack_h; ++y_offset_from_bottom) {
                int y_coord = OLED_HEIGHT - 1 - y_offset_from_bottom;
                if (y_coord >= 0) { // Garante que está dentro dos limites verticais
                    oled_set_pixel(x_col, y_coord, true); // Desenha um pixel da pilha
                } else {
                    break; // Sai se ultrapassar o topo do display
                }
            }
        }
    }
    oled_render(); // Atualiza o display
}

// Desenha a tela de estatísticas (total de partículas)
void renderer_draw_statistics(uint32_t total_count) {
    oled_clear_buffer();        // Limpa o buffer do display
    char display_str[24];       // Buffer para a string de contagem

    // Exibe "TOTAL"
    oled_draw_string(OLED_WIDTH / 4, 16, "TOTAL");

    // Formata o número total de partículas em uma string
    snprintf(display_str, sizeof(display_str), "%lu", total_count);

    // Centraliza a string de contagem
    int str_pixel_width = strlen(display_str) * 8; // Largura da string em pixels (8 pixels por caractere)
    int x_coord = (OLED_WIDTH - str_pixel_width) / 2;
    if (x_coord < 0) x_coord = 0; // Garante que não comece fora da tela

    oled_draw_string(x_coord, 32, display_str); // Exibe a contagem

    oled_render(); // Atualiza o display
}