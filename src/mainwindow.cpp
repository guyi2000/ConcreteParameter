#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constant.h"
#include "utilities.h"
#include <QString>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i < Concrete_Constant::SIZE; i++)
        ui->comboBox->addItem(Concrete_Constant::TYPE[i]);
    ui->label_6->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->label_2->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->label_7->setVisible(false);
    ui->lineEdit_6->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_clicked()
{
    ui->label_6->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->label_2->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->label_7->setVisible(false);
    ui->lineEdit_6->setVisible(false);
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->label_6->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->label_2->setVisible(true);
    ui->lineEdit->setVisible(true);
    ui->label_7->setVisible(false);
    ui->lineEdit_6->setVisible(false);
}

void MainWindow::on_radioButton_3_clicked()
{
    ui->label_6->setVisible(true);
    ui->lineEdit_5->setVisible(true);
    ui->label_2->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->label_7->setVisible(true);
    ui->lineEdit_6->setVisible(true);
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->radioButton->isChecked()) {
        double x_n = ui->lineEdit_2->text().toDouble();
        auto circle = new Circle(ui->comboBox->currentIndex());
        ui->lineEdit_3->setText(QString::number(circle->get_alpha(x_n, 10000)));
        ui->lineEdit_4->setText(QString::number(circle->get_beta(x_n, 10000)));
        auto scene = new QGraphicsScene();
        scene->addEllipse(0, 0, 200, 200);
        scene->addLine(100 - 100 * circle->get_width(x_n), 200 * x_n, 100 + 100 * circle->get_width(x_n), 200 * x_n);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
        delete circle;
    } else if (ui->radioButton_2->isChecked()) {
        double x_n = ui->lineEdit_2->text().toDouble();
        double r_ratio = ui->lineEdit->text().toDouble();
        auto ring = new Ring(ui->comboBox->currentIndex(), r_ratio);
        auto circle = new Circle(ui->comboBox->currentIndex());
        ui->lineEdit_3->setText(QString::number(ring->get_alpha(x_n, 10000)));
        ui->lineEdit_4->setText(QString::number(ring->get_beta(x_n, 10000)));
        auto scene = new QGraphicsScene();
        scene->addEllipse(0, 0, 200, 200);
        scene->addEllipse(100 - 100 * r_ratio, 100 - 100 * r_ratio, 200 * r_ratio, 200 * r_ratio);
        scene->addLine(100 - 100 * circle->get_width(x_n), 200 * x_n, 100 + 100 * circle->get_width(x_n), 200 * x_n);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
        delete ring;
        delete circle;
    } else {
        double x_n = ui->lineEdit_2->text().toDouble();
        double h_ratio = ui->lineEdit_6->text().toDouble();
        double b_ratio = ui->lineEdit_5->text().toDouble();
        auto tsection = new TSection(ui->comboBox->currentIndex(),
                                     h_ratio,
                                     b_ratio);
        ui->lineEdit_3->setText(QString::number(tsection->get_alpha(x_n, 10000)));
        ui->lineEdit_4->setText(QString::number(tsection->get_beta(x_n, 10000)));
        auto scene = new QGraphicsScene();
        scene->addLine(0, 0, 200, 0);
        scene->addLine(0, 0, 0, h_ratio * 200);
        scene->addLine(200, 0, 200, h_ratio * 200);
        scene->addLine(0, h_ratio * 200, 100 - b_ratio * 100, h_ratio * 200);
        scene->addLine(100 + b_ratio * 100, h_ratio * 200, 200, h_ratio * 200);
        scene->addLine(100 - b_ratio * 100, h_ratio * 200, 100 - b_ratio * 100, 200);
        scene->addLine(100 + b_ratio * 100, h_ratio * 200, 100 + b_ratio * 100, 200);
        scene->addLine(100 - b_ratio * 100, h_ratio * 200, 100 - b_ratio * 100, 200);
        scene->addLine(100 - b_ratio * 100, 200, 100 + b_ratio * 100, 200);
        scene->addLine(100 - 100 * tsection->get_width(x_n), 200 * x_n, 100 + 100 * tsection->get_width(x_n), 200 * x_n);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
        delete tsection;
    }
    ui->statusbar->showMessage(tr("计算完毕，绘图完成！"));
}
