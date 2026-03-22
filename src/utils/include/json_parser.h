#pragma once

#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <string_view>
#include <tuple>

using json = nlohmann::json;

inline std::tuple<std::string, std::string, std::string>
log_config_parser(std::string_view input) {
  try {
    json j = json::parse(input);

    std::string app_name = j.at("app_name").get<std::string>();
    std::string log_level = j.at("level").get<std::string>();
    std::string pid = j.at("pid").get<std::string>();
    return {app_name, log_level, pid};
  } catch (const json::parse_error &e) {
    std::cerr << "JSON parse error: " << e.what() << "\n";
  } catch (const json::exception &e) {
    std::cerr << "JSON access/type error: " << e.what() << "\n";
  }

  return {"", "", ""};
}
