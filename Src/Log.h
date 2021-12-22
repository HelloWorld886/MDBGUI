//
// Created by musmusliu on 2021/9/18.
//

#pragma once

#include <QString>
#include <QQueue>
#include <QObject>
#include <iostream>

enum LogLevel
{
	Debug,
	Warning,
	Error
};
Q_ENUMS(LogLevel);

struct LogData
{
public:
	LogLevel level;
	QString message;
};

class Log : public QObject
{
	Q_OBJECT
public:
	~Log();

	static void LogD(const QString& message);

	static void LogW(const QString& message);

	static void LogE(const QString& message);

	static void LogD(const char* message);

	static void LogW(const char* message);

	static void LogE(const char* message);

	static Log* GetInstance();

	static const int c_maxCount = 1000;

	void PushDebug(const QString& debug);

	void PushWarning(const QString& warning);

	void PushError(const QString& error);

	void CleanAll();

	void GetAllLog(QStringList& list) const;

	void GetAllDebug(QStringList& list) const;

	void GetAllWarning(QStringList& list) const;

	void GetAllError(QStringList& list) const;

	inline void SetIsShowTime(bool isShowTime) { m_isShowTime = isShowTime; };

	inline bool GetIsShowTime() { return m_isShowTime; };
signals:

	void DebugChanged(const QString& debug);

	void WarningChanged(const QString& warning);

	void ErrorChanged(const QString& error);

private:
	explicit Log();

	static Log* s_instance;

	void Push(LogLevel level, const QString& message);

	QString Format(LogLevel level, const QString& message, bool isShowTime) const;

	QQueue<LogData*> m_dataList;

	bool m_isShowTime;
};


