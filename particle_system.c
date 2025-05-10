#include "particle_system.h"
#include "pico/stdlib.h" // Para rand()
#include <string.h>     // Para memset()

// Inicializa o gerenciador de partículas
void ps_initialize(ParticleManager* pm) {
    if (!pm) return; // Verifica ponteiro nulo

    // Zera todas as partículas e configura como não ativas
    memset(pm->particles, 0, sizeof(pm->particles));
    for (int i = 0; i < MAX_SIM_PARTICLES; ++i) {
        pm->particles[i].is_in_play = false;
    }
    pm->next_particle_slot = 0; // Próximo slot para nova partícula

    // Zera o contador de pixels por coluna (histograma)
    memset(pm->column_pixel_counts, 0, sizeof(pm->column_pixel_counts));
    pm->total_particles_generated = 0; // Zera contador total de partículas
}

// Introduz uma nova partícula na simulação
void ps_introduce_particle(ParticleManager* pm) {
    if (!pm) return;

    int slot_to_use = -1;
    // Procura por um slot inativo para a nova partícula
    for (int i = 0; i < MAX_SIM_PARTICLES; ++i) {
        int current_slot = (pm->next_particle_slot + i) % MAX_SIM_PARTICLES;
        if (!pm->particles[current_slot].is_in_play) {
            slot_to_use = current_slot;
            break;
        }
    }

    // Se todos os slots estiverem ocupados, sobrescreve o próximo slot da fila (mais antigo)
    if (slot_to_use == -1) {
        slot_to_use = pm->next_particle_slot;
    }

    Particle* p = &pm->particles[slot_to_use];
    p->x = PARTICLE_START_X_POS;        // Define posição X inicial
    p->y = PARTICLE_START_Y_POS;        // Define posição Y inicial
    p->current_interaction_level = 0; // Nível de interação inicial (primeira fileira de pinos)
    p->is_in_play = true;               // Marca partícula como ativa

    // Atualiza o próximo slot a ser usado, de forma circular
    pm->next_particle_slot = (slot_to_use + 1) % MAX_SIM_PARTICLES;
}

// Processa o movimento e interações de uma partícula
static void _process_particle_movement(Particle* p, ParticleManager* pm) {
    if (!p->is_in_play) return; // Ignora partículas inativas

    p->y += PARTICLE_FALL_VELOCITY; // Move a partícula para baixo

    // Verifica se a partícula ainda está na área dos pinos
    if (p->current_interaction_level < BOARD_PIN_ROWS) {
        // Coordenada Y da fileira de pinos com a qual a partícula pode interagir
        float interaction_row_y = (float)(BOARD_PIN_Y_ORIGIN +
                                         (p->current_interaction_level * BOARD_PIN_ROW_GAP));

        // Se a partícula alcançou ou ultrapassou a fileira de interação
        if (p->y >= interaction_row_y) {
            p->y = interaction_row_y; // Alinha a partícula com a fileira para o evento de deflexão

            // Deflexão baseada na probabilidade configurada
            int random_value = rand() % 100; // Gera um número aleatório entre 0 e 99
            int direction;

            if (random_value < PROBABILITY_RIGHT_DEFLECTION_PERCENT) {
                direction = 1;  // Desvia para a direita
            } else {
                direction = -1; // Desvia para a esquerda
            }
            p->x += direction * PARTICLE_X_DEFLECTION_AMOUNT;

            // Mantém a partícula dentro das bordas horizontais do display
            if (p->x < 0) p->x = 0;
            if (p->x >= OLED_WIDTH) p->x = OLED_WIDTH - 1.0f;

            p->current_interaction_level++; // Avança para a próxima fileira de pinos
        }
    } else {
        // A partícula passou por todas as fileiras de pinos e está caindo para a base
        if (p->y >= OLED_HEIGHT - 1.0f) {
            p->y = OLED_HEIGHT - 1.0f; // Para a partícula no fundo do display
            p->is_in_play = false;      // Marca a partícula como inativa
            pm->total_particles_generated++; // Incrementa contador total de partículas

            // Registra a partícula no histograma de pixels
            int final_x_column = (int)(p->x + 0.5f); // Arredonda para a coluna de pixel mais próxima
            if (final_x_column >= 0 && final_x_column < OLED_WIDTH) {
                // Incrementa a contagem na coluna, limitado pela altura do display
                if (pm->column_pixel_counts[final_x_column] < OLED_HEIGHT) {
                    pm->column_pixel_counts[final_x_column]++;
                }
            }
        }
    }
}

// Atualiza o estado de todas as partículas ativas
void ps_update_all_particles(ParticleManager* pm) { 
    if (!pm) return;
    for (int i = 0; i < MAX_SIM_PARTICLES; ++i) {
        _process_particle_movement(&pm->particles[i], pm); // Processa cada partícula
    }
}