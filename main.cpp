#include "lib.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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
       
    //selecting branches for comparison
    int choice_first, choice_second, exit = 1;

    while(exit){
        std::cout << "Select the first branch for comparison (only the number before the name)" << std::endl;
        show_array(branch_list);
        std::cin >> choice_first;

        
        std::cout << "Select the second branch for comparison (only the number before the name)" << std::endl;
        show_array(branch_list);
        std::cin >> choice_second;

        //get data
        std::cout << "Getting data..." << std::endl;
        if(choice_first > 0 && choice_first <= branch_list.size()
        && choice_second > 0 && choice_second <= branch_list.size()) 
        {
            exit = 0;
            response_temp.clear();
            branch = branch_list[choice_first - 1];
            get_response(response_temp, branch);
            json_branch_first = nlohmann::json::parse(response_temp);
            
            response_temp.clear();
            branch = branch_list[choice_second - 1];
            get_response(response_temp, branch);
            json_branch_second = nlohmann::json::parse(response_temp);
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }

    //Data processing
    std::cout << "Data processing..." << std::endl;
    nlohmann::json result = compare_branches(json_branch_first, json_branch_second);

    std::ofstream out;
    out.open("Result.json");
    if (out.is_open())
    {
        out << result << std::endl;
    }
    out.close(); 
    std::cout << "The result is written in Result.json" << std::endl;
    return 0;
}