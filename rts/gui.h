#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QProgressBar>
#include <deque>

#include "../3rdparty/qcustomplot/qcustomplot.h"
#include "task.h"
#include "ui_gui.h"
namespace Ui {
class GUI;
}

class View : public QMainWindow {
  Q_OBJECT
public:
  explicit View(QWidget *parent = nullptr)
      : QMainWindow(parent), ui(new Ui::GUI) {
    ui->setupUi(this);
    connect(ui->start, &QPushButton::clicked, this, &View::start);
    connect(ui->stop, &QPushButton::clicked, this, &View::stop);
    connect(ui->draw, &QPushButton::clicked, this, &View::draw);
  }
  ~View() { delete ui; }
  std::pair<int, int> getWeightLimits() {
    return std::make_pair(ui->minWeight->value(), ui->maxWeight->value());
  };
  std::pair<int, int> getPriorityLimits() {
    return std::make_pair(ui->minPriority->value(), ui->maxPriority->value());
  };
  std::pair<int, int> getIntervalLimits() {
    return std::make_pair(ui->minTicks->value(), ui->maxTicks->value());
  };
  int getSpeed() { return ui->msPerTick->value(); }
  int getQuantSize() { return ui->quantSize->value(); }
signals:
  void start();
  void stop();
  void draw();
public slots:
  void setCurrentTasks(const std::vector<std::shared_ptr<Task>> &task) {
    ui->currentTaskLabel->setText(task[0]->name());
    ui->currentTaskProgressBar->setMaximum(task[0]->weight());
    ui->currentTaskProgressBar->setValue(task[0]->progress());
    ui->currentTaskLabel_2->setText(task[1]->name());
    ui->currentTaskProgressBar_2->setMaximum(task[1]->weight());
    ui->currentTaskProgressBar_2->setValue(task[1]->progress());
  }
  void setTaskList(const std::deque<std::shared_ptr<Task>> &list) {
    int i = 0;
    ui->tasksTable->clearContents();
    ui->tasksTable->setRowCount(list.size());
    for (const auto &task : list) {
      ui->tasksTable->setItem(i, 0, new QTableWidgetItem(task->name()));
      ui->tasksTable->setItem(
          i, 1, new QTableWidgetItem(QString::number(task->priority())));
      ui->tasksTable->setItem(
          i, 2, new QTableWidgetItem(QString::number(task->weight())));
      QProgressBar *bar = new QProgressBar(ui->tasksTable);
      bar->setMaximum(task->weight());
      bar->setValue(task->progress());
      ui->tasksTable->setCellWidget(i, 3, bar);
      ++i;
    }
  }
  void setTimeGraph(QCustomPlot *plot) {
    ui->graphs->setCellWidget(0, 0, plot);
    ui->graphs->update();
  }
  void setIdleGraph(QCustomPlot *plot) {
    ui->graphs->setCellWidget(0, 1, plot);
    ui->graphs->update();
  }
  void setByPriorityGraph(QCustomPlot *plot) {
    ui->graphs->setCellWidget(0, 2, plot);
    ui->graphs->update();
  }

private:
  Ui::GUI *ui;
};

#endif // GUI_H
