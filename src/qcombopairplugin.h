#ifndef QCOMBOPAIRPLUGIN_H
#define QCOMBOPAIRPLUGIN_H
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QComboBox>
class QComboPairPlugin : public QObject
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
#endif // QT_VERSION >= 0x050000

public:
    QComboPairPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize();
    QString title() const;
    QComboBox *combobox;
    QLabel *label;
    

private:
    bool m_initialized;
};

#endif // QCOMBOPAIRPLUGIN_H
