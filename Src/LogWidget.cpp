//
// Created by musmusliu on 2021/9/17.
//

#include "LogWidget.h"
#include "Log.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSizePolicy>


LogWidget::LogWidget(QWidget* parent) : QWidget(parent)
{
	m_clearButton = new QPushButton(tr("Clear All"),this);
	auto sizePolicy = m_clearButton->sizePolicy();
	sizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
	m_clearButton->setSizePolicy(sizePolicy);
	connect(m_clearButton, &QPushButton::clicked, this, &LogWidget::OnClearButtonClicked);

	m_edit = new QTextEdit(this);
	m_edit->acceptRichText();
	m_edit->setReadOnly(true);
	m_edit->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);

	QVBoxLayout* rootLayout = new QVBoxLayout;
	rootLayout->addWidget(m_clearButton);
	rootLayout->addWidget(m_edit);
	setLayout(rootLayout);

	Log::GetInstance()->SetIsShowTime(true);
	Log::GetInstance()->GetAllLog(m_list);
	for (auto data: m_list)
	{
		m_edit->insertPlainText(data + "\n");
	}

	Log* logInstance = Log::GetInstance();
	connect(logInstance, &Log::DebugChanged, this, &LogWidget::OnDebugChanged);
	connect(logInstance, &Log::WarningChanged, this, &LogWidget::OnWarningChanged);
	connect(logInstance, &Log::ErrorChanged, this, &LogWidget::OnErrorChanged);

	m_corlor = m_edit->textColor();
}

LogWidget::~LogWidget() noexcept
{
}

void LogWidget::OnDebugChanged(const QString& debug)
{
	m_edit->setTextColor(QColor("#ffffff"));
	m_edit->insertPlainText(debug.trimmed() + "\n");
}

void LogWidget::OnWarningChanged(const QString& warning)
{
	m_edit->setTextColor(QColor("#DAA520"));
	m_edit->insertPlainText(warning.trimmed() + "\n");
}

void LogWidget::OnErrorChanged(const QString& error)
{
	m_edit->setTextColor(QColor("#DC143C"));
	m_edit->insertPlainText( error.trimmed() + "\n");
}

void LogWidget::OnClearButtonClicked(bool checked)
{
	Log::GetInstance()->CleanAll();
	m_edit->clear();
}
