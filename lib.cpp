#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>


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



void get_branch_list(std::vector <std::string>& array, nlohmann::json& json_branch){
    std::string response = json_branch["validation_message"][1];
    while(!(response.find("\'") == std::string::npos))
    {       
        response.erase(0, response.find("\'") + 1);
        array.push_back(response.substr(0, response.find("\'")));
        response.erase(0, response.find("\'") + 1);
    }    
}


nlohmann::json compare_branches(nlohmann::json& json_branch_first, nlohmann::json& json_branch_second){
    
    nlohmann::json task_first;
    nlohmann::json task_second;
    nlohmann::json task_third;
    int pointer_first = 0, pointer_second = 0;
    int length_first = json_branch_first["length"];
    int length_second = json_branch_second["length"];

    int exit = 1;
    while(exit){
        nlohmann::json temp_json;
        if(pointer_first < length_first || pointer_second < length_second)
        {
            if(json_branch_first["packages"][pointer_first]["arch"] == json_branch_second["packages"][pointer_second]["arch"]){
                if (json_branch_first["packages"][pointer_first]["name"] == json_branch_second["packages"][pointer_second]["name"])
                {       
                    if(json_branch_first["packages"][pointer_first]["release"] > json_branch_second["packages"][pointer_second]["release"])
                    {
                        temp_json["bigger_version-release"] = json_branch_first["packages"][pointer_first]["name"];
                        temp_json["arch"] = json_branch_first["packages"][pointer_first]["arch"];
                        task_third.push_back(temp_json);
                    }
                    ++pointer_first;
                    ++pointer_second;
                }
                else if (json_branch_first["packages"][pointer_first]["name"] > json_branch_second["packages"][pointer_second]["name"])
                {
                    temp_json["name"] = json_branch_second["packages"][pointer_second]["name"];
                    temp_json["arch"] = json_branch_second["packages"][pointer_second]["arch"];
                    task_second.push_back(temp_json);
                    ++pointer_second;
                }
                else if (json_branch_first["packages"][pointer_first]["name"] < json_branch_second["packages"][pointer_second]["name"])
                {
                    temp_json["name"] = json_branch_first["packages"][pointer_first]["name"];
                    temp_json["arch"] = json_branch_first["packages"][pointer_first]["arch"];
                    task_first.push_back(temp_json);
                    ++pointer_first;
                }
            }
            else 
            {
                if(json_branch_first["packages"][pointer_first]["arch"] > json_branch_second["packages"][pointer_second]["arch"])
                {
                    temp_json["name"] = json_branch_second["packages"][pointer_second]["name"];
                    temp_json["arch"] = json_branch_second["packages"][pointer_second]["arch"];
                    task_second.push_back(temp_json);
                    ++pointer_second;
                }
                if(json_branch_first["packages"][pointer_first]["arch"] < json_branch_second["packages"][pointer_second]["arch"])
                {
                    temp_json["name"] = json_branch_first["packages"][pointer_first]["name"];
                    temp_json["arch"] = json_branch_first["packages"][pointer_first]["arch"];
                    task_first.push_back(temp_json);
                    ++pointer_first;
                }
            }
        } 
        else {
            if(pointer_first == length_first && pointer_second == length_second){
                exit = 0;
            }
            else if(pointer_first == length_first){
                temp_json["name"] = json_branch_second["packages"][pointer_second]["name"];
                temp_json["arch"] = json_branch_second["packages"][pointer_second]["arch"];
                task_second.push_back(temp_json);
                ++pointer_second;
            } 
            else if(pointer_second == length_second){
                temp_json["name"] = json_branch_first["packages"][pointer_first]["name"];
                temp_json["arch"] = json_branch_first["packages"][pointer_first]["arch"];
                task_first.push_back(temp_json);
                ++pointer_first;
            }
        }
    }
    

    nlohmann::json result;
    result["first_task"] = task_first;
    result["second_task"] = task_second;
    result["third_task"] = task_third;

    return result;
}