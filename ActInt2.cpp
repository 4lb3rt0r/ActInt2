#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <unordered_set>

using namespace std;

#define MAX 100

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
    int cost, fromN, toN;
    Edge(string a, string b, int c, int d, int e){
        from = a;
        to = b;
        cost = c;
        fromN = d;
        toN = e;
	}
};

struct Exclude {
    int a, b;
};

void funcion1 (ofstream & checking2, Graph & g, vector<string> & oci) {
    checking2 << "-------------------" << endl << "1 - Cableado óptimo de nueva conexión." << endl << endl;
    
    g.primMST();


	g.printEdgesP(oci, checking2);
    
}

void funcion2 (ofstream & checking2) {
    // Formato de lo que debemos imprimir
    checking2 << "-------------------" << endl << "2 - La ruta óptima." << endl << endl;
}

void leeArcos (int mat[MAX][MAX], int p[MAX][MAX], vector<Edge> &edges, int m) {
    int a, b, c; // de Nodo a <-> b con costo
    // Incialización de matriz
    for (int i = 0; i < MAX; i++) {
        mat[i][i] = 0;
        p[i][i] = -1; // -1 significa Conexión Directa
        for (int j = i + 1; j < MAX; j++){
            mat[i][j] = mat[j][i] = INT_MAX; // INT_MAX = infinito
            p[i][j] = p[j][i] = -1; // Significa que no existe un nodo intermedio
        }
    }
    for (int i = 0; i < m; i++) {
        //  Recordar que la base es 0 para los arreglos
        // base1 para la entrada del programa
        mat[edges[i].fromN][edges[i].toN] = mat[edges[i].toN][edges[i].fromN] = edges[i].cost;
    }
}

// Complejidad: O(n^3)
void floyd(int mat[MAX][MAX], int p[MAX][MAX], int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                /* Se comprueba que exista una conexión entre nodos corroborando que tanto
                 mat[i][k] como mat[k][j] no contengan un valor infinito. Además, si la suma de los
                 elementos contenidos en mat[i][k] y mat[k][j] es menor al valor contenido en mat[i][j],
                 actualizamos el valor de mat[i][j] al de esta sumatoria. */
                if (mat[i][k] != INT_MAX && mat[k][j] != INT_MAX && mat[i][k] + mat[k][j] < mat[i][j]) {
                    mat[i][j] = mat[i][k] + mat[k][j];
                    p[i][j] = k;
                }
            }
        }
    }
}

string getName(int index, vector<Nodo> &colonias) {
    for (int i = 0; i < colonias.size(); i++) {
        if (index == colonias[i].numero) {
            return colonias[i].nombre;
        }
    }
    return "";
}

int getIndex(string nombre, vector<Nodo> colonias) {
    for (int i = 0; i < colonias.size(); i++) {
        if (nombre == colonias[i].nombre) {
            return colonias[i].numero;
        }
    }
    return 0;
}

bool notRepeat(int a, int b, vector<Exclude> &excluidos) {
    if (excluidos.empty()) {
        return true;
    }
    else {
            for (int i = 0; i < excluidos.size(); i++) {
        if (a != excluidos[i].b && b != excluidos[i].a) {
            return true;
        }
    }
    }
    return false;
}

void checaTrayectoria (ofstream & checking2, int p[MAX][MAX], int x, int y, vector<int> &centrales, vector<Nodo> &colonias) {
    // Si la conexión no se trata de una conexión directa entre nodos (osea que sea una conexión de 2 nodos a -> b):
    if (p[x][y] != -1) {
        /* Desplegamos la trayectoria desde el inicia (a) hasta el número de 
        en medio (p[x][y]) y desde ese número de en medio hasta el final (b)
        */
        checaTrayectoria(checking2, p, x, p[x][y], centrales, colonias);
        checking2 << getName(p[x][y], colonias) << " - ";
        checaTrayectoria(checking2, p, p[x][y], y, centrales, colonias);
    }
}

void funcion3 (ofstream & checking2, int n, int m, vector<Edge> &edges, vector<int> &centrales, vector<Nodo> &colonias) {
    // Formato de lo que debemos imprimir
    checking2 << "-------------------" << endl << "3 - Caminos más cortos entre centrales." << endl << endl;

    Exclude excluido;
    vector<Exclude> excluidos;

    int mat[MAX][MAX], p[MAX][MAX];

	leeArcos(mat, p, edges, m);
	floyd(mat, p, n);

    for (int i = 0; i < centrales.size(); i++) {
        for (int j = 0; j < centrales.size(); j++) {
            if (mat[centrales[i]][centrales[j]] != INT_MAX && mat[centrales[i]][centrales[j]] != 0 && notRepeat(centrales[i], centrales[j], excluidos)) {
                    // Desplegamos el costo de la trayectoria y la misma trayectoria
                    checking2 << getName(centrales[i], colonias) << " - ";
                    checaTrayectoria(checking2, p, centrales[i], centrales[j], centrales, colonias);
                    checking2 << getName(centrales[j], colonias) <<" (" << mat[centrales[i]][centrales[j]] << ")" << endl;

                    excluido.a = centrales[i];
                    excluido.b = centrales[j];
                    excluidos.push_back(excluido);
            }
        }
    }
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

    vector<int> centrales;

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

        if (central) {
            centrales.push_back(i);
        }

        ordenCol[nombre] = i;
        ordenColInv.push_back(nombre);
        Nodo auxNodo(nombre, i, x, y, central);

        colonias.push_back(auxNodo);
    }

    // Variables auxiliares para los nombres de los nodos conectados, sus índices y su costo
    string a, b;
    int c, d, e;
    bool f;

    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;

        d = getIndex(a, colonias);
        e = getIndex(b, colonias);

        Edge auxEdge(a, b, c, d, e);

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
    funcion2(checking2);
    funcion3(checking2, n, m, conexiones, centrales, colonias);
    
    // 4. Definir conexión de nuevas colonias
    checking2 << "-------------------" << endl <<  "4 - Conexión de nuevas colonias." << endl << endl;

    for (int i = 0; i < q; i++) {
        cin >> nombre >> x >> y;
        n++;
        Nodo auxNodo(nombre, n, x, y, 0);

        checking2 << auxNodo.nombre << " debe conectarse con " << whereToConnect(auxNodo,colonias) << endl;

    }

    checking2 << "-------------------" << endl;

    return 0;
}