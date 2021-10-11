//
// Created by musmusliu on 2021/10/8.
//

#include "PathKit.h"
#include <QFileInfo>

QString PathKit::GetFileName(const QString& path)
{
	QFileInfo file(path);
	if(!file.exists())
		return "";

	QString fileName = file.fileName();
	return fileName;
}

