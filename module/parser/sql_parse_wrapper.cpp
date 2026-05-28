#include "sql_parser.h"
#include "sql_parser.tab.hpp"

extern int yyparse();
extern std::vector<sql::Command> parsedCommands;

struct yy_buffer_state;
typedef yy_buffer_state* YY_BUFFER_STATE;

extern YY_BUFFER_STATE yy_scan_string(const char*);
extern void yy_delete_buffer(YY_BUFFER_STATE);

bool parse_sql(const std::string& text, std::vector<sql::Command>& out) {
    parsedCommands.clear();

    YY_BUFFER_STATE buffer = yy_scan_string(text.c_str());
    int result = yyparse();
    yy_delete_buffer(buffer);

    if (result != 0) {
        return false;
    }

    out = parsedCommands;
    return true;
}