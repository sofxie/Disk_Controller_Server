#include "Disk_Controller.h"
#include "ConeccionHTTP.h"
#include "PDF_Reader_Compresion.h"
#include "RAID5.h"  // Incluir el nuevo archivo RAID5

using namespace System;
using namespace System::Windows::Forms;
using namespace DiskControllerServer;
using namespace System::Threading;

void RunServidorHTTP() {
	ConeccionHTTP* conn = new ConeccionHTTP();
	conn->run();  // Inicializar el server HTTP
}

[STAThread]
int main(array<String^>^ args) {

	PDF_Reader_Compresion lector;

	// Ruta del PDF con la que se va a trabajar
	std::string rutaPDF = "C:\\PDFR\\PDFTEST.pdf";

	// Ruta base donde guardar archivos temporales y comprimidos
	std::string rutaBase = "C:\\PDFR\\";

	// Procesar PDF y comprimir usando Huffman
	std::string resultado = lector.procesarPDFyGuardarHuffman(rutaPDF, rutaBase);

	if (resultado.empty()) {
		std::cerr << "Error: No se pudo procesar el PDF y comprimir.\n";
		return 1;
	}

	// El resultado es "codigo|textoCodificado"
	std::cout << "Proceso completado. Resultado:\n" << resultado.substr(0, 50) << "...\n";

	// Extraer solo el texto codificado (parte después del '|')
	size_t separador = resultado.find('|');
	std::string textoCodificado = "";
	if (separador != std::string::npos) {
		textoCodificado = resultado.substr(separador + 1);
	}

	// ========== Implementacion del RAID ==========
	if (!textoCodificado.empty()) {
		// Variables para almacenar los bloques finales
		std::string bloque1, bloque2, bloque3, bloque4;

		// Crear instancia de RAID5 y ejecutar el algoritmo
		RAID5 raid5;
		raid5.raid5Algorithm(textoCodificado, bloque1, bloque2, bloque3, bloque4);

		// Los bloques se guardan en archivos de texto como un extra, pueden utilizar los strings directamente 
		std::ofstream archivo1(rutaBase + "RAID5_Bloque1.txt");
		std::ofstream archivo2(rutaBase + "RAID5_Bloque2.txt");
		std::ofstream archivo3(rutaBase + "RAID5_Bloque3.txt");
		std::ofstream archivo4(rutaBase + "RAID5_Bloque4.txt");

		if (archivo1 && archivo2 && archivo3 && archivo4) {
			archivo1 << bloque1;
			archivo2 << bloque2;
			archivo3 << bloque3;
			archivo4 << bloque4;

			archivo1.close();
			archivo2.close();
			archivo3.close();
			archivo4.close();

		}

	}
	else {
		std::cerr << "Error: No es posible aplicar el algoritmo" << std::endl;
	}
	// ====================================================

	// Descomprimir el texto codificado y mostrar resultados
	lector.DecomprimirFile(resultado, rutaBase);

	////////////

	// Para iniciar la interfaz
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	MyForm^ form = gcnew MyForm();

	// Crear e iniciar hilo para el servidor HTTP
	Thread^ serverThread = gcnew Thread(gcnew ThreadStart(&RunServidorHTTP));
	serverThread->IsBackground = true; // Se cierra junto con la app
	serverThread->Start();

	Application::Run(form);

	return 0;
}
