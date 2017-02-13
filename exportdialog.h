#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "wheelwidget.h"

class WheelWidget;

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();
    static bool exportWheel(WheelWidget& wheel, QWidget *parent);

public slots:
    void chooseExportFileName();
    void writeAWheel(WheelWidget& wheel);
    void setExport();

private:
    Ui::ExportDialog *ui;
    bool exported;
};

#endif // EXPORTDIALOG_H
