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

#ifndef MARKER_MODEL_LUA_EXPRESSIONS
#define MARKER_MODEL_LUA_EXPRESSIONS

#include <rbdl/rbdl_math.h>
#include <rbdl/addons/luamodel/luatables.h>
#include <QtGui/QVector3D>
#include <vendor/QtPropertyBrowser/src/QtDoublePropertyManager>
#include <QtCore/QMap>
#include <SimpleMath/SimpleMath.h>

using namespace std;

struct LuaParameterExpression {
    string operation = "const";
    vector<LuaParameterExpression> parameters;
    string name = "";
    double value = 0;

    string serialize(int level);

    string serialize();

    double evaluate();

    bool operator==(const LuaParameterExpression &other) const;

    LuaParameterExpression operator+(const float &other) const;

    LuaParameterExpression operator-(const float &other) const;
};

LuaParameterExpression zeroExpression();

LuaTable luaTableFromFileWithExpressions(const char *_filename);

LuaTable luaTableFromExpressionWithExpressions(const char *lua_expr);

LuaParameterExpression parseExpression(const std::string &lua_expr, const std::map<std::string, double> &known_vars);

std::string serializeLuaTableWithExpressions(LuaTable table);

std::string serializeOrderedLuaTableWithExpressions(LuaTable table);

void updateVariables(LuaTable &table, std::map<std::string, double> &vars);

class ExpressionVector3D {
public:
    ExpressionVector3D() : ExpressionVector3D(zeroExpression(), zeroExpression(), zeroExpression()) {
    }

    ExpressionVector3D(double xpos, double ypos, double zpos) {
        v.push_back(zeroExpression());
        v.push_back(zeroExpression());
        v.push_back(zeroExpression());
        v[0].value = xpos;
        v[1].value = ypos;
        v[2].value = zpos;
    }

    ExpressionVector3D(LuaParameterExpression xpos, LuaParameterExpression ypos, LuaParameterExpression zpos) {
        v.push_back(xpos);
        v.push_back(ypos);
        v.push_back(zpos);
    }

    LuaParameterExpression x() const;

    LuaParameterExpression y() const;

    LuaParameterExpression z() const;

    void setX(LuaParameterExpression x);

    void setY(LuaParameterExpression y);

    void setZ(LuaParameterExpression z);

    LuaParameterExpression operator[](int i) const;

    bool operator==(const ExpressionVector3D &other) const;

    ExpressionVector3D operator+(const Vector3f &other) const;

    ExpressionVector3D operator-(const Vector3f &other) const;

    QVector3D toQVector3D() const;

    Vector3f toVector3f() const;

    RigidBodyDynamics::Math::Vector3d toVector3d() const;

private:
    vector<LuaParameterExpression> v;
};


class ExpressionMatrix33 {
public:
    ExpressionMatrix33() : ExpressionMatrix33(ExpressionVector3D(), ExpressionVector3D(), ExpressionVector3D()) {
    }

    ExpressionMatrix33(ExpressionVector3D row1, ExpressionVector3D row2, ExpressionVector3D row3) {
        v.push_back(row1);
        v.push_back(row2);
        v.push_back(row3);
    }

    ExpressionVector3D row1() const;

    ExpressionVector3D row2() const;

    ExpressionVector3D row3() const;

    void setRow1(ExpressionVector3D x);

    void setRow2(ExpressionVector3D y);

    void setRow3(ExpressionVector3D z);

    ExpressionVector3D operator[](int i) const;

    bool operator==(const ExpressionMatrix33 &other) const;

    Matrix33f toMatrix33f();

private:
    vector<ExpressionVector3D> v;
};


/* MARKER_MODEL_LUA_EXPRESSIONS */
#endif
