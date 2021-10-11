#pragma once

#include <QStack>
#include <QObject>
#include "luna/luna.h"
#include <QLayoutItem>

class QWidget;

class QTabWidget;

class QLayout;

class GUIKit final : public QObject
{
Q_OBJECT
public:
	enum LayoutType
	{
		Form = 1,
		VBox = 2,
		HBox = 3,
		Grid = 4
	};

	explicit GUIKit(QWidget*, LayoutType);

	~GUIKit();

	void BeginTabWidget(const char* objectName,
			const char* prefixTitle = nullptr);

	void EndTabWidget();

	void BeginTab(const char* title,
			int layoutType,
			const char* objectName);

	void EndTab();

	void BeginLayout(int layoutType,
			int stretch = 0,
			const char* prefixTitle = nullptr);

	void EndLayout();

	void Button(const char* text,
			const char* objectName,
			int stretch = 0,
			const char* prefixTitle = nullptr);

	void Label(const char* text,
			const char* objectName,
			int stretch = 0,
			const char* prefixTitle = nullptr);

	void TextField(const char* text,
			const char* objectName,
			bool isReadOnly,
			int stretch = 0,
			const char* prefixTitle = nullptr);

	void LineField(const char* text,
			const char* objectName,
			bool isReadOnly,
			int stretch = 0,
			const char* prefixTitle = nullptr);

	void SetLineFieldText(const char* objectName,
			const char* text);

	std::string GetLineFieldText(const char* objectName);

	void ComboBox(const char* menus,
			const char* objectName,
			int defaultIndex,
			int stretch = 0,
			const char* prefixTitle = nullptr);

	void SetComboBoxItems(const char* objectName,
			const char* items);

	std::string OpenFileDialog(const char* title,
			const char* dir,
			const char* filter);

	std::string OpenDirDialog(const char* title,
			const char* dir);

	void SetSizePolicy(const char* objectName,
			int horizontal,
			int vertical);

	DECLARE_LUA_CLASS(GUIKit);
signals:
	void ButtonClicked(const char* objectName);
	void TextFieldChanged(const char* objectName,
			const QString& text);
	void ComboBoxChanged(const char* objectName,
			int index);
private:
	QWidget* m_rootWidget;
	QLayout* m_rootLayout;
	QStack<QWidget*> m_widgetStack;
	QStack<QLayout*> m_layoutStack;

	QLayout* CreateLayout(int layoutType);

	void AddComponent(QWidget* component,
			int stretch,
			const char* prefixTitle);

	QWidget* GetTopWidget()
	{
		return m_widgetStack.isEmpty() ? m_rootWidget : m_widgetStack.top();
	}

	QLayout* GetTopLayout()
	{
		return m_layoutStack.isEmpty() ? m_rootLayout : m_layoutStack.top();
	}
};