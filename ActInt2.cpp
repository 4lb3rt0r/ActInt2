#include <iostream>
#include <vector>

using namespace std;

struct Nodo {
    string nombre;
    int x;
    int y;
    bool central;
};

struct Edge {
    string from, to;
    int cost;
    Edge(string a, string b, int c){
        from = a;
        to = b;
        cost = c;
	}
};

void funcion1 () {
    cout << "-------------------" << endl << "1 - Cableado óptimo de nueva conexión." << endl << endl;

    for(int i = 0; i < 3; i++){
        cout << "Colonia1 - Colonia2 10" << endl;
    }
    cout << endl << "Costo total: 50" << endl << endl << "-------------------" << endl;
}

void funcion2 () {
    // Formato de lo que debemos imprimir
}

void funcion3 () {
    // Formato de lo que debemos imprimir
}

void funcion4 () {
    // Formato de lo que debemos imprimir
}

void addConnection (string c1, string c2) {
    // Agregar la conexión con el nuevo cableado

}


int main () {

    int n, m, k, q, c;
    string nombre, a, b, c1, c2, name;
    Nodo auxNodo;
    Nodo colonia;
    vector<Nodo> colonias;
    vector<Nodo> nuevasColonias;
    vector<Edge> conexiones;

    cin >> n >> m >> k >> q;

    for (int i = 0; i < n; i++) {
        cin >> auxNodo.nombre >> auxNodo.x >> auxNodo.y >> auxNodo.central;

        colonias.push_back(auxNodo);
    }

    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        Edge auxEdge(a, b, c);

        conexiones.push_back(auxEdge);
    }

    for (int i = 0; i < k; i++) {
        cin >> c1 >> c2;

        // addConnection();
    }
    
    funcion1();
    funcion2();
    funcion3();
    
    for (int i = 0; i < q; i++) {
        cin >> colonia.nombre >> colonia.x >> colonia.y;

        colonia.central = 0;

        nuevasColonias.push_back(colonia);

        funcion4();
    }

    return 0;
}