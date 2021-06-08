#pragma once
#include "../3rdparty/qcustomplot/qcustomplot.h"
#include "task.h"
QCustomPlot *generateByPriorityGraph(std::vector<std::shared_ptr<Task>> tasks) {
  auto res = new QCustomPlot(nullptr);
  res->addGraph();
  res->graph(0)->setPen(QPen(Qt::blue));
  res->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
  std::map<int, std::pair<double, int>> entries;
  for (const auto &task : tasks) {
    entries[task->priority()] = std::make_pair(
        entries[task->priority()].first + task->timeBeforeEnding(),
        entries[task->priority()].second + 1);
  }
  QVector<double> x, y;
  for (const auto &entry : entries) {
    x.push_back(entry.first);
    y.push_back(entry.second.first / entry.second.second);
  }
  res->graph(0)->setData(x, y);
  res->graph(0)->rescaleAxes();
  return res;
}

QCustomPlot *generateTimeGraph(
    std::map<int, std::vector<std::shared_ptr<Task>>> simulations) {
  auto res = new QCustomPlot(nullptr);
  res->addGraph();
  res->graph(0)->setPen(QPen(Qt::blue));
  res->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

  QVector<double> x, y;
  for (const auto &entry : simulations) {
    x.push_back(entry.first);
    double sum = 0;
    for (const auto &task : entry.second) {
      sum += task->timeBeforeEnding();
    }
    y.push_back(sum / entry.second.size());
  }
  res->graph(0)->setData(x, y);
  res->graph(0)->rescaleAxes();
  return res;
}
QCustomPlot *generateIdleGraph(std::map<int, double> simulations) {
  auto res = new QCustomPlot(nullptr);
  res->addGraph();
  res->graph(0)->setPen(QPen(Qt::blue));
  res->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

  QVector<double> x, y;
  for (const auto &entry : simulations) {
    x.push_back(entry.first);
    y.push_back(100.0 - 100.0 * entry.second);
  }
  res->graph(0)->setData(x, y);
  res->graph(0)->rescaleAxes();
  return res;
}
