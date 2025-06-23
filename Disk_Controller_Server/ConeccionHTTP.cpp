#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include "ConeccionHTTP.h"
#include "json.hpp"
#include <WinSock2.h>
#include <iostream>
#include <string>
#define addr "127.0.0.1" // Direccion del server con PDF App
#define port 8080 // Puerto con PDF App
using json = nlohmann::json;

void ConeccionHTTP::run() {

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

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Error de Inicialización: " << WSAGetLastError() << std::endl;
        return;
    }

	// Crear socket
	wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (wsocket == INVALID_SOCKET) {
		std::cout << "Socket no creado";
		WSACleanup();
	}

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
		std::string request(buff, bytes);
        std::string method = request.substr(0, request.find(" "));
        if (method != "POST") {
            std::cout << "Método no permitido: " << method << std::endl;
            std::string errorResponse =
                "HTTP/1.1 405 Method Not Allowed\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 23\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Solo se permite POST";
            send(new_wsocket, errorResponse.c_str(), errorResponse.size(), 0);
            closesocket(new_wsocket);
            continue;
        }

		// Buscar dónde empieza el cuerpo JSON 
		size_t pos = request.find("\r\n\r\n");
		std::string body = (pos != std::string::npos) ? request.substr(pos + 4) : "";

        json responseJson;
        responseJson["status"] = "active";
        responseJson["mensaje"] = u8"\u00A1Bienvenido al servidor Disk Controller!"; // Cadena UTF-8 explícita
        if (body.empty()) {
            std::cout << "Cuerpo de la solicitud vacío" << std::endl;
            std::string errorResponse =
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";
            send(new_wsocket, errorResponse.c_str(), errorResponse.size(), 0);
            closesocket(new_wsocket);
            continue;
        }

        try {
            json j = json::parse(body);
            std::cout << "JSON recibido: " << j.dump() << std::endl;

            if (!j.contains("accion")) {
                throw std::runtime_error("Falta el campo 'accion'");
            }

            std::string accion = j["accion"];
            if (accion == "save") {
                CargarArchivo(j);
            }
            else if (accion == "delete") {
                eliminardocu(j);
            }
            else if (accion == "search") {
                odtenerdocu(j);
			}
            else {
                std::cout << "Acción desconocida: " << accion << std::endl;
                responseJson["error"] = "Acción desconocida";
            }

            responseJson["eco"] = j;

        }
        catch (json::parse_error& e) {
            std::cout << "Error al parsear JSON: " << e.what() << std::endl;
            std::string errorResponse =
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";
            send(new_wsocket, errorResponse.c_str(), errorResponse.size(), 0);
            closesocket(new_wsocket);
            continue;
        }
        catch (std::exception& e) {
            std::cout << "Error al procesar acción: " << e.what() << std::endl;
            responseJson["error"] = e.what();
        }

        std::string responseBody = responseJson.dump();
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
				std::cout << "no se envio respuesta";
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
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Error al iniciar Winsock";
	};

	// Crear socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "No se pudo conectar al Socket";
		WSACleanup();
	}

	// Consigurar direccion 
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(Nodeip.c_str());

	// Conectar al servidor
	if (::connect(clientSocket, (SOCKADDR*)&server, sizeof(server)) < 0) {
		std::cout << "No se conecto al server";
		closesocket(clientSocket);
		WSACleanup();
	}

	// Armar JSON
	// Ejemplo de mensaje
	std::string jBody = R"({"comando":"guardar","contenido": "Hola desde el DisskControler"})";

	// Construir encabezado HTTP
	std::string request =
		"POST / HTTP/1.1\r\n"
		"Host: " + Nodeip + "\r\n"
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
		std::cout << "Respuesta del servidor:\n";
		std::cout << std::string(buffer, bytesReceived) << std::endl;
	}
	// Cerrar conexión
	closesocket(clientSocket);
	WSACleanup();
}


std::string ConeccionHTTP::sendStatusToNode(const std::string& nodeIP, int nodePort) {
	std::cout << "aca";
	WSADATA wsaData;
	SOCKET clientSocket;
	struct sockaddr_in server;
	std::string responseString;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return "Error al iniciar Winsock";
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		WSACleanup();
		return "No se pudo crear el socket";
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(nodePort);
	server.sin_addr.s_addr = inet_addr(nodeIP.c_str());

	if (::connect(clientSocket, (SOCKADDR*)&server, sizeof(server)) < 0) {
		closesocket(clientSocket);
		WSACleanup();
		return "No se pudo conectar al nodo " + nodeIP + ":" + std::to_string(nodePort);
	}

	// Crear JSON
	json jBody;
	jBody["comando"] = "status";

	std::string jBodyStr = jBody.dump();

	std::string request =
		"POST / HTTP/1.1\r\n"
		"Host: " + nodeIP + "\r\n"
		"Content-Type: application/json\r\n"
		"Content-Length: " + std::to_string(jBodyStr.length()) + "\r\n"
		"Connection: close\r\n"
		"\r\n" +
		jBodyStr;

	send(clientSocket, request.c_str(), request.length(), 0);
	// Recibir respuesta
	char buffer[8192] = { 0 };
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived <= 0) {
		closesocket(clientSocket);
		WSACleanup();
		return "No se recibió respuesta del nodo.";
	}

	std::string rawResponse(buffer, bytesReceived);

	// Extraer solo el cuerpo del JSON (después del doble salto de línea)
	size_t pos = rawResponse.find("\r\n\r\n");
	if (pos == std::string::npos) {
		closesocket(clientSocket);
		WSACleanup();
		return "Respuesta HTTP inválida.";
	}

	std::string jsonResponse = rawResponse.substr(pos + 4);

	closesocket(clientSocket);
	WSACleanup();

	return jsonResponse;
}



/// Comandos


void ConeccionHTTP::CargarArchivo(const json& data) {
    std::cout << "Cargando archivo desde JSON...\n";

    // Validar que exista el campo "contenido"
    if (!data.contains("contenido") || !data["contenido"].is_string()) {
        std::cerr << "Error: JSON no contiene un campo 'contenido' válido.\n";
        return;
    }

    // Obtener ruta del archivo desde el JSON
    std::string rutaPDFOriginal = data["contenido"];
    PDF_Reader_Compresion lector;

    // Carpeta base fija
    std::string rutaBase = "C:\\PDFR\\";
    std::string RutaPDFNueva = rutaBase + lector.obtenerNombreBaseConTipo(rutaPDFOriginal);

    // Copiar PDF a carpeta fija
    std::string rutaPDF = lector.copiarArchivo(rutaPDFOriginal, RutaPDFNueva);
    if (rutaPDF.empty()) {
        std::cerr << "Error: no se pudo copiar el archivo desde " << rutaPDFOriginal << "\n";
        return;
    }

    // Procesar PDF y comprimir con Huffman
    std::string resultado = lector.procesarPDFyGuardarHuffman(rutaPDF, rutaBase);
    if (resultado.empty()) {
        std::cerr << "Error: No se pudo procesar el PDF y comprimir.\n";
        return;
    }

    // Mostrar un resumen del resultado
    std::cout << "Proceso completado. Resultado (inicio):\n" << resultado.substr(0, 50) << "...\n";

    // mostrar resultado parcial en consola
    std::cout << "Proceso completado. Resultado (inicio):\n" << resultado.substr(0, 50) << "...\n";


    // ========== Aplicar el RAID 5  ==========
    // Extraer solo el texto codificado (parte despu?s del '|')
    size_t separador = resultado.find('|');
    std::string textoCodificado = "";
    if (separador != std::string::npos) {
        textoCodificado = resultado.substr(separador + 1);
    }

    if (!textoCodificado.empty()) {
        // Variables para almacenar los bloques finales
        std::string bloque1, bloque2, bloque3, bloque4;

        // Crear instancia de RAID5 y ejecutar el algoritmo
        RAID5 raid5;
        raid5.raid5Algorithm(textoCodificado, bloque1, bloque2, bloque3, bloque4);

        // Obtener nombre base del PDF para los archivos RAID5
        std::string nombrePDF = data["nombre"];

        // Guardar los bloques en archivos separados
        std::ofstream archivo1(rutaBase + "RAID5_" + nombrePDF + "_Bloque1.txt");
        std::ofstream archivo2(rutaBase + "RAID5_" + nombrePDF + "_Bloque2.txt");
        std::ofstream archivo3(rutaBase + "RAID5_" + nombrePDF + "_Bloque3.txt");
        std::ofstream archivo4(rutaBase + "RAID5_" + nombrePDF + "_Bloque4.txt");

        if (archivo1 && archivo2 && archivo3 && archivo4) {
            archivo1 << bloque1;
            archivo2 << bloque2;
            archivo3 << bloque3;
            archivo4 << bloque4;

            archivo1.close();
            archivo2.close();
            archivo3.close();
            archivo4.close();

            enviarBloquesGeneradosARaidNodes(bloque1, bloque2, bloque3, bloque4, nombrePDF);

            std::cout << "Se logro almacenar los bloques con raid 5" << std::endl;
        }
        else {
            std::cerr << "Error: No se pudieron crear los archivos del raid" << std::endl;
        }
    }
    else {
        std::cerr << "Error: No hay texto codificado para aplicar raid." << std::endl;
    }

    // Descomprimir el texto codificado y mostrar resultados
    lector.DecomprimirFile(resultado, rutaBase, rutaPDF);
}



void ConeccionHTTP::odtenerdocu(const json& data) {
    std::cout << "Obteniendo información: " << data.dump() << std::endl;
    // Lógica de consulta
}

void ConeccionHTTP::eliminardocu(const json& data) {
    std::cout << "Eliminando documento desde JSON...\n";

    if (!data.contains("contenido") || !data["contenido"].is_string()) {
        std::cerr << "Error: JSON no contiene un campo 'contenido' válido.\n";
        return;
    }

    std::string rutaPDF = data["contenido"];
    std::string rutaBase = "C:\\PDFR\\";

    PDF_Reader_Compresion lector;
    lector.eliminarArchivosGenerados(rutaPDF, rutaBase);

    std::cout << "Archivos relacionados eliminados correctamente para: " << rutaPDF << "\n";
}
