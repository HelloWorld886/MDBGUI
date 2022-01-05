set_config("version", "0.0.3")
add_rules("mode.debug", "mode.release")

add_repositories("musmus-repo https://gitee.com/musmus9405/xmake-repo.git")
add_requires("lua v5.4.3", {configs = {shared = false}})
add_requires("hellolua main", {configs = {test = false, lua = "v5.4.3"}})

target("MDBGUI")
    add_packages("lua")
    add_packages("hellolua")
    add_rules("qt.widgetapp")
    add_files("Src/*.cpp")
    add_files("Styles/Style.qrc")
    add_files("Images/Image.qrc")
    add_files("Images/Icon.rc")
    add_files("Src/*.h")
    add_headerfiles("Src/*.h")
    add_values("qt_runtimes",
        "QtCore",
        "QtGui",
        "QtWidgets"
    )
    add_values("runtime_folders",
        "$(projectdir)/Plugins/",
        "$(projectdir)/libimobiledevice/",
        "$(projectdir)/ifuse/"
    )
    add_values("runtime_files",
        "$(buildir)/$(plat)/$(arch)/$(mode)/MDBGUI.exe",
        "$(env ADB_HOME)/adb.exe",
        "$(env ADB_HOME)/AdbWinApi.dll",
        "$(env ADB_HOME)/AdbWinUsbApi.dll"
    )

    if is_plat("windows") and is_mode("debug") then
        add_ldflags("/SUBSYSTEM:CONSOLE", {force = true})
    end

    after_build(function(target)
        local folders = target:values("runtime_folders")
        for _, folder in ipairs(folders) do
            os.cp(folder, "$(buildir)/$(plat)/$(arch)/$(mode)")
        end
    end)

    on_package(function(target)
        local copy_plugins = function( sourcedir, targetdir, ...)
            if not os.isdir(targetdir) then
                os.mkdir(targetdir)
            end

            for _, basename in ipairs({...}) do
                if is_mode("debug") then
                    basename = basename .. "d"
                end
                basename = basename .. ".dll"

                os.cp(
                    path.join(sourcedir, basename),
                    path.join(targetdir, basename)
                )
            end
        end

        import("core.base.semver")

        local buildir = "$(buildir)/$(plat)/$(arch)/$(mode)"
        local packagedir = "$(buildir)/packages/MDBGUI_" .. "$(plat)_$(arch)_$(mode)_v$(version)"
        if os.isdir(packagedir) then
            os.rm(packagedir)
        end
        os.mkdir(packagedir)

        local files = target:values("runtime_files")
        for _, file in ipairs(files) do
            os.cp(file, packagedir)
        end

        local folders = target:values("runtime_folders")
        for _, folder in ipairs(folders) do
            os.cp(folder, packagedir)
        end

        local qt = target:data("qt")
        assert(qt, "qt not found!")
        local qt_sdkver = semver.new(qt.sdkver)

        local frameworks = target:values("qt_runtimes")
        for _, framework in ipairs(frameworks) do
            local debug_suffix = "d"
            local file_suffix = ".dll"
            framework = "Qt" .. qt_sdkver:major() .. framework:sub(3) .. (is_mode("debug") and debug_suffix or "") .. file_suffix

            os.cp(
                path.join(qt.bindir, framework),
                path.join(packagedir, framework)
            )
        end

        copy_plugins(
            path.join(qt.pluginsdir, "platforms"),
            path.join(packagedir, "platforms"),
            "qwindows"
        )
    end)