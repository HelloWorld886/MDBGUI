//
// Created by musmusliu on 2021/9/17.
//

#include "LogWidget.h"
#include "Log.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QScrollBar>


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
		auto cursor = m_edit->textCursor();
		cursor.movePosition(QTextCursor::End);
		cursor.insertText(data + "\n");
	}

	m_edit->moveCursor(QTextCursor::End);

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
	auto cursor = m_edit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat charFormat;
	charFormat.setForeground(QBrush("#ffffff"));
	cursor.insertText(debug.trimmed()+ "\n", charFormat);

	m_edit->moveCursor(QTextCursor::End);
}

void LogWidget::OnWarningChanged(const QString& warning)
{
	auto cursor = m_edit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat charFormat;
	charFormat.setForeground(QBrush("#DAA520"));
	cursor.insertText(warning.trimmed()+ "\n", charFormat);

	m_edit->moveCursor(QTextCursor::End);
}

void LogWidget::OnErrorChanged(const QString& error)
{
	auto cursor = m_edit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat charFormat;
	charFormat.setForeground(QBrush("#DC143C"));
	cursor.insertText(error.trimmed() + "\n", charFormat);

	m_edit->moveCursor(QTextCursor::End);
}

void LogWidget::OnClearButtonClicked(bool checked)
{
	Log::GetInstance()->CleanAll();
	m_edit->clear();
}
