#ifndef RAID5_H
#define RAID5_H

#include <iostream>
#include <string>
#include <vector>

//Nota: using namespace std causa conflictos con otros archivos

class RAID5 {
public:
    RAID5() = default;

	// Metodo main RAID 5
    void raid5Algorithm(const std::string& binaryString, std::string& bloque1, std::string& bloque2, std::string& bloque3, std::string& bloque4);

private:
    // Metodos auxiliares
    std::string xorManual(const std::string& a, const std::string& b);
    std::string xorMultiple(const std::vector<std::string>& blocks);
    std::string padWithZeros(const std::string& str, int targetSize);
};

#endif // RAID5_H

