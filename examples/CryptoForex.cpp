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
    
    auto currentCryptoPrices = api.getCryptoPrices();                               // fetch cryptocurrency exchange rate quotes
    auto historicalBTCUSD = api.getHistoricalChart("BTCUSD", FMP::FOUR_HOUR);       // fetch historical BTC/USD exchange rates
    
    auto currentForexQuotes = api.getAllForexPrices();                              // fetches the current exchange rates for all available forex
    auto allForexTickData = api.getAllForexTick();                                  // fetches the tick data for all available forex
    auto historicalUSDGBP = api.getHistoricalChart("USDGBP", FMP::FIVE_MINUTE);     // fetch historical 5-minute USD/GBP exchange rate data
    prettyPrint(historicalUSDGBP);

    return 0;
}