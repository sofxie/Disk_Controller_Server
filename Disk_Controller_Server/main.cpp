#include "Disk_Controller.h"
#include "ConeccionHTTP.h"
#include <msclr/marshal_cppstd.h> // para String^
#include <string>
#include "PDF_Reader_Compresion.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace DiskControllerServer;
using namespace msclr::interop;
using namespace System::Threading;

void RunServidorHTTP() {
	ConeccionHTTP* conn = new ConeccionHTTP();
	conn->run();
}
void ConectNode(int port, String^ ip) {
	// convertir tipo string a String^
	marshal_context context;
	std::string  ip_std = context.marshal_as<std::string>(ip);
	ConeccionHTTP* conn = new ConeccionHTTP();
	conn->connectDiskNode(port, ip_std);
}

[STAThread]
int main(cli::array<String^>^ args) {

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