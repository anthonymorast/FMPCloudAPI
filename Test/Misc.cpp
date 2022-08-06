#include "Global.hpp"

auto api = getAPIObject();
TEST_CASE("Batch EOD Prices")
{
    SECTION("ticker list and date")
    {
        auto results = api.getBatchEODPrices({"AAPL", "INTC", "AMD"}, "2021-06-10");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 3);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("close"));
    }
}

TEST_CASE("Stock Market Performers")
{
    SECTION("gainers")
    {
        auto gainers = api.getStockMarketPerformers(GAINERS);
        REQUIRE(gainers.IsArray());
        REQUIRE(gainers.Size() > 0);
        REQUIRE(gainers[0].HasMember("ticker"));
        REQUIRE(gainers[0].HasMember("changes"));
    }

    SECTION("losers")
    {
        auto losers = api.getStockMarketPerformers(LOSERS);
        REQUIRE(losers.IsArray());
        REQUIRE(losers.Size() > 0);
        REQUIRE(losers[0].HasMember("ticker"));
        REQUIRE(losers[0].HasMember("changes"));
    }

    SECTION("actives")
    {
        auto actives = api.getStockMarketPerformers(ACTIVE);
        REQUIRE(actives.IsArray());
        REQUIRE(actives.Size() > 0);
        REQUIRE(actives[0].HasMember("ticker"));
        REQUIRE(actives[0].HasMember("changes"));
    }
}

TEST_CASE("Sector Performance")
{
    SECTION("Sector Performance")
    {
        auto performance = api.getSectorPerformance();
        REQUIRE(performance.IsArray());
        REQUIRE(performance.Size() == 15);
        REQUIRE(performance[0].HasMember("changesPercentage"));
        REQUIRE(performance[0].HasMember("sector"));
    }

    SECTION("Historical Sector Performance")
    {
        auto performance = api.getSectorPerformance(true, 10);
        REQUIRE(performance.IsArray());
        REQUIRE(performance.Size() == 10);
        REQUIRE(performance[0].HasMember("date"));
        REQUIRE(performance[0].HasMember("utilitiesChangesPercentage"));
        REQUIRE(performance[0].HasMember("financialChangesPercentage"));
        REQUIRE(performance[0].HasMember("industrialsChangesPercentage"));
        REQUIRE(performance[0].HasMember("technologyChangesPercentage"));
    }
}

TEST_CASE("technical indicators")
{
    SECTION("Daily - EMA")
    {
        auto results = api.getTechnicalIndicatorData("AAPL", DAILY, EMA, 7);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0  );
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("ema"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));
    }

    SECTION("5Min - SMA")
    {
        auto results = api.getTechnicalIndicatorData("INTC", FIVE_MINUTE, SMA, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("sma"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));
    }

    SECTION("4Hour - STDDEV")
    {
        auto results = api.getTechnicalIndicatorData("AAPL", FOUR_HOUR, STDDEV, 5);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("standardDeviation"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));
    }
}

TEST_CASE("SIC")
{
    SECTION("Specific SIC")
    {
        auto symbolResults = api.getSpecificStandardIndustrialClassification("", "AAPL", "");
        REQUIRE(symbolResults.IsArray());
        REQUIRE(symbolResults.Size() == 1);
        REQUIRE(symbolResults[0].HasMember("symbol"));
        REQUIRE(std::strcmp(symbolResults[0]["symbol"].GetString(), "AAPL") == 0);
        REQUIRE(symbolResults[0].HasMember("cik"));
        REQUIRE(std::strcmp(symbolResults[0]["cik"].GetString(), "0000320193") == 0);
        REQUIRE(symbolResults[0].HasMember("sicCode"));
        REQUIRE(std::strcmp(symbolResults[0]["sicCode"].GetString(), "3571") == 0);

        auto cikResults = api.getSpecificStandardIndustrialClassification("0000320193");
        REQUIRE(cikResults.IsArray());
        REQUIRE(cikResults.Size() == 1);
        REQUIRE(cikResults[0].HasMember("symbol"));
        REQUIRE(std::strcmp(cikResults[0]["symbol"].GetString(), "AAPL") == 0);
        REQUIRE(cikResults[0].HasMember("cik"));
        REQUIRE(std::strcmp(cikResults[0]["cik"].GetString(), "0000320193") == 0);
        REQUIRE(cikResults[0].HasMember("sicCode"));
        REQUIRE(std::strcmp(cikResults[0]["sicCode"].GetString(), "3571") == 0);

        auto sicResults = api.getSpecificStandardIndustrialClassification("", "", "3571");
        REQUIRE(sicResults.IsArray());
        REQUIRE(sicResults.Size() == 10);
        REQUIRE(sicResults[0].HasMember("symbol"));
        REQUIRE(std::strcmp(sicResults[0]["symbol"].GetString(), "AAPL") == 0);
        REQUIRE(sicResults[0].HasMember("cik"));
        REQUIRE(std::strcmp(sicResults[0]["cik"].GetString(), "0000320193") == 0);
        REQUIRE(sicResults[0].HasMember("sicCode"));
        REQUIRE(std::strcmp(sicResults[0]["sicCode"].GetString(), "3571") == 0);
    }

    SECTION("All SIC")
    {
        auto sicResults = api.getAllStandardIndustrialClassification();
        REQUIRE(sicResults.IsArray());
        REQUIRE(sicResults.Size() > 0);
        REQUIRE(sicResults[0].HasMember("symbol"));
        REQUIRE(sicResults[0].HasMember("cik"));
        REQUIRE(sicResults[0].HasMember("sicCode"));
    }

    SECTION("SIC List")
    {
        auto sicResults = api.getAllStandardIndustrialClassificationList();
        REQUIRE(sicResults.IsArray());
        REQUIRE(sicResults.Size() > 0);
        REQUIRE(sicResults[0].HasMember("office"));
        REQUIRE(sicResults[0].HasMember("sicCode"));
        REQUIRE(sicResults[0].HasMember("industryTitle"));
    }

    SECTION("SIC Specific List")
    {
        auto sicResults = api.getSpecificStandardIndustrialClassificationList("", "700");
        REQUIRE(sicResults.IsArray());
        REQUIRE(sicResults.Size() > 0);
        REQUIRE(sicResults[0].HasMember("office"));
        REQUIRE(sicResults[0].HasMember("sicCode"));
        REQUIRE(std::strcmp(sicResults[0]["sicCode"].GetString(), "700") == 0);
        REQUIRE(sicResults[0].HasMember("industryTitle"));

        sicResults = api.getSpecificStandardIndustrialClassificationList("AGRICULTURAL");
        REQUIRE(sicResults.IsArray());
        REQUIRE(sicResults.Size() > 0);
        REQUIRE(sicResults[0].HasMember("office"));
        REQUIRE(sicResults[0].HasMember("sicCode"));
        REQUIRE(sicResults[0].HasMember("industryTitle"));
    }
}