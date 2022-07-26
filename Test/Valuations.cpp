#include "Global.hpp"

TEST_CASE("Ratios")
{
    auto api = getAPIObject();
    SECTION("U.S. Annual Ratios")
    {
        auto results = api.getFinancialRatios("AAPL", ANNUAL, 5);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 5);
        REQUIRE(results[0].HasMember("quickRatio"));
        REQUIRE(results[0].HasMember("priceBookValueRatio"));
        REQUIRE(results[0].HasMember("debtEquityRatio"));

        results = api.getFinancialRatios("AAPL", ANNUAL, 7);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 7);
        REQUIRE(results[0].HasMember("quickRatio"));
        REQUIRE(results[0].HasMember("priceBookValueRatio"));
        REQUIRE(results[0].HasMember("debtEquityRatio"));
    }

    SECTION("U.S. Quarterly Ratios")
    {
        auto results = api.getFinancialRatios("JNJ", QUARTERLY, 60);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 60);
        REQUIRE(results[0].HasMember("quickRatio"));
        REQUIRE(results[0].HasMember("priceBookValueRatio"));
        REQUIRE(results[0].HasMember("debtEquityRatio"));

        results = api.getFinancialRatios("JNJ", QUARTERLY);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 40);
        REQUIRE(results[0].HasMember("quickRatio"));
        REQUIRE(results[0].HasMember("priceBookValueRatio"));
        REQUIRE(results[0].HasMember("debtEquityRatio"));
    }
    
    SECTION("U.S. TTM Ratios")
    {
        auto results = api.getFinancialRatios("INTC", TTM, 60);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("dividendYielTTM"));
        REQUIRE(results[0].HasMember("receivablesTurnoverTTM"));
        REQUIRE(results[0].HasMember("freeCashFlowPerShareTTM"));
        REQUIRE(results[0].HasMember("priceFairValueTTM"));
        REQUIRE(results[0].HasMember("enterpriseValueMultipleTTM"));

        results = api.getFinancialRatios("AMD", TTM);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("dividendYielTTM"));
        REQUIRE(results[0].HasMember("receivablesTurnoverTTM"));
        REQUIRE(results[0].HasMember("freeCashFlowPerShareTTM"));
        REQUIRE(results[0].HasMember("priceFairValueTTM"));
        REQUIRE(results[0].HasMember("enterpriseValueMultipleTTM"));
    }
}

TEST_CASE("Key Metrics")
{
    auto api = getAPIObject();
    SECTION("U.S. Annual Key Metrics")
    {
        auto results = api.getKeyMetrics("AAPL", ANNUAL, 5);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 5);
        REQUIRE(results[0].HasMember("revenuePerShare"));
        REQUIRE(results[0].HasMember("currentRatio"));
        REQUIRE(results[0].HasMember("workingCapital"));

        results = api.getKeyMetrics("TGT", ANNUAL, 7);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 7);
        REQUIRE(results[0].HasMember("revenuePerShare"));
        REQUIRE(results[0].HasMember("currentRatio"));
        REQUIRE(results[0].HasMember("workingCapital"));

    }

    SECTION("U.S. Quarterly Key Metrics")
    {
        auto results = api.getKeyMetrics("JNJ", QUARTERLY, 60);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 60);
        REQUIRE(results[0].HasMember("revenuePerShare"));
        REQUIRE(results[0].HasMember("currentRatio"));
        REQUIRE(results[0].HasMember("workingCapital"));

        results = api.getKeyMetrics("AMD", QUARTERLY);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 40);
        REQUIRE(results[0].HasMember("revenuePerShare"));
        REQUIRE(results[0].HasMember("currentRatio"));
        REQUIRE(results[0].HasMember("workingCapital"));
    }
    
    SECTION("U.S. TTM Key Metrics")
    {
        auto results = api.getKeyMetrics("INTC", TTM, 60);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("revenuePerShareTTM"));
        REQUIRE(results[0].HasMember("operatingCashFlowPerShareTTM"));
        REQUIRE(results[0].HasMember("peRatioTTM"));
        REQUIRE(results[0].HasMember("earningsYieldTTM"));
        REQUIRE(results[0].HasMember("payoutRatioTTM"));

        results = api.getKeyMetrics("QCOM", TTM);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("revenuePerShareTTM"));
        REQUIRE(results[0].HasMember("operatingCashFlowPerShareTTM"));
        REQUIRE(results[0].HasMember("peRatioTTM"));
        REQUIRE(results[0].HasMember("earningsYieldTTM"));
        REQUIRE(results[0].HasMember("payoutRatioTTM"));
    }
}

TEST_CASE("Enterprise Value")
{
    auto api = getAPIObject();
    SECTION("U.S. Annual Enterprise Value")
    {
        auto results = api.getEnterpriseValue("AAPL", ANNUAL, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("stockPrice"));
        REQUIRE(results[0].HasMember("enterpriseValue"));
        REQUIRE(results[0].HasMember("addTotalDebt"));
        REQUIRE(results[0].HasMember("minusCashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("marketCapitalization"));
        REQUIRE(results[0].HasMember("numberOfShares"));

        results = api.getEnterpriseValue("TSLA", ANNUAL, 1);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("stockPrice"));
        REQUIRE(results[0].HasMember("enterpriseValue"));
        REQUIRE(results[0].HasMember("addTotalDebt"));
        REQUIRE(results[0].HasMember("minusCashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("marketCapitalization"));
        REQUIRE(results[0].HasMember("numberOfShares"));
    }

    SECTION("U.S. Quarterly Enterprise Value")
    {
        auto results = api.getEnterpriseValue("AAPL", QUARTERLY, 60);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 60);
        REQUIRE(results[0].HasMember("stockPrice"));
        REQUIRE(results[0].HasMember("enterpriseValue"));
        REQUIRE(results[0].HasMember("addTotalDebt"));
        REQUIRE(results[0].HasMember("minusCashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("marketCapitalization"));
        REQUIRE(results[0].HasMember("numberOfShares"));

        results = api.getEnterpriseValue("WMT", QUARTERLY);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 40);
        REQUIRE(results[0].HasMember("stockPrice"));
        REQUIRE(results[0].HasMember("enterpriseValue"));
        REQUIRE(results[0].HasMember("addTotalDebt"));
        REQUIRE(results[0].HasMember("minusCashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("marketCapitalization"));
        REQUIRE(results[0].HasMember("numberOfShares"));
    }
}

TEST_CASE("Financial Statement Growth")
{
    auto api = getAPIObject();
    SECTION("U.S. Annual Financial Statement Growth")
    {
        auto results = api.getFinancialStatementGrowth("AAPL", ANNUAL, 6);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 6);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(results[0].HasMember("revenueGrowth"));
        REQUIRE(results[0].HasMember("ebitgrowth"));
        REQUIRE(results[0].HasMember("epsgrowth"));
        REQUIRE(results[0].HasMember("threeYDividendperShareGrowthPerShare"));

        results = api.getFinancialStatementGrowth("WMT", ANNUAL, 1);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(results[0].HasMember("revenueGrowth"));
        REQUIRE(results[0].HasMember("ebitgrowth"));
        REQUIRE(results[0].HasMember("epsgrowth"));
        REQUIRE(results[0].HasMember("threeYDividendperShareGrowthPerShare"));
    }

    SECTION("U.S. Quarterly Financial Statement Growth")
    {
        auto results = api.getFinancialStatementGrowth("INTC", QUARTERLY, 60);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 60);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(results[0].HasMember("revenueGrowth"));
        REQUIRE(results[0].HasMember("ebitgrowth"));
        REQUIRE(results[0].HasMember("epsgrowth"));
        REQUIRE(results[0].HasMember("threeYDividendperShareGrowthPerShare"));

        results = api.getFinancialStatementGrowth("QCOM", QUARTERLY);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 40);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(results[0].HasMember("revenueGrowth"));
        REQUIRE(results[0].HasMember("ebitgrowth"));
        REQUIRE(results[0].HasMember("epsgrowth"));
        REQUIRE(results[0].HasMember("threeYDividendperShareGrowthPerShare"));
    }
}

TEST_CASE("DCF")
{
    auto api = getAPIObject();
    SECTION("DCF JSON")
    {
        auto results = api.getCurrentDiscountCashFlowValue("AAPL");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dcf"));

        results = api.getCurrentDiscountCashFlowValue("INTC");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dcf"));
    }

    SECTION("Historical DCF - Daily")
    {
        auto results = api.getDailyDiscountedCashFlowValue("AAPL", 100);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 100);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dcf"));

        results = api.getDailyDiscountedCashFlowValue("JNJ", 50);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 50);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("dcf"));
    }

    SECTION("Historical DCF - Annual")
    {
        auto results = api.getHistoricalDiscountCashFlowValue("INTC", ANNUAL, 6);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 6);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("price"));
        REQUIRE(results[0].HasMember("dcf"));

        results = api.getHistoricalDiscountCashFlowValue("QCOM", ANNUAL, 1);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 1);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("price"));
        REQUIRE(results[0].HasMember("dcf"));
    }

    SECTION("Historical DCF - Quarterly")
    {
        auto results = api.getHistoricalDiscountCashFlowValue("INTC", QUARTERLY, 60);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 60);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("price"));
        REQUIRE(results[0].HasMember("dcf"));

        results = api.getHistoricalDiscountCashFlowValue("QCOM", QUARTERLY);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 40);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0].HasMember("price"));
        REQUIRE(results[0].HasMember("dcf"));
    }
}

TEST_CASE("Sectors P/E Ratio")
{
    auto api = getAPIObject();
    SECTION("Fetch Sectors P/E Ratio")
    {
        auto results = api.getAvgSectorPERatio("NASDAQ", "2021-06-01");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(std::strcmp(results[0]["date"].GetString(), "2021-06-01") == 0);
        REQUIRE(results[0].HasMember("sector"));
        REQUIRE(results[0].HasMember("exchange"));
        REQUIRE(results[0].HasMember("pe"));

        results = api.getAvgSectorPERatio("NYSE", "2022-06-01");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(std::strcmp(results[0]["date"].GetString(), "2022-06-01") == 0);
        REQUIRE(results[0].HasMember("sector"));
        REQUIRE(results[0].HasMember("exchange"));
        REQUIRE(results[0].HasMember("pe"));
    }
}

TEST_CASE("Industries P/E Ratio")
{
    auto api = getAPIObject();
    SECTION("Fetch Industries P/E Ratio")
    {
       auto results = api.getAvgIndustryPERatio("NASDAQ", "2021-06-01");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(std::strcmp(results[0]["date"].GetString(), "2021-06-01") == 0);
        REQUIRE(results[0].HasMember("industry"));
        REQUIRE(results[0].HasMember("exchange"));
        REQUIRE(results[0].HasMember("pe"));

        results = api.getAvgIndustryPERatio("NYSE", "2022-06-01");
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
        REQUIRE(results[0].HasMember("date"));
        REQUIRE(std::strcmp(results[0]["date"].GetString(), "2022-06-01") == 0);
        REQUIRE(results[0].HasMember("industry"));
        REQUIRE(results[0].HasMember("exchange"));
        REQUIRE(results[0].HasMember("pe"));
    }
}