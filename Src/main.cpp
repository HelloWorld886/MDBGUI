#include "MainWindow.h"
#include <QApplication>
#include <QFile>

QByteArray ReadTextFile(const QString &file_path) {
	QFile input_file(file_path);
	QByteArray input_data;

	if (input_file.open(QIODevice::Text | QIODevice::Unbuffered | QIODevice::ReadOnly)) {
		input_data = input_file.readAll();
		input_file.close();
		return input_data;
	}
	else {
		return QByteArray();
	}
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QString style = ReadTextFile(":/Style/MacOS.qss");
	app.setStyleSheet(style);

	MainWindow mainWindow;
	mainWindow.resize(600, 800);
	mainWindow.show();

	int ret = app.exec();

	return ret;
}