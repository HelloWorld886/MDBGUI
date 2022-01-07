---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by musmusliu.
--- DateTime: 2021/10/10 19:08
---

local ImportTabGUIClass = DeclareClass("ImportTabGUIClass", ClassLib.TabGUIClass)

function ImportTabGUIClass:ctor()
    ImportTabGUIClass.__super.AddSerialField(self, "FilePath", "Tool.Import.FilePath", false)
    ImportTabGUIClass.__super.AddSerialField(self, "DeviceDir", "Tool.Import.DeviceDir", false)
end

function ImportTabGUIClass:vGetTabName()
    return "导入"
end

function ImportTabGUIClass:vGetLayoutType()
    return LayoutType.Form
end

function ImportTabGUIClass:vGetTabObjectName()
    return "import_tab"
end

function ImportTabGUIClass:vSetupWidgetUI()
    self:BeginLayout(LayoutType.HBox, 1, "导入文件")
    self:LineField(self:GetSerialField("FilePath"), "import_lf", true)
    self:Button("浏览", "import_explore_btn")
    self:EndLayout()

    self:LineField(self:GetSerialField("DeviceDir"), "dst_lf", false, 1, "设备目录")
    self:Button("导入", "import_btn")
end

function ImportTabGUIClass:vOnButtonClicked(objectName)
    if objectName == "import_explore_btn" then
        local filePath = self:OpenFileDialog("导入文件", self:GetSerialField("FilePath"), "All File (*.*)")
        self:SetLineFieldText("import_lf",  filePath)
        self:SetSerialField("FilePath", filePath)
        return true
    elseif objectName == "import_btn" then
        self:Import()
        return true
    end
end

function ImportTabGUIClass:vOnLineFieldChanged(objectName, text)
    if objectName == "import_lf" then
        self:SetSerialField("FilePath", text)
        return true
    elseif objectName == "dst_lf" then
        self:SetSerialField("DeviceDir", text)
        return true
    end
end

function ImportTabGUIClass:Import()
    if DataService.PackageName == "" then
        LogE("包名不能为空")
        return
    end

    local deviceName = DataService:GetDeviceName()
    local deviceDir = self:GetLineFieldText("dst_lf")
    if deviceDir == "" then
        LogE("设备目录不能为空")
        return
    end

    local filePath = self:GetSerialField("FilePath")
    if filePath == "" then
        LogE("导入文件不能为空")
        return
    end

    MDBService:Push(deviceName, filePath, DataService:GetDataPath() .. deviceDir)
end