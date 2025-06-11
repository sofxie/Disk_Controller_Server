#include "ConeccionHTTP.h"
#include "json.hpp"
using json = nlohmann::json;
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#define addr "127.0.0.1" // Direccion del server con PDF App
#define port 8080 // Puerto con PDF App
using namespace std;

void ConeccionHTTP::run() {
	SOCKET wsocket;
	SOCKET new_wsocket;
	WSADATA wsaData;
	struct sockaddr_in server;
	int server_len;
	const int BUFFER_SIZE = 30720;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Error de Inicialización";
	}

	// Crear socket
	wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (wsocket == INVALID_SOCKET) {
		std::cout << "Socket no creado";
	}

	// Configurar direccion
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(addr);
	server.sin_port = htons(port);
	server_len = sizeof(server);


	// Escuchar en la direccion
	if (listen(wsocket, 20) != 0) {
		std::cout << "No se puede escuchar";
	}

	char buff[BUFFER_SIZE] = { 0 };

	while (true) {
		// Aceptar clientes
		new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
		if (new_wsocket == INVALID_SOCKET) {
			std::cout << "No se pudo conectar";
			continue;
		}

		// Leer request
		int bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
		if (bytes < 0) {
			std::cout << "no se pudo leer request";
			closesocket(new_wsocket);
			continue;
		}

		// Convertir buffer a string
		std::string request(buff, bytes);

		// Buscar dónde empieza el cuerpo JSON (después de "\r\n\r\n")
		size_t pos = request.find("\r\n\r\n");
		std::string body = (pos != std::string::npos) ? request.substr(pos + 4) : "";

		// Parsear JSON si existe
		json j;
		if (!j["nombre"].is_null()) {
			std::cout << "Nombre: " << j["nombre"] << std::endl;
		}
		try {
			j = json::parse(body);
			std::cout << "JSON recibido: " << j.dump() << std::endl;
		}
		catch (json::parse_error& e) {
			std::cout << "Error al parsear JSON: " << e.what() << std::endl;
			// Puedes responder con un error HTTP aquí si quieres
		}

		// Construir respuesta JSON
		json responseJson;
		responseJson["status"] = "active";
		responseJson["mensaje"] = "¡Bienvenido al servidor Disk Controller!";
		responseJson["eco"] = j;

		std::string responseBody = responseJson.dump();

		// Construir encabezado HTTP para respuesta JSON
		std::string serverMessage =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: " + std::to_string(responseBody.size()) + "\r\n"
			"Connection: close\r\n"
			"\r\n" +
			responseBody;

		int totalBytesSent = 0;
		int messageSize = serverMessage.size();
		while (totalBytesSent < messageSize) {
			int bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
			if (bytesSent < 0) {
				cout << "no se envio respuesta";
			}
			totalBytesSent += bytesSent;
		}
		closesocket(new_wsocket);
	}

	WSACleanup();

}