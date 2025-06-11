#include "Disk_Controller.h"
#include "ConeccionHTTP.h"

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