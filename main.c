/*
 * compile for debug (and launch): gcc -ggdb -o0 *.c -llua53 -L. -o lua-test.exe && lua-test
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* the Lua interpreter */
lua_State* L;

/*
 * function called from Lua
 * first parameter (compulsory): request string
 * second parameter (optional): one-based field number (int)
 * note: if no field number is provided or if it is out of bounds (e.g. 0)
 *       then returns all fields (multiple values returned)
 */
int getValueFromController(lua_State *L)
{
  const char* sysvarData[] = {"1", "9", "16", "22"};
  const char* systemData[] = {"16", "22", "119"};
  const char* nominalData[] = {"44", "4", "155", "12", "27"};
  const char** pData = NULL;
  int nbValues; /* number of returned values */
  int nbArgs = lua_gettop(L); /* number of args passed */
  int fieldNumber = 0; /* one-based */
  const char* input = lua_tolstring(L, 1, NULL);
  if(nbArgs>1)
    fieldNumber = (int)lua_tointeger(L, 2);
  if(input==NULL) {
    lua_pushstring(L, "#ERROR/UNEXPECTED");
    nbValues=1;
  } else if(!strcmp(input, "RDAT/SYSVAR")) {
    pData = sysvarData;
    nbValues = sizeof(sysvarData) / sizeof(const char*);
  } else if(!strcmp(input, "RDAT/SYSTEM")) {
    pData = systemData;
    nbValues = sizeof(systemData) / sizeof(const char*);
    nbValues=3;
 } else if(!strcmp(input, "RDAT/NOMINAL")) {
    pData = nominalData;
    nbValues = sizeof(nominalData) / sizeof(const char*);
 } else {
    lua_pushstring(L, "#ERROR/NOT-FOUND");
    nbValues=1;
 }
 if(pData!=NULL) {
   if(fieldNumber < 1 || fieldNumber > nbValues) {
     for(fieldNumber=0; fieldNumber<nbValues; fieldNumber++)
       lua_pushstring(L, pData[fieldNumber]);
   } else {
     lua_pushstring(L, pData[fieldNumber-1]);
     nbValues = 1;
   }
 }
 return nbValues; /* number of values pushed to stack */
}

int main(int argc, char*argv[])
{
  L = luaL_newstate(); /* Create Lua state variable */
  luaL_openlibs(L);    /* Load Lua libraries */

  /* register C function */
  lua_register(L, "getValueFromController", getValueFromController);

  /*load script */
  if(luaL_dofile(L, "test1.lua")) {
    printf("failed to load script");
    return 1;
  }
  
  /* first the lua function to call */
  lua_getglobal(L, "process");
  /* then the arguments */
  lua_pushliteral(L, "RDAT");
  lua_pushliteral(L, "SYSTEM");
  /* call the function with number of arguments and number of returned values */
  lua_call(L, 2, 2);
  printf("Result: sum=%i, average=%.2f\n", (int)lua_tointeger(L, -2), (double)lua_tonumber(L, -1));

  /* cleanup Lua */
  lua_close(L);

  return 0;
 }
