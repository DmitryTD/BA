#include <curl/curl.h>
#include <string>


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string get_response(std::string &response, std::string &branch){
    CURL *curl;
    int res = 0;
    
    std::string request = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch;
    branch.clear();
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror((CURLcode)res));
        }

        curl_easy_cleanup(curl);
  }
  return response;
}