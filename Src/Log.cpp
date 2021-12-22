//
// Created by musmusliu on 2021/9/18.
//

#include "Log.h"
#include <QQueue>
#include <QDateTime>

Log* Log::s_instance = nullptr;

Log* Log::GetInstance()
{
	if (nullptr == s_instance)
	{
		s_instance = new Log;
	}

	return s_instance;
}

void Log::LogD(const QString& message)
{
	GetInstance()->PushDebug(message);
}

void Log::LogW(const QString& message)
{
	GetInstance()->PushWarning(message);
}

void Log::LogE(const QString& message)
{
	GetInstance()->PushError(message);
}

void Log::LogD(const char* message)
{
	GetInstance()->PushDebug(message);
}

void Log::LogW(const char* message)
{
	GetInstance()->PushWarning(message);
}

void Log::LogE(const char* message)
{
	GetInstance()->PushError(message);
}

Log::Log()
{
	m_dataList.clear();
}

Log::~Log()
{
	for (int i = 0; i < m_dataList.count(); ++i)
	{
		delete m_dataList[i];
	}
	m_dataList.clear();
}

void Log::PushDebug(const QString& debug)
{
	if (debug.isEmpty())
		return;

	Push(LogLevel::Debug, debug);
	emit DebugChanged(Format(LogLevel::Debug, debug, m_isShowTime));
}

void Log::PushWarning(const QString& warning)
{
	if (warning.isEmpty())
		return;

	Push(LogLevel::Warning, warning);
	emit WarningChanged(Format(LogLevel::Warning, warning, m_isShowTime));
}

void Log::PushError(const QString& error)
{
	if (error.isEmpty())
		return;

	Push(LogLevel::Error, error);
	emit ErrorChanged(Format(LogLevel::Error, error, m_isShowTime));
}

void Log::CleanAll()
{
	for (int i = 0; i < m_dataList.count(); ++i)
	{
		delete m_dataList[i];
	}
	m_dataList.clear();

}

void Log::GetAllLog(QStringList& list) const
{
	list.clear();
	for (int i = 0; i < m_dataList.count(); ++i)
	{
		list.append(Format(m_dataList[i]->level, m_dataList[i]->message, m_isShowTime));
	}
}

void Log::GetAllDebug(QStringList& list) const
{
	list.clear();
	for (int i = 0; i < m_dataList.count(); ++i)
	{
		if (m_dataList[i]->level == LogLevel::Debug)
		{
			list.append(Format(Debug, m_dataList[i]->message, m_isShowTime));
		}
	}
}

void Log::GetAllWarning(QStringList& list) const
{
	list.clear();
	for (int i = 0; i < m_dataList.count(); ++i)
	{
		if (m_dataList[i]->level == LogLevel::Warning)
		{
			list.append(Format(Warning, m_dataList[i]->message, m_isShowTime));
		}
	}
}

void Log::GetAllError(QStringList& list) const
{
	list.clear();
	for (int i = 0; i < m_dataList.count(); ++i)
	{
		if (m_dataList[i]->level == LogLevel::Error)
		{
			list.append(Format(Error, m_dataList[i]->message, m_isShowTime));
		}
	}
}

void Log::Push(LogLevel level, const QString& message)
{
	if (message.isEmpty())
		return;

	if (m_dataList.count() == c_maxCount)
	{
		delete m_dataList.dequeue();
	}

	LogData* data = new LogData;
	data->level = level;
	data->message = message;
	m_dataList.enqueue(data);
}

QString Log::Format(LogLevel level, const QString& message, bool isShowTime) const
{
	QString prefix;
//	switch (level)
//	{
//	case LogLevel::Debug:
//		prefix.append("[Debug]");
//		break;
//	case LogLevel::Warning:
//		prefix.append("[Warning]");
//		break;
//	case LogLevel::Error:
//		prefix.append("[Error]");
//		break;
//	}

	if (isShowTime)
	{
		QDateTime currentTime = QDateTime::currentDateTime();
		prefix.append(QString("[%1]").arg(currentTime.toString("yyyy-MM-dd hh:mm:ss")));
	}

	return QString("%1:%2").arg(prefix, message);
}