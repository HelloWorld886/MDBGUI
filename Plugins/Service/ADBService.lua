---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by musmusliu.
--- DateTime: 2021/9/30 16:56
---

ExitCode = {
    Success = 0,
    Crashed = 1,
    Unknown = 3,
    FailToStart = 4,
    HasProcRunning = 5
}

local ADBServiceClass = DeclareClass("ADBServiceClass")

function ADBServiceClass:ctor()
    self._defaultElapsed = 1000
    self._proc = false
end

function ADBServiceClass:Initialize(processKit)
    self._proc = processKit
end

function ADBServiceClass:KillServer()
    if self._proc:Start("adb.exe", "kill-server", "停止中...", "停止完毕", self._defaultElapsed) ~= ExitCode.Success then
        LogE("停止服务失败：" .. self._proc:GetError())
        return false
    end

    local output = self._proc:GetOutput()
    return true
end

function ADBServiceClass:StartServer()
    if self._proc:Start("adb.exe", "start-server", "启动中...", "启动完毕", self._defaultElapsed) ~= ExitCode.Success then
        LogE("启动服务失败：".. self._proc:GetError())
        return false
    end

    local output = self._proc:GetOutput()
    return true
end

function ADBServiceClass:Push(deviceName, srcPath, dstPath)
    dstPath = string.gsub(dstPath, "\\", "/")
    local splits = string.split(dstPath, "/")
    if #splits > 0 then
        local lastName = nil
        for i = #splits, 1, -1 do
            if splits[i] ~= "" then
                lastName = splits[i]
                break
            end
            -- body
        end
        if lastName and not string.find(lastName, "%.") then
            dstPath = dstPath .. "/"
        end
    end
    LogD(string.format("-s %s push %s %s", deviceName, srcPath, dstPath))
    if self._proc:Start("adb.exe", string.format("-s %s push %s %s", deviceName, srcPath, dstPath),  "拷贝中...", "拷贝完毕", 2000) ~= ExitCode.Success then
        LogE("拷贝电脑文件失败：".. self._proc:GetError())
        return false
    end

    return true
end

function ADBServiceClass:Pull(deviceName, srcPath, dstPath)
    LogD(string.format("-s %s pull %s %s", deviceName, srcPath, dstPath))
    if self._proc:Start("adb.exe", string.format("-s %s pull %s %s", deviceName, srcPath, dstPath), "拷贝中...", "拷贝完毕", self._defaultElapsed) ~= ExitCode.Success then
        LogE("拷贝设备文件失败：" .. self._proc:GetError())
        return false
    end

    return true
end

function ADBServiceClass:Install(deviceName, apkPath)
    LogD(string.format("-s %s install %s", deviceName, apkPath))
    if self._proc:Start("adb.exe", string.format("-s %s install %s", deviceName, apkPath), "安装中...", "安装完毕", 3000) ~= ExitCode.Success then
        LogE("安装文件失败：".. self._proc:GetError())
        return false
    end

    return true
end

function ADBServiceClass:Logcat(deviceName, priority, format, outputFilePath)
    LogD(string.format("-s %s logcat -%s %s", deviceName, priority, format))
    local result = self._proc:Start("adb.exe", string.format("-s %s logcat -%s %s", deviceName, priority, format), "抓取中...", "抓取完毕", self._defaultElapsed,false)
    if result == ExitCode.Success or result == ExitCode.Crashed then
        local output = self._proc:GetOutput()
        local file = io.open(outputFilePath, "w+")
        file:write(output)
        io.close(file)
        return true
    end

    LogE("实时日志失败: " .. self._proc:GetError())
    return false
end

function ADBServiceClass:Connect(address)
    LogD(string.format("connect %s", address))
    if self._proc:Start("adb.exe", string.format("connect %s", address), "连接中...", "连接完毕", self._defaultElapsed) ~= ExitCode.Success then
        LogE("连接远程设备失败：".. self._proc:GetError())
        return false
    end

    return true
end

function ADBServiceClass:Disconnect(address)
    string.format("disconnect %s", address)
    if self._proc:Start("adb.exe", string.format("disconnect %s", address), "断开连接中...", "断开完毕", self._defaultElapsed) ~= ExitCode.Success then
        LogE("断开远程设备失败：".. self._proc:GetError())
        return false
    end

    return true
end

function ADBServiceClass:StopProcess()
    self._proc:Stop()
end

function ADBServiceClass:GetDevices()
    if self._proc:Start("adb.exe", "devices", "获取设备中...", "获取设备完毕", self._defaultElapsed) ~= ExitCode.Success then
        LogE("获取设备失败：".. self._proc:GetError())
        return {}
    end

    local output = self._proc:GetOutput()
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
        result[#result + 1] = {Alias = data[1], Name = data[1], Status = data[2]}
    end

    return result
end

function ADBServiceClass:KillProcess()
    self._proc:Stop()
    self._proc:Start("cmd", "/c taskkill /im adb.exe /f", "结束adb进程中...", "结束完毕", self._defaultElapsed)
end