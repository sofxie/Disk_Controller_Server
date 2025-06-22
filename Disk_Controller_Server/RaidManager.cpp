#include "json.hpp"
#include <iostream>
#include <vector>
#include "RAID5.h"

using json = nlohmann::json;

// Simula el envío a cada nodo (debes reemplazar con socket o HTTP si es distribuido)
void enviarABlockADiskNode(const std::string& mensajeJSON);

void enviarBloquesARaidNodes(const std::string& textoCodificado, const std::string& nombrePDF) {
    std::string bloque1, bloque2, bloque3, bloque4;
    RAID5 raid5;
    raid5.raid5Algorithm(textoCodificado, bloque1, bloque2, bloque3, bloque4);

    std::vector<std::string> bloques = { bloque1, bloque2, bloque3, bloque4 };

    for (int i = 0; i < 4; ++i) {
        json bloqueJson;
        bloqueJson["nodo"] = i + 1;
        bloqueJson["archivo"] = "RAID5_" + nombrePDF + "_Bloque" + std::to_string(i + 1) + ".txt";
        bloqueJson["contenido"] = bloques[i];

        enviarABlockADiskNode(bloqueJson.dump());
    }
}
