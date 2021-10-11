//
// Created by musmusliu on 2021/10/8.
//

#include "PathKit.h"
#include <QFileInfo>

QString PathKit::GetFileName(const QString& path)
{
	QString tmp = QString(path);
	tmp.replace('\\', "/");
	int lastIndex = tmp.lastIndexOf('/');
	return tmp.mid(lastIndex + 1, tmp.length());
}

