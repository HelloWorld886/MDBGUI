---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by musmusliu.
--- DateTime: 2021/9/30 18:08
---

null = {}

ClassLib = {}

local __instanceid = 1

local __classPool = {}

local __emptyMeta =
{
    __index = function(c, k)
        LogE("DeclareClass : forbid get field -- getting  "..k)
    end,
    __newindex = function(tb, k, v)
        local value = v or "nil"
        LogE("DeclareClass : forbid add field -- setting " .. k .. " to " .. tostring(value))
    end
}

local function CreateClass()
    local c = next(__classPool)
    if c then
        __classPool[c] = nil
        setmetatable(c, nil)
        return c
    end

    return {}
end

local function DestroyClass(c)
    if not c then
        LogE("DeclareClass : DestroyClass param is nil")
        return
    end

    table.clear(c)

    setmetatable(c, __emptyMeta)

    if __classPool[c] then
        LogE("DeclareClass : duplicate destroy class")
        return
    end

    __classPool[c] = true
end

local class_type_meta =
{
    __index = function(t, k)
        return t.vtbl[k]
    end,

    __newindex = function(t, k, v)
        if type(v) ~= "function" then
            LogE("DeclareClass : not a function")
            return
        end

        t.vtbl[k] = v
    end
}

local vtbl_meta =
{
    __index = function(t, k)
        return t.__supervtbl[k]
    end
}

local obj_meta =
{
    __index = function(t, k)
        return t.__vtbl[k]
    end,

    __newindex = function(t, k, v)
        local value = v or "nil"
        print(debug.traceback())
        LogE("DeclareClass : forbid add field -- " .. t.__classname .. "." .. k .. " to " .. tostring(value))
    end
}

local function Construct(cls, obj, ...)
    if cls.super then
        Construct(cls.super, obj, ...)
    end

    if cls.ctor then
        cls.ctor(obj, ...)
    end
end

function DeclareClass(name, super)
    if not name then
        return nil
    end

    local class_type = {}
    class_type.__classname = name
    class_type.ctor = false
    class_type.super = super
    class_type.__super = super and super.vtbl -- 调用父类的快捷方式：BaseClass.__super.Method(self, argument)

    local vtbl = {}
    class_type.vtbl = vtbl

    class_type.new = function(...)
        local obj = CreateClass()
        obj.__classname = class_type.__classname
        obj.__instanceid = __instanceid
        obj.__vtbl = class_type.vtbl
        obj.__delete = DestroyClass

        __instanceid = __instanceid + 1

        Construct(class_type, obj, ...)
        setmetatable(obj, obj_meta)
        return obj
    end

    setmetatable(class_type, class_type_meta)

    if super then
        vtbl.__supervtbl = super.vtbl
        setmetatable(vtbl, vtbl_meta)
    end

    ClassLib[name] = class_type
    return class_type
end

function string.split(str, delimiter, result)
    if str==nil or str=='' or delimiter==nil then
        return nil
    end

    result = result or {}
    string.gsub(str, '[^'..delimiter..']+', function(w) table.insert(result, w) end )
    return result
end

function string.trim(s)
    return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function PrintTable( tbl , level, filteDefault)
    local msg = ""
    filteDefault = filteDefault or true --默认过滤关键字（DeleteMe, _class_type）
    level = level or 1
    local indent_str = ""
    for i = 1, level do
        indent_str = indent_str.."  "
    end

    print(indent_str .. "{")
    for k,v in pairs(tbl) do
        if filteDefault then
            if k ~= "_class_type" and k ~= "DeleteMe" then
                local item_str = string.format("%s%s = %s", indent_str .. " ",tostring(k), tostring(v))
                print(item_str)
                if type(v) == "table" then
                    PrintTable(v, level + 1)
                end
            end
        else
            local item_str = string.format("%s%s = %s", indent_str .. " ",tostring(k), tostring(v))
            print(item_str)
            if type(v) == "table" then
                PrintTable(v, level + 1)
            end
        end
    end
    print(indent_str .. "}")
end