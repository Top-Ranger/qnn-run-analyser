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
    void on_pushButton_clicked();

private:
    Ui::Analyser *ui;
};

#endif // ANALYSER_H
