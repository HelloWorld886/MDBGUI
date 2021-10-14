//
// Created by musmusliu on 2021/10/8.
//

#include "PathKit.h"
#include <QFileInfo>
#include <QDir>
#include "Log.h"

QString PathKit::GetFileName(const QString& path)
{
	QString tmp = QString(path);
	tmp.replace('\\', "/");
	int lastIndex = tmp.lastIndexOf('/');
	return tmp.mid(lastIndex + 1, tmp.length());
}

void PathKit::TryRemoveDir(const QString& dirName)
{
	QDir dir;
	if (dir.exists(dirName))
		dir.rmdir(dirName);
}

void PathKit::TryMakeDir(const QString& dirName)
{
	QDir dir;
	if (!dir.exists(dirName))
		dir.mkdir(dirName);
}

void PathKit::TryCopy(const QString& src, const QString& dst)
{
	QString srcPath(src);
	QString dstPath(dst);
	srcPath.replace("\\", "/");
	dstPath.replace("\\", "/");

	QFileInfo dstFileInfo(dst);
	QFileInfo srcFileInfo(src);
	if (srcFileInfo.isFile())
	{
		if(dstFileInfo.isFile())
			TryCopyFile(srcFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath());
		else if(dstFileInfo.isDir())
		{
			QFile file(srcFileInfo.absoluteFilePath());
			QString fileName = file.fileName();
			file.close();
			QString dstFilePath = QString("%1/%2").arg(dstPath, fileName);
			TryCopyFile(srcFileInfo.absoluteFilePath(), dstFilePath);
		}
	}
	else if (srcFileInfo.isDir() && dstFileInfo.isDir())
	{
		TryCopyDir(srcFileInfo.absolutePath(), dstFileInfo.absolutePath());
	}
}

void PathKit::TryCopyFile(const QString& srcFilePath, const QString& dstFilePath)
{
	if (srcFilePath == srcFilePath)
		return;

	if (!QFile::exists(srcFilePath))
		return;

	if (QFile::exists(dstFilePath))
		QFile::remove(dstFilePath);

	QFile::copy(srcFilePath, dstFilePath);
}

void PathKit::TryCopyDir(const QString& srcDirPath, const QString& dstDirPath)
{
	QDir srcDir(srcDirPath);
	QDir dstDir(dstDirPath);
	if (!dstDir.exists() || !dstDir.mkdir(dstDir.absolutePath()))//目的文件目录不存在则创建文件目录
			return;

	QFileInfoList fileInfoList = srcDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList)
	{
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir())
			TryCopyDir(fileInfo.filePath(), dstDir.filePath(fileInfo.fileName()));
		else
		{
			//当允许覆盖操作时，将旧文件进行删除操作
			if (dstDir.exists(fileInfo.fileName()))
				dstDir.remove(fileInfo.fileName());

			// 进行文件copy
			QFile::copy(fileInfo.filePath(), dstDir.filePath(fileInfo.fileName()));
		}
	}
}

int PathKit::GetFileNameLuaWrapper(lua_State* state)
{
	const char* path = lua_tostring(state, 1);

	std::string fileName = GetFileName(path).toLatin1().data();
	lua_pushstring(state, fileName.c_str());

	return 1;
}

int PathKit::TryRemoveDirLuaWrapper(lua_State* state)
{
	const char* path = lua_tostring(state, 1);

	TryRemoveDir(path);

	return 0;
}

int PathKit::TryMakeDirLuaWrapper(lua_State* state)
{
	const char* path = lua_tostring(state, 1);

	TryMakeDir(path);

	return 0;
}

int PathKit::TryCopyLuaWrapper(lua_State* state)
{
	const char* src = lua_tostring(state, 1);
	const char* dst = lua_tostring(state, 1);

	TryCopy(src, dst);

	return 0;
}

QString PathKit::GetCurrentPath()
{
	return QDir::currentPath();
}

int PathKit::GetCurrentPathLuaWrapper(lua_State* state)
{
	std::string currentPath = GetCurrentPath().toLatin1().data();
	lua_pushstring(state, currentPath.c_str());

	return 1;
}

