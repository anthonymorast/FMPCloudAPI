#include "Global.hpp"

TEST_CASE("Holders")
{
    auto api = getAPIObject();
    SECTION("Institutional")
    {
        auto results = api.getInstitutionalStockHolders("INTC");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("holder"));
        REQUIRE(results[0].HasMember("shares"));
    }

    SECTION("Mutual Funds")
    {
        auto results = api.getMutualFundHolders("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("holder"));
        REQUIRE(results[0].HasMember("shares"));
    }

    SECTION("ETF Holders")
    {
        auto results = api.getETFHolders("SPY");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("asset"));
        REQUIRE(results[0].HasMember("sharesNumber"));
    }

}

TEST_CASE("Weightings")
{
    auto api = getAPIObject();
    SECTION("Sector")
    {
        auto results = api.getETFSectorWeightings("SPY");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("sector"));
        REQUIRE(results[0].HasMember("weightPercentage"));
    }

    SECTION("Country")
    {
        auto results = api.getETFCountryWeightings("SPY");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("country"));
        REQUIRE(results[0].HasMember("weightPercentage"));
    }
}

TEST_CASE("Ownership")
{
    auto api = getAPIObject();

    SECTION("Institutional Portfolio Holdings")
    {
        auto results = api.getInstitutionalHoldingSummary("0001067983");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("investorName"));
        REQUIRE(std::strcmp(results[0]["investorName"].GetString(), "BERKSHIRE HATHAWAY INC") == 0);
        REQUIRE(results[0].HasMember("portfolioSize"));
    }

    SECTION("Institutional List")
    {
        auto results = api.getInstitutionalHoldersList();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("cik"));
        REQUIRE(results[0].HasMember("name"));
    }

    SECTION("Institutional Search")
    {
        auto results = api.searchInstitutionalHoldersByName("Berkshire%20Hathaway%20Inc");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("cik"));
        REQUIRE(results[0].HasMember("name"));
    }

    SECTION("Institutional Available")
    {
        auto results = api.getPortfolioIndustrySummary("0001067983", "2021-09-30");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("industryTitle"));
        REQUIRE(results[0].HasMember("investorName"));
        REQUIRE(results[0].HasMember("changeInWeightPercentage"));
    }
}

TEST_CASE("Insiders")
{
    auto api = getAPIObject();
    SECTION("By Ticker")
    {
        auto results = api.searchInsiderTradingBySymbol("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(std::strcmp(results[0]["symbol"].GetString(), "AAPL") == 0);
        REQUIRE(results[0].HasMember("filingDate"));
        REQUIRE(results[0].HasMember("transactionDate"));
        REQUIRE(results[0].HasMember("securitiesTransacted"));
    }

    SECTION("By Reporting CIK")
    {
        auto results = api.searchInsiderTradingByReportingCIK("0001067983");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("filingDate"));
        REQUIRE(results[0].HasMember("transactionDate"));
        REQUIRE(results[0].HasMember("securitiesTransacted"));
    }

    SECTION("By Company CIK")
    {
        auto results = api.searchInsiderTradingByCompanyCIK("0001067983");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("filingDate"));
        REQUIRE(results[0].HasMember("transactionDate"));
        REQUIRE(results[0].HasMember("securitiesTransacted"));

    }

    SECTION("CIK-Name List")
    {
        // with and without optional "name=(e.g. zuckerberg)" parameter
        auto results = api.getNameCIKMap();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("reportingCik"));
        REQUIRE(results[0].HasMember("reportingName"));

        results = api.getNameCIKMap("zuckerberg");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("reportingCik"));
        REQUIRE(results[0].HasMember("reportingName"));
    }

    SECTION("CIK-Company List")
    {
        auto results = api.getCompanyCIKMap("TGT");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(std::strcmp(results[0]["symbol"].GetString(), "TGT") == 0);  
        REQUIRE(results[0].HasMember("companyCik"));
    }


}