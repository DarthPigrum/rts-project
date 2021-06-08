#include <QApplication>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include <QVector>
#include <deque>
#include <random>

#include "rts/graphs.h"
#include "rts/gui.h"
#include "rts/simulator.h"
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  View view;
  Simulator<2> *sim;
  QTimer *timer = new QTimer();
  QObject::connect(&view, &View::start, [&sim, &view, &timer]() {
    sim = new Simulator<2>(view.getQuantSize(), view.getWeightLimits(),
                           view.getPriorityLimits(), view.getIntervalLimits());
    timer->setInterval(view.getSpeed());
    timer->start();
    QObject::connect(timer, &QTimer::timeout, [&sim, &view]() {
      sim->tick();
      view.setCurrentTasks(sim->currentTasks());
      view.setTaskList(sim->queue());
      view.setByPriorityGraph(generateByPriorityGraph(sim->finishedTasks()));
    });
  });
  QObject::connect(&view, &View::stop, [&sim, &timer]() {
    timer->stop();
    delete sim;
  });
  QObject::connect(&view, &View::draw, [&view]() {
    const auto [minInterval, maxInterval] = view.getIntervalLimits();
    std::map<int, std::vector<std::shared_ptr<Task>>> time;
    std::map<int, double> idle;
    for (int i = minInterval; i <= maxInterval; ++i) {
      auto simulation =
          new Simulator<2>(view.getQuantSize(), view.getWeightLimits(),
                           view.getPriorityLimits(), std::make_pair(i, i));
      for (int n = 0; n < 100000; ++n)
        simulation->tick();
      time[i] = simulation->finishedTasks();
      idle[i] = simulation->loadFactor();
    }
    view.setTimeGraph(generateTimeGraph(time));
    view.setIdleGraph(generateIdleGraph(idle));
  });
  view.show();
  return a.exec();
}
