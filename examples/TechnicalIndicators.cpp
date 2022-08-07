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
    
    // Available indicators are: SMA, EMA, WMA, DEMA, TEMA, WILLIAMS, RSI, ADX, STDDEV

    // fetch and print the 10-period exponential moving average (EMA) for FSLY
    auto tenPeriodEMA = api.getTechnicalIndicatorData("FSLY", FMP::DAILY, FMP::EMA, 10);
    prettyPrint(tenPeriodEMA);

    // fetch and print a 5-period simple moving average (SMA) for PINS
    auto fivePeriodSMA = api.getTechnicalIndicatorData("PINS", FMP::DAILY, FMP::SMA, 5);
    prettyPrint(fivePeriodSMA);

    return 0;
}