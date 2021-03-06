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

#include "plotter.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <qcustomplot.h>
#include <QtCore/qmath.h>

namespace {
double calculate_standard_derivation(QList<double> &list, double &average)
{
    if(list.length() > 1)
    {
        double standard_derivation = 0.0;
        foreach (double d, list)
        {
            standard_derivation += qPow(d-average, 2);
        }
        standard_derivation /= (list.length()-1);
        return qSqrt(standard_derivation);
    }
    else
    {
        return 0.0;
    }
}
}

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
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->pushButton->setEnabled(ui->lineEdit->text() != "");
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
}

void Analyser::on_toolButton_4_clicked()
{
    QFileDialog dialog(this, tr("Set plot output file"), "", "png-image (*.png)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("png");
    if(dialog.exec() == QFileDialog::Accepted && dialog.selectedFiles()[0].length() > 0)
    {
        ui->lineEdit_4->setText(dialog.selectedFiles()[0]);
    }
    else
    {
        ui->lineEdit_4->setText("");
    }
}

void Analyser::on_toolButton_5_clicked()
{
    QFileDialog dialog(this, tr("Set data output file"), "", "comma-seperated values (*.csv)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("csv");
    if(dialog.exec() == QFileDialog::Accepted && dialog.selectedFiles()[0].length() > 0)
    {
        ui->lineEdit_5->setText(dialog.selectedFiles()[0]);
    }
    else
    {
        ui->lineEdit_5->setText("");
    }
}

void Analyser::on_pushButton_clicked()
{
    int rounds = ui->spinBox->value()+1;
    int runs = 0;

    double needed_rounds = 0.0;
    double fitness = 0.0;
    double average = 0.0;

    double best_fitness[rounds];
    double average_fitness[rounds];

    QList<double> l_best;
    QList<double> l_average;
    QList<double> l_rounds;

    QString s;
    QStringList l;

    for(int i = 0; i < rounds; ++i)
    {
        best_fitness[i] = 0.0;
        average_fitness[i] = 0.0;
    }

    QStringList filter;
    QStringList files;
    filter << "*.csv";
    QDir dir(ui->lineEdit->text());
    files = dir.entryList(filter);

    foreach(QString f, files)
    {
        QString filename = dir.absolutePath();
        filename.append("/");
        filename.append(f);
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "WARNING in " __FILE__ << __LINE__ << ": can not open file" << filename << file.errorString();
            continue;
        }
        QTextStream stream(&file);
        stream >> s;
        if(s != "round;best_fitness;average_fitness")
        {
            qWarning() << "WARNING in " __FILE__ << __LINE__ << ": not a run result file" << filename;
            continue;
        }

        double max_best_fitness = -1.0;
        double max_average_fitness = -1.0;
        bool atEnd = false;
        ++runs;

        stream >> s;

        for(int i = 0; i < rounds; ++i)
        {
            if(!stream.atEnd())
            {
                l = s.split(";");
                if(l.length() != 3)
                {
                    qCritical() << "CRITICAL ERROR in " __FILE__ << __LINE__ << ": Unknown entry length" << l.length() << "at" << filename << i << ":" << s;
                    QMessageBox::critical(this,
                                          tr("Error"),
                                          tr("Unknown entry length - can not process further"));
                    file.close();
                    return;
                }
                best_fitness[i] += l[1].toDouble();
                average_fitness[i] += l[2].toDouble();
                max_best_fitness = l[1].toDouble();
                max_average_fitness = l[2].toDouble();

                stream >> s;

                if(stream.atEnd())
                {
                    atEnd = true;
                    needed_rounds += i;
                    fitness += max_best_fitness;
                    average += max_average_fitness;
                    l_best.append(max_best_fitness);
                    l_average.append(max_average_fitness);
                    l_rounds.append(i);
                }
            }
            else
            {
                best_fitness[i] += max_best_fitness;
                average_fitness[i] += max_average_fitness;
            }
        }

        if(!atEnd)
        {
            qCritical() << "CRITICAL ERROR in " __FILE__ << __LINE__ << ": Do not reached and at" << filename;
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("Did not reached end at %1 - maybe you have more rounds?").arg(filename)));
            return;
        }

        file.close();
    }

    if(runs == 0)
    {
        qCritical() << "CRITICAL ERROR in " __FILE__ << __LINE__ << ": Zero runs";
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("The folder does not contain any runs"));
        return;
    }

    // Calculate average
    for(int i = 0; i < rounds; ++i)
    {
        average_fitness[i] /= runs;
        best_fitness[i] /= runs;
    }
    needed_rounds /= runs;
    average /= runs;
    fitness /= runs;

    // Write csv
    if(ui->lineEdit_2->text() != "")
    {
        QFile file(ui->lineEdit_2->text());
        if(!file.open(QIODevice::WriteOnly))
        {
            qWarning() << "WARNING in " __FILE__ << __LINE__ << ": can not open file" << ui->lineEdit_2->text() << file.errorString();
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("Can not open %1").arg(ui->lineEdit_2->text())));
        }
        else
        {
            QTextStream stream(&file);
            stream << "round;best_fitness;average_fitness\n";
            for(int i = 0; i < rounds; ++i)
            {
                stream << i << ";";
                stream << best_fitness[i] << ";";
                stream << average_fitness[i] << ";\n";
            }
        }
    }

    // Write metadata
    if(ui->lineEdit_3->text() != "")
    {
        QFile file(ui->lineEdit_3->text());
        if(!file.open(QIODevice::WriteOnly))
        {
            qWarning() << "WARNING in " __FILE__ << __LINE__ << ": can not open file" << ui->lineEdit_3->text() << file.errorString();
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("Can not open %1").arg(ui->lineEdit_2->text())));
        }
        else
        {
            QTextStream stream(&file);
            stream << "runs: " << runs << "\n";
            stream << "average_rounds: " << needed_rounds << "\n";
            stream << "average_rounds_standard_deviation: " << calculate_standard_derivation(l_rounds, needed_rounds) << "\n";
            stream << "average_best_fitness: " << fitness << "\n";
            stream << "average_best_fitness_standard_deviation: " << calculate_standard_derivation(l_best, fitness) << "\n";
            stream << "average_average_fitness: " << average << "\n";
            stream << "average_average_fitness_standard_deviation: " << calculate_standard_derivation(l_average, average) << "\n";
        }
    }

    // Write graph
    if(ui->lineEdit_4->text() != "")
    {
        QCustomPlot plot;
        plot.addGraph();
        plot.graph(0)->setPen(QPen(Qt::red));
        plot.graph(0)->setName("Best fitness");
        plot.graph(0)->addToLegend();
        plot.addGraph();
        plot.graph(1)->setPen(QPen(Qt::blue));
        plot.graph(1)->setName("Average fitness");
        plot.graph(1)->addToLegend();
        for(int i = 0; i < rounds; ++i)
        {
            plot.graph(0)->addData(i, best_fitness[i]);
            plot.graph(1)->addData(i, average_fitness[i]);
        }
        plot.xAxis->setLabel("Round");
        plot.xAxis->setRange(0.0d, rounds-1);
        plot.yAxis->setLabel("Fitness");
        plot.yAxis->setRange(0.0d, 1.2d);
        plot.legend->setVisible(true);
        plot.savePng(ui->lineEdit_4->text(), 500, 500);
    }

    // Write data
    if(ui->lineEdit_5->text() != "")
    {
        QFile file(ui->lineEdit_5->text());
        if(!file.open(QIODevice::WriteOnly))
        {
            qWarning() << "WARNING in " __FILE__ << __LINE__ << ": can not open file" << ui->lineEdit_5->text() << file.errorString();
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("Can not open %1").arg(ui->lineEdit_5->text())));
        }
        else
        {
            QTextStream stream(&file);
            stream << "rounds;best_fitness;average_fitness\n";
            for(int i = 0; i < l_best.size(); ++i)
            {
                stream << l_rounds[i] << ";";
                stream << l_best[i] << ";";
                stream << l_average[i] << ";\n";
            }
        }
    }

    QMessageBox::information(this,
                             tr("Finished"),
                             tr("Finished analysis :)"));
}


void Analyser::on_actionAbout_QCustomPlot_triggered()
{
    QMessageBox::information(this,
                             tr("About QCustomPlot"),
                             tr("This program uses QCustomPlot, which is available under the GPL3+\nFor more information see http://qcustomplot.com/"));
}

void Analyser::on_actionOpen_run_plotter_triggered()
{
    Plotter plotter;
    plotter.exec();
}

