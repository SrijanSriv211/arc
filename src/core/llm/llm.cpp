#include "arcpch.h"
#include "arc.h"
#include "llm.h"

#include "settings/settings.h"
#include "console/console.h"

#include "libcurl/include/curl/curl.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace llm
{
    // global vector to maintain conversation history
    std::vector<json> conversation = {
        {
            {"role", "system"},
            {"content", "you are a helpful assistant."}
        }
    };

    // callback function to process streamed data chunk-by-chunk
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        size_t total_size = size * nmemb;
        std::string chunk((char*)contents, total_size);

        std::istringstream stream(chunk);
        std::string assistant_response;
        std::string line;

        while (std::getline(stream, line))
        {
            if (line.empty() || line == "data: [DONE]")
                continue;

            // remove "data: " from the beginning
            if (line.find("data: ") == 0)  
                line = line.substr(6);

            try
            {
                json response = json::parse(line);
                if (response.contains("choices") && response["choices"].size() > 0)
                {
                    if (response["choices"][0].contains("delta") && response["choices"][0]["delta"].contains("content"))
                    {
                        std::string content = response["choices"][0]["delta"]["content"];
                        console::print(content, console::GRAY, false, true);
                        assistant_response += content;
                    }
                }
            }

            catch (json::parse_error& e)
            {
                // std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            }
        }

        // append the assistant response to conversation history
        if (!assistant_response.empty())
            conversation.push_back({{"role", "assistant"}, {"content", assistant_response}});

        return total_size;
    }

    void generate(const std::string& input, const std::string& model, const std::string& api_key)
    {
        CURL* curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (!curl)
        {
            curl_global_cleanup();
            return;
        }

        struct curl_slist* headers = NULL;
        std::string auth_header = "Authorization: Bearer " + api_key;
        headers = curl_slist_append(headers, auth_header.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string cacert_path = arc::get_root_path() + "\\cacert.pem";
        curl_easy_setopt(curl, CURLOPT_CAINFO, cacert_path.c_str());

        // append input to conversation history
        conversation.push_back({{"role", "user"}, {"content", input}});

        // create request payload with full conversation history
        json payload = {
            {"model", model},
            {"stream", true},
            {"messages", conversation}
        };

        std::string payload_str = payload.dump();

        // set up CURL request
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.groq.com/openai/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());

        // enable streaming response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

        // perform request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "\nCURL Error: " << curl_easy_strerror(res) << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        std::cout << std::endl;
    }
}
