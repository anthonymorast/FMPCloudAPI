#include "Global.hpp"

TEST_CASE("Screeners")
{
    auto api = getAPIObject();
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