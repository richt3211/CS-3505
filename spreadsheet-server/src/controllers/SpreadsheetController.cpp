#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>
#include <set>
#include <algorithm>
#include "SpreadsheetController.h"
#include "../../libs/json.hpp"
#include "../models/UserModel.h"

using json = nlohmann::json;

std::vector<std::string> split(std::string s, std::string delimiter);
// std::string check_type
std::string SpreadsheetController::get_list_of_spreadsheets(std::set<std::shared_ptr<SpreadsheetModel>> spreadsheets)
{
    std::cout << "Getting list of spreadsheets in SpreadsheetController" << std::endl;
    json j_spreadsheet_list;
    j_spreadsheet_list["type"] = "list";
    j_spreadsheet_list["spreadsheets"] = json::array();
    for (std::shared_ptr<SpreadsheetModel> sm : spreadsheets)
    {
        std::string name = sm->get_name();
        std::cout << "pushing spreadsheet " << name << " to json object" << std::endl;
        j_spreadsheet_list["spreadsheets"].push_back(name);
        std::cout << "successfully pushed to json arrar" << std::endl;
    }
    return j_spreadsheet_list.dump();
}

std::string SpreadsheetController::get_list_of_users(std::vector<UserModel> users)
{
    json j_user_list;
    j_user_list["type"] = "list";
    j_user_list["users"] = json::array();
    for (UserModel um : users)
    {
        j_user_list["users"].push_back(um.name);
    }
    return j_user_list.dump(); 
}

std::string SpreadsheetController::full_send(std::unordered_map<std::string, std::shared_ptr<Cell>> & cell_dictionary)
{
    json ss;
    json cells;
    if (cell_dictionary.begin() == cell_dictionary.end())
    {
        // std::cout << "setting cells to empty object " << std::endl;
        cells = json({});
    }
    else
    {
        for (std::pair<std::string, std::shared_ptr<Cell>> cell : cell_dictionary)
        {
            // std::cout << "entered loop in full send " << std::endl;
            std::string name = cell.second->get_name();
            std::string contents = cell.second->get_contents();
            std::string type = cell.second->get_type();
            if (type == "int")
            {
                cells[name] = stoi(contents);
            }
            else if (type == "double")
            {
                std::cout << "creating full send with contents: " << contents << std::endl;
                cells[name] = stod(contents);
            }
            else
            {
                cells[name] = contents;
            }
        }
    }
    
    ss["type"] = "full send";
    ss["spreadsheet"] = cells;
    return ss.dump();
}


bool SpreadsheetController::validate_admin(json message, std::string &error_message)
{
    if (!message.contains("type")|| message["type"]!="Admin") return false;
    if (message["type"] != "Admin")
    {
        error_message = "not an Admin user";
        return false;
    }
    
    return true;
}

bool SpreadsheetController::validate_login_message(json & message)
{
    if (!message.contains("username") || !message.contains("password") || !message.contains("type") || !message.contains("name"))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool SpreadsheetController::check_if_spreadsheet_in_storage(json & message, std::string & spreadsheet)
{
    //mu_lock_file_spreadsheet_txt.lock();
    std::ifstream file("../../data/spreadsheets.txt");
    std::string line;
    std::vector<std::string> spreadsheet_names;
    int count = 0;
    while (std::getline(file, line))
    {
        if (message["name"] == line)
        {
            spreadsheet = line;
            //mu_lock_file_spreadsheet_txt.unlock();
            return true;
        }
    }
    //mu_lock_file_spreadsheet_txt.unlock();
    return false;
}



// LOOK AT THIS AGAIN
bool SpreadsheetController::handle_edit_message(json & message, std::shared_ptr<SpreadsheetModel> sm)
{
    if (!message.contains("type")) return false;
    std::string type = message.at("type");
    if (type == "edit") return handle_edit(message, sm);
    else if (type == "undo") return handle_undo(message, sm);
    else if (type == "revert") return handle_revert(message, sm);
}

bool SpreadsheetController::handle_edit(json & message, std::shared_ptr<SpreadsheetModel> sm)
{
    // std::cout << message.value("cell", "-1")  << message.value("value", "-1") << std::endl;
    if (!message.contains("cell") || !message.contains("value") || !message.contains("dependencies"))
    {
        // std::cout << "edit message is invalid" << std::endl;
        return false;
    }
    else
    {
        std::cout << "edit message is valid" << std::endl;
        std::string cell = message.value("cell", "-1");
        json value_ = message["value"];
        std::string value;
        std::string type = get_type(value_);
        if (type == "string")
        {
            value = value_.get<std::string>();
        }
        else if (type == "int")
        {
            value = std::to_string(value_.get<int>());
        }
        else if (type == "double")
        {
            value = std::to_string(value_.get<double>());
        }
        
        std::vector<std::string> dependents = message["dependencies"].get<std::vector<std::string>>();
        sm->do_edit(cell, value, dependents, type);
        return true;
    }
    
}

bool SpreadsheetController::handle_undo(json & message, std::shared_ptr<SpreadsheetModel> sm)
{
    sm->do_undo();
    return true;
}

bool SpreadsheetController::handle_revert(json & message, std::shared_ptr<SpreadsheetModel> sm)
{
    std::string cell = message.value("cell", "-1");
    sm->do_revert(cell);
    return true;
}

std::string SpreadsheetController::create_type_1_error()
{
    json message = {
        {"type", "error"},
        {"code", 1},
        {"source", " "}};
    return message.dump();
}

std::string SpreadsheetController::create_type_2_error(std::string name)
{
    json message = {
        {"type", "error"},
        {"code", 2},
        {"source", name}};
    return message.dump();
}

std::string SpreadsheetController::get_type(json &message)
{
    if (message.type() == json::value_t::string)
    {
        return "string";
    }
    else if (message.type() == json::value_t::number_integer)
    {
        return "int";
    }
    else if (message.type() == json::value_t::number_float)
    {
        return "double";
    }
}

std::vector<std::string> SpreadsheetController::split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
