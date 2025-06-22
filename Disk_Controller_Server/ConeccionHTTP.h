#pragma once

class ConeccionHTTP {
public:
	void run();
	void CargarArchivo(const json& data);
	void obtenerdocu(const json& data);
	void eliminardocu(const json& data);

};
