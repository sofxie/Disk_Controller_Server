#ifndef RAIDMANAGER_H
#define RAIDMANAGER_H

#include <string>

// Declaración de la función que simula el envío de un bloque en formato JSON
void enviarABlockADiskNode(const std::string& mensajeJSON);

// Declaración de la función que divide y envía los bloques RAID5 a los nodos
void enviarBloquesGeneradosARaidNodes(
    const std::string& bloque1,
    const std::string& bloque2,
    const std::string& bloque3,
    const std::string& bloque4,
    const std::string& nombrePDF);

#endif







