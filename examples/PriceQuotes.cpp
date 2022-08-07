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
    
    // fetch and print single stock quote
    auto intcQuote = api.getStockQuote("INTC");
    prettyPrint(intcQuote);

    // fetch and print a batch of stock quotes
    auto batchQuote = api.getStockQuote({"INTC", "AAPL", "QCOM", "PARA", "BABA", "AMD", "TSLA", "WMT", "WM"});
    prettyPrint(batchQuote);

    // fetch and print hourly and minute by minute historical stock data for AMD
    // TIME period values are: ONE_MINUTE, FIVE_MINUTE, FIFTEEN_MINUTE, THIRTY_MINUTE, ONE_HOUR, FOUR_HOUR, DAILY
    auto hourlyHistoricalAmd = api.getHistoricalChart("AMD", FMP::ONE_HOUR);
    prettyPrint(hourlyHistoricalAmd);
    auto minuteHistoricalAmd = api.getHistoricalChart("AMD", FMP::ONE_MINUTE);
    prettyPrint(minuteHistoricalAmd);
    
    return 0;
}