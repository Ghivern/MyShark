#ifndef DISPLAYINTERFACES_H
#define DISPLAYINTERFACES_H

#include <QListWidget>
#include <QListWidgetItem>
#include "../../../dissector/units/device.h"

class DisplayInterfaces : public QListWidget
{
    Q_OBJECT
public:
    explicit DisplayInterfaces(QWidget *parent = nullptr);

    QString GetSelectedInterface();

private:
    QString selectedInterface;

private slots:
    void slot_updateSelectedInterface(QListWidgetItem *item);
    void slot_updateSelectedInterfaceAndEmitSignal(QListWidgetItem *item);

signals:
    void selectedInterfaceChanged(QString interfaceName,bool fromFile);
};

#endif // DISPLAYINTERFACES_H
