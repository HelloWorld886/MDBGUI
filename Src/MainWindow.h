#pragma once

#include <QMainWindow>

class MainWidget;

class QMenuBar;

class TitleBar;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);

	~MainWindow();

private:
	void SetupUI();

	TitleBar* m_titleBar;
	QMenuBar* m_menuBar;
	MainWidget* m_mainWidget;
};