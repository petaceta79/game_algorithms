#include <iostream>
#include <stack>
using namespace std;



class Torres {
private:
    stack<int> A;
    stack<int> B;
    stack<int> C;
    int n;
    
    int mov;
    
    void clear_stack(stack<int> &X) {
        int n = X.size();
        for (int i = 0; i < n; i++) {
            X.pop();
        }
    }
    
    void mover_X_to_Y(stack<int> &X, stack<int> &Y) {
        int i = X.top();
        X.pop();
    
        Y.push(i);
        
        mov++;
    }
    
    void resolver_(stack<int> &Ini, stack<int> &Aux, stack<int> &Fin, int num) {
        if (num == 1) {
            mover_X_to_Y(Ini, Fin);
            pintar();
            return;
        }
    
        resolver_(Ini, Fin, Aux, num-1);
    
        mover_X_to_Y(Ini, Fin);
        pintar();
    
        resolver_(Aux, Ini, Fin, num-1);
    }
    
    void reset() {
        clear_stack(A);
        clear_stack(B);
        clear_stack(C);
    
        for (int i = 0; i < n; i++) {
            A.push(n - i);
        }
        
        mov = 0;
    }
    
public:
    Torres(int num) {
        n = num;
    
        reset();
    }
    
    void pintar() {
    
        stack<int> A_ = A;
        stack<int> B_ = B;
        stack<int> C_ = C;
    
        for (int i = n; i > 0; i--) {
            cout << "|  ";
            if (A_.size() == i) {
                cout << A_.top();
                A_.pop();
            } else cout << " ";
            cout << "  |  ";
            if (B_.size() == i) {
                cout << B_.top();
                B_.pop();
            } else cout << " ";
            cout << "  |  ";
            if (C_.size() == i) {
                cout << C_.top();
                C_.pop();
            } else cout << " ";
            cout << "  |" << endl;
        }
    
        cout << "___________________" << endl;
    
        cout << "|  ";
        cout << "A";
        cout << "  |  ";
        cout << "B";
        cout << "  |  ";
        cout << "C";
        cout << "  |" << endl;
        
        cout << endl << endl; 
    }
    
    void resolver() {
        pintar();
        
        resolver_(A, B, C, n);
        
        cout << "Tardo " << mov << " movimientos" << endl;
        
        reset();
    }
    
};


int main()
{
    int n;
    cout << "Numero de discos: ";
    cin >> n;
    cout << endl;

    Torres t = Torres(n);

    t.resolver();
    
    
}