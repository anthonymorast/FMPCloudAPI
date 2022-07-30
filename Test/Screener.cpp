#include "Global.hpp"

#include <map>
#include <vector>

TEST_CASE("Screeners")
{
    auto api = getAPIObject();
    SECTION("Market Cap Screener")
    {
        std::map<CRITERIA, int> numericCriteria;
        numericCriteria[MAX_MARKET_CAP] = 1000000000;
        numericCriteria[MAX_MARKET_CAP] = 500000000;
        
        auto results1 = api.screenSymbols(numericCriteria);
        REQUIRE(results1.IsArray());
        REQUIRE(results1.Size() > 0);
        REQUIRE(results1[0].HasMember("symbol"));
        REQUIRE(results1[0].HasMember("marketCap"));
        REQUIRE(results1[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MAX_MARKET_CAP] = 1000000000;
        
        auto results2 = api.screenSymbols(numericCriteria);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() > results1.Size());
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MAX_MARKET_CAP] = 500000000;
        
        auto results3 = api.screenSymbols(numericCriteria);
        REQUIRE(results3.IsArray());
        REQUIRE(results3.Size() < results2.Size());
        REQUIRE(results3[0].HasMember("symbol"));
        REQUIRE(results3[0].HasMember("marketCap"));
        REQUIRE(results3[0].HasMember("isActivelyTrading"));
    }

    SECTION("Beta Screener")
    {
        std::map<CRITERIA, int> numericCriteria;
        numericCriteria[MIN_BETA] = -0.1;
        numericCriteria[MAX_BETA] = 0.1;
        
        auto results1 = api.screenSymbols(numericCriteria);
        REQUIRE(results1.IsArray());
        REQUIRE(results1.Size() > 0);
        REQUIRE(results1[0].HasMember("symbol"));
        REQUIRE(results1[0].HasMember("marketCap"));
        REQUIRE(results1[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MAX_BETA] = 0;
        
        auto results2 = api.screenSymbols(numericCriteria);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() > results1.Size());
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MIN_BETA] = -2;
        
        auto results3 = api.screenSymbols(numericCriteria);
        REQUIRE(results3.IsArray());
        REQUIRE(results3.Size() > results2.Size());
        REQUIRE(results3[0].HasMember("symbol"));
        REQUIRE(results3[0].HasMember("marketCap"));
        REQUIRE(results3[0].HasMember("isActivelyTrading"));
    }

    SECTION("Dividend Screener")
    {
        std::map<CRITERIA, int> numericCriteria;
        numericCriteria[MIN_DIVIDEND] = 0;
        numericCriteria[MAX_DIVIDEND] = 2;
        
        auto results1 = api.screenSymbols(numericCriteria);
        REQUIRE(results1.IsArray());
        REQUIRE(results1.Size() > 0);
        REQUIRE(results1[0].HasMember("symbol"));
        REQUIRE(results1[0].HasMember("marketCap"));
        REQUIRE(results1[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MAX_DIVIDEND] = 4;
        
        auto results2 = api.screenSymbols(numericCriteria);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() == results1.Size());    // max
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MIN_DIVIDEND] = 0;
        
        auto results3 = api.screenSymbols(numericCriteria);
        REQUIRE(results3.IsArray());
        REQUIRE(results3.Size() == results2.Size());    // max size = 10000
        REQUIRE(results3[0].HasMember("symbol"));
        REQUIRE(results3[0].HasMember("marketCap"));
        REQUIRE(results3[0].HasMember("isActivelyTrading"));
    }

    SECTION("Volume Screener")
    {
        std::map<CRITERIA, int> numericCriteria;
        numericCriteria[MIN_AVG_VOLUME] = 1000;
        numericCriteria[MAX_AVG_VOLUME] = 2000;
        
        auto results1 = api.screenSymbols(numericCriteria);
        REQUIRE(results1.IsArray());
        REQUIRE(results1.Size() > 0);
        REQUIRE(results1[0].HasMember("symbol"));
        REQUIRE(results1[0].HasMember("marketCap"));
        REQUIRE(results1[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MAX_AVG_VOLUME] = 10000;
        
        auto results2 = api.screenSymbols(numericCriteria);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() > results1.Size());
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MIN_AVG_VOLUME] = 10000;
        
        auto results3 = api.screenSymbols(numericCriteria);
        REQUIRE(results3.IsArray());
        REQUIRE(results3.Size() > results2.Size());
        REQUIRE(results3[0].HasMember("symbol"));
        REQUIRE(results3[0].HasMember("marketCap"));
        REQUIRE(results3[0].HasMember("isActivelyTrading"));
    }

    SECTION("Price Screener")
    {
        std::map<CRITERIA, int> numericCriteria;
        numericCriteria[MIN_PRICE] = 1;
        numericCriteria[MAX_PRICE] = 5;
        
        auto results1 = api.screenSymbols(numericCriteria);
        REQUIRE(results1.IsArray());
        REQUIRE(results1.Size() > 0);
        REQUIRE(results1[0].HasMember("symbol"));
        REQUIRE(results1[0].HasMember("marketCap"));
        REQUIRE(results1[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MAX_PRICE] = 10;
        
        auto results2 = api.screenSymbols(numericCriteria);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() > results1.Size());
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isActivelyTrading"));

        numericCriteria.clear();
        numericCriteria[MIN_PRICE] = 2;
        
        auto results3 = api.screenSymbols(numericCriteria);
        REQUIRE(results3.IsArray());
        REQUIRE(results3.Size() > results2.Size());
        REQUIRE(results3[0].HasMember("symbol"));
        REQUIRE(results3[0].HasMember("marketCap"));
        REQUIRE(results3[0].HasMember("isActivelyTrading"));
    }

    SECTION("Actively Trading Screener")
    {
        auto results = api.screenSymbols({}, {}, {}, {}, {}, true);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isActivelyTrading"));
        REQUIRE(results[0]["isActivelyTrading"].GetBool());

        results = api.screenSymbols({}, {}, {}, {}, {}, false);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isActivelyTrading"));
        REQUIRE(!results[0]["isActivelyTrading"].GetBool());
    }

    SECTION("Is ETF") 
    {
        auto results = api.screenSymbols({}, {}, {}, {}, {}, true, false);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isEtf"));
        REQUIRE(!results[0]["isEtf"].GetBool());

        results = api.screenSymbols({}, {}, {}, {}, {}, true, true);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isEtf"));
        REQUIRE(results[0]["isEtf"].GetBool());
    }

    SECTION("Sector, Industry, Exchange, Country Screener")
    {
        using Vector = std::vector<std::string>;
        Vector exchange {};
        Vector country {};
        Vector industry {};
        Vector sector {};

        exchange.push_back("NYSE");
        auto results = api.screenSymbols({}, country, sector, industry, exchange);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isEtf"));

        exchange.push_back("NASDAQ");
        auto results2 = api.screenSymbols({}, country, sector, industry, exchange);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() > results.Size());
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isEtf"));
        exchange.clear();
        
        country.push_back("US");
        results = api.screenSymbols({}, country, sector, industry, exchange);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isEtf"));
        country.clear();

        country.push_back("CA");
        results2 = api.screenSymbols({}, country, sector, industry, exchange);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() > 0);
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isEtf"));
        country.clear();

        industry.push_back("Oil");
        results = api.screenSymbols({}, country, sector, industry, exchange);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isEtf"));
        industry.clear();

        industry.push_back("Software");
        results2 = api.screenSymbols({}, country, sector, industry, exchange);
        REQUIRE(results2.IsArray());
        REQUIRE(results2.Size() > 0);
        REQUIRE(results2[0].HasMember("symbol"));
        REQUIRE(results2[0].HasMember("marketCap"));
        REQUIRE(results2[0].HasMember("isEtf"));
        industry.clear();

        sector.push_back("Energy");
        results = api.screenSymbols({}, country, sector, industry, exchange);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isEtf"));
        sector.clear();
    } 

    SECTION("All Criteria")
    {
        using Vector = std::vector<std::string>;
        Vector exchange {};
        Vector country {};
        Vector industry {};
        Vector sector {};
        std::map<CRITERIA, int> numericCriteria;
        bool isEtf {}, isActivelyTraded {};

        numericCriteria[MIN_MARKET_CAP] = 100000000;
        numericCriteria[MAX_MARKET_CAP] = 1000000000;

        numericCriteria[MIN_BETA] = -2;
        numericCriteria[MAX_BETA] = 2;

        numericCriteria[MIN_PRICE] = 2;
        numericCriteria[MAX_PRICE] = 200;

        numericCriteria[MIN_DIVIDEND] = 0.1;
        numericCriteria[MAX_DIVIDEND] = 100;

        numericCriteria[MIN_AVG_VOLUME] = 100000;
        numericCriteria[MAX_AVG_VOLUME] = 100000000;

        exchange.push_back("NASDAQ");
        country.push_back("US");
        industry.push_back("Software");
        sector.push_back("Technology");
        
        isEtf = false;
        isActivelyTraded = true;
        auto results = api.screenSymbols(numericCriteria, country, sector, industry, exchange, isActivelyTraded, isEtf);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("isEtf"));
    }
}

TEST_CASE("Ticker Search")
{
    auto api = getAPIObject();
    SECTION("One Ticker")
    {
        auto results = api.tickerSearch("AAPL", 20);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(std::strcmp(results[0]["symbol"].GetString(), "AAPL") == 0);
        REQUIRE(results[0].HasMember("name"));
        REQUIRE(std::strcmp(results[0]["name"].GetString(), "Apple Inc.") == 0);

        results = api.tickerSearch("AAPL", 1);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(std::strcmp(results[0]["symbol"].GetString(), "AAPL") == 0);
        REQUIRE(results[0].HasMember("name"));
        REQUIRE(std::strcmp(results[0]["name"].GetString(), "Apple Inc.") == 0);
    }

    SECTION("General Search")
    {
        auto results = api.tickerSearch("A");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("name"));

        results = api.tickerSearch("A", 20);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 20);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("name"));
    }
}