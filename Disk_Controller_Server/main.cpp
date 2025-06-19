#include "Disk_Controller.h"
#include "ConeccionHTTP.h"
#include "PDF_Reader_Compresion.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace DiskControllerServer;

using namespace System::Threading;

void RunServidorHTTP() {
	ConeccionHTTP* conn = new ConeccionHTTP();
	conn->run();  // Esta es tu función con while(true)
}

[STAThread]
int main(array<String^>^ args) {

	PDF_Reader_Compresion lector;

	// Ruta del PDF que quieres procesar
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