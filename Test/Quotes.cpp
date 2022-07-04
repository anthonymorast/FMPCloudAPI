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

TEST_CASE("Stock Quotes")
{
	auto api = getAPIObject();
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
		// api.printDocument(results, std::cout);
		REQUIRE(results.IsArray() == true);
		REQUIRE(results.Size() == 4);
	}
}

TEST_CASE("Crypto Quotes")
{
	auto api = getAPIObject();
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
	auto api = getAPIObject();
	SECTION("Available FOREX")
	{
		Document results = api.getAvailableForex();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));
		REQUIRE(results[0].HasMember("currency"));
	}

	SECTION("All FOREX")
	{
		Document results = api.getAllForexPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));
		REQUIRE(results[0].HasMember("change"));
	}

	SECTION("All FOREX Tick")
	{
		Document results = api.getAllForexTick();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);	
		REQUIRE(results[0].HasMember("ticker"));
		REQUIRE(results[0].HasMember("bid"));
		REQUIRE(results[0].HasMember("ask"));
		REQUIRE(results[0].HasMember("open"));
		REQUIRE(results[0].HasMember("low"));
		REQUIRE(results[0].HasMember("high"));
	}
}

TEST_CASE("Other Availability and Quotes")
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

	SECTION("ETF")
	{

	}

	SECTION("Commodities")
	{

	}

	SECTION("Euronext")
	{

	}
	
	SECTION("NYSE")
	{

	}

	SECTION("AMEX")
	{

	}


	SECTION("TSX")
	{

	}

	SECTION("Market Indicies")
	{

	}


	SECTION("Mutual Funds")
	{

	}

	SECTION("Nasdaq")
	{

	}
}