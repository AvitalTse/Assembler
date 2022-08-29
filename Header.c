#include "ConstVal.h"
#include "ConstTab.h"

/**********************************************************************************************/
/****************************** A file to initialize fixed tables******************************/
/**********************************************************************************************/

char const data[MAX_DATA_DIR][MAX_CHARS] =
{{".string"},
 {".struct"},
 {".data"}};
 
  char const ex_en[EX_EN][MAX_CHARS]=
{{".extern"},
 {".entry"}};
 
 
char const code[MAX_CODE_INS][MAX_CHARS]=
{{"mov"},
 {"cmp"},
 {"add"},
 {"sub"},
 {"not"},
 {"clr"},
 {"lea"},
 {"inc"},
 {"dec"},
 {"jmp"},
 {"bne"},
 {"get"},
 {"prn"},
 {"jsr"},
 {"rts"},
 {"hlt"}};
 
  char const regi[MAX_REGIS][MAX_CHARS] =
 {{"r0"},
  {"r1"},
  {"r2"},
  {"r3"},
  {"r4"},
  {"r5"},
  {"r6"},
  {"r7"}};
  
   char const add_method_bin[ADD_METHOD][MAX_CHARS]=
 {{"00"},
  {"01"},
  {"10"},
  {"11"}};
 
 char const code_bin[MAX_CODE_INS][MAX_CHARS]=
{{"0000"},
 {"0001"},
 {"0010"},
 {"0011"},
 {"0100"},
 {"0101"},
 {"0110"},
 {"0111"},
 {"1000"},
 {"1001"},
 {"1010"},
 {"1011"},
 {"1100"},
 {"1101"},
 {"1110"},
 {"1111"}};
 
 char const regi_bin[MAX_REGIS][MAX_CHARS]=
{{"0000"},
 {"0001"},
 {"0010"},
 {"0011"},
 {"0100"},
 {"0101"},
 {"0110"},
 {"0111"}};

 
  char const A_R_E[ARE][MAX_CHARS]=
 {{"00"},
  {"01"},
  {"10"}};
  
  char const CPU_LANG[CPU_BASE]=
  {'!',
   '@',
   '#',
   '$',
   '%',
   '^',
   '&',
   '*',
   '<',
   '>',
   'a',
   'b',
   'c',
   'd',
   'e',
   'f',
   'g',
   'h',
   'i',
   'j',
   'k',
   'l',
   'm',
   'n',
   'o',
   'p',
   'q',
   'r',
   's',
   't',
   'u',
   'v',};
  
 

