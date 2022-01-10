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
#include "GUIKit.h"
#include "ProcessKit.h"
#include "lua.hpp"
#include <QTimer>
#include "PathKit.h"
#include <QScrollArea>
#include "LuaGenerator.h"
#include "HelloLua.h"


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

	REGISTER_CLASS(Log, m_luaState);
	REGISTER_CLASS(GUIKit, m_luaState);
	REGISTER_CLASS(ProcessKit, m_luaState);

	int ret = luaL_dofile(m_luaState, "Plugins/Main.lua");
	if (ret)
	{
		const char* error = lua_tostring(m_luaState, -1);
		Log::GetInstance()->PushError(error);
		lua_close(m_luaState);
		m_luaState = nullptr;
		SetupUI();
		return;
	}

	SetupUI();

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "Main", &m_guiKit, &m_procKit);
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
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
	connect(m_guiKit, &GUIKit::RadioGroupToggled, this, &MainWidget::OnRadioGroupToggled);
	connect(m_guiKit, &GUIKit::LineFieldChanged, this, &MainWidget::OnLineFieldChanged);

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

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "OnButtonClicked", objectName);
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
	}
}

void MainWidget::OnTextFieldChanged(const char* objectName, const QString& text)
{
	if (!m_luaState)
		return;

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "OnTextFieldChanged", objectName, (std::string)text.toLatin1().data());
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
	}
}

void MainWidget::OnLineFieldChanged(const char* objectName, const QString& text)
{
	if (!m_luaState)
		return;

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "OnLineFieldChanged", objectName, (std::string)text.toLatin1().data());
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
	}
}


void MainWidget::OnComboBoxChanged(const char* objectName, int index)
{
	if (!m_luaState)
		return;

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "OnComboBoxChanged", objectName, index);
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
	}
}

void MainWidget::OnTick()
{
	if (!m_luaState)
		return;

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "OnTick", 1);
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
	}
}

void MainWidget::OnDestroyed(QObject* obj)
{
	if(obj != this || !m_luaState)
		return;

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "OnDestroyed", 1);
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
	}
}

void MainWidget::OnRadioGroupToggled(const char* objectName, int id, bool checked)
{
	if (!m_luaState)
		return;

	try
	{
		CallLuaGlobalFunctionParamNoRet(m_luaState, "OnRadioGroupToggled", objectName, id, checked);
	}
	catch (LuaException& e)
	{
		Log::GetInstance()->PushError(e.what());
	}
}
