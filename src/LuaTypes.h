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

#ifndef MARKER_MODEL_LUA_TYPES
#define MARKER_MODEL_LUA_TYPES

#include "Model.h"
#include "ExpressionLuaTable.h"
#include <rbdl/rbdl_math.h>
#include <rbdl/addons/luamodel/luatables.h>

extern "C"
{
#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"
}

// SimpleMath Vector3f
template<>
Vector3f LuaTableNode::getDefault<Vector3f>(const Vector3f &default_value) {
    Vector3f result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d vector!" << std::endl;
            abort();
        }

        result[0] = static_cast<float>(vector_table[1].get<double>());
        result[1] = static_cast<float>(vector_table[2].get<double>());
        result[2] = static_cast<float>(vector_table[3].get<double>());
    }

    stackRestore();

    return result;
}

template<>
void LuaTableNode::set<Vector3f>(const Vector3f &value) {
    LuaTable custom_table = stackCreateLuaTable();

    custom_table[1] = static_cast<double>(value[0]);
    custom_table[2] = static_cast<double>(value[1]);
    custom_table[3] = static_cast<double>(value[2]);

    stackRestore();
}

// SimpleMath Vector4f
template<>
Vector4f LuaTableNode::getDefault<Vector4f>(const Vector4f &default_value) {
    Vector4f result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 4) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 4d vector!" << std::endl;
            abort();
        }

        result[0] = static_cast<float>(vector_table[1].get<double>());
        result[1] = static_cast<float>(vector_table[2].get<double>());
        result[2] = static_cast<float>(vector_table[3].get<double>());
        result[3] = static_cast<float>(vector_table[4].get<double>());
    }

    stackRestore();

    return result;
}

// SimpleMath Matrix33f
template<>
SimpleMath::Fixed::Matrix<float, 3, 3> LuaTableNode::getDefault<SimpleMath::Fixed::Matrix<float, 3, 3> >(
        const SimpleMath::Fixed::Matrix<float, 3, 3> &default_value) {
    SimpleMath::Fixed::Matrix<float, 3, 3> result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d matrix!" << std::endl;
            abort();
        }

        if (vector_table[1].length() != 3
            || vector_table[2].length() != 3
            || vector_table[3].length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d matrix!" << std::endl;
            abort();
        }

        result(0, 0) = static_cast<float>(vector_table[1][1].get<double>());
        result(0, 1) = static_cast<float>(vector_table[1][2].get<double>());
        result(0, 2) = static_cast<float>(vector_table[1][3].get<double>());

        result(1, 0) = static_cast<float>(vector_table[2][1].get<double>());
        result(1, 1) = static_cast<float>(vector_table[2][2].get<double>());
        result(1, 2) = static_cast<float>(vector_table[2][3].get<double>());

        result(2, 0) = static_cast<float>(vector_table[3][1].get<double>());
        result(2, 1) = static_cast<float>(vector_table[3][2].get<double>());
        result(2, 2) = static_cast<float>(vector_table[3][3].get<double>());
    }

    stackRestore();

    return result;
}

template<>
void LuaTableNode::set<SimpleMath::Fixed::Matrix<float, 3, 3> >(const SimpleMath::Fixed::Matrix<float, 3, 3> &value) {
    LuaTable custom_table = stackCreateLuaTable();

    custom_table[1][1] = static_cast<double>(value(0, 0));
    custom_table[1][2] = static_cast<double>(value(0, 1));
    custom_table[1][3] = static_cast<double>(value(0, 2));

    custom_table[2][1] = static_cast<double>(value(1, 0));
    custom_table[2][2] = static_cast<double>(value(1, 1));
    custom_table[2][3] = static_cast<double>(value(1, 2));

    custom_table[3][1] = static_cast<double>(value(2, 0));
    custom_table[3][2] = static_cast<double>(value(2, 1));
    custom_table[3][3] = static_cast<double>(value(2, 2));

    stackRestore();
}

template<>
RigidBodyDynamics::Math::Vector3d
LuaTableNode::getDefault<RigidBodyDynamics::Math::Vector3d>(const RigidBodyDynamics::Math::Vector3d &default_value) {
    RigidBodyDynamics::Math::Vector3d result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d vector!" << std::endl;
            abort();
        }

        result[0] = vector_table[1];
        result[1] = vector_table[2];
        result[2] = vector_table[3];
    }

    stackRestore();

    return result;
}

template<>
RigidBodyDynamics::Math::SpatialVector LuaTableNode::getDefault<RigidBodyDynamics::Math::SpatialVector>(
        const RigidBodyDynamics::Math::SpatialVector &default_value) {
    RigidBodyDynamics::Math::SpatialVector result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 6) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 6d vector!" << std::endl;
            abort();
        }
        result[0] = vector_table[1];
        result[1] = vector_table[2];
        result[2] = vector_table[3];
        result[3] = vector_table[4];
        result[4] = vector_table[5];
        result[5] = vector_table[6];
    }

    stackRestore();

    return result;
}

template<>
RigidBodyDynamics::Math::Matrix3d
LuaTableNode::getDefault<RigidBodyDynamics::Math::Matrix3d>(const RigidBodyDynamics::Math::Matrix3d &default_value) {
    RigidBodyDynamics::Math::Matrix3d result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d matrix!" << std::endl;
            abort();
        }

        if (vector_table[1].length() != 3
            || vector_table[2].length() != 3
            || vector_table[3].length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d matrix!" << std::endl;
            abort();
        }

        result(0, 0) = vector_table[1][1];
        result(0, 1) = vector_table[1][2];
        result(0, 2) = vector_table[1][3];

        result(1, 0) = vector_table[2][1];
        result(1, 1) = vector_table[2][2];
        result(1, 2) = vector_table[2][3];

        result(2, 0) = vector_table[3][1];
        result(2, 1) = vector_table[3][2];
        result(2, 2) = vector_table[3][3];
    }

    stackRestore();

    return result;
}

template<>
RigidBodyDynamics::Joint
LuaTableNode::getDefault<RigidBodyDynamics::Joint>(const RigidBodyDynamics::Joint &default_value) {
    using namespace RigidBodyDynamics;
    using namespace RigidBodyDynamics::Math;

    Joint result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        int joint_dofs = vector_table.length();

        if (joint_dofs == 1) {
            std::string dof_string = vector_table[1].getDefault<std::string>("");
            if (dof_string == "JointTypeSpherical") {
                stackRestore();
                return Joint(JointTypeSpherical);
            } else if (dof_string == "JointTypeEulerZYX") {
                stackRestore();
                return Joint(JointTypeEulerZYX);
            }
            if (dof_string == "JointTypeEulerXYZ") {
                stackRestore();
                return Joint(JointTypeEulerXYZ);
            }
            if (dof_string == "JointTypeEulerYXZ") {
                stackRestore();
                return Joint(JointTypeEulerYXZ);
            }
            if (dof_string == "JointTypeTranslationXYZ") {
                stackRestore();
                return Joint(JointTypeTranslationXYZ);
            }
        }

        if (joint_dofs > 0) {
            if (vector_table[1].length() != 6) {
                std::cerr << "LuaModel Error: invalid joint motion subspace description at " << this->keyStackToString()
                          << std::endl;
                abort();
            }
        }

        switch (joint_dofs) {
            case 0:
                result = Joint(JointTypeFixed);
                break;
            case 1:
                result = Joint(vector_table[1].get<SpatialVector>());
                break;
            case 2:
                result = Joint(
                        vector_table[1].get<SpatialVector>(),
                        vector_table[2].get<SpatialVector>()
                );
                break;
            case 3:
                result = Joint(
                        vector_table[1].get<SpatialVector>(),
                        vector_table[2].get<SpatialVector>(),
                        vector_table[3].get<SpatialVector>()
                );
                break;
            case 4:
                result = Joint(
                        vector_table[1].get<SpatialVector>(),
                        vector_table[2].get<SpatialVector>(),
                        vector_table[3].get<SpatialVector>(),
                        vector_table[4].get<SpatialVector>()
                );
                break;
            case 5:
                result = Joint(
                        vector_table[1].get<SpatialVector>(),
                        vector_table[2].get<SpatialVector>(),
                        vector_table[3].get<SpatialVector>(),
                        vector_table[4].get<SpatialVector>(),
                        vector_table[5].get<SpatialVector>()
                );
                break;
            case 6:
                result = Joint(
                        vector_table[1].get<SpatialVector>(),
                        vector_table[2].get<SpatialVector>(),
                        vector_table[3].get<SpatialVector>(),
                        vector_table[4].get<SpatialVector>(),
                        vector_table[5].get<SpatialVector>(),
                        vector_table[6].get<SpatialVector>()
                );
                break;
            default:
                std::cerr << "Invalid number of DOFs for joint." << std::endl;
                abort();
        }
    }

    stackRestore();

    return result;
}

template<>
RigidBodyDynamics::Body
LuaTableNode::getDefault<RigidBodyDynamics::Body>(const RigidBodyDynamics::Body &default_value) {
    RigidBodyDynamics::Body result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        double mass = 0.;
        RigidBodyDynamics::Math::Vector3d com(RigidBodyDynamics::Math::Vector3d::Zero(3));
        RigidBodyDynamics::Math::Matrix3d inertia(RigidBodyDynamics::Math::Matrix3d::Identity(3, 3));

        mass = vector_table["mass"];
        com = vector_table["com"].getDefault<RigidBodyDynamics::Math::Vector3d>(com);
        inertia = vector_table["inertia"].getDefault<RigidBodyDynamics::Math::Matrix3d>(inertia);

        result = RigidBodyDynamics::Body(mass, com, inertia);
    }

    stackRestore();

    return result;
}

template<>
LuaParameterExpression LuaTableNode::getDefault<LuaParameterExpression>(const LuaParameterExpression &def) {
    LuaParameterExpression result = LuaParameterExpression();

    if (exists()) {
        LuaTable custom_table;
        int type;
        if (stackQueryValue()) {
            type = lua_type(luaTable->L, -1);
            if (type == LUA_TTABLE) {
                stackRestore();
                custom_table = stackQueryTable();
                result.operation = custom_table["operation"].get<std::string>();
                result.name = custom_table["name"].get<std::string>();
                result.value = custom_table["value"].get<double>();
                stackRestore();

                if (custom_table["p1"].exists()) {
                    result.parameters.push_back(custom_table["p1"].get<LuaParameterExpression>());
                    if (custom_table["p2"].exists()) {
                        result.parameters.push_back(custom_table["p2"].get<LuaParameterExpression>());
                    }
                }
            } else {
                result.value = 0;
                result.operation = "const";
                result.value = lua_tonumber(luaTable->L, -1);
                stackRestore();
            }
        }
    } else {
        result = def;
        stackRestore();
    }

    return result;
}

template<>
void LuaTableNode::set<LuaParameterExpression>(const LuaParameterExpression &value) {
    LuaTable custom_table = stackCreateLuaTable();

    custom_table["_type"] = std::string("expression");
    custom_table["operation"] = value.operation;
    custom_table["name"] = value.name;
    custom_table["value"] = value.value;
    if (value.parameters.size() > 0) {
        custom_table["p1"] = value.parameters[0];
    }
    if (value.parameters.size() > 1) {
        custom_table["p2"] = value.parameters[1];
    }
    stackRestore();
}

template<>
ExpressionVector3D LuaTableNode::getDefault<ExpressionVector3D>(const ExpressionVector3D &default_value) {
    ExpressionVector3D result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d vector!" << std::endl;
            abort();
        }

        result.setX(vector_table[1].get<LuaParameterExpression>());
        result.setY(vector_table[2].get<LuaParameterExpression>());
        result.setZ(vector_table[3].get<LuaParameterExpression>());
    }

    stackRestore();

    return result;
}

template<>
ExpressionMatrix33 LuaTableNode::getDefault<ExpressionMatrix33>(const ExpressionMatrix33 &default_value) {
    ExpressionMatrix33 result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        if (vector_table.length() != 3) {
            std::cerr << "LuaModel Error at " << keyStackToString() << " : invalid 3d vector!" << std::endl;
            abort();
        }

        result.setRow1(vector_table[1].get<ExpressionVector3D>());
        result.setRow2(vector_table[2].get<ExpressionVector3D>());
        result.setRow3(vector_table[3].get<ExpressionVector3D>());
    }

    stackRestore();

    return result;
}

template<>
void LuaTableNode::set<ExpressionVector3D>(const ExpressionVector3D &value) {
    LuaTable custom_table = stackCreateLuaTable();
    custom_table[1] = value.x();
    custom_table[2] = value.y();
    custom_table[3] = value.z();
    stackRestore();
}

template<>
void LuaTableNode::set<ExpressionMatrix33>(const ExpressionMatrix33 &value) {
    LuaTable custom_table = stackCreateLuaTable();

    custom_table[1][1] = value.row1().x();
    custom_table[1][2] = value.row1().y();
    custom_table[1][3] = value.row1().z();
    custom_table[2][1] = value.row2().x();
    custom_table[2][2] = value.row2().y();
    custom_table[2][3] = value.row2().z();
    custom_table[3][1] = value.row3().x();
    custom_table[3][2] = value.row3().y();
    custom_table[3][3] = value.row3().z();
    stackRestore();
}

template<>
VisualsData LuaTableNode::getDefault<VisualsData>(const VisualsData &default_value) {
    VisualsData result = default_value;

    if (stackQueryValue()) {
        LuaTable visuals_table = LuaTable::fromLuaState(luaTable->L);

        result.scale = visuals_table["scale"].getDefault(ExpressionVector3D(1., 1., 1.));
        result.dimensions = visuals_table["dimensions"].getDefault(ExpressionVector3D(0., 0., 0.));
        if (visuals_table["color"].length() == 4)
            result.color = visuals_table["color"].getDefault(Vector4f(-1.f, -1.f, -1.f, 1.f));
        else {
            Vector3f vec3_color = visuals_table["color"].getDefault(Vector3f(-1.f, -1.f, -1.f));
            result.color = Vector4f(vec3_color[0], vec3_color[1], vec3_color[2], 1.f);
        }
        result.mesh_center = visuals_table["mesh_center"].getDefault(ExpressionVector3D());
        result.translate = visuals_table["translate"].getDefault(ExpressionVector3D(-1.f, -1.f, -1.f));

        if (visuals_table["rotate"].exists()) {
            Vector3f axis = visuals_table["rotate"]["axis"];
            double angle = visuals_table["rotate"]["angle"];

            result.orientation = SimpleMath::GL::Quaternion::fromGLRotate(angle, axis[0], axis[1], axis[2]);
        }

        result.src = visuals_table["src"].getDefault<std::string>("");
    }

    stackRestore();

    return result;
}

template<>
RigidBodyDynamics::Math::SpatialTransform LuaTableNode::getDefault<RigidBodyDynamics::Math::SpatialTransform>(
        const RigidBodyDynamics::Math::SpatialTransform &default_value) {
    RigidBodyDynamics::Math::SpatialTransform result = default_value;

    if (stackQueryValue()) {
        LuaTable vector_table = LuaTable::fromLuaState(luaTable->L);

        result.r = vector_table["r"].getDefault<ExpressionVector3D>(ExpressionVector3D()).toVector3d();
        result.E = vector_table["E"].getDefault<RigidBodyDynamics::Math::Matrix3d>(
                RigidBodyDynamics::Math::Matrix3d::Identity(3, 3));
    }

    stackRestore();

    return result;
}

/* MARKER_MODEL_LUA_TYPES */
#endif
