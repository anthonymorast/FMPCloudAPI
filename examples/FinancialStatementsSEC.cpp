#include "../API/FMPCloud.hpp"
#include "../API/rapidjson/prettywriter.h"
#include "../API/rapidjson/stringbuffer.h"

#include <string>
#include <iostream>

void prettyPrint(const Document& d)
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<StringBuffer> writer(buffer);
    d.Accept(writer);
    std::cout << buffer.GetString() << std::endl;
}

int main()
{
    // create the API object
    std::string apiKey = "YOUR_API_KEY";
    FMP::FMPCloudAPI api(apiKey);
    
    // fetch income statements for AAPL
    auto quarterlyIncomeStatements = api.getIncomeStatement("AAPL", FMP::QUARTERLY, 10);        // 10 periods of quarterly income statements
    auto annualIncomeStatements = api.getIncomeStatement("AAPL", FMP::ANNUAL, 5);               // 5 years of annual income statements
    auto ttmIncomeStatements = api.getIncomeStatement("AAPL", FMP::TTM);

    // fetch historical balance sheets for INTC
    auto quarterlyBalanceSheet = api.getBalanceSheet("INTC", FMP::QUARTERLY);
    auto annualBalanceSheet = api.getBalanceSheet("INTC", FMP::ANNUAL); 

    // fetch historical cash flow statements for PARA
    auto quarterlyCashFlow = api.getCashFlowStatement("PARA", FMP::QUARTERLY);
    auto annualCashFlow = api.getCashFlowStatement("PARA", FMP::ANNUAL); 

    // fetch balance sheet, income statement, and cash flow statement "growth" data which
    // returns the growth rates between recent period financial statements.
    auto incomeStatementGrowth = api.getIncomeStatementGrowth("FSLY", FMP::ANNUAL, 4);      // income statement growth (differences) for 4 annual periods
    auto balanceSheetGrowth = api.getBalanceSheetGrowth("WMT", FMP::QUARTERLY);             // balance sheet growth for 80 (the default limit) quarterly peridos
    auto cashFlowGrowth = api.getCashFlowStatementGrowth("WM", FMP::ANNUAL);                // cash flow statement growth for 80 (the default limit) annual periods 
                                                                                            // (it's unlikely that this will return 80 results)

    // SEC filings
    auto secDocumentsAll = api.getSECFilings("AAPL");                                       // returns all SEC filings for AAPL (100 document limit by default)
    auto sec10k = api.getSECFilings("AAPL", "10-K", 5);                                     // returns 5 most recent 10-K filings for AAPL
    auto sec10q = api.getSECFilings("AAPL", "10-Q", 10);                             // returns 10 most recent 10-Q filings for AAPL

    return 0;
}