#pragma once
#include "PDF_Reader_Compresion.h"
#include "json.hpp"
#include "RaidManager.h"
#include "RAID5.h" 

using json = nlohmann::json;

class ConeccionHTTP {
public:
	void run();
	void CargarArchivo(const json& data);
	void odtenerdocu(const json& data);
	void eliminardocu(const json& data);
	void connectDiskNode(int Nodeport, const std::string& Nodeip);
};
