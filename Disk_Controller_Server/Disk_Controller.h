	#pragma once

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
			System::ComponentModel::Container ^components;

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
				this->label1->Location = System::Drawing::Point(184, 23);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(59, 16);
				this->label1->TabIndex = 0;
				this->label1->Text = L"IP / Port :";
				// 
				// label2
				// 
				this->label2->AutoSize = true;
				this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				this->label2->Location = System::Drawing::Point(200, 46);
				this->label2->Name = L"label2";
				this->label2->Size = System::Drawing::Size(43, 16);
				this->label2->TabIndex = 1;
				this->label2->Text = L"Path : ";
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
				// 
				// button2
				// 
				this->button2->Location = System::Drawing::Point(597, 143);
				this->button2->Name = L"button2";
				this->button2->Size = System::Drawing::Size(75, 23);
				this->button2->TabIndex = 7;
				this->button2->Text = L"Add";
				this->button2->UseVisualStyleBackColor = true;
				// 
				// button3
				// 
				this->button3->Location = System::Drawing::Point(597, 91);
				this->button3->Name = L"button3";
				this->button3->Size = System::Drawing::Size(75, 23);
				this->button3->TabIndex = 8;
				this->button3->Text = L"Delete";
				this->button3->UseVisualStyleBackColor = true;
				// 
				// MyForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(684, 461);
				this->Controls->Add(this->button3);
				this->Controls->Add(this->button2);
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
	};
}
