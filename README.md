# Galton BitDogLab

Este projeto é uma simulação interativa da Tábua de Galton, projetada para no BitDogLab.

## O Que Faz?

Com um simples toque de botão, partículas virtuais (bolinhas) são liberadas no topo do display. Elas descem, encontrando uma série de pinos dispostos em formato piramidal. A cada encontro com um pino, a partícula toma uma decisão — ir para a esquerda ou para a direita — com uma probabilidade que pode ser configurada.

Ao atingirem a base, as partículas acumulam-se, visualmente demonstrando a formação de uma distribuição estatística, que tende a uma distribuição normal sob certas condições de probabilidade.

O projeto permite alternar entre diferentes visualizações:
1.  **Animação em Tempo Real:** Observe as partículas caindo e interagindo com os pinos.
2.  **Histograma de Distribuição:** Veja um gráfico de barras mostrando como as partículas se acumularam nas colunas da base.
3.  **Estatísticas:** Uma tela simples exibindo o número total de partículas que já foram simuladas.

## Como Funciona por Dentro:

* **Controles Intuitivos:**
    * O botão B é dedicado a liberar novas partículas na simulação.
    * O botão A botão permite ciclar entre os modos de visualização (Animação, Histograma, Estatísticas).
* **Configuração Flexível dos Pinos e Partículas:** Através do arquivo `galton_config.h`, você pode ajustar diversos parâmetros da simulação:
    * `BOARD_PIN_ROWS`: Número de fileiras de pinos.
    * `BOARD_PIN_Y_ORIGIN`: Posição Y inicial da primeira fileira de pinos.
    * `BOARD_PIN_ROW_GAP`: Espaçamento vertical entre as fileiras de pinos.
    * `BOARD_PIN_HORIZ_GAP`: Espaçamento horizontal entre pinos em uma mesma fileira.
    * `PARTICLE_START_X_POS`, `PARTICLE_START_Y_POS`: Posição inicial de onde as partículas são liberadas.
    * `PARTICLE_FALL_VELOCITY`: Velocidade com que as partículas caem.
    * `PARTICLE_X_DEFLECTION_AMOUNT`: O quanto uma partícula se desvia horizontalmente ao colidir com um pino.
    * **`PROBABILITY_RIGHT_DEFLECTION_PERCENT`**: A chance (em porcentagem, de 0 a 100) de uma partícula desviar para a **direita** ao encontrar um pino. Isso permite enviesar a simulação! Um valor de `50` resulta em chances iguais (50/50).

Este é um projeto divertido para explorar conceitos de probabilidade, estatística e visualização de dados de forma interativa com a BitDogLab
