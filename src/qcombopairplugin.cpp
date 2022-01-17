#include "qcombopair.h"
#include "qcombopairplugin.h"
#include <QGridLayout>
#include <QtPlugin>
#include <QWidget>
QComboPairPlugin::QComboPairPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QComboPairPlugin::initialize()
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QComboPairPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QComboPairPlugin::createWidget(QWidget *parent)
{
    auto QCP =new QComboPair(parent);
    combobox = new QComboBox();
    label = new QLabel();
    auto lay = new QGridLayout();
    lay->addWidget(label, 0, 0, 1, 1);
    lay->addWidget(combobox, 0, 1, 1, 2);
    lay->setAlignment(Qt::AlignTop);
    QCP->setLayout(lay);
    return QCP ;
}

QString QComboPairPlugin::name() const
{
    return QLatin1String("QComboPair");
}

QString QComboPairPlugin::group() const
{
    return QLatin1String("");
}

QIcon QComboPairPlugin::icon() const
{
    return QIcon();
}

QString QComboPairPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QComboPairPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QComboPairPlugin::isContainer() const
{
    return false;
}

QString QComboPairPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QComboPair\" name=\"qComboPair\">\n</widget>\n");
}

QString QComboPairPlugin::includeFile() const
{
    return QLatin1String("qcombopair.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qcombopairplugin, QComboPairPlugin)
#endif // QT_VERSION < 0x050000
