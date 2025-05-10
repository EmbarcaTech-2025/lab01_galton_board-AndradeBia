#ifndef BOARD_RENDERER_H
#define BOARD_RENDERER_H

#include "particle_system.h" // Para definição de ParticleManager
#include <stdint.h>         // Para tipos inteiros como uint8_t, uint32_t

// Desenha o quadro atual da simulação (pinos e partículas).
void renderer_draw_simulation_frame(const ParticleManager* pm);

// Desenha o histograma da distribuição de partículas acumuladas.
void renderer_draw_pixel_distribution(const uint8_t* column_stacks);

// Desenha a tela de estatísticas (ex: contagem total de partículas).
void renderer_draw_statistics(uint32_t total_count);

#endif // BOARD_RENDERER_H