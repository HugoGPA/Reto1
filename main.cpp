#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;


struct Registro {
    string mes;
    int dia;
    string hora;
    string ip;
    string razon;
    int mesNumerico;

    
    string toString() const {
        ostringstream oss;
        oss << mes << " " << dia << " " << hora << " " << ip << " " << razon;
        return oss.str();
    }
};


int convertirMes(const string& mes) {
    static map<string, int> mesMap = {
        {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6},
        {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
    };
    return mesMap[mes];
}


bool compararFechas(const Registro &a, const Registro &b) {
    if (a.mesNumerico != b.mesNumerico)
        return a.mesNumerico < b.mesNumerico;
    if (a.dia != b.dia)
        return a.dia < b.dia;
    return a.hora < b.hora;
}


void ordenamiento(vector<Registro> &arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<Registro> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compararFechas(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void ordenamiento2(vector<Registro> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        ordenamiento2(arr, left, mid);
        ordenamiento2(arr, mid + 1, right);
        ordenamiento2(arr, left, mid, right);
    }
}


void leer(const string &nombreArchivo, vector<Registro> &registros) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Registro reg;
        ss >> reg.mes >> reg.dia >> reg.hora >> reg.ip;
        getline(ss, reg.razon);
        reg.mesNumerico = convertirMes(reg.mes);
        registros.push_back(reg);
    }

    archivo.close();
}

void registrosentrefechas(const vector<Registro> &registros, const Registro &inicio, const Registro &fin) {
    for (const auto &reg : registros) {
        if (compararFechas(inicio, reg) && compararFechas(reg, fin)) {
            cout << reg.toString() << endl;
        }
    }
}


void escribir(const string &nombreArchivo, const vector<Registro> &registros) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo para escribir el resultado ordenado" << endl;
        return;
    }

    for (const auto &reg : registros) {
        archivo << reg.toString() << endl;
    }

    archivo.close();
}

int main() {
    vector<Registro> registros;
    leer("bitacora.txt", registros);

  
    ordenamiento2(registros, 0, registros.size() - 1);

    
    escribir("bitacora_ordenada.txt", registros);

    
    Registro fechaInicio, fechaFin;
    cout << "Ingrese la fecha de inicio (mes dia hora): ";
    cin >> fechaInicio.mes >> fechaInicio.dia >> fechaInicio.hora;
    fechaInicio.mesNumerico = convertirMes(fechaInicio.mes);

    cout << "Ingrese la fecha de fin (mes dia hora): ";
    cin >> fechaFin.mes >> fechaFin.dia >> fechaFin.hora;
    fechaFin.mesNumerico = convertirMes(fechaFin.mes);

    
    registrosentrefechas(registros, fechaInicio, fechaFin);

    return 0;
}
