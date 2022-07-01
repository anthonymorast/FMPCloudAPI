#ifndef FMPCLOUDAPI
#define FMPCLOUDAPI

#include "rapidjson/document.h"
#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <functional>
#include <stdio.h>
#include <ctime>
#include <map>

using namespace rapidjson;

namespace FMP
{
    struct FMPCloudAPIError : public std::runtime_error
    {
        ~FMPCloudAPIError() {}
        FMPCloudAPIError(const std::string error) : std::runtime_error(error) {}
    };

    enum CACHE_LENGTH { SHORT, LONG };

    struct FMPCloudAPI
    {
        FMPCloudAPI(std::string apiKey) : _apiKey(apiKey) {}

        Document getStockQuote(const std::string& symbol) const;                // no cache
        Document getStockQuote(const std::vector<std::string>& symbols) const;  // no cache
        Document getAvailableCrypto();                                          // long cache
        Document getCryptoPrices() const;                                       // no cache
        Document getAvailableForex();                                           // long cache
        Document getAllForexPrices() const;                                     // no cache

        // on construction, create a timer to periodically expire the caches
        // ideally, there would be 2 or 3 caches, short, medium, and long or short and long, 
        // which are all expired at different intervals.
        void expireCache() { _expireLongCache(); _expireShortCache(); };
        private:
            std::string _apiKey;
            std::string _baseUrl {"https://fmpcloud.io/api/"};
            std::map<std::string, std::string> _longJsonContentsCache; 
            std::map<std::string, std::string> _shortjsonContentsCache;

            std::string _toCommaDelimited(const std::vector<std::string>& stringVector) const;
            std::string _downloadAndGetJsonFile(const std::string& url, const std::string& filename) const;    // download data, return as string
            std::string _generateFilename(const std::string& endpoint) const;
            Document _return(const std::string& contents) const;

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

    Document FMPCloudAPI::getStockQuote(const std::string& symbol) const
    {
        std::vector<std::string> symbols { symbol };
        return getStockQuote(symbols);
    }

    Document FMPCloudAPI::getStockQuote(const std::vector<std::string>& symbols) const
    {
        std::string url = _baseUrl + "v3/quote/" + _toCommaDelimited(symbols) + "?apikey=" + _apiKey;
        std::string filename = _generateFilename("quote");
        std::string contents = _downloadAndGetJsonFile(url, filename);
        return _return(contents);
    }

    Document FMPCloudAPI::getAvailableCrypto()
    {
        CACHE_LENGTH duration = LONG;
        std::string contents = _getCachedContents("available crypto", duration);
        if(contents.empty()) 
        {
            std::string url = _baseUrl + "v3/symbol/available-cryptocurrencies?apikey=" + _apiKey;
            std::string filename = _generateFilename("available_crypto");
            contents = _downloadAndGetJsonFile(url, filename);
            _cache("available crypto", contents, duration);
        }
        return _return(contents);
    }

    Document FMPCloudAPI::getCryptoPrices() const
    {
        std::string url = _baseUrl + "v3/quotes/crypto?apikey=" + _apiKey;
        std::string filename = _generateFilename("crypto_quotes");
        std::string contents = _downloadAndGetJsonFile(url, filename);
        return _return(contents);
    }

    Document FMPCloudAPI::getAvailableForex()
    {
        
    }
    
    Document FMPCloudAPI::getAllForexPrices() const
    {

    }

}

#endif