#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include "ConeccionHTTP.h"
#include "json.hpp"
#include <WinSock2.h>
#include <string>
#include <iostream>
using json = nlohmann::json;
#define addr "127.0.0.1"
#define port 8080

void ConeccionHTTP::run() {
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    const int BUFFER_SIZE = 30720;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Error de Inicialización: " << WSAGetLastError() << std::endl;
        return;
    }

    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET) {
        std::cout << "Socket no creado: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(addr);
    server.sin_port = htons(port);
    server_len = sizeof(server);

    if (bind(wsocket, (SOCKADDR*)&server, server_len) == SOCKET_ERROR) {
        std::cout << "Error en bind: " << WSAGetLastError() << std::endl;
        closesocket(wsocket);
        WSACleanup();
        return;
    }

    if (listen(wsocket, 20) != 0) {
        std::cout << "No se puede escuchar: " << WSAGetLastError() << std::endl;
        closesocket(wsocket);
        WSACleanup();
        return;
    }

    char buff[BUFFER_SIZE] = { 0 };
    while (true) {
        new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
        if (new_wsocket == INVALID_SOCKET) {
            std::cout << "No se pudo conectar: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::string request;
        memset(buff, 0, BUFFER_SIZE);
        int bytes;
        while ((bytes = recv(new_wsocket, buff, BUFFER_SIZE - 1, 0)) > 0) {
            buff[bytes] = '\0';
            request += buff;
            if (request.find("\r\n\r\n") != std::string::npos) {
                break;
            }
        }
        if (bytes < 0) {
            std::cout << "No se pudo leer request: " << WSAGetLastError() << std::endl;
            closesocket(new_wsocket);
            continue;
        }

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

        size_t pos = request.find("\r\n\r\n");
        std::string body = (pos != std::string::npos) ? request.substr(pos + 4) : "";
        std::cout << "Cuerpo recibido: [" << body << "]" << std::endl;

        json responseJson;
       // responseJson["status"] = "active";
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
                obtenerdocu(j);
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
        while (totalBytesSent < messageSize) {
            int bytesSent = send(new_wsocket, serverMessage.c_str() + totalBytesSent, messageSize - totalBytesSent, 0);
            if (bytesSent <= 0) {
                std::cout << "Error al enviar respuesta: " << WSAGetLastError() << std::endl;
                break;
            }
            totalBytesSent += bytesSent;
        }
        closesocket(new_wsocket);
    }

    closesocket(wsocket);
    WSACleanup();
}

void ConeccionHTTP::CargarArchivo(const json& data) {
    std::cout << "Archivo cargado: " << data.dump() << std::endl;
    // Lógica de registro
}

void ConeccionHTTP::obtenerdocu(const json& data) {
    std::cout << "Obteniendo información: " << data.dump() << std::endl;
    // Lógica de consulta
}

void ConeccionHTTP::eliminardocu(const json& data) {
    std::cout << "Obteniendo información: " << data.dump() << std::endl;
    // Lógica de consulta
}
