%{
#include "sql_parser.tab.hpp"
#include <string.h>
#include <ctype.h>

// helper to strdup
static char* strdupcanon(const char* s) {
    if (!s) return nullptr;
    size_t l = strlen(s);
    char* r = (char*)malloc(l+1);
    memcpy(r, s, l+1);
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

"CREATE"                { return CREATE; }
"create"                { return CREATE; }
"DROP"                  { return DROP; }
"drop"                  { return DROP; }
"DATABASE"              { return DATABASE; }
"database"              { return DATABASE; }
"USE"                   { return USE; }
"use"                   { return USE; }
"TABLE"                 { return TABLE; }
"table"                 { return TABLE; }
"INSERT"                { return INSERT; }
"insert"                { return INSERT; }
"INTO"                  { return INTO; }
"into"                  { return INTO; }
"VALUES"                { return VALUES; }
"values"                { return VALUES; }
"VALUE"                 { return VALUE; }
"value"                 { return VALUE; }
"SELECT"                { return SELECT; }
"select"                { return SELECT; }
"FROM"                  { return FROM; }
"from"                  { return FROM; }
"WHERE"                 { return WHERE; }
"where"                 { return WHERE; }
"UPDATE"                { return UPDATE; }
"update"                { return UPDATE; }
"SET"                   { return SET; }
"set"                   { return SET; }
"DELETE"                { return DELETE; }
"delete"                { return DELETE; }
"AS"                    { return AS; }
"as"                    { return AS; }
"BETWEEN"               { return BETWEEN; }
"between"               { return BETWEEN; }
"LIKE"                  { return LIKE; }
"like"                  { return LIKE; }
"AND"                   { return AND; }
"and"                   { return AND; }

{ID}                     { yylval.str = strdupcanon(yytext); return IDENTIFIER; }

\"{STRINGCHAR}\"       { // string literal in double quotes
                            int len = yyleng;
                            // strip quotes
                            char* s = (char*)malloc(len-1);
                            memcpy(s, yytext+1, len-2);
                            s[len-2] = '\0';
                            yylval.str = s;
                            return STRING_LITERAL; }

{DIGIT}+                 { yylval.str = strdupcanon(yytext); return NUMBER; }

"NULL"                  { return NULL_TOKEN; }
"null"                  { return NULL_TOKEN; }

.                        { /* ignore unknown */ }
%%
