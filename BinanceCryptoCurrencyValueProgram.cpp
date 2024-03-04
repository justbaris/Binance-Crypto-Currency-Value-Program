#include <iostream>
#include <string>
#include <map>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

class BinanceAPI {
public:
    BinanceAPI() : api_key(""), secret_key("") {}

    void setAPIKeys(const std::string& api_key, const std::string& secret_key) {
        this->api_key = api_key;
        this->secret_key = secret_key;
    }

    double getCoinPrice(const std::string& symbol) {
        std::string url = "https://api.binance.com/api/v3/ticker/price?symbol=" + symbol;
        CURL* curl = curl_easy_init();
        std::string readBuffer;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                Json::Reader reader;
                Json::Value root;
                if (reader.parse(readBuffer, root)) {
                    double price = root["price"].asDouble();
                    return price;
                }
            }
        }
        return -1; // Error
    }

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
        size_t totalSize = size * nmemb;
        buffer->append((char*)contents, totalSize);
        return totalSize;
    }

    std::string api_key;
    std::string secret_key;
};

int main() {
    BinanceAPI binanceAPI;
    std::string api_key = "YOUR_BINANCE_API_KEY";
    std::string secret_key = "YOUR_BINANCE_SECRET_KEY";
    binanceAPI.setAPIKeys(api_key, secret_key);

    std::string symbol;
    double amount;

    std::cout << "Enter the symbol or full name of the cryptocurrency: ";
    std::cin >> symbol;
    std::cout << "Enter the amount of the cryptocurrency: ";
    std::cin >> amount;

    double price = binanceAPI.getCoinPrice(symbol);
    if (price != -1) {
        double totalValue = price * amount;
        std::cout << "Total value of " << amount << " " << symbol << " is: $" << totalValue << std::endl;
    } else {
        std::cout << "Error fetching price for " << symbol << std::endl;
    }

    return 0;
}

