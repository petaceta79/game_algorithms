#include <iostream>
#include <vector>
#include <climits>
using namespace std;

typedef vector<int> manos;

// Devuelve quien gana si 1 o 2
int win(manos player1, manos player2) {
    if (!player1[0] && !player1[1]) return 2;
    else if (!player2[0] && !player2[1]) return 1;
    
    return 0;
}

// Suma la mano 2 a la 1
bool sumar_manos(int &player1_hand, int player2_hand) {
    if (!player1_hand || !player2_hand) return false;
    player1_hand += player2_hand;
    player1_hand = player1_hand % 5;
    return true;
}

// Asigna el valor x a las dos manos del player
void dividir_x(manos &player, int x) {
    player[0] = x;
    player[1] = x;
}

// Divide si se puede las manos del player
bool dividir_manos(manos &player) {
    if (!player[0]) {
        if (player[1] == 2) {
            dividir_x(player, 1);
            return true;
        }
        else if (player[1] == 4) {
            dividir_x(player, 2);
            return true;
        }
    }
    else if (!player[1]) {
        if (player[0] == 2) {
            dividir_x(player, 1);
            return true;
        }
        else if (player[0] == 4) {
            dividir_x(player, 2);
            return true;
        }
    }
    
    return false;
}

// Dibuja la mano del player, mas la flecha de si le toca
void print_hand(manos player, bool turn) {
    cout << "|";
    for (int i = 0; i < player[0]; i++) {
        cout << "I";
    }
    for (int i = 0; i < 5 - player[0]; i++) {
        cout << " ";
    }
    cout << "|";
    for (int i = 0; i < player[1]; i++) {
        cout << "I";
    }
    for (int i = 0; i < 5 - player[1]; i++) {
        cout << " ";
    }
    cout << "|";
    
    if (turn) cout << "<--";
    
    cout << endl;
}

// Hace un moviminto el player
void player_move(manos &player1, manos &player2) {
    bool finish = false;
    
    cout << "Select move:" << endl;
    cout << "1: sumar R -> R" << endl;
    cout << "2: sumar R -> L" << endl;
    cout << "3: sumar L -> R" << endl;
    cout << "4: sumar L -> L" << endl;
    cout << "5: dividir" << endl;
    
    while (!finish) {
        int sel;
        cin >> sel;
        
        if (1 <= sel && sel <= 5) {
            if (sel == 1) {
                finish = sumar_manos(player2[1], player1[1]);
            }
            if (sel == 2) {
                finish = sumar_manos(player2[0], player1[1]);
            }
            if (sel == 3) {
                finish = sumar_manos(player2[1], player1[0]);
            }
            if (sel == 4) {
                finish = sumar_manos(player2[0], player1[0]);
            }
            if (sel == 5) {
                finish = dividir_manos(player1);
            }
        }
    }
}

// Devuelve si dividir es possible 
bool dividir_posible(manos &player) {
    if (!player[0]) {
        if (player[1] == 2 || player[1] == 4) {
            return true;
        }
    }
    else if (!player[1]) {
        if (player[0] == 2 || player[0] == 4) {
            return true;
        }
    }
    
    return false;
}

// Devuelve las posibles jugadas
/*
0 -> (R > R)
1 -> (R > L)
2 -> (L > R)
3 -> (L > L)
4 -> (divide)
*/
vector<bool> options(manos &player1, manos &player2) {
    vector<bool> aux(5, false);
    
    aux[0] = (player1[1] != 0 && player2[1] != 0);
    aux[1] = (player1[1] != 0 && player2[0] != 0);
    aux[2] = (player1[0] != 0 && player2[1] != 0);
    aux[3] = (player1[0] != 0 && player2[0] != 0);
    aux[4] = dividir_posible(player1);
    
    return aux;
}

// Devuelve una puntuación dado la situación
int evaluar_estado(manos &jugador, manos &oponente) {
    int score = 0;

    // --- 1. Conteo de manos vacías ---
    int manos_propias_vacias = (jugador[0] == 0) + (jugador[1] == 0);
    int manos_oponente_vacias = (oponente[0] == 0) + (oponente[1] == 0);

    // --- 2. Prioridad: ganar o evitar perder ---
    if (manos_oponente_vacias == 2) return 1000;  // victoria segura
    if (manos_propias_vacias == 2) return -1000;  // derrota segura

    // --- 3. Manos activas (más importante que suma total) ---
    int manos_activas_jugador = 2 - manos_propias_vacias;
    int manos_activas_oponente = 2 - manos_oponente_vacias;
    score += (manos_activas_jugador - manos_activas_oponente) * 15;

    // --- 4. Suma total de dedos ---
    int suma_jugador = jugador[0] + jugador[1];
    int suma_oponente = oponente[0] + oponente[1];
    score += (suma_jugador - suma_oponente) * 2;

    // --- 5. Bonus por posiciones difíciles de atacar o dividir ---
    if ((jugador[0] == 2 && jugador[1] == 2) || (jugador[0] == 1 && jugador[1] == 4) || (jugador[0] == 4 && jugador[1] == 1)) {
        score += 4;
    }

    // --- 6. Penaliza si el oponente está en posiciones estratégicas ---
    if ((oponente[0] == 2 && oponente[1] == 2) || (oponente[0] == 1 && oponente[1] == 4) || (oponente[0] == 4 && oponente[1] == 1)) {
        score -= 4;
    }

    // --- 7. Penaliza tener una sola mano (expuesto) ---
    if (manos_propias_vacias == 1) score -= 3;

    // --- 8. Penaliza fuertemente tener 1 sola mano contra 2 del oponente ---
    if (manos_propias_vacias == 1 && manos_oponente_vacias == 0) score -= 6;

    // --- 9. Bonus por poder dividir en próximo turno (flexibilidad) ---
    if (dividir_posible(jugador)) score += 3;

    // --- 10. Penaliza si el oponente puede dividir ---
    if (dividir_posible(oponente)) score -= 3;

    return score;
}

// Immersion de la función move_ia
int move_ia_(manos player1, manos player2, bool turn, int movement, int deep) {
    // Aplicar el movimiento actual
    switch (movement) {
        case 0:
            sumar_manos(player1[1], player2[1]);
            break;
        case 1:
            sumar_manos(player1[0], player2[1]);
            break;
        case 2:
            sumar_manos(player1[1], player2[0]);
            break;
        case 3:
            sumar_manos(player1[0], player2[0]);
            break;
        case 4:
            dividir_manos(player2);
            break;    
        default:
            break;
    }
    
    

    // Verificar condición de victoria
    int result = win(player1, player2);
    
    if (result == 1 && !turn) {
        return 10000 * deep;  // el jugador actual gana
    } 
    if (result == 1 && turn) {
        return -10000 * deep;  // el jugador actual pierde
    }
    
    if (deep <= 0) {
        return evaluar_estado(player1, player2);
    }

    vector<bool> opciones = options(player1, player2);
    

    // Inicializar con el peor caso posible
    int bestValue = turn ? INT_MAX : INT_MIN;

    for (int i = 0; i < 5; i++) {
        if (opciones[i]) {
            int currentValue = move_ia_(player2, player1, !turn, i, deep - 1);
            
            if (!turn) {
                // Turno de la IA (maximizar)
                if (currentValue > bestValue) {
                    bestValue = currentValue;
                }
            } else {
                // Turno del oponente (minimizar)
                if (currentValue < bestValue) {
                    bestValue = currentValue;
                }
            }
        }
    }

    return bestValue;
}

// Devuelve el movimiento que la IA predice que es mejor 
//condición: Deep % 2 == 1
void move_ia(manos &player1, manos &player2, int deep) {
     vector<bool> opciones = options(player1, player2);
    
    int max = INT_MIN;  // Inicializar con el peor caso para la IA
    int max_mov = -1;
    
    for (int i = 0; i < 5; i++) {
        if (opciones[i]) {
        
            int currentValue = move_ia_(player2, player1, true, i, deep);
            
            
            if (currentValue > max || max_mov == -1) {
                max = currentValue;
                max_mov = i;
            }
        }
    }
    
    // Aplicar el mejor movimiento encontrado
    switch (max_mov) {
        case 0:
            sumar_manos(player2[1], player1[1]);
            break;
        case 1:
            sumar_manos(player2[0], player1[1]);
            break;
        case 2:
            sumar_manos(player2[1], player1[0]);
            break;
        case 3:
            sumar_manos(player2[0], player1[0]);
            break;
        case 4:
            dividir_manos(player1);
            break;    
        default:
            break;
    }
}

// Lógica principal de la partida
int main() {
    manos player1(2, 1);
    manos player2(2, 1);
    
    
    cout << "Inicio" << endl;
    
    bool turn = true;
    int winner = 0;
    while (winner == 0) {
         print_hand(player1, turn);
         print_hand(player2, !turn);
         cout << endl;
         
         if (turn) {
             player_move(player1, player2);
         } else {
             move_ia(player2, player1, 7);
             cout << "IA moved" << endl;
         }
         
         cout << endl;
         
         turn = !turn; 
         winner = win(player1, player2);
    } 
    
    print_hand(player1, turn);
    print_hand(player2, !turn);
    cout << endl;
    
    if (winner == 1) winner = 2;
    else winner = 1;
    if (turn) cout << "winner: player2" << endl;
    else cout << "winner: player1" << endl;
    cout << "fin" << endl;
}