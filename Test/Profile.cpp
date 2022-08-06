#include "Global.hpp"

auto api = getAPIObject();
TEST_CASE("Profile")
{
    SECTION("Profile")
    {
        auto results = api.getCompanyProfile("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("description"));
        REQUIRE(results[0].HasMember("website"));
        REQUIRE(results[0].HasMember("defaultImage"));
    }
}

TEST_CASE("Key Executives")
{   
    SECTION("Executives")
    {
        auto results = api.getKeyExecutives("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("title"));
        REQUIRE(results[0].HasMember("name"));
        REQUIRE(results[0].HasMember("pay"));
    }
}

TEST_CASE("Stock Peers")
{
    SECTION("Peers")
    {
        auto results = api.getStockPeers("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("peersList"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["peersList"].IsArray());
    }
}

TEST_CASE("Shares Float")
{  
    SECTION("Shares Float")
    {
        auto results = api.getSharesFloat("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("freeFloat"));
        REQUIRE(results[0].HasMember("floatShares"));
        REQUIRE(results[0].HasMember("outstandingShares"));
    }

    SECTION("Shares Float All")
    {
        auto results = api.getSharesFloat("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("freeFloat"));
        REQUIRE(results[0].HasMember("floatShares"));
        REQUIRE(results[0].HasMember("outstandingShares"));
    }
}

TEST_CASE("Rating")
{
    SECTION("Rating")
    {
        auto results = api.getRating("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("ratingRecommendation"));
        REQUIRE(results[0].HasMember("ratingDetailsROERecommendation"));
        REQUIRE(results[0].HasMember("ratingDetailsDERecommendation"));
    }

    SECTION("Historical Rating")
    {
        auto results = api.getHistoricalRating("AAPL", 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("ratingDetailsDERecommendation"));
        REQUIRE(results[0].HasMember("ratingDetailsROERecommendation"));
        REQUIRE(results[0].HasMember("ratingRecommendation"));
    }
}

TEST_CASE("Earnings Surprises")
{
    SECTION("Earnings Surprises")
    {
        auto results = api.getEarningsSurprises("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("actualEarningResult"));
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("estimatedEarning"));
    }
}

TEST_CASE("Earnings Transcripts")
{
    SECTION("Earnings Transcripts")
    {
        auto results = api.getEarningsCallTranscripts("AAPL", 1, 2020);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("quarter"));
        REQUIRE(results[0].HasMember("year"));
        REQUIRE(results[0].HasMember("date"));
    }

    SECTION("Earnings Transcripts Dates")
    {
        auto results = api.getEarningsCallTranscriptDates("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].IsArray());
        REQUIRE(results[0].Size() == 3);
    }

    SECTION("Earnings Transcripts For Year")
    {
        auto results = api.getEarningsCallTranscriptsForYear("AAPL", 2018);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("quarter"));
        REQUIRE(results[0].HasMember("year"));
        REQUIRE(results[0].HasMember("date"));
    }
}

TEST_CASE("Market Cap")
{
    SECTION("Market Cap")
    {
        auto results = api.getMarketCap("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("date"));
    }

    SECTION("Daily Market Cap")
    {
        auto results = api.getDailyMarketCap("INTC");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 100);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("date"));

        results = api.getDailyMarketCap("WM", 25);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 25);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("marketCap"));
        REQUIRE(results[0].HasMember("date"));
    }
}

TEST_CASE("SEC Filings")
{
    SECTION("SEC Filings")
    {
        auto results = api.getSECFilings("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("fillingDate"));
        REQUIRE(results[0].HasMember("type"));
        REQUIRE(results[0].HasMember("finalLink"));

        results = api.getSECFilings("AAPL", "10-Q", 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("fillingDate"));
        REQUIRE(results[0].HasMember("type"));
        REQUIRE(std::strcmp(results[0]["type"].GetString(), "10-Q") == 0);
        REQUIRE(results[0].HasMember("finalLink"));

        results = api.getSECFilings("AAPL", "10-K", 5);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 5);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("fillingDate"));
        REQUIRE(results[0].HasMember("type"));
        REQUIRE(std::strcmp(results[0]["type"].GetString(), "10-K") == 0);
        REQUIRE(results[0].HasMember("finalLink"));
    }
}

TEST_CASE("Press Releases")
{
    SECTION("Press Releases")
    {
        auto results = api.getPressReleases("AAPL", 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("title"));
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("text"));
    }
}

TEST_CASE("Financial Report Dates")
{
    SECTION("Financial Report Dates")
    {
        auto results = api.getFinancialReportDates("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("linkXlsx"));
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("linkJson"));
    }
}

TEST_CASE("10x Reports")
{
    SECTION("10x Reports")
    {
        auto results = api.get10xReports("AAPL", 2020, "Q1");
        results = api.get10xReports("AAPL", 2020, "FY");
    }
}

TEST_CASE("Fail to Deliver")
{
    SECTION("Earnings Surprises")
    {
        auto results = api.getFailToDeliver("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
    }
}

TEST_CASE("Stock News")
{
    SECTION("Stock News")
    {
        auto results = api.getStockNews({"AAPL"}, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("publishedDate"));
        REQUIRE(results[0].HasMember("image"));
        REQUIRE(results[0].HasMember("url"));

        results = api.getStockNews({"AAPL", "INTC", "PARA", "WMT"}, 100);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 100);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("publishedDate"));
        REQUIRE(results[0].HasMember("image"));
        REQUIRE(results[0].HasMember("url"));
    }
}