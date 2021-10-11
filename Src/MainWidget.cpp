//
// Created by musmusliu on 2021/9/17.
//

#include "MainWidget.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QSplitter>
#include "LogWidget.h"
#include "Log.h"
#include "luna/luna.h"
#include "GUIKit.h"
#include "ProcessKit.h"
#include "lua.hpp"
#include <QTimer>
#include "PathKit.h"
#include <QScrollArea>

int LogD(lua_State* luaState)
{
	const char* message = lua_tostring(luaState, 1);

	std::string error;
	lua_call_global_function(luaState, &error, "print", std::tie(), message);

	Log::GetInstance()->PushDebug(message);
	return 0;
}

int LogW(lua_State* luaState)
{
	const char* message = lua_tostring(luaState, 1);

	std::string error;
	lua_call_global_function(luaState, &error, "print", std::tie(), message);

	Log::GetInstance()->PushWarning(message);

	return 0;
}

int LogE(lua_State* luaState)
{
	const char* message = lua_tostring(luaState, 1);

	std::string error;
	lua_call_global_function(luaState, &error, "print", std::tie(), message);

	Log::GetInstance()->PushError(message);

	return 0;
}

int GetFileName(lua_State* luaState)
{
	const char* path = lua_tostring(luaState, 1);

	std::string fileName = PathKit::GetFileName(path).toLatin1().data();
	lua_pushstring(luaState, fileName.c_str());

	return 1;
}

MainWidget::MainWidget(QWidget* parent) :
		QWidget(parent),
		m_luaState(nullptr)
{
	connect(this, &QWidget::destroyed, this, &MainWidget::OnDestroyed);

	m_procKit = new ProcessKit(this);
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &MainWidget::OnTick);
	m_timer->start(1000);

	m_luaState = luaL_newstate();
	if (m_luaState == nullptr)
	{
		Log::GetInstance()->PushError(tr("no enough memory to new lua state"));
		return;
	}

	luaL_openlibs(m_luaState);

	lua_newtable(m_luaState);
	lua_pushstring(m_luaState, "GetFileName");
	lua_pushcfunction(m_luaState, GetFileName);
	lua_settable(m_luaState, 1);
	lua_setglobal(m_luaState, "Path");

	lua_register(m_luaState, "LogD", LogD);
	lua_register(m_luaState, "LogW", LogW);
	lua_register(m_luaState, "LogE", LogE);

	int ret = luaL_dofile(m_luaState, "Plugins/Main.lua");
	if (ret)
	{
		const char* error = lua_tostring(m_luaState, -1);
		Log::GetInstance()->PushError(error);
		lua_close(m_luaState);
		m_luaState = nullptr;
		return;
	}

	SetupUI();

	lua_getglobal(m_luaState, "Main");
	if (lua_gettop(m_luaState) != 0 && lua_isfunction(m_luaState, -1))
	{
		std::string error;
		lua_call_function(m_luaState, &error, std::tie(), m_guiKit, m_procKit);
		if (!error.empty())
		{
			Log::GetInstance()->PushError(error.c_str());
		}
	}
	else
	{
		Log::GetInstance()->PushError(tr("fail to call Main function"));
	}
}

MainWidget::~MainWidget()
{
}

void MainWidget::SetupUI()
{
	//function
	QGroupBox* functionBox = new QGroupBox(tr("Function"), this);
	functionBox->setMinimumSize(200, 200);
	auto functionSizePolicy = functionBox->sizePolicy();
	functionSizePolicy.setVerticalStretch(5);
	functionBox->setSizePolicy(functionSizePolicy);
	QLayout* functionLayout = SetupFunctionUI(functionBox);
	functionBox->setLayout(functionLayout);

	//log
	QGroupBox* logBox = new QGroupBox(tr("Log"), this);
	logBox->setMinimumSize(200, 100);
	auto logSizePolicy = logBox->sizePolicy();
	logSizePolicy.setVerticalStretch(1);
	logBox->setSizePolicy(logSizePolicy);
	QLayout* logLayout = SetupLogUI(logBox);
	logBox->setLayout(logLayout);

	QSplitter* splitter = new QSplitter(this);
	splitter->setOrientation(Qt::Orientation::Vertical);
	splitter->addWidget(functionBox);
	splitter->addWidget(logBox);
	splitter->setCollapsible(0, false);
	splitter->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));

	//root
	QVBoxLayout* rootLayout = new QVBoxLayout;
	rootLayout->addWidget(splitter);

	setLayout(rootLayout);
}

QLayout* MainWidget::SetupFunctionUI(QWidget* parent)
{
	m_functionWidget = new QWidget(parent);

	QScrollArea* scrollArena = new QScrollArea(parent);
	scrollArena->setWidgetResizable(true);
	scrollArena->setWidget(m_functionWidget);

	QHBoxLayout* functionLayout = new QHBoxLayout;
	functionLayout->addWidget(scrollArena);

	m_guiKit = new GUIKit(m_functionWidget, GUIKit::VBox);
	connect(m_guiKit, &GUIKit::ButtonClicked, this, &MainWidget::OnButtonClicked);
	connect(m_guiKit, &GUIKit::TextFieldChanged, this, &MainWidget::OnTextFieldChanged);
	connect(m_guiKit, &GUIKit::ComboBoxChanged, this, &MainWidget::OnComboBoxChanged);

	return functionLayout;
}

QLayout* MainWidget::SetupLogUI(QWidget* parent)
{
	m_logWidget = new LogWidget(parent);

	QHBoxLayout* logLayout = new QHBoxLayout;
	logLayout->addWidget(m_logWidget);

	return logLayout;
}

void MainWidget::OnButtonClicked(const char* objectName)
{
	if (!m_luaState)
		return;

	std::string error;
	lua_call_global_function(m_luaState, &error, "OnButtonClicked", std::tie(), objectName);
	if(!error.empty())
		Log::LogE(error.c_str());
}

void MainWidget::OnTextFieldChanged(const char* objectName, const QString& text)
{
	if (!m_luaState)
		return;

	std::string error;
	lua_call_global_function(m_luaState, &error, "OnTextFieldChanged", std::tie(), objectName);
	if(!error.empty())
		Log::LogE(error.c_str());
}

void MainWidget::OnComboBoxChanged(const char* objectName, int index)
{
	if (!m_luaState)
		return;

	std::string error;
	lua_call_global_function(m_luaState, &error, "OnComboBoxChanged", std::tie(), objectName, index);
	if(!error.empty())
		Log::LogE(error.c_str());
}

void MainWidget::OnTick()
{
	std::string error;
	lua_call_global_function(m_luaState, &error, "OnTick", std::tie(), 1);
	if(!error.empty())
		Log::LogE(error.c_str());
}

void MainWidget::OnDestroyed(QObject* obj)
{
	if(obj != this)
		return;

	std::string error;
	lua_call_global_function(m_luaState, &error, "OnDestroyed", std::tie(), 1);
	if(!error.empty())
		Log::LogE(error.c_str());
}
