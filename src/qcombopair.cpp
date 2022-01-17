#include "qcombopair.h"

QComboPair::QComboPair(QWidget *parent) :
    QWidget(parent)
{
	combobox = new QComboBox();
	label = new QLabel();
	lay = new QGridLayout();
	lay->addWidget(label, 0, 0, 1, 1);
	lay->addWidget(combobox, 0, 1, 1, 2);
	lay->setAlignment(Qt::AlignTop);
	this->setLayout(lay);
}

QWidget *QComboPair::getWidget()
{
	return this;
}

void QComboPair::setText(QString text)
{
	label->setText(text);
}

void QComboPair::addItem(QString item)
{
	combobox->addItem(item);
}

void QComboPair::addItems(QStringList list)
{
	combobox->addItems(list);
}

void QComboPair::clear()
{
	combobox->clear();
}
QString QComboPair::currentText()
{
	return combobox->currentText();
};
