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

#ifndef ANALYSER_H
#define ANALYSER_H

#include <QMainWindow>

namespace Ui {
class Analyser;
}

class Analyser : public QMainWindow
{
    Q_OBJECT

public:
    explicit Analyser(QWidget *parent = 0);
    ~Analyser();

private slots:
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_pushButton_clicked();
    void on_actionAbout_QCustomPlot_triggered();
    void on_actionOpen_run_plotter_triggered();

private:
    Ui::Analyser *ui;
};

#endif // ANALYSER_H
