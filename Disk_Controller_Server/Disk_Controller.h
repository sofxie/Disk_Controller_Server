#pragma once
#include <string>
#include <iostream>
#include "ConeccionHTTP.h"
#include "PDF_Reader_Compresion.h"
#include "RAID5.h" 
#include <msclr/marshal_cppstd.h>
#include "RaidManager.h"
using namespace std;

namespace DiskControllerServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();

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



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
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
			this->button1 = (gcnew System::Windows::Forms::Button());
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
			// button1
			// 
			this->button1->Location = System::Drawing::Point(597, 39);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 6;
			this->button1->Text = L"Connect";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(684, 461);
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

	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		using namespace msclr::interop;
		String^ texto = textBox2->Text;
		PDF_Reader_Compresion lector;

		// Convertir managed String^ a std::string 
		// Aqui debe ir la direccion que manda sofi del pdf
		std::string rutaPDFOriginal = "C:\\Users\\sofia\\Downloads\\Pruebba.pdf";

		// Carpeta base fija
		std::string rutaBase = "C:\\PDFR\\";

		std::string RutaPDFNueva = rutaBase + lector.obtenerNombreBaseConTipo(rutaPDFOriginal);

		// Copiar PDF a carpeta fija manteniendo nombre original
		std::string rutaPDF = lector.copiarArchivo(rutaPDFOriginal, RutaPDFNueva);

		if (rutaPDF.empty()) {
			std::string mensaje = rutaPDFOriginal;
			System::String^ msg = gcnew System::String(mensaje.c_str());
			MessageBox::Show(msg, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		// Procesar PDF y comprimir Huffman
		std::string resultado = lector.procesarPDFyGuardarHuffman(rutaPDF, rutaBase);

		if (resultado.empty()) {
			MessageBox::Show("Error: No se pudo procesar el PDF y comprimir.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		// Opcional: mostrar resultado parcial en consola
		std::cout << "Proceso completado. Resultado (inicio):\n" << resultado.substr(0, 50) << "...\n";


		// ========== Aplicar el RAID 5  ==========
		// Extraer solo el texto codificado (parte despu?s del '|')
		size_t separador = resultado.find('|');
		std::string textoCodificado = "";
		if (separador != std::string::npos) {
			textoCodificado = resultado.substr(separador + 1);
		}

		if (!textoCodificado.empty()) {
			// Variables para almacenar los bloques finales
			std::string bloque1, bloque2, bloque3, bloque4;

			// Crear instancia de RAID5 y ejecutar el algoritmo
			RAID5 raid5;
			raid5.raid5Algorithm(textoCodificado, bloque1, bloque2, bloque3, bloque4);

			// Obtener nombre base del PDF para los archivos RAID5
			std::string nombrePDF = obtenerNombreBaseLocal(rutaPDF);

			// Guardar los bloques en archivos separados
			std::ofstream archivo1(rutaBase + "RAID5_" + nombrePDF + "_Bloque1.txt");
			std::ofstream archivo2(rutaBase + "RAID5_" + nombrePDF + "_Bloque2.txt");
			std::ofstream archivo3(rutaBase + "RAID5_" + nombrePDF + "_Bloque3.txt");
			std::ofstream archivo4(rutaBase + "RAID5_" + nombrePDF + "_Bloque4.txt");

			if (archivo1 && archivo2 && archivo3 && archivo4) {
				archivo1 << bloque1;
				archivo2 << bloque2;
				archivo3 << bloque3;
				archivo4 << bloque4;

				archivo1.close();
				archivo2.close();
				archivo3.close();
				archivo4.close();

				enviarBloquesGeneradosARaidNodes(bloque1, bloque2, bloque3, bloque4, nombrePDF);

				std::cout << "Se logro almacenar los bloques con raid 5" << std::endl;
			}
			else {
				std::cerr << "Error: No se pudieron crear los archivos del raid" << std::endl;
			}
		}
		else {
			std::cerr << "Error: No hay texto codificado para aplicar raid." << std::endl;
		}

		// Descomprimir el texto codificado y mostrar resultados
		lector.DecomprimirFile(resultado, rutaBase, rutaPDF);
	}
	};
}