#include <curl/curl.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

using namespace std;

const char* webhookUrl = "hahaha";

void post(const char *content)
{
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, "Content-Type: multipart/form-data");

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        struct curl_httppost *post = NULL;
        struct curl_httppost *last = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, webhookUrl);
        
        string foo = "username=c'est ne pas peep? :O&content=";

        foo.append(content);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, foo.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}
