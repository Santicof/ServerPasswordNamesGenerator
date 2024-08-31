#include <iostream>
#include <winsock2.h>
#include <cstring>  // Incluir para memset y strlen

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
        addr.sin_addr.s_addr = inet_addr("192.168.0.174"); // Aseg�rate de que esta direcci�n IP sea correcta
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5005); // Aseg�rate de que este puerto sea correcto

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

    void Interactuar() {
        string respuesta;

        // Recibe el men� inicial del servidor
        respuesta = Recibir();

        while (true) {
            // Enviar opci�n seleccionada
            cout << "Selecciona una opci�n (0 - Generador de Nombres, 1 - Generador de Contrase�as, 2 - Salir): ";
            string opcion;
            cin >> opcion;
            Enviar(opcion);

            // Procesar opci�n
            if (opcion == "0" || opcion == "1") {
                // Recibe petici�n de longitud desde el servidor
                respuesta = Recibir();

                // Enviar longitud deseada
                cout << "Ingresa la longitud deseada: ";
                string longitud;
                cin >> longitud;
                Enviar(longitud);

                // Recibir el resultado generado por el servidor
                respuesta = Recibir();
                cout << "El servidor ha generado: " << respuesta << endl;

            } else if (opcion == "2") {
                // Salir del programa
                cout << "Saliendo..." << endl;
                break;
            } else {
                // Manejar opciones no v�lidas
                cout << "Opci�n no v�lida. Intenta de nuevo." << endl;
            }
        }

        // Cerrar la conexi�n
        CerrarSocket();
    }
};

int main() {
    Cliente* cliente = new Cliente();
    cliente->Interactuar();
    delete cliente;
    return 0;
}
