#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>   
#include <ctime>   
using namespace std;

const int INF = numeric_limits<int>::max();      // +∞
const int NEG_INF = numeric_limits<int>::min();  // -∞

class TresEnRaya {
 private:
    vector<vector<int>> tablero;
    int n;
    int n_ganar;
    int depth;
    bool start = false;
 
    void reset() {
        tablero = vector<vector<int>>(n, vector<int>(n, 0));
    }

    void move(pair<int, int> pos, int ficha) {
        tablero[pos.first][pos.second] = ficha;
    }

    void pintarTablero() {
        cout << "_____";
        for (int i = 0; i < (n-1); i++) cout << "____";
        cout << endl;

        for (int i = 0; i < n; i++) {
            cout << "| ";

            for (int e = 0; e < (n-1); e++) {

                switch (tablero[i][e])
                {
                case 0:
                    cout << ' ';
                    break;
                case 1:
                    cout << 'X';
                    break;
                case 2:
                    cout << 'O';
                    break;                
                default:
                    break;
                }

                cout << " | ";
            }

            switch (tablero[i][n-1])
            {
            case 0:
                cout << ' ';
                break;
            case 1:
                cout << 'X';
                break;
            case 2:
                cout << 'O';
                break;                
            default:
                break;
            }

            cout << " |" << endl;
        }
        
        cout << endl;
    }

    void moveJugador(int ficha) {
        pintarTablero();

        pair<int, int> pos;


        do {
            cout << "pos x (0-" << (n-1) << ") : " << endl;

            do {
                cin >> pos.second;
            } while (!(0 <= pos.second <= (n-1)));

            cout << "pos y (0-" << (n-1) << ") : " << endl;

            do {
                cin >> pos.first;
            } while (!(0 <= pos.first <= (n-1)));
        } while (tablero[pos.first][pos.second] != 0);


        move(pos, ficha);
    }


    static int hadWin_(vector<vector<int>> tab, int n, int n_ganar) {
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                if (tab[y][x] != 0) {

                    if ((n-y) >= n_ganar) {
                        // Comprobar vertical
                        bool win = true;
                        int i = 1;
                        while (i < n_ganar && win) {
                            if (tab[y][x] != tab[y+i][x]) win = false;
                            i++;
                        }

                        if (win) return tab[y][x];
                    }
                    
                    if ((n-x) >= n_ganar) {
                        // Comprobar horizontal
                        bool win = true;
                        int i = 1;
                        while (i < n_ganar && win) {
                            if (tab[y][x] != tab[y][x+i]) win = false;
                            i++;
                        }

                        if (win) return tab[y][x];
                    }

                    if ((n-x) >= n_ganar && (n-y) >= n_ganar) {
                        // Comprobar '\'
                        bool win = true;
                        int i = 1;
                        while (i < n_ganar && win) {
                            if (tab[y][x] != tab[y+i][x+i]) win = false;
                            i++;
                        }

                        if (win) return tab[y][x];
                    }

                    if ((x+1) >= n_ganar && (n-y) >= n_ganar) {
                        // Comprobar '/'
                        bool win = true;
                        int i = 1;
                        while (i < n_ganar && win) {
                            if (tab[y][x] != tab[y+i][x-i]) win = false;
                            i++;
                        }

                        if (win) return tab[y][x];
                    }

                }                
            }
        }

        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                if (tab[y][x] == 0) return 0;
            }
        }

        return -1;
    }

    int hadWin() {
        return hadWin_(tablero, n, n_ganar);
    }

    static int moveIA_(pair<int, int> pos, int depth, vector<vector<int>> tablero, vector<int> &fichas, bool turn, vector<int> &nes, int alfa, int beta) {
        if (turn) tablero[pos.first][pos.second] = fichas[0];
        else tablero[pos.first][pos.second] = fichas[1];
        

        if (depth <= 0) return 0; // funcion depende la partida
        int whoWin = hadWin_(tablero, nes[0], nes[1]);
        if (whoWin != 0) {
            if (whoWin == fichas[0]) return 10 * depth;
            else return -10 * depth;
        } 
        if (whoWin == -1) return -5;

        pair<int, int> betterPos = {-1, -1};
        int maxPoints = 0;

        for (int y = 0; y < nes[0]; y++) {
            for (int x = 0; x < nes[0]; x++) {
                if (tablero[y][x] == 0) {
                    int points = moveIA_({y, x}, depth-1, tablero, fichas, !turn, nes, alfa, beta);
                    if (betterPos.first == -1 ||(maxPoints <= points && !turn) || (maxPoints >= points && turn)) {
                        betterPos = {y, x};
                        maxPoints = points;
                    }

                    if (!turn) alfa = max(alfa, points);
                    else beta = min(beta, points);

                    if (beta <= alfa) break;
                }
            }
        }

        return maxPoints;
    }

    void moveIA(vector<int> fichas) {
        vector<pair<int, int>> bestOptions;
        int maxPoints = 0;

        vector<int> nes = {n, n_ganar};


        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                if (tablero[y][x] == 0) {
                    int points = moveIA_({y, x}, depth, tablero, fichas, true, nes, NEG_INF, INF);
                    if (bestOptions.empty() || maxPoints < points) {
                        bestOptions.clear();
                        bestOptions.push_back({y, x});
                        maxPoints = points;
                    }
                    else if (maxPoints == points) bestOptions.push_back({y, x});
                }
            }
        }

        std::srand(std::time(nullptr)); 
        move(bestOptions[std::rand() % bestOptions.size()], fichas[0]);
    }

 public:
    TresEnRaya(int num, int num_para_ganar, int depth_, bool start_) 
    : n(num), n_ganar(num_para_ganar), depth(depth_), start(start_)
    {
        reset();
    }

    void jugar() {

        bool turn = start;
        while (hadWin() == 0) {
            if (turn) moveJugador(1);
            else {
                cout << "Ia:" << endl;
                moveIA({2,1});
            }

            turn = !turn;
        }

        pintarTablero();

        cout << hadWin() << endl;
    }
    
};


// Lógica principal de la partida
int main() {
    int tam;
    int xray;
    int depth;
    int start;

    cout << "Tres en raya, X->player, O->IA" << endl;
    cout << endl;

    cout << "Anchura del recuadro: ";
    cin >> tam;
    cout << "Longitud necesaria para ganar: ";
    cin >> xray;
    cout << "Profundidad maxima del arbol: ";
    cin >> depth;
    cout << "Quien Comienza (0->IA  1->Player): ";
    cin >> start;

    cout << endl;

    TresEnRaya t = TresEnRaya(tam, xray, depth, start);

    t.jugar();    
}