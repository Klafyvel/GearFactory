#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog),
    exported(false)
{
    ui->setupUi(this);
    QObject::connect(ui->fileNamePushButton, SIGNAL(clicked()), this, SLOT(chooseExportFileName()));
    QObject::connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->exportPushButton, SIGNAL(clicked()), this, SLOT(setExport()));

}

void ExportDialog::chooseExportFileName()
{
    ui->fileNameLineEdit->setText(QFileDialog::getSaveFileName(this, "Save SVG", "gearFactory.svg", tr("SVG files (*.svg)")));
}

void ExportDialog::setExport()
{
    exported = true;
    ExportDialog::close();
}

bool ExportDialog::exportWheel(WheelWidget &wheel, QWidget *parent=0)
{
    ExportDialog e(parent);
    e.setModal(true);
    e.exec();
    if(e.exported)
        e.writeAWheel(wheel);
    return e.exported;
}

void ExportDialog::writeAWheel(WheelWidget& wheel)
{
    if(ui->fileNameLineEdit->text() != "")
    {
        wheel.exportSVG(ui->fileNameLineEdit->text());
    }
}

ExportDialog::~ExportDialog()
{
    delete ui;
}
