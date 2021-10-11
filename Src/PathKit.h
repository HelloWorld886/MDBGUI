//
// Created by musmusliu on 2021/10/8.
//

#pragma once

#include <QString>

class PathKit
{
public:
	PathKit() = delete;
	QString static GetFileName(const QString& path);
};
