---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by musmusliu.
--- DateTime: 2021/10/8 11:25
---

local DataServiceClass = DeclareClass("DataServiceClass")

function DataServiceClass:ctor()
    self.PackageName = ""
    self.DeviceData = false
    self.DeviceDataList = false
end

function DataServiceClass:SetDeviceIndex(index)
    if index <= 0 or index > #self.DeviceDataList then
        self.DeviceData = false
    else
        self.DeviceData = self.DeviceDataList[index]
    end
end

function DataServiceClass:GetDeviceName()
    if not self.DeviceData then
        return ""
    end

    return self.DeviceData.Name
end

function DataServiceClass:GetObbPath()
    return string.format("/sdcard/Android/obb/%s/", self.PackageName)
end

function DataServiceClass:GetDataPath()
    return string.format("/sdcard/Android/data/%s/", self.PackageName)
end

DataService = DataServiceClass.new()