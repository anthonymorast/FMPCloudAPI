#include "Global.hpp"

TEST_CASE("Earnings Calendar")
{
	auto api = getAPIObject();
    SECTION("Get Calendar")
    {
        auto results = api.getEarningsCalendar();
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("revenueEstimated"));

    }
    
    SECTION("Get Historical Calendar")
    {
        // limit = 80 (default)
        auto results = api.getHistoricalEarningsCalendar("AAPL");
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() == 80);
        REQUIRE(results[0].HasMember("revenueEstimated"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "AAPL");

        // limit = 1
        auto limitedResults = api.getHistoricalEarningsCalendar("AMD", 1);
		REQUIRE(limitedResults.IsArray());
		REQUIRE(limitedResults.Size() == 1);
        REQUIRE(limitedResults[0].HasMember("revenueEstimated"));
        REQUIRE(limitedResults[0].HasMember("symbol"));
        REQUIRE(limitedResults[0]["symbol"] == "AMD");

         // limit = 100
        limitedResults = api.getHistoricalEarningsCalendar("JNJ", 100);
		REQUIRE(limitedResults.IsArray());
		REQUIRE(limitedResults.Size() == 100);
        REQUIRE(limitedResults[0].HasMember("revenueEstimated"));
        REQUIRE(limitedResults[0].HasMember("symbol"));
        REQUIRE(limitedResults[0]["symbol"] == "JNJ");
    }
}

TEST_CASE("IPO Calendar")
{
	auto api = getAPIObject();
    SECTION("Get Calendar")
    {
        auto results = api.getIPOCalendar("2022-01-01", "2022-02-28");
		REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("company"));
        REQUIRE(results[0].HasMember("symbol"));
    }
}

TEST_CASE("Stock Split Calendar")
{
	auto api = getAPIObject();
    SECTION("Get Calendar")
    {
        auto results = api.getStockSplitCalendar("2022-01-01", "2022-02-28");
        REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("numerator"));
        REQUIRE(results[0].HasMember("denominator"));
    }
}

TEST_CASE("Dividend Calendar")
{
    auto api = getAPIObject();
    SECTION("Get Calendar")
    {
        auto results = api.getDividendCalendar("2022-01-01", "2022-02-28");
        REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("adjDividend"));
        REQUIRE(results[0].HasMember("paymentDate"));
        REQUIRE(results[0].HasMember("recordDate"));
    }
}


TEST_CASE("Economic Calendar")
{
    auto api = getAPIObject();
    SECTION("Get Calendar")
    {
        auto results = api.getEconomicCalendar("2022-01-01", "2022-02-28");
        REQUIRE(results.IsArray());
		REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("event"));
        REQUIRE(results[0].HasMember("country"));
        REQUIRE(results[0].HasMember("change"));
        REQUIRE(results[0].HasMember("actual"));
        REQUIRE(results[0].HasMember("estimate"));
    }
}