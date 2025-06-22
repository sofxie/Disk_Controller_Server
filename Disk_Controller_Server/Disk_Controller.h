	#pragma once
#include "PDF_Reader_Compresion.h"
#include <msclr/marshal_cppstd.h>

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
		private: System::Windows::Forms::Button^ button2;
		private: System::Windows::Forms::Button^ button3;

		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container^ components;

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
				this->button2 = (gcnew System::Windows::Forms::Button());
				this->button3 = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				this->label1->Location = System::Drawing::Point(245, 28);
				this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(80, 20);
				this->label1->TabIndex = 0;
				this->label1->Text = L"IP / Port :";
				// 
				// label2
				// 
				this->label2->AutoSize = true;
				this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				this->label2->Location = System::Drawing::Point(267, 57);
				this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
				this->label2->Name = L"label2";
				this->label2->Size = System::Drawing::Size(58, 20);
				this->label2->TabIndex = 1;
				this->label2->Text = L"Path : ";
				// 
				// comboBox1
				// 
				this->comboBox1->FormattingEnabled = true;
				this->comboBox1->Location = System::Drawing::Point(16, 58);
				this->comboBox1->Margin = System::Windows::Forms::Padding(4);
				this->comboBox1->Name = L"comboBox1";
				this->comboBox1->Size = System::Drawing::Size(192, 24);
				this->comboBox1->TabIndex = 2;
				// 
				// label3
				// 
				this->label3->AutoSize = true;
				this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				this->label3->Location = System::Drawing::Point(16, 26);
				this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
				this->label3->Name = L"label3";
				this->label3->Size = System::Drawing::Size(97, 20);
				this->label3->TabIndex = 3;
				this->label3->Text = L"Disk Node :";
				// 
				// textBox1
				// 
				this->textBox1->Location = System::Drawing::Point(345, 27);
				this->textBox1->Margin = System::Windows::Forms::Padding(4);
				this->textBox1->Name = L"textBox1";
				this->textBox1->Size = System::Drawing::Size(417, 22);
				this->textBox1->TabIndex = 4;
				this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
				// 
				// textBox2
				// 
				this->textBox2->Location = System::Drawing::Point(345, 59);
				this->textBox2->Margin = System::Windows::Forms::Padding(4);
				this->textBox2->Name = L"textBox2";
				this->textBox2->Size = System::Drawing::Size(417, 22);
				this->textBox2->TabIndex = 5;
				// 
				// button1
				// 
				this->button1->Location = System::Drawing::Point(796, 48);
				this->button1->Margin = System::Windows::Forms::Padding(4);
				this->button1->Name = L"button1";
				this->button1->Size = System::Drawing::Size(100, 28);
				this->button1->TabIndex = 6;
				this->button1->Text = L"Connect";
				this->button1->UseVisualStyleBackColor = true;
				// 
				// button2
				// 
				this->button2->Location = System::Drawing::Point(796, 176);
				this->button2->Margin = System::Windows::Forms::Padding(4);
				this->button2->Name = L"button2";
				this->button2->Size = System::Drawing::Size(100, 28);
				this->button2->TabIndex = 7;
				this->button2->Text = L"Add";
				this->button2->UseVisualStyleBackColor = true;
				this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
				// 
				// button3
				// 
				this->button3->Location = System::Drawing::Point(796, 112);
				this->button3->Margin = System::Windows::Forms::Padding(4);
				this->button3->Name = L"button3";
				this->button3->Size = System::Drawing::Size(100, 28);
				this->button3->TabIndex = 8;
				this->button3->Text = L"Delete";
				this->button3->UseVisualStyleBackColor = true;
				this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
				// 
				// MyForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(912, 567);
				this->Controls->Add(this->button3);
				this->Controls->Add(this->button2);
				this->Controls->Add(this->button1);
				this->Controls->Add(this->textBox2);
				this->Controls->Add(this->textBox1);
				this->Controls->Add(this->label3);
				this->Controls->Add(this->comboBox1);
				this->Controls->Add(this->label2);
				this->Controls->Add(this->label1);
				this->Margin = System::Windows::Forms::Padding(4);
				this->Name = L"MyForm";
				this->Text = L"Disk Controller";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		}
		private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
			using namespace msclr::interop;

			String^ texto = textBox2->Text;
			PDF_Reader_Compresion lector;

			// Convertir managed String^ a std::string 
			// Aqui debe ir la direccion que manda sofi del pdf
			std::string rutaPDFOriginal = marshal_as<std::string>(texto);

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

			// Descomprimir y mostrar resultados
			lector.DecomprimirFile(resultado, rutaBase, rutaPDF);
		}
		

private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ texto = textBox2->Text;
	PDF_Reader_Compresion lector;

	// Ruta del PDF que quieres procesar
	std::string rutaPDF = msclr::interop::marshal_as<std::string>(texto);

	// Ruta base donde guardar archivos temporales y comprimidos
	std::string rutaBase = "C:\\PDFR\\";

	lector.eliminarArchivosGenerados(rutaPDF, rutaBase);
}
};
}
