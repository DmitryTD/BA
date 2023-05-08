#include "lib.cpp"

#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <vector>

int main(void)
{
    //получаем список доступных пакетов
    std::string response_temp;
    std::string branch = "get";
    get_response(response_temp, branch);
    
    std::vector <std::string> branch_list;
    nlohmann::json json_branch = nlohmann::json::parse(response_temp);

    //доступные пакеты записываются в branch_list
    response_temp = json_branch["validation_message"][1];
    while(!(response_temp.find("\'") == std::string::npos)){       
        response_temp.erase(0, response_temp.find("\'") + 1);
        branch_list.push_back(response_temp.substr(0, response_temp.find("\'")));
        response_temp.erase(0, response_temp.find("\'") + 1);
    }
    
    for(int i = 0; i < branch_list.size(); ++i){
        std::cout << branch_list[i] << std::endl;
    }



    return 0;
}