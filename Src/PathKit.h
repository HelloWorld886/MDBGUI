//
// Created by musmusliu on 2021/10/8.
//

#pragma once

#include <QString>
#include "lua.hpp"

class PathKit
{
public:
	PathKit() = delete;
	QString static GetFileName(const QString& path);
	void static TryRemoveDir(const QString &dir);
	void static TryMakeDir(const QString &dir);
	void static TryCopy(const QString &src, const QString &dst);
	QString static GetCurrentPath();

	int static GetFileNameLuaWrapper(lua_State* state);
	int static TryRemoveDirLuaWrapper(lua_State* state);
	int static TryMakeDirLuaWrapper(lua_State* state);
	int static TryCopyLuaWrapper(lua_State* state);
	int static GetCurrentPathLuaWrapper(lua_State* state);

private:
	void static TryCopyFile(const QString &srcFilePath, const QString &dstFilePath);
	void static TryCopyDir(const QString &srcDirPath, const QString &dstDirPath);
};
