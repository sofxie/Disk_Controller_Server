#ifndef RAIDMANAGER_H
#define RAIDMANAGER_H

#include <string>

// Declaraci�n de la funci�n que simula el env�o de un bloque en formato JSON
void enviarABlockADiskNode(const std::string& mensajeJSON);

// Declaraci�n de la funci�n que divide y env�a los bloques RAID5 a los nodos
void enviarBloquesGeneradosARaidNodes(
    const std::string& bloque1,
    const std::string& bloque2,
    const std::string& bloque3,
    const std::string& bloque4,
    const std::string& nombrePDF);

#endif







