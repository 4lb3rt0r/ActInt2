#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <unordered_set>

using namespace std;

struct Graph {
    int V, E, costMSTPrim;

    Graph(int V, int E){
		this->V = V;
		this->E = E;
		adjList.resize(V);
		costMSTPrim = 0;
	}

	vector<vector<pair<int, int>>> adjList; 	// Se utilza en Prim
	vector<pair<pair<int,int>,int>> selectedEdgesP;		// Los arcos sel Prim;

    void addEdge(string u, string v, int w, unordered_map<string,int> ordenCol){
        adjList[ordenCol[u]].push_back({ordenCol[v],w});
        adjList[ordenCol[v]].push_back({ordenCol[u],w});

    }

	void primMST();
	void printEdgesP(vector<string> ordenColInv,ofstream & checking2);
    void print(vector<string> ordenColInv);
};

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

void funcion1 (ofstream & checking2, Graph & g, vector<string> & oci) {
    checking2 << "-------------------" << endl << "1 - Cableado óptimo de nueva conexión." << endl << endl;
    
    g.primMST();


	g.printEdgesP(oci, checking2);

    checking2 << "-------------------" << endl;
    
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

// Complejidad: O(V^2)
void Graph::primMST(){
	int selSource = 0;
	costMSTPrim = 0;
	unordered_set<int> selected;
	unordered_set<int> missing;
	selected.insert(0);
	for(int i = 1; i<V; i++){
		missing.insert(i);
	}
	int conected = V - 1, minCost, selVertex;
	while (conected) {
		minCost = INT_MAX;
		for (auto it:selected){
			for (auto it2:adjList[it]){
				if(missing.find(it2.first) != missing.end() && it2.second < minCost){
					minCost = it2.second;
					selVertex = it2.first;
					selSource = it; 
				}	
			}
		}
		costMSTPrim += minCost;
		selected.insert(selVertex);
		missing.erase(selVertex);
		selectedEdgesP.push_back({{selSource,selVertex},minCost});
		conected--;
	}
	
}

    
void Graph::printEdgesP(vector<string> ordenColInv, ofstream & checking2){

    checking2 << endl;

	for (auto it:selectedEdgesP){
        
        if(it.second != 0){
		    checking2 << ordenColInv[it.first.first] << " - " << ordenColInv[it.first.second] << " " << it.second << endl;
        }
	}
	checking2 << endl;
    checking2 << "Costo Total: " << costMSTPrim << endl;
}

void Graph::print(vector<string> ordenColInv){
	cout << "Adjacent List:" << endl;
	for (int i=0; i<V; i++){
		cout << " " << ordenColInv[i] << ": ";
		for (int j=0; j<adjList[i].size(); j++){
			cout << "(" << ordenColInv[adjList[i][j].first] << "," << adjList[i][j].second << ") ";
		}
		cout << endl;
	}
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
    vector<string> ordenColInv;

    Graph g(n,m);

    // Archivo de salida
    ofstream checking2("checking2.txt");

    // Variables auxiliares para almacenar las colonias
    string nombre;
    int x, y;
    bool central;

    for (int i = 0; i < n; i++) {

        cin >> nombre >> x >> y >> central;

        ordenCol[nombre] = i;
        ordenColInv.push_back(nombre);
        Nodo auxNodo(nombre, i, x, y, central);

        colonias.push_back(auxNodo);
    }

    // Variables auxiliares para los nombres de los nodos conectados y su costo
    string a, b;
    int c;

    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        Edge auxEdge(a, b, c);

        conexiones.push_back(auxEdge);
        g.addEdge(a,b,c,ordenCol);
    }

    // Variables auxiliares para los nombres de las colonias con nuevas conexiones
    string c1, c2;

    for (int i = 0; i < k; i++) {
        cin >> c1 >> c2;

        g.addEdge(c1,c2,0,ordenCol);
    }
    
    funcion1(checking2, g, ordenColInv);
    funcion2();
    funcion3();
    
    // 4. Definir conexión de nuevas colonias
    checking2 << "4 - Conexión de nuevas colonias." << endl << endl;

    for (int i = 0; i < q; i++) {
        cin >> nombre >> x >> y;
        n++;
        Nodo auxNodo(nombre, n, x, y, 0);

        checking2 << auxNodo.nombre << " debe conectarse con " << whereToConnect(auxNodo,colonias) << endl;

    }

    checking2 << "-------------------" << endl;

    return 0;
}