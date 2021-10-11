#include "GUIKit.h"
#include <QTabWidget>
#include "Log.h"
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QScrollArea>

LUA_EXPORT_CLASS_BEGIN(GUIKit)
LUA_EXPORT_METHOD(BeginTabWidget)
LUA_EXPORT_METHOD(EndTabWidget)
LUA_EXPORT_METHOD(BeginTab)
LUA_EXPORT_METHOD(EndTab)
LUA_EXPORT_METHOD(BeginLayout)
LUA_EXPORT_METHOD(EndLayout)
LUA_EXPORT_METHOD(Button)
LUA_EXPORT_METHOD(Label)
LUA_EXPORT_METHOD(TextField)
LUA_EXPORT_METHOD(LineField)
LUA_EXPORT_METHOD(SetLineFieldText)
LUA_EXPORT_METHOD(GetLineFieldText)
LUA_EXPORT_METHOD(ComboBox)
LUA_EXPORT_METHOD(SetComboBoxItems)
LUA_EXPORT_METHOD(OpenFileDialog)
LUA_EXPORT_METHOD(SetSizePolicy)
LUA_EXPORT_METHOD(OpenDirDialog)
LUA_EXPORT_CLASS_END()

GUIKit::GUIKit(QWidget* rootWidget, LayoutType layoutType) :
	QObject(rootWidget),
	m_rootWidget(rootWidget)
{
	m_rootLayout = CreateLayout(layoutType);
	m_rootWidget->setLayout(m_rootLayout);
}

GUIKit::~GUIKit()
{}

QLayout* GUIKit::CreateLayout(int layoutType)
{
	QLayout* layout = nullptr;
	LayoutType type = (LayoutType)layoutType;
	switch (type)
	{
	case Form:
	{
		auto formLayout = new QFormLayout;
		formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
		layout = formLayout;
		break;
	}
	case VBox:
		layout = new QVBoxLayout;
		break;
	case HBox:
		layout = new QHBoxLayout;
		break;
	case Grid:
		layout = new QVBoxLayout;
		break;
	default:
		break;
	}

	return layout;
}

void GUIKit::AddComponent(QWidget* component,
		int stretch,
		const char* prefixTitle)
{
	QLayout* top = GetTopLayout();
	Q_ASSERT(top);

	if(typeid(*top) == typeid(QFormLayout))
	{
		QFormLayout* formLayout = dynamic_cast<QFormLayout*>(top);
		Q_ASSERT(formLayout);
		if(prefixTitle)
			formLayout->addRow(prefixTitle, component);
		else
			formLayout->addRow(component);
	}
	else if(typeid(*top) == typeid(QVBoxLayout))
	{
		QVBoxLayout* vboxLayout = dynamic_cast<QVBoxLayout*>(top);
		Q_ASSERT(vboxLayout);
		vboxLayout->addWidget(component, stretch);
	}
	else if(typeid(*top) == typeid(QHBoxLayout))
	{
		QHBoxLayout* hboxLayout = dynamic_cast<QHBoxLayout*>(top);
		Q_ASSERT(hboxLayout);
		hboxLayout->addWidget(component, stretch);
	}
	else if(typeid(*top) == typeid(QGridLayout))
	{
		QGridLayout* gridLayout = dynamic_cast<QGridLayout*>(top);
		Q_ASSERT(gridLayout);
		gridLayout->addWidget(component, gridLayout->rowCount(), gridLayout->columnCount());
	}
}

void GUIKit::BeginTabWidget(const char* objectName,
		const char* prefixTitle)
{
	QWidget* top = GetTopWidget();
	Q_ASSERT(top);

	QTabWidget* tabWidget = new QTabWidget(top);
	tabWidget->setObjectName(objectName);
	AddComponent(tabWidget, 1, prefixTitle);

	m_widgetStack.push(tabWidget);
}

void GUIKit::EndTabWidget()
{
	Q_ASSERT(!m_widgetStack.isEmpty());
	Q_ASSERT(typeid(*(m_widgetStack.top())) == typeid(QTabWidget));

	m_widgetStack.pop();
}

void GUIKit::BeginTab(const char* title,
		int layoutType,
		const char* objectName)
{
	QWidget* top = m_widgetStack.top();
	Q_ASSERT(top);

	QTabWidget* tabWidget = dynamic_cast<QTabWidget*>(top);
	Q_ASSERT(tabWidget);

	QLayout* layout = CreateLayout(layoutType);
	Q_ASSERT(layout);

	QWidget* tab = new QWidget(tabWidget);
	tab->setObjectName(objectName);
	tab->setLayout(layout);
	tabWidget->addTab(tab, title);

	m_widgetStack.push(tab);
	m_layoutStack.push(layout);
}

void GUIKit::EndTab()
{
	Q_ASSERT(!m_widgetStack.isEmpty());
	Q_ASSERT(!m_layoutStack.isEmpty());

	m_widgetStack.pop();
	m_layoutStack.pop();

	Q_ASSERT(typeid(*(m_widgetStack.top())) == typeid(QTabWidget));
}

void GUIKit::BeginLayout(int layoutType,
		int stretch,
		const char* prefixTitle)
{
	QLayout* top = GetTopLayout();
	Q_ASSERT(top);

	QLayout* layout = CreateLayout(layoutType);
	Q_ASSERT(layout);

	if(typeid(*top) == typeid(QFormLayout))
	{
		QFormLayout* formLayout = dynamic_cast<QFormLayout*>(top);
		Q_ASSERT(formLayout);

		if(prefixTitle)
			formLayout->addRow(prefixTitle, layout);
		else
			formLayout->addRow(layout);
	}
	else if(typeid(*top) == typeid(QVBoxLayout))
	{
		QVBoxLayout* vboxLayout = dynamic_cast<QVBoxLayout*>(top);
		Q_ASSERT(vboxLayout);
		vboxLayout->addLayout(layout, stretch);
	}
	else if(typeid(*top) == typeid(QHBoxLayout))
	{
		QHBoxLayout* hboxLayout = dynamic_cast<QHBoxLayout*>(top);
		Q_ASSERT(hboxLayout);
		hboxLayout->addLayout(layout, stretch);
	}
	else if(typeid(*top) == typeid(QGridLayout))
	{
		QGridLayout* gridLayout = dynamic_cast<QGridLayout*>(top);
		Q_ASSERT(gridLayout);
		gridLayout->addLayout(layout, gridLayout->rowCount(), gridLayout->columnCount());
	}

	m_layoutStack.push(layout);
}

void GUIKit::EndLayout()
{
	Q_ASSERT(!m_layoutStack.isEmpty());

	m_layoutStack.pop();
}

void GUIKit::Button(const char* text,
		const char* objectName,
		int stretch,
		const char* prefixTitle)
{
	QPushButton* button = new QPushButton(text, GetTopWidget());
	button->setObjectName(objectName);
	connect(button, &QPushButton::clicked, this, [this, objectName]{
		emit ButtonClicked(objectName);
	});

	AddComponent(button, stretch, prefixTitle);
}

void GUIKit::Label(const char* text,
		const char* objectName,
		int stretch,
		const char* prefixTitle)
{
	QLabel* label = new QLabel(text, GetTopWidget());
	label->setObjectName(objectName);

	AddComponent(label, stretch, prefixTitle);
}

void GUIKit::TextField(const char* text,
		const char* objectName,
		bool readOnly,
		int stretch,
		const char* prefixTitle)
{
	QTextEdit* edit = new QTextEdit(GetTopWidget());
	edit->setText(text);
	edit->setObjectName(objectName);
	edit->setReadOnly(readOnly);
	connect(edit, &QTextEdit::textChanged, this, [this, objectName, edit]{
		emit TextFieldChanged(objectName, edit->toPlainText());
	});

	AddComponent(edit, stretch, prefixTitle);
}

void GUIKit::LineField(const char* text,
		const char* objectName,
		bool readOnly,
		int stretch,
		const char* prefixTitle)
{
	QLineEdit* edit = new QLineEdit(GetTopWidget());
	edit->setText(text);
	edit->setObjectName(objectName);
	edit->setReadOnly(readOnly);

	AddComponent(edit, stretch, prefixTitle);
}

void GUIKit::SetLineFieldText(const char* objectName,
		const char* text)
{
	QLineEdit* edit = m_rootWidget->findChild<QLineEdit*>(objectName);
	Q_ASSERT(edit);
	edit->setText(text);
}

std::string GUIKit::GetLineFieldText(const char* objectName)
{
	QLineEdit* edit = m_rootWidget->findChild<QLineEdit*>(objectName);
	Q_ASSERT(edit);
	return edit->text().toLatin1().data();
}

void GUIKit::ComboBox(const char* menus,
		const char* objectName,
		int defaultIndex,
		int stretch,
		const char* prefixTitle)
{
	QComboBox* comboBox = new QComboBox(GetTopWidget());
	comboBox->setObjectName(objectName);
	QString items(menus);
	QStringList list = items.split(';');
	comboBox->addItems(list);
	comboBox->setCurrentIndex(defaultIndex);

	connect(comboBox, &QComboBox::currentIndexChanged, this, [this, objectName](int index){
		emit ComboBoxChanged(objectName, index);
	});

	AddComponent(comboBox, stretch, prefixTitle);
}

void GUIKit::SetComboBoxItems(const char* objectName,
		const char* items)
{
	QComboBox* comboBox = m_rootWidget->findChild<QComboBox*>(objectName);
	Q_ASSERT(comboBox);
	for (int i = comboBox->count(); i >= 0; --i)
	{
		comboBox->removeItem(i);
	}
	QString itemChars(items);
	QStringList itemList = itemChars.split(";");
	for(auto item : itemList)
	{
		if(item.isEmpty())
		{
			break;
		}

		comboBox->addItem(item);
	}
}

std::string GUIKit::OpenFileDialog(
		const char* title,
		const char* dir,
		const char* filter)
{
	QString fileName = QFileDialog::getOpenFileName(
			m_rootWidget,
			title,
			dir,
			filter,
			nullptr);

	std::string result = fileName.toLatin1().data();

	return result;
}

void GUIKit::SetSizePolicy(const char* objectName,
		int horizontal,
		int vertical)
{
	QWidget* widget = m_rootWidget->findChild<QWidget*>(objectName, Qt::FindChildrenRecursively);
	Q_ASSERT(widget);
	QSizePolicy policy = widget->sizePolicy();
	policy.setHorizontalPolicy((QSizePolicy::Policy)horizontal);
	policy.setVerticalPolicy((QSizePolicy::Policy)vertical);

	widget->setSizePolicy(policy);
}

std::string GUIKit::OpenDirDialog(const char* title,
		const char* dir)
{
	QString fileName = QFileDialog::getExistingDirectory(
			m_rootWidget,
			title,
			dir);

	std::string result = fileName.toLatin1().data();

	return result;
}


