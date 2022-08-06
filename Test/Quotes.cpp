#include "Global.hpp"

auto api = getAPIObject();
TEST_CASE("Stock Quotes")
{
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
	SECTION("ETF")
	{
		auto results = api.getAvailableEtf();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllETFPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);

	}

	SECTION("Commodities")
	{
		auto results = api.getCommoditiesAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllCommoditiesPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}

	SECTION("Euronext")
	{
		auto results = api.getEuronextAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllEuronextPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}
	
	SECTION("NYSE")
	{
		auto results = api.getNYSEAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllNYSEPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}

	SECTION("AMEX")
	{
		auto results = api.getAMEXAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllAMEXPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}


	SECTION("TSX")
	{
		auto results = api.getTSXAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllTSXPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}

	SECTION("Market Indicies")
	{
		auto results = api.getMarketIndexesAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllMarketIndexesPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}


	SECTION("Mutual Funds")
	{
		auto results = api.getMutualFundsAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllMutualFundsPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}

	SECTION("Nasdaq")
	{
		auto results = api.getNasdaqAvailable();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
		REQUIRE(results[0].HasMember("symbol"));
		REQUIRE(results[0].HasMember("name"));

		auto quotes = api.getAllNasdaqPrices();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
	}
}