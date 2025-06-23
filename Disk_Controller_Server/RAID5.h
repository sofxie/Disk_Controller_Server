#ifndef RAID5_H
#define RAID5_H

#include <iostream>
#include <string>
#include <vector>

class RAID5 {
public:
    RAID5() = default;

    // Método principal que replica tu función original
    void raid5Algorithm(const std::string& binaryString, std::string& bloque1, std::string& bloque2, std::string& bloque3, std::string& bloque4);

private:
    // Métodos auxiliares (igual que tus funciones originales)
    std::string xorManual(const std::string& a, const std::string& b);
    std::string xorMultiple(const std::vector<std::string>& blocks);
    std::string padWithZeros(const std::string& str, int targetSize);
};

#endif // RAID5_H
