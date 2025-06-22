#include "RAID5.h"

// Calcula XOR entre dos strings binarios sin usar ^
std::string RAID5::xorManual(const std::string& a, const std::string& b) {
    std::string res = "";
    int maxSize = std::max(a.size(), b.size());

    // Rellenar con ceros a la izquierda si es necesario
    std::string padA = std::string(maxSize - a.size(), '0') + a;
    std::string padB = std::string(maxSize - b.size(), '0') + b;

    for (int i = 0; i < maxSize; ++i) {
        int bitA = padA[i] - '0';
        int bitB = padB[i] - '0';
        int bitXOR = (bitA + bitB) % 2;
        res += (bitXOR == 1) ? '1' : '0';
    }
    return res;
}

// Calcula XOR de múltiples strings
std::string RAID5::xorMultiple(const std::vector<std::string>& blocks) {
    if (blocks.empty()) return "";

    std::string result = blocks[0];
    for (int i = 1; i < blocks.size(); ++i) {
        result = xorManual(result, blocks[i]);
    }
    return result;
}

// Rellena un string con ceros a la derecha hasta alcanzar el tamaño objetivo
std::string RAID5::padWithZeros(const std::string& str, int targetSize) {
    if (str.size() >= targetSize) return str;
    return str + std::string(targetSize - str.size(), '0');
}

// RAID 5 y retorna los bloques finales
void RAID5::raid5Algorithm(const std::string& binaryString, std::string& bloque1, std::string& bloque2, std::string& bloque3, std::string& bloque4) {
    //------------Parametros RAID 5------------
    const int BLOCK_SIZE = 20; // Tamaño de bloque en bits por rounds
    const int BLOCKS_PER_ROUND = 4; // 3 datos + 1 paridad

    bloque1 = "";
    bloque2 = "";
    bloque3 = "";
    bloque4 = "";

    // Dividir el string en bloques de BLOCK_SIZE
    std::vector<std::string> dataBlocks;
    for (int i = 0; i < binaryString.size(); i += BLOCK_SIZE) {
        std::string block = binaryString.substr(i, BLOCK_SIZE);
        // Rellenar con ceros si el bloque no está completo
        block = padWithZeros(block, BLOCK_SIZE);
        dataBlocks.push_back(block);
    }

    // Organizar en estructura RAID 5
    int totalRounds = (dataBlocks.size() + 2) / 3; // Redondear hacia arriba

    // Tabla RAID 5 con rotación de paridad
    std::vector<std::vector<std::string>> raid5Table(totalRounds, std::vector<std::string>(BLOCKS_PER_ROUND));
    std::vector<int> parityPositions(totalRounds);

    // Calcular la posicion de paridad para cada ronda con una resta circular
    for (int round = 1; round <= totalRounds; ++round) {
        int formula_result = (5 - (round % 4)) % 4;
        int pos_paridad;

        // Mapear resultado de fórmula a posición de columna correcta
        if (formula_result == 0) pos_paridad = 3; // Bloque 4 -> columna 3
        else if (formula_result == 3) pos_paridad = 2; // Bloque 3 -> columna 2
        else if (formula_result == 2) pos_paridad = 1; // Bloque 2 -> columna 1
        else if (formula_result == 1) pos_paridad = 0; // Bloque 1 -> columna 0

        parityPositions[round - 1] = pos_paridad; // Ajustar índice del array
    }

    // Llenar la tabla RAID 5
    int dataIndex = 0;
    for (int round = 0; round < totalRounds; ++round) {
        std::vector<std::string> roundDataBlocks;

        // Llenar posiciones de datos (no paridad)
        for (int col = 0; col < BLOCKS_PER_ROUND; ++col) {
            if (col != parityPositions[round]) {
                if (dataIndex < dataBlocks.size()) {
                    raid5Table[round][col] = dataBlocks[dataIndex];
                    roundDataBlocks.push_back(dataBlocks[dataIndex]);
                    dataIndex++;
                }
                else {
                    // Rellenar con ceros si no hay más datos
                    raid5Table[round][col] = std::string(BLOCK_SIZE, '0');
                    roundDataBlocks.push_back(std::string(BLOCK_SIZE, '0'));
                }
            }
        }

        // Calcular paridad para esta ronda
        std::string parity = xorMultiple(roundDataBlocks);
        raid5Table[round][parityPositions[round]] = parity;
    }

    // Concatenar todos los rounds por columna para formar los bloques finales
    for (int round = 0; round < totalRounds; ++round) {
        bloque1 += raid5Table[round][0]; // Columna 1
        bloque2 += raid5Table[round][1]; // Columna 2
        bloque3 += raid5Table[round][2]; // Columna 3
        bloque4 += raid5Table[round][3]; // Columna 4
    }
}
