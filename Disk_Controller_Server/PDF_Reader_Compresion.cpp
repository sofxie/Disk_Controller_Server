#include "PDF_Reader_Compresion.h"
#include <regex>
#include <iostream>

std::shared_ptr<PDF_Reader_Compresion::Nodo> PDF_Reader_Compresion::construirArbolHuffman(const std::string& texto) {
    std::unordered_map<char, int> frecuencia;
    for (char c : texto) {
        ++frecuencia[c];
    }

    std::priority_queue<std::shared_ptr<Nodo>, std::vector<std::shared_ptr<Nodo>>, Comparar> cola;

    for (const auto& par : frecuencia) {
        cola.push(std::make_shared<Nodo>(par.first, par.second));
    }

    while (cola.size() > 1) {
        auto izq = cola.top(); cola.pop();
        auto der = cola.top(); cola.pop();
        cola.push(std::make_shared<Nodo>(izq, der));
    }

    return cola.top();
}

void PDF_Reader_Compresion::generarCodigos(std::shared_ptr<Nodo> nodo, const std::string& codigo, std::unordered_map<char, std::string>& codigos) {
    if (!nodo) return;

    if (!nodo->izquierda && !nodo->derecha) {
        codigos[nodo->simbolo] = codigo;
        return;
    }

    generarCodigos(nodo->izquierda, codigo + "0", codigos);
    generarCodigos(nodo->derecha, codigo + "1", codigos);
}

std::string PDF_Reader_Compresion::codificarTexto(const std::string& texto, const std::unordered_map<char, std::string>& codigos) {
    std::string resultado;
    for (char c : texto) {
        resultado += codigos.at(c);
    }
    return resultado;
}

std::string PDF_Reader_Compresion::generarNumero6DigitosComoString() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(100000, 999999);
    return std::to_string(distrib(gen));
}

std::string PDF_Reader_Compresion::procesarPDFyGuardarHuffman(const std::string& rutaPDF, const std::string& rutaBase) {
    std::string codigo_del_pdf = generarNumero6DigitosComoString();
    std::string nombrePDF = obtenerNombreBase(rutaPDF);
    std::string nombreCodigo = nombrePDF + "_" + codigo_del_pdf;

    std::string archivoSalida = rutaBase + nombreCodigo + ".txt";

    std::string comando = "powershell -Command \"& C:\\PDFR\\pdftotext.exe " + rutaPDF + " " + archivoSalida + "\"";

    std::cout << "Ejecutando comando PowerShell para extraer texto del PDF...\n";
    int resultado = system(comando.c_str());

    if (resultado != 0) {
        std::cerr << "Error al ejecutar PowerShell. Código de error: " << resultado << "\n";
        return "";
    }

    std::ifstream archivo(archivoSalida);
    if (!archivo) {
        std::cerr << "Error: No se pudo abrir el archivo de salida.\n";
        return "";
    }

    std::string linea, textoCompleto;
    while (std::getline(archivo, linea)) {
        textoCompleto += linea;
    }

    auto arbol = construirArbolHuffman(textoCompleto);
    std::unordered_map<char, std::string> codigos;
    generarCodigos(arbol, "", codigos);

    std::string archivoSalidaHuff = rutaBase + "TablaCodigosHuffman" + nombrePDF + ".txt";
    std::ofstream tablaSalida(archivoSalidaHuff);
    if (!tablaSalida) {
        std::cerr << "Error: No se pudo crear el archivo de la tabla de códigos.\n";
        return "";
    }

    tablaSalida << "codigo_pdf: " << codigo_del_pdf << "\n";
    for (const auto& par : codigos) {
        if (par.first == '\n') tablaSalida << "'\\n': " << par.second << "\n";
        else if (par.first == '\r') tablaSalida << "'\\r': " << par.second << "\n";
        else if (par.first == '\t') tablaSalida << "'\\t': " << par.second << "\n";
        else if (par.first == ' ') tablaSalida << "'espacio': " << par.second << "\n";
        else tablaSalida << "'" << par.first << "': " << par.second << "\n";
    }
    tablaSalida.close();

    std::string textoCodificado = codificarTexto(textoCompleto, codigos);
    std::string archivocomprimido = rutaBase + "Textocomprimido" + nombrePDF + ".txt";

    std::ofstream salida(archivocomprimido);
    if (salida) {
        salida << textoCodificado;
        salida.close();
    }
    else {
        std::cerr << "No se pudo guardar el texto comprimido.\n";
    }

    return codigo_del_pdf + "|" + textoCodificado;
}

std::unordered_map<char, std::string> PDF_Reader_Compresion::leerTablaHuffman(const std::string& ruta) {
    std::unordered_map<char, std::string> codigos;
    std::ifstream archivo(ruta);
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t sep = linea.find(": ");
        if (sep == std::string::npos) continue;

        std::string clave = linea.substr(0, sep);
        std::string valor = linea.substr(sep + 2);

        if (clave == "'\\n'") codigos['\n'] = valor;
        else if (clave == "'\\r'") codigos['\r'] = valor;
        else if (clave == "'\\t'") codigos['\t'] = valor;
        else if (clave == "'espacio'") codigos[' '] = valor;
        else if (clave.size() >= 3 && clave.front() == '\'' && clave.back() == '\'') {
            codigos[clave[1]] = valor;
        }
    }

    return codigos;
}

std::shared_ptr<PDF_Reader_Compresion::Nodo> PDF_Reader_Compresion::reconstruirArbolDesdeTabla(const std::unordered_map<char, std::string>& codigos) {
    auto raiz = std::make_shared<Nodo>('\0', 0);

    for (const auto& par : codigos) {
        char simbolo = par.first;
        const std::string& codigo = par.second;
        auto actual = raiz;

        for (char bit : codigo) {
            if (bit == '0') {
                if (!actual->izquierda) {
                    actual->izquierda = std::make_shared<Nodo>('\0', 0);
                }
                actual = actual->izquierda;
            }
            else if (bit == '1') {
                if (!actual->derecha) {
                    actual->derecha = std::make_shared<Nodo>('\0', 0);
                }
                actual = actual->derecha;
            }
        }

        actual->simbolo = simbolo;
    }

    return raiz;
}

std::string PDF_Reader_Compresion::decodificarTexto(const std::string& textoCodificado, std::shared_ptr<Nodo> arbol) {
    std::string resultado;
    auto actual = arbol;

    for (char bit : textoCodificado) {
        if (bit == '0') actual = actual->izquierda;
        else if (bit == '1') actual = actual->derecha;

        if (!actual->izquierda && !actual->derecha) {
            resultado += actual->simbolo;
            actual = arbol;
        }
    }

    return resultado;
}

void PDF_Reader_Compresion::DecomprimirFile(std::string fileComprimido, std::string rutaBase, std::string rutaPDF) {
    size_t separador = fileComprimido.find('|');
    std::string codigoPDF = fileComprimido.substr(0, separador);
    std::string textoCodificado = fileComprimido.substr(separador + 1);

    if (fileComprimido.empty()) {
        std::cerr << "Error: No se generó texto codificado.\n";
        return;
    }

    std::string nombrePDF = obtenerNombreBase(rutaPDF);

    std::string rutaTabla = rutaBase + "TablaCodigosHuffman" + nombrePDF + ".txt";
    std::unordered_map<char, std::string> tabla = leerTablaHuffman(rutaTabla);
    if (tabla.empty()) {
        std::cerr << "No se pudo leer la tabla Huffman o está vacía.\n";
        return;
    }

    auto arbolReconstruido = reconstruirArbolDesdeTabla(tabla);
    std::string textoOriginal = decodificarTexto(textoCodificado, arbolReconstruido);

    std::cout << "\nTexto descomprimido (primeros 500 caracteres):\n";
    std::cout << textoOriginal.substr(0, 500) << "...\n";

    std::string archivoDescomprimido = rutaBase + "TextoDescomprimido" + nombrePDF + ".txt";
    std::ofstream salida(archivoDescomprimido);
    if (salida) {
        salida << textoOriginal;
        salida.close();
        std::cout << "\nTexto descomprimido guardado en: " << archivoDescomprimido << "\n";
    }
    else {
        std::cerr << "No se pudo guardar el texto descomprimido.\n";
    }

}

void PDF_Reader_Compresion::eliminarArchivosGenerados(const std::string& rutaPDF, const std::string& rutaBase) {
    std::string nombrePDF = obtenerNombreBase(rutaPDF);

    // Buscamos los archivos que empiezan con el nombre del PDF y terminan en .txt
    std::vector<std::string> extensiones = {
        rutaBase + nombrePDF + "_*.txt", // comodín para nombres generados como: reporte_123456.txt
        rutaBase + "TablaCodigosHuffman" + nombrePDF + ".txt",
        rutaBase + "Textocomprimido" + nombrePDF + ".txt",
        rutaBase + "TextoDescomprimido" + nombrePDF + ".txt",
        rutaBase + nombrePDF + ".pdf"
    };

    for (const std::string& patron : extensiones) {
        std::string comando = "for %f in (" + patron + ") do del \"%f\"";
        system(("cmd /C " + comando).c_str());
    }
}

std::string PDF_Reader_Compresion::obtenerNombreBase(const std::string& rutaPDF) {
    size_t posBarra = rutaPDF.find_last_of("\\/");
    size_t posPunto = rutaPDF.find_last_of(".");
    if (posPunto == std::string::npos || posPunto < posBarra) {
        posPunto = rutaPDF.length();
    }
    return rutaPDF.substr(posBarra + 1, posPunto - posBarra - 1);
}

#include <fstream>
#include <sys/stat.h>
#include <windows.h>

std::string PDF_Reader_Compresion::obtenerNombreBaseConTipo(const std::string& ruta) {
    size_t pos = ruta.find_last_of("\\/");
    if (pos == std::string::npos)
        return ruta;
    return ruta.substr(pos + 1);
}

std::string PDF_Reader_Compresion::copiarArchivo(const std::string& origen, const std::string& destino) {
    std::ifstream archivoOrigen(origen, std::ios::binary);
    std::ofstream archivoDestino(destino, std::ios::binary);

    if (!archivoOrigen) {
        std::cerr << "No se pudo abrir el archivo de origen.\n";
        return "";  // o podrías devolver un mensaje de error
    }
    if (!archivoDestino) {
        std::cerr << "No se pudo crear el archivo de destino.\n";
        return "";
    }

    archivoDestino << archivoOrigen.rdbuf();  // Copiar contenido binario
    return destino;  // Devuelve la ruta del archivo copiado
}
