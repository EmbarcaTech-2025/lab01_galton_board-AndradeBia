#include "pico/stdlib.h"    // Funções padrão do Pico SDK
#include "pico/util/datetime.h" // Para geração de semente aleatória baseada no tempo
#include "hardware/gpio.h"  // Para controle dos GPIOs

#include "inc/oled_driver.h"  // Driver do display OLED
#include "galton_config.h"    // Configurações da simulação
#include "particle_system.h"// Lógica do sistema de partículas
#include "board_renderer.h"   // Lógica de renderização

// Enumeração para os modos de visualização
typedef enum {
    VIEW_MODE_ANIMATION,    // Modo de visualização da animação
    VIEW_MODE_DISTRIBUTION, // Modo de visualização do histograma
    VIEW_MODE_STATS,        // Modo de visualização das estatísticas
    VIEW_MODE_CYCLE_COUNT   // Usado para ciclar entre os modos
} ViewMode;

// --- Variáveis Globais ---
static ParticleManager g_particle_manager;          // Gerenciador de partículas global
static ViewMode g_current_view = VIEW_MODE_ANIMATION; // Modo de visualização atual
static uint64_t g_time_last_particle_add_btn = 0;   // Timestamp do último pressionamento do botão de adicionar partícula
static uint64_t g_time_last_cycle_view_btn = 0;   // Timestamp do último pressionamento do botão de ciclo de visualização

// --- Protótipos de Funções Estáticas ---
static void _initialize_hardware_interfaces(void);
static bool _query_button_event(uint pin, uint64_t *last_event_time_us);
static void _application_loop_tick(void);

// Função principal
int main(void) {
    stdio_init_all(); // Inicializa STDIO (para debug via USB, se habilitado)
    _initialize_hardware_interfaces(); // Configura GPIOs e I2C
    srand(time_us_32()); // Semente para o gerador de números aleatórios

    // Inicializa o display OLED
    if (!oled_init()) {
        // Tratamento de erro crítico se o OLED não inicializar (loop infinito)
        while (1) { tight_loop_contents(); }
    }

    ps_initialize(&g_particle_manager); // Inicializa o sistema de partículas

    // Loop principal da aplicação
    while (true) {
        _application_loop_tick();        // Executa a lógica do quadro atual
        sleep_ms(SIM_FRAME_INTERVAL_MS); // Aguarda o intervalo definido para o próximo quadro
    }
    return 0;
}

// Inicializa interfaces de hardware (GPIOs para botões)
static void _initialize_hardware_interfaces(void) {
    // Configura pino para adicionar partícula
    gpio_init(PIN_ADD_PARTICLE);
    gpio_set_dir(PIN_ADD_PARTICLE, GPIO_IN);
    gpio_pull_up(PIN_ADD_PARTICLE); // Habilita pull-up interno

    // Configura pino para ciclar visualização
    gpio_init(PIN_CYCLE_VIEW);
    gpio_set_dir(PIN_CYCLE_VIEW, GPIO_IN);
    gpio_pull_up(PIN_CYCLE_VIEW);   // Habilita pull-up interno
}

// Verifica se um botão foi pressionado, com debounce
static bool _query_button_event(uint pin, uint64_t *last_event_time_us) {
    uint64_t current_time_us = time_us_64(); // Tempo atual em microssegundos
    // Verifica se o intervalo de debounce passou
    if (current_time_us - *last_event_time_us > BUTTON_DEBOUNCE_INTERVAL_US) {
        if (!gpio_get(pin)) { // Botão pressionado (nível baixo devido ao pull-up)
            *last_event_time_us = current_time_us; // Atualiza o timestamp do último evento
            return true; // Evento de botão detectado
        }
    }
    return false; // Nenhum evento de botão
}

// Lógica principal executada a cada quadro da simulação
static void _application_loop_tick(void) {
    // Verifica o botão de ciclo de visualização
    if (_query_button_event(PIN_CYCLE_VIEW, &g_time_last_cycle_view_btn)) {
        g_current_view = (g_current_view + 1) % VIEW_MODE_CYCLE_COUNT; // Alterna para o próximo modo
    }

    // Executa ações baseadas no modo de visualização atual
    switch (g_current_view) {
        case VIEW_MODE_ANIMATION:
            // Verifica o botão de adicionar partícula
            if (_query_button_event(PIN_ADD_PARTICLE, &g_time_last_particle_add_btn)) {
                ps_introduce_particle(&g_particle_manager); // Adiciona nova partícula
            }
            ps_update_all_particles(&g_particle_manager); // Atualiza estado de todas as partículas
            renderer_draw_simulation_frame(&g_particle_manager); // Desenha o quadro da simulação
            break;
        case VIEW_MODE_DISTRIBUTION:
            // Desenha o histograma da distribuição de partículas
            // Os dados são atualizados no ParticleManager durante o VIEW_MODE_ANIMATION
            renderer_draw_pixel_distribution(g_particle_manager.column_pixel_counts);
            break;
        case VIEW_MODE_STATS:
            // Desenha a tela de estatísticas
            renderer_draw_statistics(g_particle_manager.total_particles_generated);
            break;
        default:
            // Estado de fallback, caso algo inesperado ocorra
            g_current_view = VIEW_MODE_ANIMATION;
            break;
    }
}