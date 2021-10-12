#include "MainWindow.h"
#include "MainWidget.h"
#include <QMenuBar>
#include "TitleBar.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setMinimumSize(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT);

	SetupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetupUI()
{
//	m_menuBar = new QMenuBar(this);
//	m_menuBar->addMenu(tr("File"));
//	setMenuBar(m_menuBar);

	m_titleBar = new TitleBar(this);
	m_titleBar->SetTitle(WINDOW_TITLE);
	m_titleBar->SetIcon(QPixmap(":/Image/android.png"));

	m_mainWidget = new MainWidget(this);

	QWidget* wrapperWidget = new QWidget(this);
	setCentralWidget(wrapperWidget);

	auto layout = new QVBoxLayout(this);
	centralWidget()->setLayout(layout);

	layout->addWidget(m_titleBar);
	layout->addWidget(m_mainWidget);
	layout->setStretch(1, 100);
	layout->setSpacing(0);
}


