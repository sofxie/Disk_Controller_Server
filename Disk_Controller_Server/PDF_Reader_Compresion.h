#ifndef PDF_READER_COMPRESION_H
#define PDF_READER_COMPRESION_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <memory>
#include <random>

class PDF_Reader_Compresion {
public:
    PDF_Reader_Compresion() = default;

    std::string procesarPDFyGuardarHuffman(const std::string& rutaPDF, const std::string& rutaBase);
    void DecomprimirFile(std::string fileComprimido, std::string rutaBase, std::string rutaPDF);
    void eliminarArchivosGenerados(const std::string& rutaPDF, const std::string& rutaBase);
    std::string obtenerNombreBase(const std::string& rutaPDF);  

private:
    struct Nodo {
        char simbolo;
        int frecuencia;
        std::shared_ptr<Nodo> izquierda;
        std::shared_ptr<Nodo> derecha;

        Nodo(char simbolo, int frecuencia)
            : simbolo(simbolo), frecuencia(frecuencia), izquierda(nullptr), derecha(nullptr) {
        }

        Nodo(std::shared_ptr<Nodo> izq, std::shared_ptr<Nodo> der)
            : simbolo('\0'), frecuencia(izq->frecuencia + der->frecuencia), izquierda(izq), derecha(der) {
        }
    };

    struct Comparar {
        bool operator()(const std::shared_ptr<Nodo>& a, const std::shared_ptr<Nodo>& b) {
            return a->frecuencia > b->frecuencia;
        }
    };

    std::shared_ptr<Nodo> construirArbolHuffman(const std::string& texto);
    void generarCodigos(std::shared_ptr<Nodo> nodo, const std::string& codigo, std::unordered_map<char, std::string>& codigos);
    std::string codificarTexto(const std::string& texto, const std::unordered_map<char, std::string>& codigos);
    std::string generarNumero6DigitosComoString();
    std::unordered_map<char, std::string> leerTablaHuffman(const std::string& ruta);
    std::shared_ptr<Nodo> reconstruirArbolDesdeTabla(const std::unordered_map<char, std::string>& codigos);
    std::string decodificarTexto(const std::string& textoCodificado, std::shared_ptr<Nodo> arbol);
    // obtenerNombreBase, lo movi public

};

#endif // PDF_READER_COMPRESION_H
