import socket

class Cliente:
    def __init__(self, ip_servidor, puerto):
        self.server = None
        try:
            print("Conectando al servidor...\n")
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server.connect((ip_servidor, puerto))
            print("Conectado al servidor!")
        except ConnectionError as e:
            print(f"No se pudo conectar al servidor: {e}")
            self.cerrar_socket()

    def enviar(self, mensaje):
        if self.server:
            try:
                self.server.sendall(mensaje.encode('utf-8'))
                print(f"Mensaje enviado: {mensaje}")
            except OSError as e:
                print(f"Error al enviar el mensaje: {e}")
                self.cerrar_socket()

    def recibir(self):
        if self.server:
            try:
                buffer = self.server.recv(1024).decode('latin-1')
                print(f"El servidor dice: {buffer}")
                return buffer
            except OSError as e:
                print(f"Error al recibir datos: {e}")
                self.cerrar_socket()
        return ""

    def cerrar_socket(self):
        if self.server:
            try:
                self.server.close()
                print("Socket cerrado\n")
            except OSError as e:
                print(f"Error al cerrar el socket: {e}")
            finally:
                self.server = None  # Asegurarse de que no se intente usar el socket cerrado

def main():
    cliente = Cliente("192.168.0.174", 5005)

    if cliente.server:
        while True:
            respuesta = cliente.recibir()

            if not respuesta or "Saliendo" in respuesta:
                break

            mensaje = input("Respuesta: ")
            cliente.enviar(mensaje)

        cliente.cerrar_socket()

if __name__ == "__main__":
    main()
