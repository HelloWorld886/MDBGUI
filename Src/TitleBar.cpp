#include "TitleBar.h"

TitleBar::TitleBar(QWidget *parent /* = nullptr */)
		:QWidget(parent)
{
	this->win = parent;
	InitializeWidget();
}

TitleBar::~TitleBar()
{
}

void TitleBar::InitializeWidget()
{
	isPressed = false;
	setFixedHeight(TITLE_BAR_HEIGHT);

	InitializeViews();
}

void TitleBar::InitializeViews()
{
	iconLabel = new QLabel(this);
	titleLabel = new QLabel(this);

	minButton = new QPushButton(this);
	restoreButton = new QPushButton(this);
	closeButton = new QPushButton(this);

	minButton->setFixedSize(TITLE_BUTTON_SIZE, TITLE_BUTTON_SIZE);
	restoreButton->setFixedSize(TITLE_BUTTON_SIZE, TITLE_BUTTON_SIZE);
	closeButton->setFixedSize(TITLE_BUTTON_SIZE, TITLE_BUTTON_SIZE);

	iconLabel->setFixedSize(TITLE_LABEL_SIZE, TITLE_LABEL_SIZE);
	titleLabel->setFixedHeight(TITLE_LABEL_SIZE);

	iconLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setAlignment(Qt::AlignCenter);

	minButton->setIcon(QIcon(":/Image/min.png"));
	restoreButton->setIcon(QIcon(":/Image/restore.png"));
	closeButton->setIcon(QIcon(":/Image/exit.png"));

	connect(minButton, &QPushButton::clicked, this, &TitleBar::ShowMinimizedWindow);
	connect(restoreButton, &QPushButton::clicked, this, &TitleBar::ShowRestoreWindow);
	connect(closeButton, &QPushButton::clicked, this, &TitleBar::CloseWindow);

	lay = new QHBoxLayout(this);
	this->setLayout(lay);
	lay->setSpacing(0);
	lay->setContentsMargins(0,0,0,0);

	lay->addWidget(iconLabel);
	lay->addWidget(titleLabel);
	lay->addWidget(minButton);
	lay->addWidget(restoreButton);
	lay->addWidget(closeButton);
}

void TitleBar::ShowMaximizedWindow()
{
	win->showMaximized();
}

void TitleBar::ShowMinimizedWindow()
{
	win->showMinimized();
}

void TitleBar::ShowRestoreWindow()
{
	if (win->isMaximized())
	{
		win->showNormal();
	}
	else
	{
		win->showMaximized();
	}
}

void TitleBar::CloseWindow()
{
	win->close();
}

void TitleBar::SetTitle(const QString &str)
{
	titleLabel->setText(str);
}

void TitleBar::SetIcon(const QPixmap &pix)
{
	iconLabel->setPixmap(pix.scaled(iconLabel->size() - QSize(TITLE_ICON_MAG, TITLE_ICON_MAG)));
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	ShowRestoreWindow();

	return QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
	/*if (win->isMaximized())
		return;*/

	isPressed = true;
	startPos = event->globalPos();

	return QWidget::mousePressEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
	isPressed = false;

	return QWidget::mouseReleaseEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
	if (isPressed)
	{
		if (win->isMaximized())
		{
			win->showNormal();
		}

		QPoint movePos = event->globalPos() - startPos;
		startPos = event->globalPos();

		win->move(win->pos() + movePos);
	}

}