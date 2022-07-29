#include "Global.hpp"

TEST_CASE("Ticker Lists")
{
    auto api = getAPIObject();
    SECTION("Delisted")
    {
        auto results = api.getDelistedCompanyList();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 100);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("delistedDate"));

        results = api.getDelistedCompanyList(30);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 30);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("delistedDate"));
    }

    SECTION("S&P 500")
    {
        auto results = api.getSP500Constituents();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 500);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dateFirstAdded"));
        REQUIRE(results[0].HasMember("subSector"));
        REQUIRE(results[0].HasMember("headQuarter"));
        REQUIRE(results[0].HasMember("cik"));

        results = api.getSP500Constituents(true);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 500);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dateAdded"));
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("reason"));
        REQUIRE(results[0].HasMember("removedSecurity"));
    }

    SECTION("NASDAQ 100")
    {
        auto results = api.getNasdaq100Constituents();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 90);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dateFirstAdded"));
        REQUIRE(results[0].HasMember("subSector"));
        REQUIRE(results[0].HasMember("headQuarter"));
        REQUIRE(results[0].HasMember("cik"));

        results = api.getNasdaq100Constituents(true);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 90);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dateAdded"));
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("reason"));
        REQUIRE(results[0].HasMember("removedSecurity"));
    }

    SECTION("DJIA")
    {
        auto results = api.getDJIAConstituents();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 30);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dateFirstAdded"));
        REQUIRE(results[0].HasMember("subSector"));
        REQUIRE(results[0].HasMember("headQuarter"));
        REQUIRE(results[0].HasMember("cik"));

        results = api.getDJIAConstituents(true);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 30);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dateAdded"));
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("reason"));
        REQUIRE(results[0].HasMember("removedSecurity"));
    }

    SECTION("Symbol List")
    {
        auto results = api.getSymbolList();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("name"));
        REQUIRE(results[0].HasMember("price"));
        REQUIRE(results[0].HasMember("exchange"));
        REQUIRE(results[0].HasMember("type"));
    }

    SECTION("ETF List")
    {
        auto results = api.getETFList();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("name"));
        REQUIRE(results[0].HasMember("price"));
        REQUIRE(results[0].HasMember("exchange"));
        REQUIRE(results[0].HasMember("exchangeShortName"));
    }

    SECTION("Tradeable Symbols List")
    {
        auto results = api.getTradableSymbolsList();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("name"));
        REQUIRE(results[0].HasMember("price"));
        REQUIRE(results[0].HasMember("exchange"));
        REQUIRE(results[0].HasMember("exchangeShortName"));
    }
}

TEST_CASE("Misc")
{
    auto api = getAPIObject();
    SECTION("Trading Hours")
    {
        auto results = api.getTradingHours();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() >= 0);
        REQUIRE(results[0].HasMember("stockExchangeName"));
        REQUIRE(results[0].HasMember("stockMarketHours"));
        REQUIRE(results[0].HasMember("stockMarketHolidays"));
    }


}

TEST_CASE("Commitment of Traders")
{
    auto api = getAPIObject();
    SECTION("COT List")
    {
        auto results = api.getCOTSymbolsList();
        REQUIRE(results.IsArray());
        REQUIRE(results[0].HasMember("trading_symbol"));
    }
}

TEST_CASE("13F")
{
    auto api = getAPIObject();
}