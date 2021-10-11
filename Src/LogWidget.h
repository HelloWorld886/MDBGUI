//
// Created by musmusliu on 2021/9/17.
//

#pragma once

#include <QWidget>
#include <QStringList>

class QTextEdit;
class QPushButton;

class LogWidget : public QWidget
{
	Q_OBJECT
public:
	explicit LogWidget(QWidget* parent = nullptr);

	~LogWidget();

private slots:

	void OnDebugChanged(const QString& debug);

	void OnWarningChanged(const QString& warning);

	void OnErrorChanged(const QString& error);

	void OnClearButtonClicked(bool checked );
private:
	QPushButton* m_clearButton;
	QTextEdit* m_edit;
	QStringList m_list;
	QColor m_corlor;
};