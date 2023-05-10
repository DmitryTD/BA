#include "lib.cpp"

#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int main(void)
{
    //получаем список доступных пакетов
    std::string response_temp;
    std::string branch = "get";
    get_response(response_temp, branch);
    
    std::vector <std::string> branch_list;
    nlohmann::json json_branch_first = nlohmann::json::parse(response_temp);
    nlohmann::json json_branch_second;

    //доступные пакеты записываются в branch_list
    response_temp = json_branch_first["validation_message"][1];
    while(!(response_temp.find("\'") == std::string::npos)){       
        response_temp.erase(0, response_temp.find("\'") + 1);
        branch_list.push_back(response_temp.substr(0, response_temp.find("\'")));
        response_temp.erase(0, response_temp.find("\'") + 1);
    }
    
    
    //выбор списков пакетов
    int choice_first, choice_second, exit = 1;

    while(exit){
        std::cout << "Выберите первую ветку для сравнения" << std::endl;
        for(int i = 0; i < branch_list.size(); ++i) {
            std::cout << i + 1 << " " << branch_list[i] << std::endl;
        }
        std::cin >> choice_first;

        
        std::cout << "Выберите вторую ветку для сравнения" << std::endl;
        for(int i = 0; i < branch_list.size(); ++i) {
            std::cout << i + 1 << " " << branch_list[i] << std::endl;
        }
        std::cin >> choice_second;

        //Получение данных
        std::cout << "Получение данных..." << std::endl;
        if(choice_first > 0 && choice_first <= branch_list.size()
        && choice_second > 0 && choice_second <= branch_list.size()) {
            exit = 0;
            response_temp.clear();
            get_response(response_temp, branch_list[choice_first - 1]);
            json_branch_first = nlohmann::json::parse(response_temp);
            response_temp.clear();
            get_response(response_temp, branch_list[choice_second - 1]);
            json_branch_second = nlohmann::json::parse(response_temp);
        }
        else {
            std::cout << "Ошибка" << std::endl;
        }
    }

    //Обработка данных
    std::cout << "Обработка данных..." << std::endl;
    
    nlohmann::json task_first;
    nlohmann::json task_second;
    nlohmann::json task_third;
    int pointer_first = 0, pointer_second = 0;
    int length_first = json_branch_first["length"];
    int length_second = json_branch_second["length"];

    exit = 1;
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
            else {
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

    //запись результатов
    nlohmann::json result;
    result["first_task"] = task_first;
    result["second_task"] = task_second;
    result["third_task"] = task_third;

    //std::cout << result << std::endl;
    std::ofstream out;
    out.open("Result.json");
    if (out.is_open())
    {
        out << result << std::endl;
    }
    out.close(); 

    return 0;
}