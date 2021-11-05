//
// Created by musmusliu on 2021/9/29.
//

#include "ProcessKit.h"
#include <QProcess>
#include "Log.h"
#include <QCoreApplication>

LUA_EXPORT_CLASS_BEGIN(ProcessKit)
LUA_EXPORT_METHOD(Start)
LUA_EXPORT_METHOD(GetOutput)
LUA_EXPORT_METHOD(GetError)
LUA_EXPORT_METHOD(Stop)
LUA_EXPORT_CLASS_END()

ProcessKit::ProcessKit(QObject* parent) : QObject(parent)
{
	m_proc = new QProcess(this);
}

ProcessKit::~ProcessKit()
{
	m_proc->kill();
	m_proc->close();
	m_proc->deleteLater();
}

int ProcessKit::Start(const char* program,
		const char* command,
		const char* progress,
		const char* finish,
		long elapsed,
		bool showLog)
{
	if(m_proc->state() != QProcess::NotRunning)
		return HasProcRunning;

	m_error.clear();
	m_output.clear();

	QStringList args = ParseCombinedArgString(command);
	m_proc->setProgram(program);
	m_proc->setArguments(args);
	m_proc->start();

	if (!m_proc->waitForStarted())
		return FailToStart;

	while (m_proc->state() != QProcess::NotRunning)
	{
		Log::LogD(progress);

		WaitSleep(elapsed);

		auto outBa = m_proc->readAllStandardOutput();
		auto errBa = m_proc->readAllStandardError();
		if (!outBa.isEmpty())
		{
			auto out = QString::fromLocal8Bit(outBa);
			m_output.append(out);
			if (!out.isEmpty() && showLog)
			{
				Log::LogD(out);
			}
		}
		if (!errBa.isEmpty())
		{
			auto error = QString::fromLocal8Bit(errBa);
			m_error.append(error);
			if (!error.isEmpty() && showLog)
			{
				Log::LogE(error);
			}
		}
	}

	Log::LogD(finish);

	m_proc->close();
	if(m_proc->exitStatus() != QProcess::NormalExit)
		return Crashed;
	else
		return m_proc->exitCode() == 0 ? Success : Unknown;
}

std::string ProcessKit::GetOutput() const
{
	return m_output.toUtf8().data();
}

std::string ProcessKit::GetError() const
{
	return m_error.toUtf8().data();
}

QStringList ProcessKit::ParseCombinedArgString(const QString& p_args) const
{
	QStringList args;
	QString tmp;
	int quoteCount = 0;
	bool inQuote = false;

	// Handle quoting.
	// Tokens can be surrounded by double quotes "hello world".
	// Three consecutive double quotes represent the quote character itself.
	for (int i = 0; i < p_args.size(); ++i)
	{
		if (p_args.at(i) == QLatin1Char('"'))
		{
			++quoteCount;
			if (quoteCount == 3)
			{
				// Third consecutive quote.
				quoteCount = 0;
				tmp += p_args.at(i);
			}

			continue;
		}

		if (quoteCount)
		{
			if (quoteCount == 1)
			{
				inQuote = !inQuote;
			}

			quoteCount = 0;
		}

		if (!inQuote && p_args.at(i).isSpace())
		{
			if (!tmp.isEmpty())
			{
				args += tmp;
				tmp.clear();
			}
		}
		else
		{
			tmp += p_args.at(i);
		}
	}

	if (!tmp.isEmpty())
	{
		args += tmp;
	}

	return args;
}

void ProcessKit::WaitSleep(long time)
{
	if (time <= 0) {
		return;
	}

	QElapsedTimer t;
	t.start();
	while (t.elapsed() < time) {
		QCoreApplication::processEvents();
	}
}

void ProcessKit::Stop()
{
	if(m_proc->state() == QProcess::NotRunning)
		return;

	m_proc->kill();
}
