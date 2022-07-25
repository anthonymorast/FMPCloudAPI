#ifndef FMPCLOUDAPI
#define FMPCLOUDAPI

#include "rapidjson/document.h"
#include "../API/rapidjson/ostreamwrapper.h"
#include "../API/rapidjson/writer.h"
#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <functional>
#include <stdio.h>
#include <ctime>
#include <map>
#include <thread>
#include <chrono>

using namespace rapidjson;

namespace FMP
{
    struct FMPCloudAPIError : public std::runtime_error
    {
        ~FMPCloudAPIError() {}
        FMPCloudAPIError(const std::string error) : std::runtime_error(error) {}
    };

    enum CACHE_LENGTH { SHORT, LONG };
    enum PERIOD { ANNUAL, QUARTERLY };

    struct FMPCloudAPI
    {
        FMPCloudAPI(std::string apiKey) : _apiKey(apiKey) 
        {
            /* TODO: implement timers */
            /* NOTE: These only come into play if the object is persisted for > 2 seconds. */
            // https://stackoverflow.com/questions/21057676/need-to-call-a-function-at-periodic-time-intervals-in-c
            // std::thread([this]()
            // { 
            //     while (true)
            //     { 
            //         auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(10000);
            //         this->_expireLongCache();
            //         std::this_thread::sleep_until(x);
            //     }
            // }).detach();        // expire long cache every 10 seconds

            // std::thread([this]()
            // { 
            //     while (true)
            //     { 
            //         auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(2000);
            //         this->_expireShortCache();
            //         std::this_thread::sleep_until(x);
            //     }
            // }).detach();        // expire short cache every 2 seconds
        }

        // ----------------- Endpoints -------------------
        // Quotes
        Document getStockQuote(const std::string& symbol) const;                // no cache
        Document getStockQuote(const std::vector<std::string>& symbols) const;  // no cache
        Document getAvailableCrypto();                                          // long cache
        Document getCryptoPrices() const;                                       // no cache
        Document getAvailableForex();                                           // long cache
        Document getAllForexPrices() const;                                     // no cache
        Document getAllForexTick() const;                                       // no cache
        Document getAvailableEtf();                                             // long cache
        Document getCommoditiesAvailable();                                     // long cache
        Document getEuronextAvailable();                                        // long cache
        Document getNYSEAvailable();                                            // long cache
        Document getAMEXAvailable();                                            // long cache
        Document getTSXAvailable();                                             // long cache
        Document getMarketIndexesAvailable();                                   // long cache
        Document getMutualFundsAvailable();                                     // long cache
        Document getNasdaqAvailable();                                          // long cache
        Document getAllETFPrices() const;                                       // no cache
        Document getAllCommoditiesPrices() const;                               // no cache
        Document getAllEuronextPrices() const;                                  // no cache
        Document getAllNYSEPrices () const;                                     // no cache
        Document getAllAMEXPrices () const;                                     // no cache
        Document getAllTSXPrices () const;                                      // no cache
        Document getAllMarketIndexesPrices () const;                            // no cache
        Document getAllMutualFundsPrices () const;                              // no cache
        Document getAllNasdaqPrices () const;                                   // no cache
        // Calendars
        Document getEarningsCalendar();                                         // long cache
        Document getHistoricalEarningsCalendar(
            const std::string& ticker, uint limit=80);                          // long cache
        Document getIPOCalendar(
            const std::string& fromDate, const std::string& toDate);            // long cache
        Document getStockSplitCalendar(
            const std::string& fromDate, const std::string& toDate);            // long cache
        Document getDividendCalendar(
            const std::string& fromDate, const std::string& toDate);            // long cache
        Document getEconomicCalendar(
            const std::string& fromDate, const std::string& toDate);            // long cache
        // Financial Statements
        Document getSymbolsWithFinancialStatements();                           // long cache
        Document getIncomeStatement(
            const std::string& symbol, PERIOD period, uint limit=80);           // long cache
        Document getIncomeStatementGrowth(
            const std::string& symbol, PERIOD period, uint limit=80);           // long cache
        Document getBalanceSheet(
            const std::string& symbol, PERIOD period, uint limit=80);           // long cache
        Document getBalanceSheetGrowth(
            const std::string& symbol, PERIOD period, uint limit=80);           // long cache
        Document getCashFlowStatement(
            const std::string& symbol, PERIOD period, uint limit=80);           // long cache
        Document getCashFlowStatementGrowth(
            const std::string& symbol, PERIOD period, uint limit=80);           // long cache
        // Holders and Weightings
        Document getInstitutionalStockHolders(const std::string& symbol);       // long cache
        Document getMutualFundHolders(const std::string& symbol);               // long cache
        Document getETFHolders(const std::string& symbol);                      // long cache
        Document getETFSectorWeightings(const std::string& symbol);             // long cache
        Document getETFCountryWeightings(const std::string& symbol);            // long cache
        Document getInstitutionalHoldingSummary(const std::string& CIK);        // long cache
        Document getInstitutionalHoldersList();                                 // long cache
        Document searchInstitutionalHoldersByName(const std::string& name);     // long cache
        Document getPortfolioIndustrySummary(
            const std::string& CIK, const std::string& date, uint page = 0);    // long cache
        Document getInstitutionalPortfolioCompisition(
            const std::string& CIK, const std::string& date, uint page = 0);    // long cache
        Document searchInsiderTradingBySymbol(
            const std::string& symbol, uint limit=100);                         // long cache
        Document searchInsiderTradingByReportingCIK(
            const std::string& reportingCIK, uint limit=100);                   // long cache
        Document searchInsiderTradingByCompanyCIK(
            const std::string& companyCIK, uint limit=100);                     // long cache
        Document getNameCIKMap(const std::string& name="");                     // long cache
        Document getCompanyCIKMap(const std::string& symbol);                   // long cache
        // Valuations


        // ----------------- Helper Functions ---------------
        // on construction, create a timer to periodically expire the caches.
        void expireCache() { _expireLongCache(); _expireShortCache(); };
        void printDocument(const Document& jsonDoc, std::ostream& stream) const;
        private:
            std::string _apiKey;
            std::string _baseUrl {"https://fmpcloud.io/api/"};
            std::map<std::string, std::string> _longJsonContentsCache; 
            std::map<std::string, std::string> _shortjsonContentsCache;

            std::string _toCommaDelimited(const std::vector<std::string>& stringVector) const;
            std::string _downloadAndGetJsonFile(const std::string& url, const std::string& filename) const;    // download data, return as string
            std::string _generateFilename(const std::string& endpoint) const;
            Document _return(const std::string& contents) const;
            Document _returnFromAndUpdateCache(std::string url, std::string cacheKey, std::string filenameKey, CACHE_LENGTH duration);
            Document _getNoCache(std::string url, std::string filenameKey) const;

            std::string _getCachedContents(std::string key, CACHE_LENGTH length) const;
            void _cache(std::string key, std::string results, CACHE_LENGTH length);
            void _expireShortCache() { _shortjsonContentsCache.clear(); }
            void _expireLongCache() { _longJsonContentsCache.clear(); }
    };

    std::string FMPCloudAPI::_toCommaDelimited(const std::vector<std::string>& stringVector) const
    {
        std::string csvList {""};
        for(auto value : stringVector)
            csvList += value + ",";
        csvList.pop_back();
        return csvList;
    }

    std::string FMPCloudAPI::_generateFilename(const std::string& endpoint) const
    {
        std::time_t now = std::time(nullptr);
        return endpoint + "_" + std::to_string(now) + ".json";;
    }

    std::string FMPCloudAPI::_downloadAndGetJsonFile(const std::string& url, const std::string& filename) const
    {
        CURL *curl;
        FILE *fp;
        CURLcode res;
        curl = curl_easy_init();
        if (curl)
        {
            fp = fopen(filename.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);

            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
        }

        // sucks to close then re-open file, could try to make work with C-style file I/O
        std::ifstream file(filename);
        if(!file)
            throw FMPCloudAPIError("ERROR (_dowloadAndGetJsonFile): Unable to download data from FMP Cloud.");
        std::stringstream jsonContents;
        jsonContents << file.rdbuf();
        file.close();
        std::remove(filename.c_str());
        return jsonContents.str();
    }

    Document FMPCloudAPI::_return(const std::string& contents) const
    {
        Document results;
        results.Parse(contents.c_str());
        return results;
    }

    std::string FMPCloudAPI::_getCachedContents(std::string key, CACHE_LENGTH length) const
    {
        std::string cachedResult {""};
        if(length == LONG) 
        {
            auto it = _longJsonContentsCache.find(key);
            cachedResult = it == _longJsonContentsCache.end() ? "" : it->second;
        }
        else if(length == SHORT)
        {
            auto it = _shortjsonContentsCache.find(key);
            cachedResult = it == _shortjsonContentsCache.end() ? "" : it->second;
        } 
        return cachedResult;
    }

    void FMPCloudAPI::_cache(std::string key, std::string contents, CACHE_LENGTH length)
    {
        // overrides previous cache value (for expiration)
        if(length == LONG) 
            _longJsonContentsCache[key] = contents;
        else 
            _shortjsonContentsCache[key] = contents;
    }

    Document FMPCloudAPI::_returnFromAndUpdateCache(std::string url, std::string cacheKey, std::string filenameKey, CACHE_LENGTH duration)
    {
        std::string contents = _getCachedContents(cacheKey, duration);
        if(contents.empty()) 
        {
            std::string filename = _generateFilename(filenameKey);
            contents = _downloadAndGetJsonFile(url, filename);
            _cache(cacheKey, contents, duration);
        }
        return _return(contents);
    }

    Document FMPCloudAPI::_getNoCache(std::string url, std::string filenameKey) const
    {
        std::string filename = _generateFilename(filenameKey);
        std::string contents = _downloadAndGetJsonFile(url, filename);
        return _return(contents);
    }

    /** START HELPER FUNCTIONS **/

    void FMPCloudAPI::printDocument(const Document& jsonDoc, std::ostream& stream) const
    {
        OStreamWrapper osw(stream);
        Writer<OStreamWrapper> writer(osw);
        jsonDoc.Accept(writer);
    }

    /** END HELPER FUNCTIONS **/

    /** START ENDPOINTS **/

    ////////////
    // QUOTES
    ////////////
    Document FMPCloudAPI::getStockQuote(const std::string& symbol) const
    {
        std::vector<std::string> symbols { symbol };
        return getStockQuote(symbols);
    }

    Document FMPCloudAPI::getStockQuote(const std::vector<std::string>& symbols) const
    {
        if(symbols.size() > 50)
            throw FMPCloudAPIError("ERROR (getStockQuote(vector)): Too many symbols. The maximum number of symbols is 50.");
        std::string url = _baseUrl + "v3/quote/" + _toCommaDelimited(symbols) + "?apikey=" + _apiKey;
        return _getNoCache(url, "quote");
    }

    Document FMPCloudAPI::getAvailableCrypto()
    {
        std::string url = _baseUrl + "v3/symbol/available-cryptocurrencies?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available crypto", "available_crypto", LONG);
    }

    Document FMPCloudAPI::getCryptoPrices() const
    {
        std::string url = _baseUrl + "v3/quotes/crypto?apikey=" + _apiKey;
        return _getNoCache(url, "crypto_quotes");
    }

    Document FMPCloudAPI::getAvailableForex()
    {
        std::string url = _baseUrl + "v3/symbol/available-forex-currency-pairs?apikey=" + _apiKey;   
        return _returnFromAndUpdateCache(url, "available crypto", "available_crypto", LONG);
    }
    
    Document FMPCloudAPI::getAllForexPrices() const
    {
        std::string url = _baseUrl + "v3/quotes/forex?apikey=" + _apiKey;
        return _getNoCache(url, "forex_quotes_all");
    }

    Document FMPCloudAPI::getAllForexTick() const 
    {
        std::string url = _baseUrl + "v3/fx?apikey=" + _apiKey;
        return _getNoCache(url, "all_forex_ohlc");
    }

    Document FMPCloudAPI::getAvailableEtf()
    {
        std::string url = _baseUrl + "v3/symbol/available-etfs?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_etf", "available_etf", LONG);
    }

    Document FMPCloudAPI::getCommoditiesAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-commodities?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_commodities", "available_commodities", LONG);
    }

    Document FMPCloudAPI::getEuronextAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-euronext?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_euronext", "available_euronext", LONG);
    }

    Document FMPCloudAPI::getNYSEAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-nyse?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_nyse", "available_nyse", LONG);
    }

    Document FMPCloudAPI::getAMEXAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-amex?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_amex", "available_amex", LONG);
    }

    Document FMPCloudAPI::getTSXAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-tsx?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_tsx", "available_tsx", LONG);
    }

    Document FMPCloudAPI::getMarketIndexesAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-indexes?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_market_indices", "available_market_indices", LONG);
    }

    Document FMPCloudAPI::getMutualFundsAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-mutual-funds?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_mutual_funds", "available_mutual_funds", LONG);
    }

    Document FMPCloudAPI::getNasdaqAvailable()
    {
        std::string url = _baseUrl + "v3/symbol/available-nasdaq?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "available_nasdaq", "available_nasdaq", LONG);
    }

    Document FMPCloudAPI::getAllETFPrices() const
    {
        std::string url = _baseUrl + "v3/quotes/etf?apikey=" + _apiKey;
        return _getNoCache(url, "etf_quotes_all");
    }

    Document FMPCloudAPI::getAllCommoditiesPrices() const
    {
        std::string url = _baseUrl + "v3/quotes/commodity?apikey=" + _apiKey;
        return _getNoCache(url, "commodities_quotes_all");
    }

    Document FMPCloudAPI::getAllEuronextPrices() const
    {
        std::string url = _baseUrl + "v3/quotes/euronext?apikey=" + _apiKey;
        return _getNoCache(url, "euronext_quotes_all");
    }

    Document FMPCloudAPI::getAllNYSEPrices () const
    {
        std::string url = _baseUrl + "v3/quotes/nyse?apikey=" + _apiKey;
        return _getNoCache(url, "nyse_quotes_all");
    }

    Document FMPCloudAPI::getAllAMEXPrices () const
    {
        std::string url = _baseUrl + "v3/quotes/amex?apikey=" + _apiKey;
        return _getNoCache(url, "amex_quotes_all");
    }

    Document FMPCloudAPI::getAllTSXPrices () const
    {
        std::string url = _baseUrl + "v3/quotes/tsx?apikey=" + _apiKey;
        return _getNoCache(url, "tsx_quotes_all");
    }

    Document FMPCloudAPI::getAllMarketIndexesPrices () const
    {
        std::string url = _baseUrl + "v3/quotes/index?apikey=" + _apiKey;
        return _getNoCache(url, "market_indices_quotes_all");
    }

    Document FMPCloudAPI::getAllMutualFundsPrices () const
    {
        std::string url = _baseUrl + "v3/quotes/mutual_fund?apikey=" + _apiKey;
        return _getNoCache(url, "mutual_funds_quotes_all");
    }

    Document FMPCloudAPI::getAllNasdaqPrices () const
    {
        std::string url = _baseUrl + "v3/quotes/nasdaq?apikey=" + _apiKey;
        return _getNoCache(url, "nasdaq_quotes_all");
    }

    ////////////
    // Calendars
    ////////////
    Document FMPCloudAPI::getEarningsCalendar()
    {
        std::string url = _baseUrl + "v3/earning_calendar?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "earnings_calendar", "earnings_calendar", LONG);
    }

    Document FMPCloudAPI::getHistoricalEarningsCalendar(const std::string& ticker, uint limit)
    {
        std::string url = _baseUrl + "v3/historical/earning_calendar/" + ticker + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "historical_earnings_calendar" + ticker + std::to_string(limit), "historical_earnings_calendar" + ticker + std::to_string(limit), LONG);
    }

    Document FMPCloudAPI::getIPOCalendar(const std::string& fromDate, const std::string& toDate)
    {
        std::string url = _baseUrl + "v3/ipo_calendar?from=" + fromDate + "&to=" + toDate + "&apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "ipo_calendar" + fromDate + "_" + toDate, "ipo_calendar" + fromDate + "_" + toDate, LONG);
    }

    Document FMPCloudAPI::getStockSplitCalendar(const std::string& fromDate, const std::string& toDate)
    {
        std::string url = _baseUrl + "v3/stock_split_calendar?from=" + fromDate + "&to=" + toDate + "&apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "stock_split_calendar" + fromDate + "_" + toDate, "stock_split_calendar" + fromDate + "_" + toDate, LONG);
    }

    Document FMPCloudAPI::getDividendCalendar(const std::string& fromDate, const std::string& toDate)
    {
        std::string url = _baseUrl + "v3/stock_dividend_calendar?from=" + fromDate + "&to=" + toDate + "&apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "dividend_calendar" + fromDate + "_" + toDate, "dividend_calendar" + fromDate + "_" + toDate, LONG);
    }

    Document FMPCloudAPI::getEconomicCalendar(const std::string& fromDate, const std::string& toDate)
    {
        std::string url = _baseUrl + "v3/economic_calendar?from=" + fromDate + "&to=" + toDate + "&apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, "economic_calendar" + fromDate + "_" + toDate, "economic_calendar" + fromDate + "_" + toDate, LONG);
    }

    ////////////
    // Financial Statements 
    ////////////
    Document FMPCloudAPI::getSymbolsWithFinancialStatements()
    {
        std::string url = _baseUrl + "v3/financial-statement-symbol-lists?apikey=" + _apiKey;
        std::string key = "financial_statment_symbols";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getIncomeStatement(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/income-statement/" + symbol + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "income_statement" + symbol + std::to_string(limit);
        if(period == QUARTERLY)
        {
            url += "&period=quarter";
            key += "quarterly";
        }
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getIncomeStatementGrowth(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/income-statement-growth/" + symbol + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "income_statement_growth" + symbol + std::to_string(limit);
        if(period == QUARTERLY)
        {
            url += "&period=quarter";
            key += "quarterly";
        }
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getBalanceSheet(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/balance-sheet-statement/" + symbol + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "balance-sheet-statement" + symbol + std::to_string(limit);
        if(period == QUARTERLY)
        {
            url += "&period=quarter";
            key += "quarterly";
        }
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getBalanceSheetGrowth(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/balance-sheet-statement-growth/" + symbol + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "balance-sheet-statement-growth" + symbol + std::to_string(limit);
        if(period == QUARTERLY)
        {
            url += "&period=quarter";
            key += "quarterly";
        }
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getCashFlowStatement(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/cash-flow-statement/" + symbol + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "cash-flow-statement" + symbol + std::to_string(limit);
        if(period == QUARTERLY)
        {
            url += "&period=quarter";
            key += "quarterly";
        }
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getCashFlowStatementGrowth(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/cash-flow-statement-growth/" + symbol + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "cash-flow-statement-growth" + symbol + std::to_string(limit);
        if(period == QUARTERLY)
        {
            url += "&period=quarter";
            key += "quarterly";
        }
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    ////////////
    // Holders and Weightings
    ////////////
    Document FMPCloudAPI::getInstitutionalStockHolders(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/institutional-holder/" + symbol + "?apikey=" + _apiKey;
        std::string key = "instiutional_stock_holders" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getMutualFundHolders(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/mutual-fund-holder/" + symbol + "?apikey=" + _apiKey;
        std::string key = "mutual_fund_holders" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getETFHolders(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/etf-holder/" + symbol + "?apikey=" + _apiKey;
        std::string key = "etf-holder" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getETFSectorWeightings(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/etf-sector-weightings/" + symbol + "?apikey=" + _apiKey;
        std::string key = "etf-sector-weightings" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getETFCountryWeightings(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/etf-country-weightings/" + symbol + "?apikey=" + _apiKey;
        std::string key = "etf-country-weightings" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getInstitutionalHoldingSummary(const std::string& CIK)
    {
        std::string url = _baseUrl + "v4/institutional-ownership/portfolio-holdings-summary?cik=" + CIK + "&apikey=" + _apiKey;
        std::string key = "institutional-ownership-portfolio-holdings-summary" + CIK;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getInstitutionalHoldersList()
    {
        std::string url = _baseUrl + "v4/institutional-ownership/list?apikey=" + _apiKey;
        std::string key = "institutional-ownership-list";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::searchInstitutionalHoldersByName(const std::string& name)
    {
        std::string url = _baseUrl + "v4/institutional-ownership/name?name=" + name + "&apikey=" + _apiKey;
        std::string key = "institutional-ownership-name" + name;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getPortfolioIndustrySummary(const std::string& CIK, const std::string& date, uint page)
    {
        std::string url = _baseUrl + "v4/institutional-ownership/industry/portfolio-holdings-summary?cik=" + CIK + "&date=" + date + "&page=" + std::to_string(page) + "&apikey=" + _apiKey;
        std::string key = "institutional-ownership-industry-portfolio-holdings-summary" + CIK + date + std::to_string(page);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getInstitutionalPortfolioCompisition(const std::string& CIK, const std::string& date, uint page)
    {
        std::string url = _baseUrl + "v4/institutional-ownership/industry/portfolio-holdings?cik=" + CIK + "&date=" + date + "&page=" + std::to_string(page) + "&apikey=" + _apiKey;
        std::string key = "institutional-ownership-industry-portfolio-holdings" + CIK + date + std::to_string(page);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::searchInsiderTradingBySymbol(const std::string& symbol, uint limit)
    {
        std::string url = _baseUrl + "v4/insider-trading?symbol=" + symbol + "&apikey=" + _apiKey + "&limit=" + std::to_string(limit);
        std::string key = "insider-trading-symbol" + symbol + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::searchInsiderTradingByReportingCIK(const std::string& reportingCIK, uint limit)
    {
        std::string url = _baseUrl + "v4/insider-trading?apikey=" + _apiKey + "&reportingCik=" + reportingCIK + "&limit=" + std::to_string(limit);
        std::string key = "insider-trading-reportingCIK" + reportingCIK + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::searchInsiderTradingByCompanyCIK(const std::string& companyCIK, uint limit)
    {
        std::string url = _baseUrl + "v4/insider-trading?apikey=" + _apiKey + "&companyCik=" + companyCIK + "&limit=" + std::to_string(limit);
        std::string key = "insider-trading-companyCIK" + companyCIK;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getNameCIKMap(const std::string& name)
    {
        std::string url = _baseUrl + "v4/mapper-cik-name?apikey=" + _apiKey;
        std::string key = "mapper-cik-name" + name;
        if(!name.empty())
            url += ("&name=" + name);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getCompanyCIKMap(const std::string& symbol)
    {
        std::string url = _baseUrl + "v4/mapper-cik-company/" + symbol + "?apikey=" + _apiKey;
        std::string key = "mapper-cik-symbol" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    ////////////
    // Valuations
    ////////////


// ## Financial ratios 
// + Annual JSON:  https://fmpcloud.io/api/v3/ratios/AAPL?limit=40&apikey=APIKEY
// + Quarter JSON:  https://fmpcloud.io/api/v3/ratios/AAPL?period=quarter&limit=140&apikey=APIKEY
// + Annual TTM JSON:  https://fmpcloud.io/api/v3/ratios-ttm/AAPL?apikey=APIKEY

// ## Key Metrics 
// + Company TTM key metrics JSON:  https://fmpcloud.io/api/v3/key-metrics-ttm/AAPL?limit=40&apikey=APIKEY
// + Annual JSON:  https://fmpcloud.io/api/v3/key-metrics/AAPL?limit=40&apikey=APIKEY
// + Quarter JSON:  https://fmpcloud.io/api/v3/key-metrics/AAPL?period=quarter&limit=130&apikey=APIKEY

// ## Enterprise Value 
// + Annual JSON:  https://fmpcloud.io/api/v3/enterprise-values/AAPL?limit=40&apikey=APIKEY
// + Quarter JSON:  https://fmpcloud.io/api/v3/enterprise-values/AAPL?period=quarter&limit=130&apikey=APIKEY

// ## Financial Statements Growth 
// + Annual JSON:  https://fmpcloud.io/api/v3/financial-growth/AAPL?limit=20&apikey=APIKEY
// + Quarter JSON:  https://fmpcloud.io/api/v3/financial-growth/AAPL?period=quarter&limit=80&apikey=APIKEY

// ## Discounted Cash Flow Value 
// + DCF JSON:  https://fmpcloud.io/api/v3/discounted-cash-flow/AAPL?apikey=APIKEY
// + Daily Historical DCF JSON:  https://fmpcloud.io/api/v3/historical-daily-discounted-cash-flow/AAPL?limit=100&apikey=APIKEY
// + Annual Historical DCF JSON:  https://fmpcloud.io/api/v3/historical-discounted-cash-flow-statement/AAPL?limit=40&apikey=APIKEY
// + Quarter Historical DCF JSON:  https://fmpcloud.io/api/v3/historical-discounted-cash-flow-statement/AAPL?period=quarter&limit=120&apikey=APIKEY

// ## Sectors PE Ratio
// + Average price to earnings ratio for sectors JSON:  https://fmpcloud.io/api/v4/sector_price_earning_ratio?date=2021-05-07&exchange=NYSE&apikey=APIKEY

// ## Industries PE Ratio
// + Average price to earnings ratio for industries JSON:  https://fmpcloud.io/api/v4/industry_price_earning_ratio?date=2021-05-07&exchange=NYSE&apikey=APIKEY



    /** END ENDPOINTS **/
}

#endif