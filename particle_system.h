#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "galton_config.h" // Contém definições como MAX_SIM_PARTICLES, OLED_WIDTH
#include <stdint.h>       // Para tipos como uint8_t, uint32_t
#include <stdbool.h>      // Para o tipo bool

// Estrutura para representar uma única partícula
typedef struct {
    float x;                        // Posição X atual (float para movimento suave)
    float y;                        // Posição Y atual (float para movimento suave)
    int current_interaction_level;  // Fileira de pinos atual com a qual está interagindo
    bool is_in_play;                // Indica se a partícula está ativa na simulação
} Particle;

// Estrutura para gerenciar todas as partículas e dados da simulação
typedef struct {
    Particle particles[MAX_SIM_PARTICLES];       // Array de todas as partículas
    int next_particle_slot;                      // Índice para otimizar a busca por slots livres
    uint8_t column_pixel_counts[OLED_WIDTH];     // Histograma: contagem de partículas por coluna de pixel na base
    uint32_t total_particles_generated;          // Contagem total de partículas geradas desde o início
} ParticleManager;

// --- Protótipos das Funções Públicas do Sistema de Partículas ---

// Inicializa o gerenciador de partículas.
void ps_initialize(ParticleManager* pm);

// Introduz uma nova partícula na simulação.
void ps_introduce_particle(ParticleManager* pm);

// Atualiza o estado de todas as partículas na simulação.
void ps_update_all_particles(ParticleManager* pm);

#endif // PARTICLE_SYSTEM_H