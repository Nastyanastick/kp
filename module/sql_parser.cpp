#include "sql_parser.h"
#include "sql_ast.h"
#include "sql_parser.tab.hpp"
#include <vector>
#include <string>
#include <cstdio>

extern std::vector<sql::Command> parsedCommands;
extern FILE* yyin;

bool parse_sql(const std::string& text, std::vector<sql::Command>& out) {
    parsedCommands.clear();
    
    // Записываем текст во временный файл
    FILE* tmp = tmpfile();
    if (!tmp) {
        return false;
    }
    
    fwrite(text.c_str(), 1, text.size(), tmp);
    rewind(tmp);
    
    yyin = tmp;
    
    int result = yyparse();
    
    fclose(tmp);
    yyin = stdin;
    
    if (result == 0) {
        out = parsedCommands;
        return true;
    }
    
    return false;
}