---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by musmusliu.
--- DateTime: 2021/10/8 16:39
---

---@class ToolGUIClass
---@overload GUIClass
local ToolGUIClass = DeclareClass("ToolGUIClass", ClassLib.GUIClass)

function ToolGUIClass:ctor()
    self._tabList = {
        ClassLib.InstallTabGUIClass.new(),
        ClassLib.GameLogTabGUIClass.new(),
        ClassLib.LogcatTabGUIClass.new(),
        ClassLib.ReplaceFileTabGUIClass.new()
    }
end

function ToolGUIClass:Initialize(guiKit)
    ToolGUIClass.__super.Initialize(self, guiKit)
    for i = 1, #self._tabList do
        self._tabList[i]:Initialize(guiKit)
    end

    self:BeginTabWidget("tool_tw", "工具:")
    for i = 1, #self._tabList do
        self._tabList[i]:SetupUI()
    end
    self:EndTabWidget()
end


function ToolGUIClass:OnButtonClicked(objectName)
    for i = 1, #self._tabList do
        if self._tabList[i]:vOnButtonClicked(objectName) then
            break
        end
    end
end

function ToolGUIClass:OnComboBoxChanged(objectName, index)
    for i = 1, #self._tabList do
        if self._tabList[i]:OnComboBoxChanged(objectName, index) then
            break
        end
    end
end

function ToolGUIClass:OnTextFieldChanged(objectName, text)
    for i = 1, #self._tabList do
        if self._tabList[i]:OnTextFieldChanged(objectName, text) then
            break
        end
    end
end
