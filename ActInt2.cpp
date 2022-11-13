#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <fstream>

using namespace std;

struct Nodo {
    string nombre;
    int numero;
    int x;
    int y;
    bool central;
    Nodo(string nombre, int numero, int x, int y, bool central){
        this-> nombre = nombre;
        this-> numero = numero;
        this-> x = x;
        this-> y = y;
        this-> central = central;
    }
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

void funcion1 (ofstream & checking2) {
    checking2 << "-------------------" << endl << "1 - Cableado óptimo de nueva conexión." << endl << endl;

    for(int i = 0; i < 3; i++){
        checking2 << "Colonia1 - Colonia2 10" << endl;
    }
    checking2 << endl << "Costo total: 50" << endl << endl << "-------------------" << endl;
}

void funcion2 () {
    // Formato de lo que debemos imprimir
}

void funcion3 () {
    // Formato de lo que debemos imprimir
}

// Complejidad: O(1)
// Función que regresa la distancia entre dos nodos
int distanceCalc(Nodo a, Nodo b){
    
    int distx = ((b.x - a.x) * (b.x - a.x));
    int disty = ((b.y - a.y) * (b.y - a.y));

    return sqrt(distx + disty);
}

// Complejidad: O(n)
// Donde n es la cantidad de colonias en el grafo
// Regresa el nombre del nodo más cercano al nodo que se va a agregar
string whereToConnect (Nodo nuevaCol, vector<Nodo> col) {
    
    int minDist = INT_MAX;
    Nodo closeNode = nuevaCol;
    int distancia;

    for (Nodo i : col) {
        distancia = distanceCalc(nuevaCol,i);
        if(distancia < minDist){
            minDist = distancia;
            closeNode = i;
        }
    }

    return closeNode.nombre;
}

void addConnection (string c1, string c2) {
    // Agregar la conexión con el nuevo cableado

}


int main () {

    // Variables de control para los ciclos
    int n, m, k, q;

    cin >> n >> m >> k >> q;
    
    // Vectores que almacenan los nodos y sus conexiones
    vector<Nodo> colonias;
    vector<Edge> conexiones;

    // Set para almacenar las colonias con un entero asignado para el uso en algoritmos
    unordered_map<string,int> ordenCol;

    // Archivo de salida
    ofstream checking2("checking2.txt");

    // Variables auxiliares para almacenar las colonias
    string nombre;
    int x, y;
    bool central;

    for (int i = 0; i < n; i++) {

        cin >> nombre >> x >> y >> central;

        ordenCol[nombre] = i+1;
        Nodo auxNodo(nombre, i+1, x, y, central);

        colonias.push_back(auxNodo);
    }

    // Variables auxiliares para los nombres de los nodos conectados y su costo
    string a, b;
    int c;

    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        Edge auxEdge(a, b, c);

        conexiones.push_back(auxEdge);
    }

    // Variables auxiliares para los nombres de las colonias con nuevas conexiones
    string c1, c2;

    for (int i = 0; i < k; i++) {
        cin >> c1 >> c2;

        // addConnection();
    }
    
    funcion1(checking2);
    funcion2();
    funcion3();
    
    // 4. Definir conexión de nuevas colonias
    checking2 << "4 - Conexión de nuevas colonias." << endl << endl;

    for (int i = 0; i < q; i++) {
        cin >> nombre >> x >> y;
        n++;
        Nodo auxNodo(nombre, n, x, y, 0);

        checking2 << auxNodo.nombre << " debe conectarse con " << whereToConnect(auxNodo,colonias) << endl;

        colonias.push_back(auxNodo);
    }

    checking2 << "-------------------" << endl;

    return 0;
}