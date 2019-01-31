/* 
 * Puppeteer - A Motion Capture Mapping Tool
 * Copyright (c) 2013-2016 Martin Felis <martin.felis@iwr.uni-heidelberg.de>.
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE* 
 */


#include "ExpressionLuaTable.h"

extern "C"
{
#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"
}

#include "luatables.h"
#include "utils/expressions.lua.h"
#include "utils/serialize.lua.h"

void bail(lua_State *L, const char *msg);
std::string get_file_directory (const char* filename);

string LuaParameterExpression::serialize() {
    string result = operation + "(";
    if (operation == "var") {
        result += name;
        result += "=";
        result += to_string(value);
    } else if (operation == "const") {
        result += to_string(value);
    } else {
        int i = 0;
        for (LuaParameterExpression p : parameters) {
            result += p.serialize();
            if (i < parameters.size() - 1) {
                result += ",";
            }
            i++;
        }
    }
    result += ")";
    return result;
}

double LuaParameterExpression::evaluate() {
    if (operation == "var" || operation == "const") {
        return value;
    } else if (operation == "mul") {
        return parameters[0].evaluate() * parameters[1].evaluate();
    } else if (operation == "add") {
        return parameters[0].evaluate() + parameters[1].evaluate();
    } else if (operation == "div") {
        return parameters[0].evaluate() / parameters[1].evaluate();
    } else if (operation == "sub") {
        return parameters[0].evaluate() - parameters[1].evaluate();
    } else if (operation == "mod") {
        return (int) parameters[0].evaluate() % (int) parameters[1].evaluate();
    } else if (operation == "pow") {
        return pow(parameters[0].evaluate(), parameters[1].evaluate());
    } else if (operation == "eq") {
        return (parameters[0].evaluate() == parameters[1].evaluate()) ? 1 : 0;
    } else if (operation == "lt") {
        return (parameters[0].evaluate() < parameters[1].evaluate()) ? 1 : 0;
    } else if (operation == "le") {
        return (parameters[0].evaluate() <= parameters[1].evaluate()) ? 1 : 0;
    } else {
        cerr << "Unknown operation '" << operation << "'" << endl;
        return 0;
    }
}

std::string serializeLuaTableWithExpressions(LuaTable tbl) {
    tbl.pushRef();

    std::string result;

    int current_top = lua_gettop(tbl.L);
    if (lua_gettop(tbl.L) != 0) {
        if (luaL_loadstring(tbl.L, serialize_lua)) {
            bail(tbl.L, "Error loading serialization function: ");
        }

        if (lua_pcall(tbl.L, 0, 0, 0)) {
            bail(tbl.L, "Error compiling serialization function: ");
        }

        lua_getglobal (tbl.L, "serialize");
        assert (lua_isfunction(tbl.L, -1));
        lua_pushvalue(tbl.L, -2);
        if (lua_pcall(tbl.L, 1, 1, 0)) {
            bail(tbl.L, "Error while serializing: ");
        }
        result = string("return ") + lua_tostring (tbl.L, -1);
    } else {
        cerr << "Cannot serialize global Lua state!" << endl;
        abort();
    }

    lua_pop (tbl.L, lua_gettop(tbl.L) - current_top);

    tbl.popRef();

    return result;
}

std::string serializeOrderedLuaTableWithExpressions(LuaTable tbl) {
    tbl.pushRef();

    std::string result;

    int current_top = lua_gettop(tbl.L);
    if (lua_gettop(tbl.L) != 0) {
        if (luaL_loadstring(tbl.L, serialize_lua)) {
            bail(tbl.L, "Error loading serialization function: ");
        }

        if (lua_pcall(tbl.L, 0, 0, 0)) {
            bail(tbl.L, "Error compiling serialization function: ");
        }

        lua_getglobal (tbl.L, "serialize");
        assert (lua_isfunction(tbl.L, -1));
        lua_pushvalue(tbl.L, -2);
        lua_pushstring(tbl.L, "");
        lua_pushboolean(tbl.L, true);
        if (lua_pcall(tbl.L, 3, 1, 0)) {
            bail(tbl.L, "Error while serializing: ");
        }
        result = string("return ") + lua_tostring (tbl.L, -1);
    } else {
        cerr << "Cannot serialize global Lua state!" << endl;
        abort();
    }

    lua_pop (tbl.L, lua_gettop(tbl.L) - current_top);

    tbl.popRef();

    return result;
}

LuaTable luaTableFromFileWithExpressions (const char* _filename) {
    LuaTable result;

    result.filename = _filename;
    result.luaStateRef = new LuaStateRef();
    result.luaStateRef->L = luaL_newstate();
    result.luaStateRef->count = 1;
    luaL_openlibs (result.luaStateRef->L);

    // Add the directory of _filename to package.path
    result.addSearchPath(get_file_directory (_filename).c_str());

    luaL_dostring(result.luaStateRef->L, expressions_lua);
    // run the file we
    if (luaL_dofile (result.luaStateRef->L, _filename)) {
        bail (result.luaStateRef->L, "Error running file: ");
    }

    result.luaRef = luaL_ref (result.luaStateRef->L, LUA_REGISTRYINDEX);

    return result;
}
