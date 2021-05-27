#pragma once
#include <deque>
#include <memory>
#include <random>
#include <vector>

#include "task.h"
class Simulator {
  int timeLeftInInterval = 0;
  int totalTicks = 0;
  int productiveTicks = 0;
  int taskCounter = 0;
  int quantSize;
  int timeLeftInQuant;
  std::shared_ptr<Task> _idleTask = std::make_shared<Task>("Idle", 0, 0);
  std::shared_ptr<Task> _currentTask = _idleTask;
  std::deque<std::shared_ptr<Task>> _queue;
  std::vector<std::shared_ptr<Task>> _finishedTasks;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> weight;
  std::uniform_int_distribution<> priority;
  std::uniform_int_distribution<> interval;
  void insert(std::shared_ptr<Task> task) {
    _queue.push_back(task);
    std::sort(_queue.begin(), _queue.end(), [](const auto& a, const auto& b) {
      return a->priority() < b->priority();
    });
  }

 public:
  Simulator(int quantSize, std::pair<int, int> weightLimits,
            std::pair<int, int> priorityLimits,
            std::pair<int, int> intervalLimits)
      : quantSize(quantSize),
        timeLeftInQuant(quantSize),
        gen(rd()),
        weight(weightLimits.first, weightLimits.second),
        priority(priorityLimits.first, priorityLimits.second),
        interval(intervalLimits.first, intervalLimits.second) {
    _finishedTasks.reserve(10000);
  };
  void tick() {
    if (!timeLeftInInterval) {
      timeLeftInInterval = interval(gen);
      auto task =
          std::make_shared<Task>(++taskCounter, weight(gen), priority(gen));
      task->startAt(totalTicks);
      insert(task);
    }
    if (_queue.size() && _currentTask == _idleTask) {
      _currentTask = _queue.front();
      _queue.pop_front();
    }
    if (_currentTask != _idleTask) {
      if (_currentTask->increment()) ++productiveTicks;
      if (_currentTask->isFinished()) {
        _currentTask->endAt(totalTicks);
        _finishedTasks.push_back(_currentTask);
        _currentTask = _idleTask;
        timeLeftInQuant = quantSize;
      } else if (!timeLeftInQuant) {
        insert(_currentTask);
        _currentTask = _idleTask;
        timeLeftInQuant = quantSize;
      };
    }
    --timeLeftInQuant;
    --timeLeftInInterval;
    ++totalTicks;
  }
  const std::deque<std::shared_ptr<Task>>& queue() { return _queue; }
  const std::shared_ptr<Task>& currentTask() { return _currentTask; }
  const std::vector<std::shared_ptr<Task>>& finishedTasks() {
    return _finishedTasks;
  }
  double loadFactor() { return (double)productiveTicks / (double)totalTicks; }
};
