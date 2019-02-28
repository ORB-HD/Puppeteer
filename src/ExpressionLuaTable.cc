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
#include <QMap>
#include "SimpleMath/SimpleMath.h"

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

std::string get_file_directory(const char *filename);


struct LuaParseError : public exception {
    const char *what() const throw() {
        return "LuaParseError";
    }
};

string format_double(double value) {
    // Return value without trailing zeros, see https://stackoverflow.com/a/13709929
    string result = to_string(value);
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    result.erase(result.find_last_not_of('.') + 1, std::string::npos);
    return result;
}

string LuaParameterExpression::serialize(int level) {
    string result;
    if (operation == "var") {
        return name;
    } else if (operation == "const") {
        return format_double(value);
    } else if (operation == "mul") {
        result = parameters[0].serialize(level + 1) + " * " + parameters[1].serialize(level + 1);
    } else if (operation == "add") {
        result = parameters[0].serialize(level + 1) + " + " + parameters[1].serialize(level + 1);
    } else if (operation == "div") {
        result = parameters[0].serialize(level + 1) + " / " + parameters[1].serialize(level + 1);
    } else if (operation == "sub") {
        result = parameters[0].serialize(level + 1) + " - " + parameters[1].serialize(level + 1);
    } else if (operation == "mod") {
        result = parameters[0].serialize(level + 1) + " % " + parameters[1].serialize(level + 1);
    } else if (operation == "pow") {
        result = parameters[0].serialize(level + 1) + " ^ " + parameters[1].serialize(level + 1);
    } else if (operation == "eq") {
        result = parameters[0].serialize(level + 1) + " == " + parameters[1].serialize(level + 1);
    } else if (operation == "lt") {
        result = parameters[0].serialize(level + 1) + " < " + parameters[1].serialize(level + 1);
    } else if (operation == "le") {
        result = parameters[0].serialize(level + 1) + " <= " + parameters[1].serialize(level + 1);
    } else {
        int i = 0;
        result = operation + "(";
        for (LuaParameterExpression p : parameters) {
            result += p.serialize();
            if (i < parameters.size() - 1) {
                result += ",";
            }
            i++;
        }
        result = result + ")";
        return result;
    }
    if (level > 0) {
        return "(" + result + ")";
    }
    return result;
}

string LuaParameterExpression::serialize() {
    return serialize(0);
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

bool LuaParameterExpression::operator==(const LuaParameterExpression &other) const {
    int i = 0;
    for (auto &par: parameters) {
        if (other.parameters.size() < i + 1) {
            return false;
        }
        if (!(other.parameters[i] == par)) {
            return false;
        }
        i++;
    }
    return operation == other.operation
           && value == other.value
           && name == other.name;
}

LuaParameterExpression LuaParameterExpression::operator-(const float &other) const {
    LuaParameterExpression wrap = LuaParameterExpression();
    wrap.operation = "sub";
    wrap.parameters.push_back(*this);
    LuaParameterExpression c = LuaParameterExpression();
    c.operation = "const";
    c.value = other;
    wrap.parameters.push_back(c);
    return wrap;
}

LuaParameterExpression LuaParameterExpression::operator+(const float &other) const {
    LuaParameterExpression wrap = LuaParameterExpression();
    wrap.operation = "add";
    wrap.parameters.push_back(*this);
    LuaParameterExpression c = LuaParameterExpression();
    c.operation = "const";
    c.value = other;
    wrap.parameters.push_back(c);
    return wrap;
}

std::string serializeOrderedLuaTableWithExpressions(LuaTable tbl, std::map<std::string, double> &variables) {
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
        lua_pushnumber(tbl.L, 1);
        lua_createtable(tbl.L, 0, variables.size());

        for (auto var : variables) {
            lua_pushstring(tbl.L, var.first.c_str());
            lua_pushnumber(tbl.L, var.second);
            lua_settable(tbl.L, -3);
        }

        if (lua_pcall(tbl.L, 5, 1, 0)) {
            bail(tbl.L, "Error while serializing: ");
        }
        result = lua_tostring (tbl.L, -1);
    } else {
        cerr << "Cannot serialize global Lua state!" << endl;
        abort();
    }

    lua_pop (tbl.L, lua_gettop(tbl.L) - current_top);

    tbl.popRef();

    return result;
}

LuaTable luaTableFromFileWithExpressions(const char *_filename) {
    LuaTable result;

    result.filename = _filename;
    result.luaStateRef = new LuaStateRef();
    result.luaStateRef->L = luaL_newstate();
    result.luaStateRef->count = 1;
    luaL_openlibs(result.luaStateRef->L);

    // Add the directory of _filename to package.path
    result.addSearchPath(get_file_directory(_filename).c_str());

    luaL_dostring(result.luaStateRef->L, expressions_lua);
    // run the file we
    if (luaL_dofile (result.luaStateRef->L, _filename)) {
        bail(result.luaStateRef->L, "Error running file: ");
    }

    result.luaRef = luaL_ref(result.luaStateRef->L, LUA_REGISTRYINDEX);

    return result;
}

LuaParameterExpression parseExpression(const std::string &lua_expr, const std::map<std::string, double> &known_vars) {
    std::string body;
    for (auto &entry : known_vars) {
        body += entry.first + " = Variable(\"" + entry.first + "\", " + format_double(entry.second) + ")\n";
    }
    body += "return {value = " + lua_expr + "};";
    try {
        LuaTable t = luaTableFromExpressionWithExpressions(body.c_str());
        if (!t["value"].exists()) {
            // TODO: Feedback error to user
            cerr << body << endl;
            return LuaParameterExpression();
        }
        return t["value"].get<LuaParameterExpression>();
    } catch (LuaParseError &e) {
        // TODO: Feedback error to user
        cerr << body << endl;
        return LuaParameterExpression();
    }
}

LuaTable luaTableFromExpressionWithExpressions(const char *lua_expr) {
    LuaTable result;

    result.luaStateRef = new LuaStateRef();
    result.luaStateRef->L = luaL_newstate();
    result.luaStateRef->count = 1;
    luaL_openlibs(result.luaStateRef->L);

    luaL_dostring(result.luaStateRef->L, expressions_lua);
    if (luaL_loadstring(result.luaStateRef->L, lua_expr)) {
        std::cerr << "Error compiling expression!" << lua_tostring(result.luaStateRef->L, -1) << endl;
        throw LuaParseError();
    }

    if (lua_pcall(result.luaStateRef->L, 0, LUA_MULTRET, 0)) {
        std::cerr << "Error running expression!" << lua_tostring(result.luaStateRef->L, -1) << endl;
        throw LuaParseError();
    }

    if (lua_gettop(result.luaStateRef->L) != 0) {
        result.luaRef = luaL_ref(result.luaStateRef->L, LUA_REGISTRYINDEX);
    } else {
        result.referencesGlobal = true;
    }

    return result;
}

LuaParameterExpression zeroExpression() {
    LuaParameterExpression l = LuaParameterExpression();
    l.operation = "const";
    l.value = 0;
    l.name = "";
    return l;
}

LuaParameterExpression ExpressionVector3D::x() const {
    return v[0];
}

LuaParameterExpression ExpressionVector3D::y() const {
    return v[1];
}

LuaParameterExpression ExpressionVector3D::z() const {
    return v[2];
}

void ExpressionVector3D::setX(LuaParameterExpression x) {
    v[0] = x;
}

void ExpressionVector3D::setY(LuaParameterExpression y) {
    v[1] = y;
}

void ExpressionVector3D::setZ(LuaParameterExpression z) {
    v[2] = z;
}

LuaParameterExpression ExpressionVector3D::operator[](int i) const {
    return v[i];
}

bool ExpressionVector3D::operator==(const ExpressionVector3D &other) const {
    return x() == other.x()
           && y() == other.y()
           && z() == other.z();
}

ExpressionVector3D ExpressionVector3D::operator+(const Vector3f &other) const {
    return ExpressionVector3D(x() + other[0], y() + other[1], z() + other[2]);
}

ExpressionVector3D ExpressionVector3D::operator-(const Vector3f &other) const {
    return ExpressionVector3D(x() - other[0], y() - other[1], z() - other[2]);
}

RigidBodyDynamics::Math::Vector3d ExpressionVector3D::toVector3d() const {
    return RigidBodyDynamics::Math::Vector3d(x().evaluate(), y().evaluate(), z().evaluate());
}

Vector3f ExpressionVector3D::toVector3f() const {
    return Vector3f(static_cast<float>(x().evaluate()), static_cast<float>(y().evaluate()),
                    static_cast<float>(z().evaluate()));
}

QVector3D ExpressionVector3D::toQVector3D() const {
    return QVector3D(static_cast<float>(x().evaluate()), static_cast<float>(y().evaluate()),
                     static_cast<float>(z().evaluate()));
}

ExpressionVector3D ExpressionMatrix33::row1() const {
    return v[0];
}

ExpressionVector3D ExpressionMatrix33::row2() const {
    return v[1];
}

ExpressionVector3D ExpressionMatrix33::row3() const {
    return v[2];
}

void ExpressionMatrix33::setRow1(ExpressionVector3D x) {
    v[0] = x;
}

void ExpressionMatrix33::setRow2(ExpressionVector3D y) {
    v[1] = y;
}

void ExpressionMatrix33::setRow3(ExpressionVector3D z) {
    v[2] = z;
}

ExpressionVector3D ExpressionMatrix33::operator[](int i) const {
    return v[i];
}

bool ExpressionMatrix33::operator==(const ExpressionMatrix33 &other) const {
    return row1() == other.row1()
           && row2() == other.row2()
           && row3() == other.row3();
}

Matrix33f ExpressionMatrix33::toMatrix33f() {
    return Matrix33f(
            static_cast<float>(row1().x().evaluate()),
            static_cast<float>(row1().y().evaluate()),
            static_cast<float>(row1().z().evaluate()),
            static_cast<float>(row2().x().evaluate()),
            static_cast<float>(row2().y().evaluate()),
            static_cast<float>(row2().z().evaluate()),
            static_cast<float>(row3().x().evaluate()),
            static_cast<float>(row3().y().evaluate()),
            static_cast<float>(row3().z().evaluate())
    );
}
