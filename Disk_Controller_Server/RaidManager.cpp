#include "RaidManager.h"
#include "RAID5.h"
#include "json.hpp"
#include <vector>

using json = nlohmann::json;

void enviarABlockADiskNode(const std::string& mensajeJSON) {
    // Simulación de envío
    std::cout << "Enviando a nodo: " << mensajeJSON << std::endl;
}

void enviarBloquesGeneradosARaidNodes(
    const std::string& bloque1,
    const std::string& bloque2,
    const std::string& bloque3,
    const std::string& bloque4,
    const std::string& nombrePDF) {

    std::vector<std::string> bloques = { bloque1, bloque2, bloque3, bloque4 };

    for (int i = 0; i < 4; ++i) {
        json bloqueJson;
        bloqueJson["nodo"] = i + 1;
        bloqueJson["archivo"] = "RAID5_" + nombrePDF + "_Bloque" + std::to_string(i + 1) + ".txt";
        bloqueJson["contenido"] = bloques[i];

        enviarABlockADiskNode(bloqueJson.dump());
    }
}

