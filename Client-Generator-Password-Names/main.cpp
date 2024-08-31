#include <iostream>
#include <winsock2.h>
#include <cstring>

using namespace std;

class Cliente {
public:
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];

    Cliente() {
        cout << "Conectando al servidor..." << endl << endl;
        WSAStartup(MAKEWORD(2, 0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_addr.s_addr = inet_addr("192.168.0.174"); // Dirección IP del servidor
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5005); // Puerto del servidor

        if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            cerr << "No se pudo conectar al servidor." << endl;
            closesocket(server);
            WSACleanup();
            exit(1);
        }
        cout << "Conectado al servidor!" << endl;
    }

    void Enviar(const string& mensaje) {
        strcpy(buffer, mensaje.c_str());
        send(server, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        cout << "Mensaje enviado: " << mensaje << endl;
    }

    string Recibir() {
        recv(server, buffer, sizeof(buffer), 0);
        string recibido(buffer);
        memset(buffer, 0, sizeof(buffer));
        cout << "El servidor dice: " << recibido << endl;
        return recibido;
    }

    void CerrarSocket() {
        closesocket(server);
        WSACleanup();
        cout << "Socket cerrado" << endl << endl;
    }
};

int main() {
    Cliente* cliente = new Cliente();

    while (true) {
        // Recibir mensaje del servidor (menú u otra instrucción)
        string respuesta = cliente->Recibir();

        // Si la respuesta implica desconexión, salir del loop
        if (respuesta.find("Saliendo") != string::npos) {
            break;
        }

        // Enviar respuesta del usuario
        string mensaje;
        cout << "Respuesta: ";
        cin >> mensaje;
        cliente->Enviar(mensaje);
    }

    cliente->CerrarSocket();
    delete cliente;
    return 0;
}
