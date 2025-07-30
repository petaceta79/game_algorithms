# game_algorithms
Algoritmos para resolver juegos simples o antiguos, como el Tic Tac Toe, etc.

# Índice
- [game_algorithms](#game_algorithms)
- [chopsticks](#chopsticks)
- [tic_tac_toe_bot_algorithm](#tic_tac_toe_bot_algorithm)
- [torres_de_hanoi_bot_algorithm](#torres_de_hanoi_bot_algorithm)

# chopsticks
Este algoritmo implementa Minimax de forma recursiva, evaluando un árbol de decisiones cuyo tamaño varía según las jugadas disponibles en cada turno.

Dado que Chopsticks es un juego relativamente simple, el bot alcanza un nivel de juego casi perfecto.

Sin embargo, como el juego puede entrar en bucles (repetición de estados), el árbol podría volverse infinito. Para evitarlo, se impone una profundidad máxima al árbol. Cuando se alcanza ese límite, se asigna un valor al estado final mediante una función heurística que evalúa la posición del jugador y del oponente.

# tic_tac_toe_bot_algorithm
Implementa Minimax con poda alfa-beta para el juego Tres en Raya generalizado (NxN con M fichas para ganar).

Evalúa recursivamente movimientos posibles, asignando puntuaciones según victoria/derrota/empate y la profundidad restante.

Limita la exploración con una profundidad máxima configurable, optimizando el rendimiento.

Selecciona aleatoriamente entre las mejores jugadas si hay empate en puntuación.

Permite ajustar tamaño del tablero, fichas para ganar y quién inicia (jugador humano o IA).

# torres_de_hanoi_bot_algorithm
Una classe que dibuja la resolucion de las torres de hanoi para n discos y devuelve su número de movimientos.
