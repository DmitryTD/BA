#include "lib.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>


void write_result(std::string file_name, nlohmann::json& data){
    std::ofstream out;
    out.open(file_name);
    if (out.is_open())
    {
        out << data << std::endl;
    }
    out.close(); 
}

void select_branches(int& first, int& second, std::vector <std::string>& array){
    std::cout << "Select the first branch for comparison (only the number before the name)" << std::endl;
    for(unsigned int i = 0; i < array.size(); ++i) {
        std::cout << i + 1 << " " << array[i] << std::endl;
    }
    std::cin >> first;

        
    std::cout << "Select the second branch for comparison (only the number before the name)" << std::endl;
    for(unsigned int i = 0; i < array.size(); ++i) {
        std::cout << i + 1 << " " << array[i] << std::endl;
    }
    std::cin >> second;
}

void get_data(int& choice_first, int& choice_second, nlohmann::json& first, nlohmann::json& second, std::vector <std::string>& branch_list) {
    //get data
    std::string response_temp;
    std::string branch;
    std::cout << "Getting data..." << std::endl;
    if(choice_first > 0 && choice_first <= branch_list.size()
    && choice_second > 0 && choice_second <= branch_list.size()) 
    {
        response_temp.clear();
        branch = branch_list[choice_first - 1];
        get_response(response_temp, branch);
        first = nlohmann::json::parse(response_temp);
            
        response_temp.clear();
        branch = branch_list[choice_second - 1];
        get_response(response_temp, branch);
        second = nlohmann::json::parse(response_temp);
    }
    else {
        std::cout << "Error" << std::endl;
    }
}

int main(void)
{
    //get a list of available packages
    std::string response_temp;
    std::string branch = "get";
    get_response(response_temp, branch);
 
    std::vector <std::string> branch_list;
    nlohmann::json json_branch_first = nlohmann::json::parse(response_temp);
    nlohmann::json json_branch_second;

    get_branch_list(branch_list, json_branch_first);
       
    //select branches for comparison
    int choice_first, choice_second;
    select_branches(choice_first, choice_second, branch_list);

    //get data
    get_data(choice_first, choice_second, json_branch_first, json_branch_second, branch_list);

    //Data processing
    std::cout << "Data processing..." << std::endl;
    nlohmann::json result = compare_branches(json_branch_first, json_branch_second);

    //Save result
    write_result("Result.json", result);
    std::cout << "The result is written in Result.json" << std::endl;
    return 0;
}