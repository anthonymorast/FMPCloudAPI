#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch2.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../API/FMPCloud.hpp"
#include "../API/rapidjson/document.h"


using namespace rapidjson;
using namespace FMP;

FMPCloudAPI getAPIObject()
{
	std::ifstream file("../key.json");
	if(!file)
		throw FMPCloudAPIError("Error opening API key file: '../key.json' not found.");
	std::stringstream jsonContents;
	jsonContents << file.rdbuf();
	file.close();
	Document key;
	key.Parse(jsonContents.str().c_str());
	FMPCloudAPI api(key["APIKey"].GetString());
	return api;
}