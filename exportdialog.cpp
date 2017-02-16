/*
Gear Factory
Copyright (C) 2017  Hugo LEVY-FALK

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
