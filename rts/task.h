#pragma once
#include <QString>
class Task {
  int _progress = 0;
  int _weight;
  int _priority;
  int _startTick = 0;
  int _endTick = 0;
  QString _name;
  bool _interuptable;

public:
  explicit Task(QString name, int weight, int priority,
                bool interuptable = true)
      : _weight(weight), _priority(priority), _name(name),
        _interuptable(interuptable) {}
  explicit Task(int index, int weight, int priority, bool interuptable = true)
      : _weight(weight), _priority(priority),
        _name(QString("#") + QString::number(index)),
        _interuptable(interuptable){};
  bool isFinished() { return _weight == _progress; };
  bool increment() {
    bool res = _progress < _weight;
    if (res)
      _progress++;
    return res;
  }
  void startAt(int tick) { _startTick = tick; }
  void endAt(int tick) { _endTick = tick; }
  int timeBeforeEnding() { return _endTick - _startTick; }
  QString name() const { return _name; }
  int progress() const { return _progress; }
  int weight() const { return _weight; }
  int priority() const { return _priority; }
  bool interuptable() const { return _interuptable; }
};
