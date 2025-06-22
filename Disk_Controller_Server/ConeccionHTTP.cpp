#include "ConeccionHTTP.h"
#include "json.hpp"
using json = nlohmann::json;
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#define addr "127.0.0.1" // Direccion del server con PDF App
#define port 8080 // Puerto con PDF App
using namespace std;

void ConeccionHTTP::run() {
	std::cout << "Hola";
	// Socket escuchar las conexiones
	SOCKET wsocket;
	// Socket para clientes aceptados
	SOCKET new_wsocket;
	// Inicializa WinSock
	WSADATA wsaData;
	// Almacenar direccion del server
	struct sockaddr_in server;
	int server_len;
	// Buffer Size
	const int BUFFER_SIZE = 30720;

	// Inicializa WinSock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Error de Inicialización";
	}

	// Crear socket
	wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (wsocket == INVALID_SOCKET) {
		std::cout << "Socket no creado";
		WSACleanup();
	}

	// Configurar direccion
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(addr);
	server.sin_port = htons(port);
	server_len = sizeof(server);

	// Asociar el socket con la dirección y el puerto
	if (::bind(wsocket, (SOCKADDR*)&server, sizeof(server)) ==  SOCKET_ERROR) {
		std::cout << "No se puede asociar el socket con la dirección." << std::endl;
		closesocket(wsocket);
		WSACleanup();
		return;
	}

	// Escuchar en la direccion para aceptar conexiones
	if (listen(wsocket, 20) != 0) {
		std::cout << "No se puede escuchar";
	}

	// Almacenar datos recibidos
	char buff[BUFFER_SIZE] = { 0 };


	while (true) {
		// Aceptar clientes
		new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
		if (new_wsocket == INVALID_SOCKET) {
			std::cout << "No se pudo conectar";
			continue;
		}

		// Leer datos
		int bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
		if (bytes < 0) {
			std::cout << "no se pudo leer request";
			closesocket(new_wsocket);
			continue;
		}

		// Convertir buffer a string
		std::string request(buff, bytes);

		// Buscar dónde empieza el cuerpo JSON 
		size_t pos = request.find("\r\n\r\n");
		std::string body = (pos != std::string::npos) ? request.substr(pos + 4) : "";

		// Variable JSON para almacenar datos parsed
		json j;
		json responseJson;
		if (body.empty()) {
			// Construir respuesta JSON.
			responseJson["status"] = "activo";
			responseJson["mensaje"] = "Servidor HTTP";
		}
		else {
			try {
				j = json::parse(body); // Parcear como JSON
				// Verificar si tiene el comando y contenido
				if (!j["comando"].is_null()) {
					std::cout << "Comando: " << j["comando"] << std::endl;
				}
				if (!j["contenido"].is_null()) {
					std::cout << "Contenido: " << j["contenido"] << std::endl;
				}
				std::cout << "JSON recibido: " << j.dump() << std::endl;
			}
			catch (json::parse_error& e) {
				// Error de parse
				std::cout << "Error al parsear JSON: " << e.what() << std::endl;
			}
		}

		// Convertir JSON a string
		std::string responseBody = responseJson.dump();

		// Construir encabezado HTTP
		std::string serverMessage =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: " + std::to_string(responseBody.size()) + "\r\n"
			"Connection: close\r\n"
			"\r\n" +
			responseBody;

		int totalBytesSent = 0;
		int messageSize = serverMessage.size();
		// Enviar respueta
		while (totalBytesSent < messageSize) {
			int bytesSent = send(new_wsocket, serverMessage.c_str() + totalBytesSent, messageSize - totalBytesSent, 0);
			if (bytesSent < 0) {
				cout << "no se envio respuesta";
			}
			totalBytesSent += bytesSent;
		}
		closesocket(new_wsocket);
	}

	WSACleanup();

}

void ConeccionHTTP::connectDiskNode(int Nodeport, const std::string& Nodeip) {
	WSADATA wsaData;
	SOCKET clientSocket;
	struct sockaddr_in server;
	
	// Inicializar WinSock
	if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		cout << "Error al iniciar Winsock";
	};

	// Crear socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "No se pudo conectar al Socket";
		WSACleanup();
	}

	// Consigurar direccion 
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(Nodeip.c_str());

	// Conectar al servidor
	if (::connect(clientSocket, (SOCKADDR*)&server, sizeof(server)) < 0) {
		cout << "No se conecto al server";
		closesocket(clientSocket);
		WSACleanup();
	}

	// Armar JSON
	// Ejemplo de mensaje
	std::string jBody = R"({"comando":"guardar","contenido": "Hola desde el DisskControler"})";
	
	// Construir encabezado HTTP
	std::string request = 
		"POST / HTTP/1.1\r\n"
		"Host: "+ Nodeip + "\r\n"
		"Content-Type: application/json\r\n"
		"Content-Length: " + std::to_string(jBody.length()) + "\r\n"
		"Connection: close\r\n"
		"\r\n" +
		jBody;

	// Enviar soli
	send(clientSocket, request.c_str(), request.length(), 0);

	// Recibir respuesta
	char buffer[4096];
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived > 0) {
		cout << "Respuesta del servidor:\n";
		cout << string(buffer, bytesReceived) << endl;
	}
	// Cerrar conexión
	closesocket(clientSocket);
	WSACleanup();
}