---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by musmusliu.
--- DateTime: 2021/10/10 21:01
---

local SerializeServiceClass = DeclareClass("SerializeServiceClass")

function SerializeServiceClass:ctor()
    self._json = require("Plugins.Base.json")
    self._configTable = false
end

function SerializeServiceClass:Initialize()
    local file = io.open("Plugins/Config.json", "r")
    local jsonStr = file:read("*a")
    io.close(file)
    if not jsonStr or jsonStr == "" then
        self._configTable = {}
    else
        self._configTable = self._json.decode(jsonStr) or {}
    end
end

function SerializeServiceClass:ReadValue(str, default)
    local split = string.split(str, ".")
    local result = false
    for i = 1, #split do
        if i == 1 then
            local tmp = self._configTable[split[i]]
            if not tmp then
                return default
            else
                result = tmp
            end
        else
            local tmp = result[split[i]]
            if not tmp then
                return default
            else
                result = tmp
            end
        end
    end

    return result
end

function SerializeServiceClass:WriteValue(str, value, save)
    local split = string.split(str, ".")
    local result = false
    for i = 1, #split do
        if i == 1 then
            if not self._configTable[split[i]] then
                self._configTable[split[i]] = i == #split and value or {}
            end

            if i == #split then
                break
            else
                result = self._configTable[split[i]]
            end
        else
            if not result[split[i]] then
                result[split[i]] = i == #split and value or {}
            end

            if i == #split then
                break
            else
                result = result[split[i]]
            end
        end
    end

    if save then
        local str = self._json.encode(self._configTable)
        local file = io.open("Plugins/Config.json", "w+")
        file:write(str)
        io.close(file)
    end
end

function SerializeServiceClass:Save()
    local str = self._json.encode(self._configTable)
    local file = io.open("Plugins/Config.json", "w+")
    file:write(str)
    io.close(file)
end

SerializeService = SerializeServiceClass.new()