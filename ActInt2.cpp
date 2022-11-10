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


int main () {

    int n, m, k, q, c;
    string nombre, a, b;
    Nodo auxNodo;
    vector<Nodo> colonias;
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

    return 0;
}