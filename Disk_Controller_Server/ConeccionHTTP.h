#pragma once
#include <string>

class ConeccionHTTP {
public:
	void run();
	void connectDiskNode(int Nodeport, const std::string& Nodeip);
};
