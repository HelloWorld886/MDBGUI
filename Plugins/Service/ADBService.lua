---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by musmusliu.
--- DateTime: 2021/9/30 16:56
---

local ExitCode = {
    Success = 0,
    Crashed = 1,
    Unknown = 3,
    FailToStart = 4,
    HasProcRunning = 5
}

local ADBServiceClass = DeclareClass("ADBServiceClass")

function ADBServiceClass:ctor()
    self._proc = false
end

function ADBServiceClass:Initialize(processKit)
    self._proc = processKit
end

function ADBServiceClass:KillServer()
    LogD("adb kill-server")

    if self._proc.Start("adb.exe", "kill-server") ~= ExitCode.Success then
        LogE("停止服务失败：" .. self._proc.GetError())
        return false
    end

    local output = self._proc.GetOutput()
    return true
end

function ADBServiceClass:StartServer()
    LogD("adb start-server")

    if self._proc.Start("adb.exe", "start-server") ~= ExitCode.Success then
        LogE("启动服务失败：".. self._proc.GetError())
        return false
    end

    local output = self._proc.GetOutput()
    return true
end

function ADBServiceClass:Push(deviceName, srcPath, dstPath)
    LogD("adb push")
    LogD(string.format("-s %s push %s %s", deviceName, srcPath, dstPath))
    if self._proc.Start("adb.exe", string.format("-s %s push %s %s", deviceName, srcPath, dstPath)) ~= ExitCode.Success then
        LogE("复制电脑文件失败：".. self._proc.GetError())
        return false
    end

    return true
end

function ADBServiceClass:Pull(deviceName, srcPath, dstPath)
    LogD("adb pull")
    if self._proc.Start("adb.exe", string.format("-s %s pull %s %s", deviceName, srcPath, dstPath)) ~= ExitCode.Success then
        LogE("复制设备文件失败：" .. self._proc.GetError())
        return false
    end

    return true
end

function ADBServiceClass:Install(deviceName, apkPath)
    LogD("adb install")
    if self._proc.Start("adb.exe", string.format("-s %s install %s", deviceName, apkPath)) ~= ExitCode.Success then
        LogE("安装文件失败：".. self._proc.GetError())
        return false
    end

    return true
end

function ADBServiceClass:Logcat(deviceName, priority, format, outputFilePath)
    LogD("adb logcat")
    if self._proc.Start("adb.exe", string.format("-s %s logcat -%s %s", deviceName, priority, format), false) ~= ExitCode.Success then
        local output = self._proc.GetOutput()
        local file = io.open(outputFilePath, "w+")
        file:write(output)
        io.close(file)
        return false
    end

    return true
end

function ADBServiceClass:Connect(address)
    LogD("adb connect")
    if self._proc.Start("adb.exe", string.format("connect %s", address)) ~= ExitCode.Success then
        LogE("连接远程设备失败：".. self._proc.GetError())
        return false
    end

    return true
end

function ADBServiceClass:Disconnect(address)
    LogD("adb disconnect")
    if self._proc.Start("adb.exe", string.format("disconnect %s", address)) ~= ExitCode.Success then
        LogE("断开远程设备失败：".. self._proc.GetError())
        return false
    end

    return true
end

function ADBServiceClass:Stop()
    LogD("stop proc")
    self._proc.Stop()
end

function ADBServiceClass:GetDevices()
    LogD("adb devices")

    if self._proc.Start("adb.exe", "devices") ~= ExitCode.Success then
        LogE("获取设备失败：".. self._proc.GetError())
        return {}
    end

    local output = self._proc.GetOutput()
    local splits = string.split(output, "\n")
    if not splits then
        return {}
    end

    for i = #splits, 1, -1 do
        splits[i] = string.trim(splits[i], "\n")
        splits[i] = string.trim(splits[i], "\r")
        if not splits[i] or splits[i] == "" then
            table.remove(splits, i)
        elseif splits[i] == "List of devices attached" then
            table.remove(splits, i)
        end
    end

    local result = {}
    for i = 1, #splits do
        local data = string.split(splits[i], "\t")
        result[#result + 1] = {Name = data[1], Status = data[2]}
    end

    return result
end

function ADBServiceClass:KillProc()
    self._proc.Start("cmd", "/c taskkill /im adb.exe /f")
end

ADBService = ADBServiceClass.new()