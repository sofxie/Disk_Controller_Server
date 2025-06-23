#include "RaidManager.h"
#include "RAID5.h"
#include "json.hpp"
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using json = nlohmann::json;

// Simulación de envío
void enviarABlockADiskNode(const std::string& mensajeJSON) {
    try {
        json data = json::parse(mensajeJSON);
        int nodo = data["nodo"];
        std::string archivo = data["archivo"];
        std::string contenido = data["contenido"];

        std::string rutaBase = "C:\\PDF\\Nodo" + std::to_string(nodo) + "\\";
        fs::create_directories(rutaBase);  // crea la carpeta si no existe

        std::string rutaArchivo = rutaBase + archivo;

        std::ofstream outFile(rutaArchivo);
        if (outFile.is_open()) {
            outFile << contenido;
            outFile.close();
            std::cout << "[Nodo " << nodo << "] Archivo guardado en: " << rutaArchivo << std::endl;
        }
        else {
            std::cerr << "[Nodo " << nodo << "] ERROR al guardar: " << rutaArchivo << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Fallo al procesar JSON en enviarABlockADiskNode: " << e.what() << std::endl;
    }
}

void enviarBloquesGeneradosARaidNodes(
    const std::string& bloque1,
    const std::string& bloque2,
    const std::string& bloque3,
    const std::string& bloque4,
    const std::string& nombrePDF) {

    std::vector<std::string> bloques = { bloque1, bloque2, bloque3, bloque4 };

    for (int i = 0; i < 4; ++i) {
        std::string rutaNodo = "C:\\PDF\\Nodo" + std::to_string(i + 1);

        // Verificar si el nodo está disponible
        if (!fs::exists(rutaNodo)) {
            std::cerr << "[ADVERTENCIA] Nodo " << (i + 1) << " no está disponible (no existe la carpeta).\n";
            continue;
        }

        json bloqueJson;
        bloqueJson["nodo"] = i + 1;
        bloqueJson["archivo"] = "RAID5_" + nombrePDF + "_Bloque" + std::to_string(i + 1) + ".txt";
        bloqueJson["contenido"] = bloques[i];

        enviarABlockADiskNode(bloqueJson.dump());
    }
}



