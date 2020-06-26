#pragma once

#include <fstream>
#include <map>

// Take class fields and generate set and get methods
// write to file

class ClassGen {
public:
	explicit ClassGen(const char* ifile, const std::string clsName);
	void getSet();
	void error(const char* p, const char* p2 = "");
protected:
	void populateTypeConvMap();
private:
	const char* mInFile;
	std::string mClassName;
	std::map<std::string, std::string> typeConv;
};
