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
#include <mutex>

using namespace rapidjson;

namespace FMP
{
    using CString = const std::string;
    using CStringVector = const std::vector<std::string>;

    std::mutex _shortMtx;
    std::mutex _longMtx;
    struct FMPCloudAPIError : public std::runtime_error
    {
        ~FMPCloudAPIError() {}
        FMPCloudAPIError(const std::string error) : std::runtime_error(error) {}
    };

    enum CACHE_LENGTH { SHORT, LONG };
    enum PERIOD { ANNUAL, QUARTERLY, TTM };
    enum TIME { ONE_MINUTE, FIVE_MINUTE, FIFTEEN_MINUTE, THIRTY_MINUTE, ONE_HOUR, FOUR_HOUR, DAILY };
    enum CRITERIA { MAX_MARKET_CAP, MIN_MARKET_CAP, MAX_BETA, MIN_BETA, 
                    MAX_DIVIDEND, MIN_DIVIDEND, MAX_AVG_VOLUME, 
                    MIN_AVG_VOLUME, MAX_PRICE, MIN_PRICE };
    enum INDICATOR_TYPE {SMA, EMA, WMA, DEMA, TEMA, WILLIAMS, RSI, ADX, STDDEV};
    enum STOCK_MARKET_TOP { ACTIVE, LOSERS, GAINERS };

    struct FMPCloudAPI
    {
        FMPCloudAPI(std::string apiKey) : _apiKey(apiKey) 
        {
            /* NOTE: These only come into play if the object is persisted for > 30 seconds. */
            // https://stackoverflow.com/questions/21057676/need-to-call-a-function-at-periodic-time-intervals-in-c
            std::thread([this]()
            { 
                while (true)
                { 
                    auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(600000);
                    if(!this)
                        return;
                    this->_expireLongCache();
                    std::this_thread::sleep_until(x);
                }
            }).detach();        // expire long cache every 10 minutes

            std::thread([this]()
            { 
                while (true)
                { 
                    auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(30000);
                    if(!this)
                        return;
                    this->_expireShortCache();
                    std::this_thread::sleep_until(x);
                }
            }).detach();        // expire short cache every 30 seconds
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
        Document getFinancialRatios(
            const std::string& symbol, PERIOD period, uint limit=40);           // long cache
        Document getKeyMetrics(
            const std::string& symbol, PERIOD period, uint limit=40);           // long cache
        Document getEnterpriseValue(
            const std::string& symbol, PERIOD period, uint limit=40);           // long cache
        Document getFinancialStatementGrowth(
            const std::string& symbol, PERIOD period, uint limit=40);           // long cache
        Document getHistoricalDiscountCashFlowValue(
            const std::string& symbol, PERIOD period, uint limit=40);           // long cache
        Document getCurrentDiscountCashFlowValue(const std::string& symbol);    // long cache
        Document getDailyDiscountedCashFlowValue(
            const std::string& symbol, uint limit=100);                         // long cache
        Document getAvgSectorPERatio(
            const std::string& exchange, const std::string& date);              // long cache
        Document getAvgIndustryPERatio(
            const std::string& exchange, const std::string& date);              // long cache
        ////////////
        // Administrative
        ////////////
        Document getDelistedCompanyList(uint limit=100);                        // long cache
        Document getSP500Constituents(bool historical=false);                   // long cache
        Document getNasdaq100Constituents(bool historical=false);               // long cache
        Document getDJIAConstituents(bool historical=false);                    // long cache
        Document getSymbolList();                                               // long cache
        Document getETFList();                                                  // long cache
        Document getTradableSymbolsList();                                      // long cache
        Document getTradingHours();                                             // long cache
        Document getCOTSymbolsList();                                           // long cache
        Document getCOTReportByPeriod(
            const std::string& fromDate, const std::string& toDate);            // long cache
        Document getCOTReportByTicker(const std::string& symbol);               // long cache
        Document getCOTAnalysisByPeriod(
            const std::string& fromDate, const std::string& toDate);            // long cache
        Document getCOTAnalysisByTicker(const std::string& symbol);             // long cache
        Document get13FCIKList();                                               // long cache
        Document searchCIKByName(const std::string& name);                      // long cache
        Document getNameByCIK(const std::string& CIK);                          // long cache
        Document get13FByCIK(const std::string& CIK, const std::string& date);  // long cache
        Document cusipMapper(const std::string& CUSIP);                         // long cache
        Document getListOfCountries();                                          // long cache
        /////////////
        // HISTORICAL PRICES
        /////////////
        Document getHistoricalChart(const std::string& symbol, TIME timePeriod);// short cache
        Document getDailyLine(const std::string& symbol);                       // long cache
        Document getDailyChangeAndVolume(const std::string& symbol);            // long cache
        Document getDailyDataByPeriod(const std::string& symbol, 
            const std::string& fromDate, const std::string& toDate);            // long cache
        Document getDailyDataXDays(const std::string& symbol, uint numDays);    // long cache
        Document getDailyStockDividend(const std::string& symbol);              // long cache
        Document getDailyStockSplit(const std::string& symbol);                 // long cache
        ////////////
        // Screeners
        ////////////
        Document screenSymbols(
            const std::map<CRITERIA, int>& numericCriteriaMap={}, 
            const std::vector<std::string>& countryList={},
            const std::vector<std::string>& sectorList={},
            const std::vector<std::string>& industryList={},
            const std::vector<std::string>& exchangeList={},
            bool isActivelyTrading=true, bool isEtf=false) const;               // no cache (too unweildy)
        Document tickerSearch(const std::string& query, uint limit=10);         // long cache
        ////////////
        // Profile
        ////////////
        Document getCompanyProfile(const std::string& symbol);                  // long cache
        Document getKeyExecutives(const std::string& symbol);                   // long cache
        Document getSharesFloat(const std::string& symbol);                     // long cache
        Document getAllSharesFloat();                                           // long cache
        Document getRating(const std::string& symbol);                          // long cache
        Document getHistoricalRating(const std::string& symbol, uint limit=100);// long cache
        Document getMarketCap(const std::string& symbol);                       // long cache
        Document getDailyMarketCap(const std::string& symbol, uint limit=100);  // long cache
        Document getStockNews(
            const std::vector<std::string>& tickers = {}, uint limit=100) const;// no cache
        Document getEarningsCallTranscripts(
            const std::string& symbol, uint quarter=0, uint year=0);            // long cache
        Document getEarningsCallTranscriptDates(
            const std::string& symbol, uint quarter=0, uint year=0);            // long cache
        Document getEarningsCallTranscriptsForYear(
            const std::string& symbol, uint year=0);                            // long cache
        Document getEarningsSurprises(const std::string& symbol);               // long cache
        Document getSECFilings(const std::string& symbol, 
            const std::string& type="", uint limit=100);                        // long cache
        Document getPressReleases(const std::string& symbol, uint limit=100);   // long cache
        Document getFinancialReportDates(CString& symbol);                      // long cache
        Document get10xReports(CString& symbol, uint year, CString& period);    // long cache
        Document getStockPeers(CString& symbol);                                // long cache
        Document getFailToDeliver(CString& symbol);                             // long cache
        ////////////
        // Misc.
        ////////////
        Document getBatchEODPrices(
            CStringVector& tickerList={}, CString& date="");                    // long cache
        Document getStockMarketPerformers(STOCK_MARKET_TOP smt);                // long cache
        Document getSectorPerformance(bool historical=false, uint limit=50);    // long cache
        Document getTechnicalIndicatorData(
            CString& symbol, TIME time, INDICATOR_TYPE type, uint numPeriods);  // long cache
        Document getSpecificStandardIndustrialClassification(
            CString& cik="", CString& symbol="", CString& sicCode="");          // long cache
        Document getAllStandardIndustrialClassification();                      // long cache
        Document getAllStandardIndustrialClassificationList();                  // long cache
        Document getSpecificStandardIndustrialClassificationList(
            CString& industry="", CString& sicCode="");                         // long cache
    
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
            // TODO: test or remove...
            void _expireShortCache() 
            { 
                _shortMtx.lock(); 
                _shortjsonContentsCache.clear(); 
                _shortMtx.unlock(); 
            }
            void _expireLongCache() 
            { 
                _longMtx.lock(); 
                _longJsonContentsCache.clear(); 
                _longMtx.unlock(); 
            }
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
            _longMtx.lock();
            auto it = _longJsonContentsCache.find(key);
            cachedResult = it == _longJsonContentsCache.end() ? "" : it->second;
            _longMtx.unlock();
        }
        else if(length == SHORT)
        {
            _shortMtx.lock();
            auto it = _shortjsonContentsCache.find(key);
            cachedResult = it == _shortjsonContentsCache.end() ? "" : it->second;
            _shortMtx.unlock();
        } 
        return cachedResult;
    }

    void FMPCloudAPI::_cache(std::string key, std::string contents, CACHE_LENGTH length)
    {
        // overrides previous cache value (for expiration)
        length == LONG ? _longMtx.lock() : _shortMtx.lock();
        if(length == LONG) 
            _longJsonContentsCache[key] = contents;
        else 
            _shortjsonContentsCache[key] = contents;
        length == LONG ? _longMtx.unlock() : _shortMtx.unlock();            
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
    Document FMPCloudAPI::getFinancialRatios(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/" + ((period != TTM) ? "ratios/" : "ratios-ttm/") + symbol
                + (period == QUARTERLY ? "?period=quarter&" : "?")
                + "apikey=" + _apiKey
                + "&limit=" + std::to_string(limit);
        std::string key = "ratios" + symbol + std::to_string(limit) + (period == ANNUAL ? "annual" : (period == TTM ? "ttm" : "quarter"));
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getKeyMetrics(const std::string& symbol, PERIOD period, uint limit)
    {
        std::string url = _baseUrl + "v3/" + ((period != TTM) ? "key-metrics/" : "key-metrics-ttm/") + symbol
                + (period == QUARTERLY ? "?period=quarter&" : "?")
                + "apikey=" + _apiKey
                + "&limit=" + std::to_string(limit);
        std::string key = "key_metrics" + symbol + std::to_string(limit) + (period == ANNUAL ? "annual" : (period == TTM ? "ttm" : "quarter"));
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getEnterpriseValue(const std::string& symbol, PERIOD period, uint limit)
    {
        if(period == TTM)
            return {};
        
        std::string url = _baseUrl + "v3/enterprise-values/" + symbol 
                + "?limit=" + std::to_string(limit)
                + "&apikey=" + _apiKey 
                + (period == QUARTERLY ? "&period=quarter" : "");
        std::string key = "enterprise_values" + symbol + std::to_string(limit) + (period == ANNUAL ? "annual" : "quarter");
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getFinancialStatementGrowth(const std::string& symbol, PERIOD period, uint limit)
    {
        if(period == TTM)
            return {};
        
        std::string url = _baseUrl + "v3/financial-growth/" + symbol 
                + "?limit=" + std::to_string(limit)
                + "&apikey=" + _apiKey 
                + (period == QUARTERLY ? "&period=quarter" : "");
        std::string key = "financial_growth" + symbol + std::to_string(limit) + (period == ANNUAL ? "annual" : "quarter");
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getHistoricalDiscountCashFlowValue(const std::string& symbol, PERIOD period, uint limit)
    {
        if(period == TTM)
            return {};
        
        std::string url = _baseUrl + "v3/historical-discounted-cash-flow-statement/" + symbol 
                + "?limit=" + std::to_string(limit)
                + "&apikey=" + _apiKey 
                + (period == QUARTERLY ? "&period=quarter" : "");
        std::string key = "historical-discounted-cash-flow-statement" + symbol + std::to_string(limit) + (period == ANNUAL ? "annual" : "quarter");
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getCurrentDiscountCashFlowValue(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/discounted-cash-flow/" + symbol + "?apikey=" + _apiKey;
        std::string key = "discounted_cash_flow" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getDailyDiscountedCashFlowValue(const std::string& symbol, uint limit)
    {
        std::string url = _baseUrl + "v3/historical-daily-discounted-cash-flow/" + symbol + "?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "historical-daily-discounted-cash-flow" + symbol + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getAvgSectorPERatio(const std::string& exchange, const std::string& date)
    {
        std::string url = _baseUrl + "v4/sector_price_earning_ratio/"
                + "?apikey=" + _apiKey
                + "&date=" + date
                + "&exchange=" + exchange;
        std::string key = "sector_price_earning_ratio" + date + exchange;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getAvgIndustryPERatio(const std::string& exchange, const std::string& date)
    {
        std::string url = _baseUrl + "v4/industry_price_earning_ratio/"
                + "?apikey=" + _apiKey
                + "&date=" + date
                + "&exchange=" + exchange;
        std::string key = "industry_price_earning_ratio" + date + exchange;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    ////////////
    // Administrative
    ////////////
    Document FMPCloudAPI::getDelistedCompanyList(uint limit)
    {
        std::string url = _baseUrl + "v3/delisted-companies?limit=" + std::to_string(limit) + "&apikey=" + _apiKey;
        std::string key = "delisted_companies" + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getSP500Constituents(bool historical)
    {
        std::string url = _baseUrl + "v3/" + (historical ? "historical/" : "") + "sp500_constituent?apikey=" + _apiKey;
        std::string key = "sp500_constituent" + std::string(historical ? "historical" : "");
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getNasdaq100Constituents(bool historical)
    {
        std::string url = _baseUrl + "v3/" + (historical ? "historical/" : "") + "nasdaq_constituent?apikey=" + _apiKey;
        std::string key = "nasdaq_constituent" + std::string(historical ? "historical" : "");
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getDJIAConstituents(bool historical)
    {
        std::string url = _baseUrl + "v3/" + (historical ? "historical/" : "") + "dowjones_constituent?apikey=" + _apiKey;
        std::string key = "dowjones_constituent" + std::string(historical ? "historical" : "");
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getSymbolList()
    {
        std::string url = _baseUrl + "v3/stock/list?apikey=" + _apiKey;
        std::string key = "symbol_list";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getETFList()
    {
        std::string url = _baseUrl + "v3/etf/list?apikey=" + _apiKey;
        std::string key = "etf_list";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getTradableSymbolsList()
    {
        std::string url = _baseUrl + "v3/available-traded/list?apikey=" + _apiKey;
        std::string key = "tradeable_symbols";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getTradingHours()
    {
        std::string url = _baseUrl + "v3/market-hours?apikey=" + _apiKey;
        std::string key = "market_hours";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getCOTSymbolsList()
    {
        std::string url = _baseUrl + "v4/commitment_of_traders_report/list?apikey=" + _apiKey;
        std::string key = "commitment_of_traders_report";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getCOTReportByPeriod(const std::string& fromDate, const std::string& toDate)
    {
        std::string url = _baseUrl + "v4/commitment_of_traders_report?from=" + fromDate + "&to=" + toDate + "&apikey=" + _apiKey;
        std::string key = "commitment_of_traders_report" + fromDate + toDate;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getCOTReportByTicker(const std::string& symbol)
    {
        std::string url = _baseUrl + "v4/commitment_of_traders_report/" + symbol + "?apikey=" + _apiKey;
        std::cout << url << std::endl;
        std::string key = "commitment_of_traders_report" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getCOTAnalysisByPeriod(const std::string& fromDate, const std::string& toDate)
    {
        std::string url = _baseUrl + "v4/commitment_of_traders_report_analysis?from=" + fromDate + "&to=" + toDate + "&apikey=" + _apiKey;
        std::string key = "commitment_of_traders_report_analysis" + fromDate + toDate;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getCOTAnalysisByTicker(const std::string& symbol)
    {
        std::string url = _baseUrl + "v4/commitment_of_traders_report_analysis/" + symbol + "?apikey=" + _apiKey;
        std::string key = "commitment_of_traders_report_analysis" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::get13FCIKList()
    {
        std::string url = _baseUrl + "v3/cik_list?apikey=" + _apiKey;
        std::string key = "cik_list";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::searchCIKByName(const std::string& name)
    {
        std::string url = _baseUrl + "v3/cik-search/" + name + "?apikey=" + _apiKey;
        std::string key = "cik_search" + name;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getNameByCIK(const std::string& CIK)
    {
        std::string url = _baseUrl + "v3/cik/" + CIK + "?apikey=" + _apiKey;
        std::string key = "name_search" + CIK;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::get13FByCIK(const std::string& CIK, const std::string& date)
    {
        std::string url = _baseUrl + "v3/form-thirteen/" + CIK + "?date=" + date + "&apikey=" + _apiKey;
        std::string key = "13f" + CIK + date;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::cusipMapper(const std::string& CUSIP)
    {
        std::string url = _baseUrl + "v3/cusip/" + CUSIP + "?apikey=" + _apiKey;
        std::string key = "cusip_mapepr" + CUSIP;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getListOfCountries()
    {
        std::string url = _baseUrl + "v3/get-all-countries?apikey=" + _apiKey;
        std::string key = "country_list";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    ////////////
    // Historical Data
    ////////////
    Document FMPCloudAPI::getHistoricalChart(const std::string& symbol, TIME timePeriod)
    {
        std::string url = _baseUrl + "v3/historical-chart/";
        std::string period {};
        switch(timePeriod)
        {
            case ONE_MINUTE:
                period = "1min";
                break;
            case FIVE_MINUTE:
                period = "5min";
                break;
            case FIFTEEN_MINUTE:
                period = "15min";
                break;
            case THIRTY_MINUTE:
                period = "30min";
                break;
            case ONE_HOUR:
                period = "1hour";
                break;
            case FOUR_HOUR:
                period = "4hour";
                break;
            default:
                return {};
        }
        url += period + "/" + symbol + "?apikey=" + _apiKey;
        std::string key = "historical_chart_" + symbol + period;
        return _returnFromAndUpdateCache(url, key, key, SHORT);
    }

    Document FMPCloudAPI::getDailyLine(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/historical-price-full/" + symbol + "?serietype=line&apikey=" + _apiKey;
        std::string key = "historical_price_full_seritype_" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getDailyChangeAndVolume(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/historical-price-full/" + symbol + "?apikey=" + _apiKey;
        std::string key = "historical_price_full_" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getDailyDataByPeriod(const std::string& symbol, const std::string& fromDate, const std::string& toDate)
    {
        std::string url = _baseUrl + "v3/historical-price-full/" + symbol + "?from=" + fromDate + "&to=" + toDate + "&apikey=" + _apiKey;
        std::string key = "historical_price_full_byperiod_" + symbol + fromDate + toDate;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getDailyDataXDays(const std::string& symbol, uint numDays)
    {
        std::string url = _baseUrl + "v3/historical-price-full/" + symbol + "?timeseries=" + std::to_string(numDays) + "&apikey=" + _apiKey;
        std::string key = "historical_price_full_bynumdays_" + symbol + std::to_string(numDays);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getDailyStockDividend(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/historical-price-full/stock_dividend/" + symbol + "?apikey=" + _apiKey;
        std::string key = "daily_stock_dividend" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getDailyStockSplit(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/historical-price-full/stock_split/" + symbol + "?apikey=" + _apiKey;
        std::string key = "daily_stock_split" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    ////////////
    // Screener
    ////////////
    Document FMPCloudAPI::screenSymbols(const std::map<CRITERIA, int>& numericCriteriaMap, const std::vector<std::string>& countryList,
            const std::vector<std::string>& sectorList, const std::vector<std::string>& industryList,
            const std::vector<std::string>& exchangeList, bool isActivelyTrading, bool isEtf) const 
    {
        std::string url = _baseUrl + "v3/stock-screener?apikey=" + _apiKey;
        std::string key = "symbol_screener";

        bool first = true;
        for(const auto& sector : sectorList)
        {
            url += (first ? "&sector=" : ",") + sector;
            first = false;
        }
        first = true;
        for(const auto& country : countryList)
        {
            url += (first ? "&country=" : ",") + country;
            first = false;
        }
        first = true;
        for(const auto& industry : industryList)
        {
            url += (first ? "&industry=" : ",") + industry;
            first = false;
        }
        first = true;
        for(const auto& exchange : exchangeList)
        {
            url += (first ? "&exchange=" : ",") + exchange;
            first = false;
        }

        url += "&isActivelyTrading=" + std::string(isActivelyTrading ? "true" : "false");
        url += "&isEtf=" + std::string(isEtf ? "true" : "false");

        for(const auto& it : numericCriteriaMap)
        {
            std::string criteria {};
            switch(it.first)
            {
                case MAX_MARKET_CAP:
                    criteria = "marketCapLowerThan=" + std::to_string(it.second);
                    break;  
                case MIN_MARKET_CAP:
                    criteria = "marketCapMoreThan=" + std::to_string(it.second);
                    break;
                case MAX_BETA:
                    criteria = "betaLowerThan=" + std::to_string(it.second);
                    break;
                case MIN_BETA:
                    criteria = "betaMoreThan=" + std::to_string(it.second);
                    break;
                case MAX_DIVIDEND:
                    criteria = "dividendLowerThan=" + std::to_string(it.second);
                    break;
                case MIN_DIVIDEND:
                    criteria = "dividendMoreThan=" + std::to_string(it.second);
                    break;
                case MAX_AVG_VOLUME:
                    criteria = "volumeLowerThan=" + std::to_string(it.second);
                    break;
                case MIN_AVG_VOLUME:
                    criteria = "volumeMoreThan=" + std::to_string(it.second);
                    break;
                case MAX_PRICE:
                    criteria = "priceLowerThan=" + std::to_string(it.second);
                    break;
                case MIN_PRICE:
                    criteria = "priceMoreThan=" + std::to_string(it.second);
                    break;
                default:
                    std::cout << "(FMPCloudAPI::screenSymbols): Unkown numeric criteria." << std::endl;
            }
            url += "&" + criteria;
        }

        std::cout << "Screener URL: " << url << std::endl;
        return _getNoCache(url, key);
    }

    Document FMPCloudAPI::tickerSearch(const std::string& query, uint limit)
    {
        std::string url = _baseUrl + "v3/search?apikey=" + _apiKey + "&query=" + query + "&limit=" + std::to_string(limit);
        std::string key = "ticker_search" + query + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    ////////////
    // Profile
    ////////////
    Document FMPCloudAPI::getCompanyProfile(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/profile/" + symbol + "?apikey=" + _apiKey;
        std::string key = "profile_" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getKeyExecutives(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/key-executives/" + symbol + "?apikey=" + _apiKey;
        std::string key = "key-executives" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getSharesFloat(const std::string& symbol)
    {
        std::string url = _baseUrl + "v4/shares_float?symbol=" + symbol + "&apikey=" + _apiKey;
        std::string key = "shares_float" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getAllSharesFloat()
    {
        std::string url = _baseUrl + "v4/shares_float/all?apikey=" + _apiKey;
        std::string key = "shares_float_all";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getRating(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/rating/" + symbol + "?apikey=" + _apiKey;
        std::string key = "rating" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getHistoricalRating(const std::string& symbol, uint limit)
    {
        std::string url = _baseUrl + "v3/historical-rating/" + symbol + "?apikey=" + _apiKey + "&limit=" + std::to_string(limit);
        std::string key = "historical-rating" + symbol + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getMarketCap(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/market-capitalization/" + symbol + "?apikey=" + _apiKey;
        std::string key = "market-capitalization" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getDailyMarketCap(const std::string& symbol, uint limit)
    {
        std::string url = _baseUrl + "v3/historical-market-capitalization/" + symbol + "?apikey=" + _apiKey + "&limit=" + std::to_string(limit);
        std::string key = "historical-market-capitalization" + symbol + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getStockNews(const std::vector<std::string>& tickers, uint limit) const
    {
        auto tickerString = _toCommaDelimited(tickers);
        std::string url = _baseUrl + "v3/stock_news?apikey=" + _apiKey + "&limit=" + std::to_string(limit) 
                                   + std::string(tickerString.empty() ? tickerString : "&tickers=" + tickerString);
        std::string key = "stock_news" + tickerString + std::to_string(limit);
        return _getNoCache(url, key);
    }
    
    Document FMPCloudAPI::getEarningsCallTranscripts(const std::string& symbol, uint quarter, uint year)
    {
        std::string url = _baseUrl + "v3/earning_call_transcript/" + symbol + "?apikey=" + _apiKey
                                   + std::string(quarter == 0 ? "" : "&quarter=" + std::to_string(quarter))
                                   + std::string(year == 0 ? "" : "&year=" + std::to_string(year));
        std::string key = "earning_call_transcript" + symbol + std::to_string(quarter) + std::to_string(year);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getEarningsCallTranscriptDates(const std::string& symbol, uint quarter, uint year)
    {
        std::string url = _baseUrl + "v4/earning_call_transcript?symbol=" + symbol 
                                   + "&apikey=" + _apiKey
                                   + std::string(quarter == 0 ? "" : "&quarter=" + std::to_string(quarter))
                                   + std::string(year == 0 ? "" : "&year=" + std::to_string(year));
        std::string key = "earning_call_transcript_dates" + symbol + std::to_string(quarter) + std::to_string(year);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getEarningsCallTranscriptsForYear(const std::string& symbol, uint year)
    {
        std::string url = _baseUrl + "v4/batch_earning_call_transcript/" + symbol 
                                   + "?apikey=" + _apiKey
                                   + std::string(year == 0 ? "" : "&year=" + std::to_string(year));
        std::string key = "batch_earning_call_transcript" + symbol + std::to_string(year);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getEarningsSurprises(const std::string& symbol)
    {
        std::string url = _baseUrl + "v3/earnings-surpises/" + symbol + "?apikey=" + _apiKey;
        std::string key = "earnings-surpises" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getSECFilings(const std::string& symbol, const std::string& type, uint limit)
    {
        std::string url = _baseUrl + "v3/sec_filings/" + symbol + "?apikey=" + _apiKey
                                   + std::string(type.empty() ? "" : "&type=" + type)
                                   + "&limit=" + std::to_string(limit);
        std::string key = "sec_filings" + symbol + type + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getPressReleases(const std::string& symbol, uint limit)
    {
        std::string url = _baseUrl + "v3/press-releases/" + symbol + "?apikey=" + _apiKey + "&limit=" + std::to_string(limit);
        std::string key = "press-releases" + symbol + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getFinancialReportDates(CString& symbol)
    {
        std::string url = _baseUrl + "v4/financial-reports-dates?symbol=" + symbol + "&apikey=" + _apiKey;
        std::string key = "financial-reports-dates" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::get10xReports(CString& symbol, uint year, const std::string& period)
    {
        // valid periods: FY, Q1, Q2, Q3, Q4
        std::string url = _baseUrl + "v3/financial-reports-json?symbol=" + symbol + "&apikey=" + _apiKey
                                   + "&year=" + std::to_string(year)
                                   + "&period=" + period;
        std::string key = "financial-reports-json" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getStockPeers(CString& symbol)
    {
        std::string url = _baseUrl + "v4/stock_peers?symbol=" + symbol + "&apikey=" + _apiKey;
        std::string key = "stock_peers" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getFailToDeliver(CString& symbol)
    {
        std::string url = _baseUrl + "v4/fail_to_deliver?symbol=" + symbol + "&apikey=" + _apiKey;
        std::string key = "fail_to_deliver" + symbol;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    ////////////
    // Misc.
    ////////////
    Document FMPCloudAPI::getBatchEODPrices(CStringVector& tickerList, CString& date)
    {
        auto tickerString = _toCommaDelimited(tickerList);
        std::string url = _baseUrl + "v3/batch-request-end-of-day-prices" 
                + std::string(tickerList.empty() ? "" : "/" + tickerString)
                + "?apikey=" + _apiKey
                + std::string(date.empty() ? "" : "&date=" + date);
        std::string key = "batch_eod_prices_" + date + tickerString;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getStockMarketPerformers(STOCK_MARKET_TOP smt)
    {
        std::string top {""};
        switch (smt)
        {
            case ACTIVE:
                top = "actives";
                break;
            case LOSERS:
                top = "losers";
                break;
            case GAINERS:
                top = "gainers";
                break;
            default:
                break;
        }
        std::string url = _baseUrl + "v3/" + top + "?apikey=" + _apiKey;
        return _returnFromAndUpdateCache(url, top, top, LONG);
    }
    
    Document FMPCloudAPI::getSectorPerformance(bool historical, uint limit)
    {
        std::string url = _baseUrl + "/v3" 
                + std::string(historical ? "historical-" : "") + "sectors-performance?"
                + "limit=" + std::to_string(limit)
                + "&apikey=" + _apiKey;
        std::string key = std::string(historical ? "historical_" : "") + "sectors_performance" + std::to_string(limit);
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getTechnicalIndicatorData(CString& symbol, TIME time, INDICATOR_TYPE type, uint numPeriods)
    {
        std::string typeString {""}, timeString {""};
        switch(time)
        {
            case DAILY:
                timeString = "daily";
                break;
            case ONE_MINUTE:
                timeString = "1min";
                break;
            case FIVE_MINUTE:
                timeString = "5min";
                break;
            case FIFTEEN_MINUTE:
                timeString = "15min";
                break;
            case ONE_HOUR:
                timeString = "1hour";
                break;
            case FOUR_HOUR:
                timeString = "4hour";
                break;
            case THIRTY_MINUTE:
                timeString = "30min";
                break;
            default:
                return {};
        }

        switch(type)
        {
            case EMA:
                typeString = "ema";
                break;
            case SMA:
                typeString = "sma";
                break;
            case WMA:
                typeString = "wma";
                break;
            case DEMA:
                typeString = "dema";
                break;
            case TEMA:
                typeString = "tema";
                break;
            case WILLIAMS:
                typeString = "williams";
                break;
            case RSI:
                typeString = "rsi";
                break;
            case ADX:
                typeString = "adx";
                break;
            case STDDEV:
                typeString = "standardDeviation";
                break;
            default:
                return {};
        }
        
        std::string url = _baseUrl + "v3/technical_indicator/" + timeString + "/" + symbol + "?apikey=" + _apiKey
                        + "&period=" + std::to_string(numPeriods) + "&type=" + typeString;
        std::string key = "technical_indicator" + timeString + symbol + std::to_string(numPeriods) + typeString;
        return _returnFromAndUpdateCache(url, key, key, SHORT);
    }
    
    Document FMPCloudAPI::getSpecificStandardIndustrialClassification(CString& cik, CString& symbol, CString& sicCode)
    {
        if(cik.empty() && symbol.empty() && sicCode.empty())
            return {};
        
        std::string url = _baseUrl + "v4/standard_industrial_classification?apikey=" + _apiKey
                + std::string(cik.empty() ? "" : "cik=" + cik)
                + std::string(symbol.empty() ? "" : "symbol=" + symbol)
                + std::string(sicCode.empty() ? "" : "sicCode=" + sicCode);
        std::string key = "standard_industrial_classification" + cik + symbol + sicCode;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getAllStandardIndustrialClassification()
    {
        std::string url = _baseUrl + "v4/standard_industrial_classification/all?apikey=" + _apiKey;
        std::string key = "standard_industrial_classification_all";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }
    
    Document FMPCloudAPI::getAllStandardIndustrialClassificationList()
    {
        std::string url = _baseUrl + "v4/standard_industrial_classification_list?apikey=" + _apiKey;
        std::string key = "standard_industrial_classification_list";
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    Document FMPCloudAPI::getSpecificStandardIndustrialClassificationList(CString& industry, CString& sicCode)
    {
        if(industry.empty() && sicCode.empty())
            return {};
        std::string url = _baseUrl + "v4/standard_industrial_classification_list?apikey=" + _apiKey
                + std::string(industry.empty() ? "" : "&industry=" + industry)
                + std::string(sicCode.empty() ? "" : "&sicCode=" + sicCode);
        std::string key = "standard_industrial_classification_list" + industry + sicCode;
        return _returnFromAndUpdateCache(url, key, key, LONG);
    }

    /** END ENDPOINTS **/
}

#endif