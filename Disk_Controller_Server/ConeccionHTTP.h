#pragma once
#include "json.hpp"
#include "PDF_Reader_Compresion.h"
using json = nlohmann::json;

class ConeccionHTTP {
public:
	void run();
	void CargarArchivo(const json& data);
	void obtenerdocu(const json& data);
	void eliminardocu(const json& data);

};
