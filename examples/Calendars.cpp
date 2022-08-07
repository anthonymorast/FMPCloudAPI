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
    
    // fetch and print profile information
    auto earningsCalendar = api.getEarningsCalendar();
    prettyPrint(earningsCalendar);

    // fetch and print stock peers
    auto stockSplitCalendar = api.getStockSplitCalendar("2021-01-01", "2022-01-01");
    prettyPrint(stockSplitCalendar);

    auto dividendCalendar = api.getDividendCalendar("2021-01-01", "2022-01-01");
    prettyPrint(dividendCalendar);

    return 0;
}