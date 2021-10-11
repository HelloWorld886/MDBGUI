//
// Created by musmusliu on 2021/9/17.
//

#pragma once

#include <QWidget>

class QLayout;

class LogWidget;

struct lua_State;

class GUIKit;

class ProcessKit;

class QTimer;

class QScrollArea;

class MainWidget : public QWidget
{
Q_OBJECT
public:
	explicit MainWidget(QWidget* parent = nullptr);

	~MainWidget();

public slots:
	void OnButtonClicked(const char* objectName);

	void OnTextFieldChanged(const char* objectName, const QString& text);

	void OnComboBoxChanged(const char* objectName, int index);

	void OnDestroyed(QObject* obj);
private:
	void SetupUI();

	QLayout* SetupFunctionUI(QWidget* parent = nullptr);

	QLayout* SetupLogUI(QWidget* parent = nullptr);

	void OnTick();

	//function
	QWidget* m_functionWidget;
	GUIKit* m_guiKit;
	ProcessKit* m_procKit;
	QTimer* m_timer;

	//log
	LogWidget* m_logWidget;

	//lua
	lua_State* m_luaState;

	friend class GUIKit;
};
