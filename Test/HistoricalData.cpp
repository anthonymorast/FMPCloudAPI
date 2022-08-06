#include "Global.hpp"

auto api = getAPIObject();
TEST_CASE("Historical Chart")
{
    SECTION("Stocks")
    {
        auto results = api.getHistoricalChart("INTC", ONE_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("INTC", FIVE_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("AMD", FIFTEEN_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("AMD", THIRTY_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("AAPL", ONE_HOUR);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("AMD", FOUR_HOUR);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));
    }

    SECTION("Forex")
    {
        auto results = api.getHistoricalChart("EURUSD", ONE_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("EURUSD", FIVE_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("EURUSD", FIFTEEN_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("EURUSD", THIRTY_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("GBPUSD", ONE_HOUR);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("EURUSD", FOUR_HOUR);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));
    }

    SECTION("Crypto")
    {
        auto results = api.getHistoricalChart("BTCUSD", ONE_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("BTCUSD", FIVE_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("BTCUSD", FIFTEEN_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("BTCUSD", THIRTY_MINUTE);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("BTCUSD", ONE_HOUR);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));

        results = api.getHistoricalChart("BTCUSD", FOUR_HOUR);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("open"));
        REQUIRE(results[0].HasMember("low"));
        REQUIRE(results[0].HasMember("high"));
        REQUIRE(results[0].HasMember("close"));
        REQUIRE(results[0].HasMember("volume"));
    }
}

TEST_CASE("Daily Line")
{
    SECTION("Stocks")
    {
        auto results = api.getDailyLine("PARA");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
    }

    SECTION("Forex")
    {
        auto results = api.getDailyLine("EURUSD");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
    }

    SECTION("Crypto")
    {
        auto results = api.getDailyLine("BTCUSD");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
    }
}

TEST_CASE("Daily Data by Period")
{
    SECTION("Stocks")
    {
        auto results = api.getDailyDataByPeriod("QCOM", "2021-01-01", "2021-12-31");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
    }

    SECTION("Forex")
    {
        auto results = api.getDailyDataByPeriod("GBPUSD", "2021-01-01", "2021-12-31");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
    }

    SECTION("Crypto")
    {
        auto results = api.getDailyDataByPeriod("BTCUSD", "2021-01-01", "2021-12-31");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
    }
}

TEST_CASE("Daily Data by X days")
{
    SECTION("Stocks")
    {
        auto results = api.getDailyDataXDays("AAPL", 10);
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() == 10);
        REQUIRE(results["historical"][0].HasMember("date"));

        results = api.getDailyDataXDays("INTC", 50);
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() == 50);
        REQUIRE(results["historical"][0].HasMember("date"));
    }

    SECTION("Forex")
    {
        auto results = api.getDailyDataXDays("EURUSD", 10);
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() == 10);
        REQUIRE(results["historical"][0].HasMember("date"));

        results = api.getDailyDataXDays("GBPUSD", 50);
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() == 50);
        REQUIRE(results["historical"][0].HasMember("date"));
    }

    SECTION("Crypto")
    {
        auto results = api.getDailyDataXDays("BTCUSD", 10);
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() == 10);
        REQUIRE(results["historical"][0].HasMember("date"));

        results = api.getDailyDataXDays("BTCUSD", 50);
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() == 50);
        REQUIRE(results["historical"][0].HasMember("date"));
    }
}

TEST_CASE("Splits and Dividends")
{
    SECTION("Stock Splits")
    {
        auto results = api.getDailyStockSplit("AAPL");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
        REQUIRE(results["historical"][0].HasMember("numerator"));
        REQUIRE(results["historical"][0].HasMember("denominator"));
    }

    SECTION("Stock Dividends")
    {
        auto results = api.getDailyStockDividend("TGT");
        REQUIRE(results.HasMember("symbol"));
        REQUIRE(results.HasMember("historical"));
        REQUIRE(results["historical"].IsArray());
        REQUIRE(results["historical"].Size() > 0);
        REQUIRE(results["historical"][0].HasMember("date"));
        REQUIRE(results["historical"][0].HasMember("dividend"));
        REQUIRE(results["historical"][0].HasMember("recordDate"));
        REQUIRE(results["historical"][0].HasMember("paymentDate"));
        REQUIRE(results["historical"][0].HasMember("declarationDate"));
    }
}



