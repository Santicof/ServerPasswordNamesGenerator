#include <iostream>
#include <winsock2.h>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

void generarNombre(int longitud, string& resultado) {
    const string vocales = "AEIOUaeiou";
    const string consonantes = "BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";

    resultado = "";
    bool usarVocal = rand() % 2; // Aleatoriamente decide si empieza por vocal o consonante

    for (int i = 0; i < longitud; i++) {
        if (usarVocal) {
            resultado += vocales[rand() % vocales.length()];
        } else {
            resultado += consonantes[rand() % consonantes.length()];
        }
        usarVocal = !usarVocal; // Alterna entre vocal y consonante
    }
}

void generarContrasena(int longitud, string& resultado) {
    const string caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
    resultado = "";

    for (int i = 0; i < longitud; i++) {
        resultado += caracteres[rand() % caracteres.length()];
    }
}

void procesarCliente(SOCKET client) {
    char buffer[1024];
    string resultado;

    while (true) {
        // Enviar menú inicial al cliente
        string menu = "\n\nSelecciona una opción: \n 0 - Generador de Nombres\n 1 - Generador de Contraseñas\n 2 - Salir\n\n";
        send(client, menu.c_str(), menu.size(), 0);

        // Recibir la opción del cliente
        int bytesRecibidos = recv(client, buffer, sizeof(buffer), 0);
        if (bytesRecibidos <= 0) break; // Finaliza si el cliente se desconecta
        int opcion = atoi(buffer);

        int longitud = 0;
        bool longitudValida = false;

        switch (opcion) {
            case 0: // Generador de Nombres
                while (!longitudValida) {
                    string solicitudLongitud = "\nIngresa la longitud del nombre de usuario (5-15 caracteres):";
                    send(client, solicitudLongitud.c_str(), solicitudLongitud.size(), 0);

                    bytesRecibidos = recv(client, buffer, sizeof(buffer), 0);
                    if (bytesRecibidos <= 0) break;
                    longitud = atoi(buffer);

                    if (longitud >= 5 && longitud <= 15) {
                        longitudValida = true;
                    } else {
                        string errorMsg = "\nError: La longitud debe estar entre 5 y 15 caracteres. Intenta de nuevo.\n";
                        send(client, errorMsg.c_str(), errorMsg.size(), 0);
                    }
                }
                if (longitudValida) {
                    generarNombre(longitud, resultado);
                    send(client, resultado.c_str(), resultado.size(), 0);
                }
                break;

            case 1: // Generador de Contraseñas
                while (!longitudValida) {
                    string solicitudLongitud = "\nIngresa la longitud de la contraseña (8-50 caracteres):\n ";
                    send(client, solicitudLongitud.c_str(), solicitudLongitud.size(), 0);

                    bytesRecibidos = recv(client, buffer, sizeof(buffer), 0);
                    if (bytesRecibidos <= 0) break;
                    longitud = atoi(buffer);

                    if (longitud >= 8 && longitud < 50) {
                        longitudValida = true;
                    } else {
                        string errorMsg = "\nError: La longitud debe estar entre 8 y 50 caracteres. Intenta de nuevo\n.";
                        send(client, errorMsg.c_str(), errorMsg.size(), 0);
                    }
                }
                if (longitudValida) {
                    generarContrasena(longitud, resultado);
                    send(client, resultado.c_str(), resultado.size(), 0);
                }
                break;

            case 2: // Salir
                cout << "Cliente desconectado." << endl;
                return;

            default: // Opción no válida
                string mensaje = "Opción no válida. Intenta de nuevo.";
                send(client, mensaje.c_str(), mensaje.size(), 0);
                break;
        }
    }

    closesocket(client);
}

int main() {
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5005);

    bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    cout << "Esperando conexiones..." << endl;

    while ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
        cout << "Cliente conectado!" << endl;
        procesarCliente(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
