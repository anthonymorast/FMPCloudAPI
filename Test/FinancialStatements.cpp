#include "Global.hpp"

TEST_CASE("Financial Statements List")
{
    auto api = getAPIObject();
    SECTION("Fetch Financial Statement List")
    {
        auto results = api.getSymbolsWithFinancialStatements();
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() > 0);
    }
}

TEST_CASE("Income Statement")
{
    auto api = getAPIObject();
    SECTION("Fetch Income Statement")
    {
        auto results = api.getIncomeStatement("INTC", QUARTERLY, 4);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 4);
        REQUIRE(results[0].HasMember("revenue"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("Q", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "INTC");

        results = api.getIncomeStatement("SAP.DE", ANNUAL, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("revenue"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("FY", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "SAP.DE");
    }

    SECTION("Fetch Income Statement Growth")
    {
        auto results = api.getIncomeStatementGrowth("INTC", QUARTERLY, 4);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 4);
        REQUIRE(results[0].HasMember("growthRevenue"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("Q", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "INTC");

        results = api.getIncomeStatementGrowth("SAP.DE", ANNUAL, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("growthRevenue"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("FY", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "SAP.DE");
    }
}

TEST_CASE("Balance Sheet Statement")
{
    auto api = getAPIObject();
    SECTION("Fetch Balance Sheet Statement")
    {
        auto results = api.getBalanceSheet("INTC", QUARTERLY, 4);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 4);
        REQUIRE(results[0].HasMember("cashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("Q", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "INTC");

        results = api.getBalanceSheet("SAP.DE", ANNUAL, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("cashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("FY", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "SAP.DE");
    }
    
    SECTION("Fetch Balance Sheet Statement Growth")
    {
        auto results = api.getBalanceSheetGrowth("INTC", QUARTERLY, 4);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 4);
        REQUIRE(results[0].HasMember("growthCashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("Q", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "INTC");

        results = api.getBalanceSheetGrowth("SAP.DE", ANNUAL, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("growthCashAndCashEquivalents"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("FY", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "SAP.DE");
    }
}

TEST_CASE("Cash Flow Statement")
{
    auto api = getAPIObject();
    SECTION("Fetch Cash Flow Statement")
    {
        auto results = api.getCashFlowStatement("INTC", QUARTERLY, 4);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 4);
        REQUIRE(results[0].HasMember("freeCashFlow"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("Q", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "INTC");

        results = api.getCashFlowStatement("SAP.DE", ANNUAL, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("freeCashFlow"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("FY", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "SAP.DE");
    }

    SECTION("Fetch Cash Flow Statement Growth")
    {
        auto results = api.getCashFlowStatementGrowth("INTC", QUARTERLY, 4);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 4);
        REQUIRE(results[0].HasMember("growthFreeCashFlow"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("Q", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "INTC");

        results = api.getCashFlowStatementGrowth("SAP.DE", ANNUAL, 10);
        REQUIRE(results.IsArray());
        REQUIRE(results.Size() == 10);
        REQUIRE(results[0].HasMember("growthFreeCashFlow"));
        REQUIRE(results[0].HasMember("period"));
        REQUIRE(std::string(results[0]["period"].GetString()).rfind("FY", 0) == 0);   // starts with Q (quarterly)
        REQUIRE(results[0].HasMember("symbol"));
        REQUIRE(results[0]["symbol"] == "SAP.DE");
    }
}