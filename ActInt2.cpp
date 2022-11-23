#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <queue>

using namespace std;

#define MAX 100

// Structs que ayudarán al funcionamiento de los algoritmos

// Grafo utilizado en prim
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

// Nodo modificaado para tsp

struct NodoTSP {
    int costoP = 0;
    int nivel;
    int costoA;
    int verAnt;
    int verAct;
    bool visitados[MAX] = {false};
    vector<int> ruta;
    bool operator<(const NodoTSP &otro) const{
        return costoP > otro.costoP;
    }
};

// Nodo
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

// Funciones para la creación del archivo

// Funciones auxiliares 1

// Complejidad: O(V^2)
// Donde V son los nodos en el grafo
// Busca el Minimun spanning tree y almacena los resultados
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

// Complejidad: O(n)
// Donde n son los ejes seleccionados para el camino más corto
void Graph::printEdgesP(vector<string> ordenColInv, ofstream & checking2) {

	for (auto it:selectedEdgesP){
        
        if(it.second != 0){
		    checking2 << ordenColInv[it.first.first] << " - " << ordenColInv[it.first.second] << " " << it.second << endl;
        }
	}
	checking2 << endl;
    checking2 << "Costo Total: " << costMSTPrim << endl;
}

// Complejidad: O(V^2)
// Donde V es la cantidad de nodos del grafo
// Calcula donde se deben poner nuevas conexiones para que todos los nodos estén conectados a una 
void cableadoNuevo(ofstream & checking2, Graph & g, vector<string> & oci) {
    checking2 << "-------------------" << endl << "1 - Cableado óptimo de nueva conexión." << endl << endl;

    g.primMST();
	g.printEdgesP(oci, checking2);
    
}

// Funciones auxiliares 2

// Complejidad: O(1)
// Crea una sola matriz con los valores de las conexiones
void iniciaMat(int mat[MAX][MAX], int m, vector<Edge> conexiones, unordered_map<string,int> ordenCol){

    for(int i = 0; i < MAX; i++){
        mat[i][i] = 0;
        for (int j = i+1; j < MAX; j++){
            mat[i][j] = mat[j][i] = INT_MAX;   
        }
    }

    for (int i = 0; i < m; i++)
    {
        int a = ordenCol[conexiones[i].from];
        int b = ordenCol[conexiones[i].to];
        int c = conexiones[i].cost;
        
        mat[a][b] = mat[b][a] = c;
    }
    
}

// Complejidad: Peor caso: O(n^2)/Mejor caso: O(n)
// Donde n es la cantidad de nodos activos en el grafo
void calculoCostoP(NodoTSP &nodoAct, int mat[MAX][MAX], int n, int primero){
    nodoAct.costoP = nodoAct.costoA;
    int costoObtenido;

    for (int i = 0; i < n; i++) {
        // No he visitado al nodo i o es el ultimo que visité
        costoObtenido = INT_MAX;
        if(!nodoAct.visitados[i] || i == nodoAct.verAct){
            
            // Si no lo he visitado... 
            if(!nodoAct.visitados[i]){
                // El costo obtenido es el menor de los arcos con sus nodos adyacentes
                for (int j = 0; j < n; j++) {
                   if (i != j && (!nodoAct.visitados[j] || j == primero)) {
                        //cout << "Origen: " << i << " Destino: " << j << endl;
                        costoObtenido = min(costoObtenido,mat[i][j]);
                        //cout << "Costo Obtenido: " << costoObtenido << endl;
                    }
                }
            }
            else{ // Si es el último que visité
                // El costo obtenido es el menor de los arcos no visitados
                for (int j = 0; j < n; j++) {
                    if(!nodoAct.visitados[j]) {
                        costoObtenido = min(costoObtenido,mat[i][j]);
                    }
                }
            }
            // Si no sigue el camino regresa infinito
            if (costoObtenido == INT_MAX) {

                nodoAct.costoP = INT_MAX;
            }
            // Si sigue, suma el costo posible
            else {
                nodoAct.costoP += costoObtenido;
            }
        }
        
    }
    
}

// Complejidad: O(n)
// Donde n es la cantidad de nodos no centrales
// Compara el arreglo de visitados con el de nocentrales para ver si ya se visitaron todos los nodos del último
bool noCentralesVisitados(bool visitados[MAX], vector<int> nocentrales){

    for (int i : nocentrales){
        if (!visitados[i])
        {
            return false;
        }
        
    }
    return true;
}

void printRuta(vector<int> & rutaOpt, vector<string> & ordenColInv){

    for (int i = 0; i < rutaOpt.size()-1; i++)
    {
        cout << ordenColInv[rutaOpt[i]] << " - ";
    }
    cout << ordenColInv[rutaOpt[rutaOpt.size()-1]] << endl;

}

void printNodo(NodoTSP & nodo, vector<string> ordenColInv){
    cout << "Nivel: " << nodo.nivel << endl;
    cout << "Costo Act: " << nodo.costoA << endl;
    printRuta(nodo.ruta, ordenColInv);
    cout << "Costo Posible: " << nodo.costoP << endl << endl;
}

// Complejidad: (2^n)
// Donde n es la cantidad de nodos en el grafo
pair<vector<int>,int> tsp(int mat[MAX][MAX], int n, ofstream & checking2, vector<int> nocentrales, vector<string> & ordenColInv, int primero){

    // Creamos el nodo raíz
    int costoOpt = INT_MAX;
    vector<int> rutaOpt;
    NodoTSP raiz;
    raiz.nivel = 0;
    raiz.costoA = 0;
    raiz.verAct = primero;
    raiz.verAnt = 0;
    raiz.visitados[primero] = true;
    raiz.ruta.push_back(primero);
    calculoCostoP(raiz, mat, n, primero);

    priority_queue<NodoTSP> pq; 

    pq.push(raiz);

    // Nodos auxiliares
    NodoTSP actual, nuevo;
    int costoR;

    while (!pq.empty()){
        // Sacar de pq
        actual = pq.top();
        pq.pop();
        // CostoPos < CostoOpt
        if (actual.costoP < costoOpt) {
            // Si es menor, Generar todos los hijos de este nodo
            for (int i = 0; i < n; i++) {
                if (!actual.visitados[i] && mat[i][actual.verAct] != INT_MAX){
                    // Por cada hijo generar nuevo nodo y actualizar datos
                    nuevo.nivel = actual.nivel+1;
                    nuevo.costoA = actual.costoA + mat[actual.verAct][i];
                    nuevo.verAct = i;
                    nuevo.verAnt = actual.verAct;
                    for (int j = 0; j < n; j++) {
                        nuevo.visitados[j] = actual.visitados[j];
                    }
                    nuevo.visitados[i] = true;
                    nuevo.ruta = actual.ruta;
                    nuevo.ruta.push_back(i);
                    calculoCostoP(nuevo, mat, n, primero);

                    //printNodo(nuevo, ordenColInv);

                    // Cuando el nivel sea n-1 calcular costo real
                    if (noCentralesVisitados(nuevo.visitados, nocentrales)){
                        // El costo real solo se calcula si existe el camino entre el nodo actual y el que falta y el que falta y el primero
                        if (mat[i][primero] != INT_MAX && nuevo.verAnt != primero)
                        {
                            costoR = nuevo.costoA + mat[i][primero];
                            // costoReal mejor que costoOptimo, actualizar
                            if (costoR < costoOpt) {
                                costoOpt = costoR;
                                rutaOpt = nuevo.ruta;
                            }
                        }
                    }
                    // Si el nivel < n-2 checar si costo posible es mejor e introducirlo a pq
                    if (nuevo.nivel < n-1 && nuevo.costoP < costoOpt) {
                        pq.push(nuevo);
                    }   
                }
            }
        }
       
    }

    rutaOpt.push_back(primero);

    return {rutaOpt,costoOpt};
}

// Complejidad: O(m2^n)
// Donde m son las colonias no centrales y n el total de colonias
// Busca el ciclo hamiltoniano más pequeño que pasa por todas las colonias no centrales
void cicloNoCentral (ofstream & checking2, int n, int m, vector<Edge> conexiones, unordered_map<string, int> ordenCol, vector<int> nocentrales, vector<string> & ordenColInv) {
    // Formato de lo que debemos imprimir
    checking2 << "-------------------" << endl << "2 - La ruta óptima." << endl << endl;

    if(!nocentrales.empty()){

        vector<pair<vector<int>, int>> resultados;
        
        int mat[MAX][MAX];

        iniciaMat(mat,m, conexiones, ordenCol);

        
        for(int i : nocentrales){
            resultados.push_back(tsp(mat, n, checking2,nocentrales, ordenColInv,i));
        }

        vector<int> rutaOpt;
        int costoOpt = INT_MAX;

        for (int i = 0; i < resultados.size(); i++)
        {
            if (costoOpt > resultados[i].second)
            {
                rutaOpt = resultados[i].first;
                costoOpt = resultados[i].second;
            }
        }
        
        for (int j = 0; j < rutaOpt.size()-1; j++)
        {
            checking2 << ordenColInv[rutaOpt[j]] << " - ";
        }
        checking2 << ordenColInv[rutaOpt[rutaOpt.size()-1]] << endl << endl;

        checking2 << "La Ruta Óptima tiene un costo total de: " <<  costoOpt << endl << endl;

    }
}

// Funciones auxiliares 3

// Complejidad: O(1)
// Crea una matriz con los caminos se un nodo a otro
void leeArcos (int mat[MAX][MAX], int p[MAX][MAX], vector<Edge> &edges, int m, unordered_map<string, int> & ordenCol) {
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
        mat[ordenCol[edges[i].from]][ordenCol[edges[i].to]] = mat[ordenCol[edges[i].to]][ordenCol[edges[i].from]] = edges[i].cost;
    }
}

// Complejidad: O(n^3)
// Donde n es la cantidad de nodos en el grafo
// Guarda en las matrices el camino más corto de cada nodo a otro
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

// Complejidad: O(n)
// Donde n es la cantidad de nodos totales en el camino de un nodo a otro
// Imprime los nodos que hay en el camino de uno a otro
void checaTrayectoria (ofstream & checking2, int p[MAX][MAX], int x, int y, vector<int> &centrales, vector<Nodo> &colonias, vector<string> & ordenColInv) {
    // Si la conexión no se trata de una conexión directa entre nodos (osea que sea una conexión de 2 nodos a -> b):
    if (p[x][y] != -1) {
        /* Desplegamos la trayectoria desde el inicia (a) hasta el número de 
        en medio (p[x][y]) y desde ese número de en medio hasta el final (b)
        */
        checaTrayectoria(checking2, p, x, p[x][y], centrales, colonias, ordenColInv);
        checking2 << ordenColInv[p[x][y]] << " - ";
        checaTrayectoria(checking2, p, p[x][y], y, centrales, colonias, ordenColInv);
    }
}

// Complejidad: O(n^3)
// Donde n es la cantidad de nodos del grafo
// Imprime en el archiivo la ruta de todas las colonias centrales entre sí
void rutaOptima(ofstream & checking2, int n, int m, vector<Edge> &edges, vector<int> &centrales, vector<Nodo> &colonias, vector<string> & ordenColInv, unordered_map<string, int> & ordenCol) {
    // Formato de lo que debemos imprimir
    checking2 << "-------------------" << endl << "3 - Caminos más cortos entre centrales." << endl << endl;

    int mat[MAX][MAX], p[MAX][MAX];

	leeArcos(mat, p, edges, m, ordenCol);
	floyd(mat, p, n);

    for (int i = 0; i < centrales.size(); i++) {
        for (int j = i+1; j < centrales.size(); j++) {
            if (mat[centrales[i]][centrales[j]] != INT_MAX) {
                    // Desplegamos el costo de la trayectoria y la misma trayectoria
                    checking2 << ordenColInv[centrales[i]] << " - ";
                    checaTrayectoria(checking2, p, centrales[i], centrales[j], centrales, colonias, ordenColInv);
                    checking2 << ordenColInv[centrales[j]] <<" (" << mat[centrales[i]][centrales[j]] << ")" << endl;
            }
        }
    }
}

// Funciones auxiliares 4

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


int main () {

    // Variables de control para los ciclos
    int n, m, k, q;
    cin >> n >> m >> k >> q;
    
    // Vectores que almacenan los nodos y sus conexiones
    vector<Nodo> colonias;
    vector<Edge> conexiones;

    vector<int> centrales;
    vector<int> nocentrales;

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
        else{
            nocentrales.push_back(i);
        }

        ordenCol[nombre] = i;
        ordenColInv.push_back(nombre);
        Nodo auxNodo(nombre, i, x, y, central);

        colonias.push_back(auxNodo);
    }

    // Variables auxiliares para los nombres de los nodos conectados, sus índices y su costo
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
    
    // Función 1. Cableado nuevo
    cableadoNuevo(checking2, g, ordenColInv);

    // Función 2. Ruta no central
    cicloNoCentral(checking2,n,m,conexiones,ordenCol, nocentrales, ordenColInv);

    // Función 3. Ruta central
    rutaOptima(checking2, n, m, conexiones, centrales, colonias, ordenColInv, ordenCol);
    
    // 4. Definir conexión de nuevas colonias
    checking2 << endl << "-------------------" << endl <<  "4 - Conexión de nuevas colonias." << endl << endl;

    for (int i = 0; i < q; i++) {
        cin >> nombre >> x >> y;
        n++;
        Nodo auxNodo(nombre, n, x, y, 0);

        checking2 << auxNodo.nombre << " debe conectarse con " << whereToConnect(auxNodo,colonias) << endl;

    }

    checking2 << endl << "-------------------" << endl;

    return 0;
}