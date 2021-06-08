#pragma once
#include <deque>
#include <memory>
#include <random>
#include <vector>

#include "task.h"
template <unsigned Cores> class Simulator {
  int timeLeftInInterval = 0;
  int totalTicks = 0;
  int productiveTicks = 0;
  int taskCounter = 0;
  int quantSize;
  std::vector<int> timeLeftInQuant;
  std::shared_ptr<Task> _idleTask = std::make_shared<Task>("Idle", 0, 0);
  std::vector<std::shared_ptr<Task>> _currentTask =
      std::vector<std::shared_ptr<Task>>(Cores, _idleTask);
  std::deque<std::shared_ptr<Task>> _queue;
  std::vector<std::shared_ptr<Task>> _finishedTasks;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> weight;
  std::uniform_int_distribution<> priority;
  std::uniform_int_distribution<> interval;
  std::uniform_real_distribution<> interuptable{0.0, 1.0};
  void insert(std::shared_ptr<Task> task) {
    _queue.push_back(task);
    std::sort(_queue.begin(), _queue.end(), [](const auto &a, const auto &b) {
      return a->priority() < b->priority();
    });
  }

public:
  Simulator(int quantSize, std::pair<int, int> weightLimits,
            std::pair<int, int> priorityLimits,
            std::pair<int, int> intervalLimits)
      : quantSize(quantSize),
        timeLeftInQuant(std::vector<int>(Cores, quantSize)), gen(rd()),
        weight(weightLimits.first, weightLimits.second),
        priority(priorityLimits.first, priorityLimits.second),
        interval(intervalLimits.first, intervalLimits.second) {
    _finishedTasks.reserve(10000);
  };
  void tick() {
    if (!timeLeftInInterval) {
      timeLeftInInterval = interval(gen);
      auto task = std::make_shared<Task>(
          ++taskCounter, weight(gen), priority(gen), interuptable(gen) < 0.9);
      task->startAt(totalTicks);
      insert(task);
    }
    for (unsigned i = 0; i < Cores; ++i) {
      if (_queue.size() && _currentTask[i] == _idleTask) {
        _currentTask[i] = _queue.front();
        _queue.pop_front();
      }
      if (_currentTask[i] != _idleTask) {
        if (_currentTask[i]->increment())
          ++productiveTicks;
        if (_currentTask[i]->isFinished()) {
          _currentTask[i]->endAt(totalTicks);
          _finishedTasks.push_back(_currentTask[i]);
          _currentTask[i] = _idleTask;
          timeLeftInQuant[i] = quantSize;
        } else if (!timeLeftInQuant[i]) {
          if (_currentTask[i]->interuptable()) {
            insert(_currentTask[i]);
            _currentTask[i] = _idleTask;
          }
          timeLeftInQuant[i] = quantSize;
        };
      }
      --timeLeftInQuant[i];
    }
    --timeLeftInInterval;
    ++totalTicks;
  }
  const std::deque<std::shared_ptr<Task>> &queue() { return _queue; }
  const std::vector<std::shared_ptr<Task>> &currentTasks() {
    return _currentTask;
  }
  const std::vector<std::shared_ptr<Task>> &finishedTasks() {
    return _finishedTasks;
  }
  double loadFactor() { return (double)productiveTicks / (double)totalTicks; }
};
