#include <iostream>
#include <vector>

using namespace std;

struct Nodo {
    string nombre;
    int x;
    int y;
    bool central;
};


int main () {

    int n, m, k, q;
    string nombre;
    Nodo aux;
    vector<Nodo> colonias;

    cin >> n >> m >> k >> q;

    for (int i = 0; i < n; i++) {
        cin >> aux.nombre >> aux.x >> aux.y >> aux.central;

        colonias.push_back(aux);
    }

    for(int i = 0; i < colonias.size(); i++){
        cout << "Nombre:" << colonias[i].nombre << endl;
        cout << "x:" << colonias[i].x << endl;
        cout << "y:" << colonias[i].y << endl;
        cout << "Central:" << colonias[i].central << endl;
    }

    return 0;
}