#include "plotter.h"
#include "ui_plotter.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QStringList>

Plotter::Plotter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Plotter)
{
    ui->setupUi(this);
}

Plotter::~Plotter()
{
    delete ui;
}

void Plotter::on_toolButton_clicked()
{
    QFileDialog dialog(this, tr("Open CSV"), "", "comma-seperated values (*.csv)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if(dialog.exec() == QFileDialog::Accepted && dialog.selectedFiles()[0].length() > 0)
    {
        ui->lineEdit->setText(dialog.selectedFiles()[0]);
        ui->plotButton->setEnabled(true);
    }
    else
    {
        ui->lineEdit->setText("");
        ui->plotButton->setEnabled(false);
    }
}

void Plotter::on_plotButton_clicked()
{
    ui->saveButton->setEnabled(false);
    ui->plot->clearGraphs();

    QString s;
    QStringList l;

    QFile file(ui->lineEdit->text());
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "WARNING in " __FILE__ << __LINE__ << ": can not open file";
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Can not open file"));
        return;
    }
    QTextStream stream(&file);
    stream >> s;
    if(s != "round;best_fitness;average_fitness")
    {
        qWarning() << "WARNING in " __FILE__ << __LINE__ << ": not a run result file";
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Not a ga run file"));
        return;
    }

    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::red));
    ui->plot->graph(0)->setName("Best fitness");
    ui->plot->graph(0)->addToLegend();
    ui->plot->addGraph();
    ui->plot->graph(1)->setPen(QPen(Qt::blue));
    ui->plot->graph(1)->setName("Average fitness");
    ui->plot->graph(1)->addToLegend();

    do {
        stream >> s;
        l = s.split(";");
        if(l.length() != 3)
        {
            continue;
        }

        ui->plot->graph(0)->addData(l[0].toDouble(), l[1].toDouble());
        ui->plot->graph(1)->addData(l[0].toDouble(), l[2].toDouble());
    } while(!stream.atEnd());

    ui->plot->rescaleAxes();
    ui->plot->xAxis->setLabel("Round");
    ui->plot->yAxis->setLabel("Fitness");
    ui->plot->yAxis->setRange(0.0, 1.2);
    ui->plot->legend->setVisible(true);
    ui->plot->replot();

    ui->saveButton->setEnabled(true);
}

void Plotter::on_saveButton_clicked()
{
    QFileDialog dialog(this, tr("Save Plot"), "", "png-image (*.png)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("png");
    if(dialog.exec() == QFileDialog::Accepted && dialog.selectedFiles()[0].length() > 0)
    {
        ui->plot->savePng(dialog.selectedFiles()[0]);
    }
}
