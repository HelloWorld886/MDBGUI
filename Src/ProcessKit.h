//
// Created by musmusliu on 2021/9/29.
//

#pragma once

#include <QThread>
#include "luna/luna.h"

class QProcess;

class ProcessKit final : public QObject
{
	Q_OBJECT
public:
	enum ExitCode
	{
		Success,
		Crashed,
		Unknown,
		FailToStart,
		HasProcRunning
	};

	explicit ProcessKit(QObject* parent = nullptr);

	~ProcessKit();

	int Start(const char* program, const char* command, bool showLog = true);

	void Stop();

	std::string GetOutput() const;

	std::string GetError() const;

	DECLARE_LUA_CLASS(ProcessKit);

private:
	QStringList ParseCombinedArgString(const QString& p_args) const;
	void WaitSleep(long time);

	QString m_output;
	QString m_error;
	QProcess* m_proc;
};
