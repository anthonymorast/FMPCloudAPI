#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch2.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "../API/FMPCloud.hpp"
#include "../API/rapidjson/document.h"

using namespace rapidjson;
using namespace FMP;

TEST_CASE("Stock Quotes")
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

	SECTION("Fetch Single Stock")
	{
		Document results = api.getStockQuote("AAPL");
		REQUIRE(results.IsArray() == true);
		REQUIRE(results.Size() == 1);
		REQUIRE(strcmp(results[0]["symbol"].GetString(), "AAPL") == 0);
	}

	SECTION("Fetch Batch Stocks")
	{
		Document results = api.getStockQuote({"AAPL", "AMD", "INTC", "TSLA"});
		REQUIRE(results.IsArray() == true);
		REQUIRE(results.Size() == 4);
		REQUIRE(strcmp(results[0]["symbol"].GetString(), "AAPL") == 0);
		REQUIRE(strcmp(results[1]["symbol"].GetString(), "AMD") == 0);
		REQUIRE(strcmp(results[2]["symbol"].GetString(), "INTC") == 0);
		REQUIRE(strcmp(results[3]["symbol"].GetString(), "TSLA") == 0);
	}
}

TEST_CASE("Crypto Quotes")
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

	SECTION("Available Crypto")
	{
		Document results = api.getAvailableCrypto();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("currency"));
		REQUIRE(results[0].HasMember("stockExchange"));
		// verified cached output statement (statement later removed)
		Document results2 = api.getAvailableCrypto();
		REQUIRE(results.Size() > 0);
	}

	SECTION("Fetch Crypto")
	{
		Document results = api.getCryptoPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("price"));
		REQUIRE(results[0].HasMember("changesPercentage"));
		REQUIRE(results[0].HasMember("dayLow"));
		REQUIRE(results[0].HasMember("dayHigh"));
		REQUIRE(results[0].HasMember("timestamp"));
	}
}

TEST_CASE("FOREX Quotes")
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

	SECTION("Available FOREX")
	{

	}

	SECTION("All FOREX")
	{

	}
}