function Expression (operation, p1, p2, name, value)
    if type(p1) == "number" then
        p1 = Expression("const", nil, nil, nil, p1)
    end
    if type(p2) == "number" then
        p2 = Expression("const", nil, nil, nil, p2)
    end

    local tbl = {
        _type="expression",
        operation=operation,
        p1=p1,
        p2=p2,
        name=name or "",
        value=value or 0
    }

    local mt = {
        __mul = function (left, right)
            return Expression("mul", left, right)
        end,
        __add = function (left, right)
            return Expression("add", left, right)
        end,
        __sub = function (left, right)
            return Expression("sub", left, right)
        end,
        __div = function (left, right)
            return Expression("div", left, right)
        end,
        __mod = function (left, right)
            return Expression("mod", left, right)
        end,
        __pow = function (left, right)
            return Expression("pow", left, right)
        end,
        __unm = function (left)
            return Expression("mul", left, -1)
        end,
        __eq = function (left, right)
            return Expression("eq", left, right)
        end,
        __lt = function (left, right)
            return Expression("lt", left, right)
        end,
        __le = function (left, right)
            return Expression("le", left, right)
        end,
    }
    setmetatable(tbl, mt)
    return tbl
end
_expressions_variable_index = {}
function Variable (name, value)
    if _expressions_variable_index[name] then
        error("You cannot re-define the variable '" .. name .. "'!")
    end
    local exp = Expression("var", nil, nil, name, value)
    _expressions_variable_index[name] = exp
    return exp
end
