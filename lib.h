#ifndef MYLIB_H
#define MYLIB_H

#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>


extern void get_response(std::string &response, std::string &branch);
extern void get_branch_list(std::vector <std::string>& array, nlohmann::json& json_branch);
extern nlohmann::json compare_branches(nlohmann::json& json_branch_first, nlohmann::json& json_branch_second);


#endif /* MYLIB_H */