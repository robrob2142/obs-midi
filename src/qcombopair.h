#ifndef QCOMBOPAIR_H
#define QCOMBOPAIR_H

#include <QWidget>
#include <QGridLayout>
#include <QtPlugin>
#include <QString>
#include <QLabel>
#include <QComboBox>
class QComboPair : public QWidget
{
    Q_OBJECT

public:
	QComboPair(QWidget *parent = 0);
	QComboBox *combobox;
	QLabel *label;
	QWidget *getWidget();
	QGridLayout* lay;
	void setText(QString text);
	void addItem(QString item);
	void addItems(QStringList list);
	void clear();
	QString currentText();
	
};

#endif // QCOMBOPAIR_H
