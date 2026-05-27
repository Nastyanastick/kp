%{
#include "sql_parser.tab.hpp"
#include <string.h>
#include <stdlib.h>

static char* strdupcanon(const char* s) {
    if (!s) return nullptr;
    size_t l = strlen(s);
    char* r = (char*)malloc(l + 1);
    memcpy(r, s, l + 1);
    return r;
}
%}

%option noyywrap

DIGIT      [0-9]
ID         [A-Za-z_][A-Za-z0-9_]*
STRINGCHAR [^\"]*

%%
[ \t\r\n]+               ;

"=="                    { return EQ; }
"!="                    { return NEQ; }
"<="                    { return LE; }
">="                    { return GE; }
"<"                     { return LT; }
">"                     { return GT; }
","                     { return ','; }
"("                     { return '('; }
")"                     { return ')'; }
"*"                     { return '*'; }
"="                     { return '='; }
";"                     { return ';'; }

"CREATE"|"create"      { return CREATE; }
"DROP"|"drop"          { return DROP; }
"DATABASE"|"database"  { return DATABASE; }
"USE"|"use"            { return USE; }
"TABLE"|"table"        { return TABLE; }
"INSERT"|"insert"      { return INSERT; }
"INTO"|"into"          { return INTO; }
"VALUES"|"values"      { return VALUES; }
"VALUE"|"value"        { return VALUES; }
"SELECT"|"select"      { return SELECT; }
"FROM"|"from"          { return FROM; }
"WHERE"|"where"        { return WHERE; }
"UPDATE"|"update"      { return UPDATE; }
"SET"|"set"            { return SET; }
"DELETE"|"delete"      { return DELETE; }
"AS"|"as"              { return AS; }
"BETWEEN"|"between"    { return BETWEEN; }
"LIKE"|"like"          { return LIKE; }
"AND"|"and"            { return AND; }
"OR"|"or"              { return OR; }
"NULL"|"null"          { return NULL_TOKEN; }

{ID}                    { yylval.str = strdupcanon(yytext); return IDENTIFIER; }

\"{STRINGCHAR}\"        {
                            int len = yyleng;
                            char* s = (char*)malloc(len - 1);
                            memcpy(s, yytext + 1, len - 2);
                            s[len - 2] = '\0';
                            yylval.str = s;
                            return STRING_LITERAL;
                         }

-?{DIGIT}+              { yylval.str = strdupcanon(yytext); return NUMBER; }

.                       { return yytext[0]; }
%%
