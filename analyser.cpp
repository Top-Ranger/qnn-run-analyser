/*
 * Copyright (C) 2015 Marcus Soll
 * This file is part of qnn-run-analyser.
 *
 * qnn-run-analyser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * qnn-run-analyser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qnn-run-analyser. If not, see <http://www.gnu.org/licenses/>.
 */

#include "analyser.h"
#include "ui_analyser.h"

#include <QMessageBox>
#include <QFileDialog>

Analyser::Analyser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Analyser)
{
    ui->setupUi(this);
}

Analyser::~Analyser()
{
    delete ui;
}

void Analyser::on_actionAbout_triggered()
{
    QMessageBox::information(this,
                             tr("About qnn-run-analyser"),
                             tr("qnn-run-analyser is a simple tool that processes all csv-files in a folder from an qnn-ui run\nAuthor: Marcus Soll\nLicense: GPL3+"));
}

void Analyser::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void Analyser::on_toolButton_clicked()
{
    QFileDialog dialog(this, tr("Open ga run folder"), "", "comma-seperated values (*.csv)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::Directory);
    if(dialog.exec() == QFileDialog::Accepted && dialog.selectedFiles()[0].length() > 0)
    {
        ui->lineEdit->setText(dialog.selectedFiles()[0]);
    }
    else
    {
        ui->lineEdit->setText("");
    }
    ui->pushButton->setEnabled(ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "" && ui->lineEdit_3->text() != "");
}

void Analyser::on_toolButton_2_clicked()
{
    QFileDialog dialog(this, tr("Set csv output file"), "", "comma-seperated values (*.csv)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("csv");
    if(dialog.exec() == QFileDialog::Accepted && dialog.selectedFiles()[0].length() > 0)
    {
        ui->lineEdit_2->setText(dialog.selectedFiles()[0]);
    }
    else
    {
        ui->lineEdit_2->setText("");
    }
    ui->pushButton->setEnabled(ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "" && ui->lineEdit_3->text() != "");
}

void Analyser::on_toolButton_3_clicked()
{
    QFileDialog dialog(this, tr("Set metadata output file"), "", "Run metadata (*.metadata)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("metadata");
    if(dialog.exec() == QFileDialog::Accepted && dialog.selectedFiles()[0].length() > 0)
    {
        ui->lineEdit_3->setText(dialog.selectedFiles()[0]);
    }
    else
    {
        ui->lineEdit_3->setText("");
    }
    ui->pushButton->setEnabled(ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "" && ui->lineEdit_3->text() != "");
}

void Analyser::on_pushButton_clicked()
{

}

