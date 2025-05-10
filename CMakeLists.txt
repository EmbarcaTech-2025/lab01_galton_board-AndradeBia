# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Define o nome do projeto como 'Galton'
project(Galton C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable.
# O nome do executável será 'Galton'
add_executable(Galton
        main.c              # Arquivo principal da aplicação
        particle_system.c   # Sistema de partículas
        board_renderer.c    # Renderizador do tabuleiro/telas
        src/oled_driver.c   # Driver OLED existente na pasta src
        inc/ssd1306_i2c.c   # Funções base I2C do OLED existentes na pasta inc
        )

pico_set_program_name(Galton "Galton") # Define o nome do programa para os binários
pico_set_program_version(Galton "0.1")       # Define a versão do programa

# Modifique as linhas abaixo para habilitar/desabilitar saída via UART/USB
pico_enable_stdio_uart(Galton 0)
pico_enable_stdio_usb(Galton 1)

# Adiciona a biblioteca padrão do Pico SDK ao build
target_link_libraries(Galton
        pico_stdlib)

# Adiciona os diretórios de include padrão ao build
target_include_directories(Galton PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}       # Para galton_config.h, particle_system.h, board_renderer.h na raiz
  ${CMAKE_CURRENT_LIST_DIR}/inc  # Garante que a pasta inc seja incluída (para headers do OLED)
  ${CMAKE_CURRENT_LIST_DIR}/src  # Garante que a pasta src seja incluída (se houver headers lá, como oled_driver.h)
)

# Adiciona quaisquer bibliotecas solicitadas pelo usuário
target_link_libraries(Galton
        hardware_i2c
        )

pico_add_extra_outputs(Galton)
