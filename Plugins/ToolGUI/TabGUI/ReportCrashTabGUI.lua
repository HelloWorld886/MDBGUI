local ReportCrashTabGUIClass = DeclareClass("ReportCrashTabGUIClass", ClassLib.TabGUIClass)

function ReportCrashTabGUIClass:ctor()
    ReportCrashTabGUIClass.__super.AddSerialField(self, "CrashDir", "Tool.ReportCrash.CrashDir", false)
end

function ReportCrashTabGUIClass:vGetTabName()
    return "获取崩溃日志"
end

function ReportCrashTabGUIClass:vGetLayoutType()
    return LayoutType.Form
end

function ReportCrashTabGUIClass:vGetTabObjectName()
    return "reportcrash_tab"
end

function ReportCrashTabGUIClass:vSetupWidgetUI()
    self:BeginLayout(LayoutType.HBox, 1, "路径")
    self:LineField(self:GetSerialField("CrashDir"), "reportcrash_lf", true)
    self:Button("浏览", "reportcrash_explore_btn")
    self:EndLayout()

    self:Button("获取", "reportcrash_grab_btn")
end

function ReportCrashTabGUIClass:vOnButtonClicked(objectName)
    if objectName == "reportcrash_explore_btn" then
        local crashDir = self:OpenDirDialog("路径", self:GetSerialField("CrashDir"))
        self:SetLineFieldText("reportcrash_lf",  crashDir)
        self:SetSerialField("CrashDir", crashDir)
        return true
    elseif objectName == "reportcrash_grab_btn" then
        self:Grab()
    end
end

function ReportCrashTabGUIClass:Grab()
    local deviceName = DataService:GetDeviceName()
    local crashDir = self:GetSerialField("CrashDir")
    MDBService:ReportCrash(deviceName, crashDir)
end