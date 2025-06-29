#pragma once
#include <string>
#include <iostream>
#include <map>
#include <iomanip>
#include <sstream>
#include "ConeccionHTTP.h"
#include "PDF_Reader_Compresion.h"
#include "RAID5.h" 
#include <msclr/marshal_cppstd.h>
#include "RaidManager.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

namespace DiskControllerServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref struct DatosNodo {
		System::String^ ip;
		System::String^ puerto;
		System::String^ estado;
		System::String^ espacioDisponible;
	};
	ConeccionHTTP* con = new ConeccionHTTP();

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent(); 

			nodoAnterior = nullptr;
			datosPorNodo = gcnew System::Collections::Generic::Dictionary<System::String^, DatosNodo^>();
			
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ LSpace;
	private: System::Windows::Forms::Label^ LStatus;



	private: System::Windows::Forms::Label^ NumNode;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		/// 
		// Estructura para guardar valores por nodo
		System::Collections::Generic::Dictionary<System::String^, DatosNodo^>^ datosPorNodo;

		// Variable para recordar la selecci�n anterior del ComboBox
		System::String^ nodoAnterior;
		
		System::ComponentModel::Container^ components;

		// Funci?n auxiliar para obtener nombre base del archivo
		std::string obtenerNombreBaseLocal(const std::string& rutaPDF) {
			size_t posBarra = rutaPDF.find_last_of("\\/");
			size_t posPunto = rutaPDF.find_last_of(".");
			if (posPunto == std::string::npos || posPunto < posBarra) {
				posPunto = rutaPDF.length();
			}
			return rutaPDF.substr(posBarra + 1, posPunto - posBarra - 1);
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			//this->button1 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->LSpace = (gcnew System::Windows::Forms::Label());
			this->LStatus = (gcnew System::Windows::Forms::Label());
			this->NumNode = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(218, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(25, 16);
			this->label1->TabIndex = 0;
			this->label1->Text = L"IP :";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(206, 46);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(40, 16);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Port : ";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(12, 47);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(145, 21);
			this->comboBox1->TabIndex = 2;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// Insertar opci�n neutral al inicio
			comboBox1->Items->Insert(0, "Seleccione un nodo");
			comboBox1->SelectedIndex = 0;  // Selecci�n inicial en opci�n neutral

			// Agregar nodos reales
			comboBox1->Items->Add("Nodo 1");
			comboBox1->Items->Add("Nodo 2");
			comboBox1->Items->Add("Nodo 3");
			comboBox1->Items->Add("Nodo 4");
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(12, 21);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(77, 16);
			this->label3->TabIndex = 3;
			this->label3->Text = L"Disk Node :";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(259, 22);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(314, 20);
			this->textBox1->TabIndex = 4;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(259, 48);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(314, 20);
			this->textBox2->TabIndex = 5;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::SystemColors::ScrollBar;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(32, 111);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(587, 55);
			this->label4->TabIndex = 7;
			this->label4->Text = L"DISK NODE                       ";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(50, 210);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(97, 29);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Status : ";
			// 
			// LStatus
			// 
			this->LStatus->AutoSize = true;
			this->LStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LStatus->Location = System::Drawing::Point(150, 210);
			this->LStatus->Name = L"LStatus";
			this->LStatus->Size = System::Drawing::Size(133, 29);
			this->LStatus->TabIndex = 10;
			this->LStatus->Text = L"---------------";
			// 
			// NumNode
			// 
			this->NumNode->AutoSize = true;
			this->NumNode->BackColor = System::Drawing::SystemColors::ScrollBar;
			this->NumNode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->NumNode->Location = System::Drawing::Point(338, 111);
			this->NumNode->Name = L"NumNode";
			this->NumNode->Size = System::Drawing::Size(0, 55);
			this->NumNode->TabIndex = 12;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(684, 461);
			this->Controls->Add(this->NumNode);
			this->Controls->Add(this->LStatus);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"Disk Controller";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		}	
	
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		// Obtener la selecci�n actual
		System::String^ nodoActual = comboBox1->SelectedItem->ToString();

		// Guardar datos del nodo anterior solo si no es la opci�n neutral
		if (nodoAnterior != nullptr && nodoAnterior != "Seleccione un nodo") {
			DatosNodo^ nodo = gcnew DatosNodo();
			nodo->ip = textBox1->Text;
			nodo->puerto = textBox2->Text;
			nodo->estado = LStatus->Text;
			datosPorNodo[nodoAnterior] = nodo;
		}

		if (nodoActual == "Seleccione un nodo") {
			// Limpiar los campos si se elige el placeholder
			textBox1->Clear();
			textBox2->Clear();
			LStatus->Text = "---------------";
			nodoAnterior = nodoActual;
			return;
		}
		else {
			// Restaurar datos si ya existen
			if (datosPorNodo->ContainsKey(nodoActual)) {
				DatosNodo^ datos = datosPorNodo[nodoActual];
				textBox1->Text = datos->ip;
				textBox2->Text = datos->puerto;
				LStatus->Text = datos->estado;
			}
			else {
				// Limpiar si es nuevo nodo
				textBox1->Clear();
				textBox2->Clear();
				LStatus->Text = "---------------";
			}
			// Obtener IP y puerto para enviar estado
			System::String^ ipStr = textBox1->Text;
			System::String^ puertoStr = textBox2->Text;
			if (String::IsNullOrEmpty(ipStr) || String::IsNullOrEmpty(puertoStr)) {
				LStatus->Text = "Faltan datos.";
				nodoAnterior = nodoActual;
				return;
			}

			// Validar puerto
			int puerto = 0;
			if (!Int32::TryParse(puertoStr, puerto)) {
				LStatus->Text = "Puerto inv�lido.";
				nodoAnterior = nodoActual;
				return;
			}
			// Enviar solicitud al nodo seleccionado para obtener estado
			try {
				std::string ip = msclr::interop::marshal_as<std::string>(ipStr);
				ConeccionHTTP* conn = new ConeccionHTTP();
				std::string respuesta = conn->sendStatusToNode(ip, puerto);
				delete conn;

				nlohmann::json jsonResp = nlohmann::json::parse(respuesta);
				std::string estadoTexto = jsonResp.contains("texto_formateado") ? jsonResp["texto_formateado"] : jsonResp.dump(2);

				System::String^ textoEstado = gcnew System::String(estadoTexto.c_str());
				LStatus->Text = textoEstado;
				LStatus->Refresh(); // <-- Asegura la actualizaci�n inmediata

				if (!datosPorNodo->ContainsKey(nodoActual))
					datosPorNodo[nodoActual] = gcnew DatosNodo();

				datosPorNodo[nodoActual]->ip = ipStr;
				datosPorNodo[nodoActual]->puerto = puertoStr;
				datosPorNodo[nodoActual]->estado = textoEstado;
			}
			catch (const std::exception& ex) {
				LStatus->Text = gcnew System::String(("Error: " + std::string(ex.what())).c_str());
				LStatus->Refresh();
			}

			nodoAnterior = nodoActual;
		}
	}
};
}
