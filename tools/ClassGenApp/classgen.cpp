#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <iterator>

#include "classgen.h"

ClassGen::ClassGen(const char* ifile, const std::string clsName)
	:mInFile{ifile}
	,mClassName{clsName}
{
	populateTypeConvMap();
}

void ClassGen::populateTypeConvMap()
{
	typeConv["IntegerField"] = "int";
	typeConv["DecimalField"] = "double";
	typeConv["StringField"]  = "std::string";
	typeConv["IntegerField"] = "int";
}

void ClassGen::error(const char* p, const char* p2)
{
	std::cerr << p << ' ' << p2 << '\n';
	std::exit(1);
}

void ClassGen::getSet() {
	// open file 
	std::ifstream from(mInFile);
	if (!from) error("Cannot open file :", mInFile);

	std::map<std::string, std::vector<std::string>> declMap;
	std::map<std::string, std::vector<std::string>> defMap;

	std::string line;
	while (std::getline(from, line))
	{
		
		std::string type, fldName, tmpVar, getVar, setVar, getName, setName;
		std::string getBody, setBody;
		std::string getDef, setDef;
		std::string tmpType;

		std::istringstream iss(line);

		iss >> type >> fldName;

		tmpVar = fldName;
		tmpType = type;
		tmpType.replace(tmpType.find("*"), 1, "");

		tmpVar.erase(0, 1);
		tmpVar.replace(tmpVar.find(";"), 1, "");

		getVar = tmpVar;
		setVar = tmpVar;

		getVar[0] = tolower(getVar[0]);

		getBody = getVar+"() const";
		getName = type + " " + getBody;

		setBody = setVar + "(" + typeConv[tmpType] + " val)";
		setName = "void set" + setBody;

		getDef = type + " " + mClassName + "::" + getBody + '\n' + "{"
			+ '\n' + '\t' + "return " + fldName + '\n' + "}";

		setDef = "void " + mClassName + "::set" + setBody + '\n' + "{"
			+ '\n' + '\t' + fldName.replace(fldName.find(";"), 1, "")+"->setValue( val );" + '\n' + "}";


		declMap[fldName].push_back(getName+";");
		declMap[fldName].push_back(setName+";");

		defMap[fldName].push_back(getDef);
		defMap[fldName].push_back(setDef);

	} // while

	std::ofstream outf("decl.fld", std::ostream::trunc);
	for (auto[k, v] : declMap) {
		std::cout << k << '\n';
		for (auto func : v) {
			outf << func << '\n';
		}
	}

	std::ofstream outdef("def.fld", std::ostream::trunc);
	for (auto[k, v] : declMap) {
		for (auto def : defMap[k]) {
		    outdef << def << '\n';
		}
	}

}