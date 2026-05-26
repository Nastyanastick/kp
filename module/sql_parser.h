#pragma once
#include <string>
#include <vector>
#include "sql_ast.h"

bool parse_sql(const std::string& text, std::vector<sql::Command>& out);