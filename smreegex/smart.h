#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <boost/config.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

std::string replace_dot_to_comma(std::string);
std::string get_smart_str(std::string, std::string, std::string);
std::string get_command_str(std::string);
std::string get_smart_str_autodiscovery(std::string smart_expath);
std::string m2converter(std::string);
bool search_smart_id_value(std::string, int, std::string, std::string*);

