#include "MyAlgorithm.h"
#include "AlgoUtils.h"
#include <queue>
#include <iostream>

MyAlgorithm::MyAlgorithm()
    : steps_(0), state_(AlgoState::CHARGING), current_position_(DOCK_POS),
      house_manager_() {
  house_manager_.setDirt(current_position_, int(LocType::Dock));
}

MyAlgorithm::MyAlgorithm(AbstractAlgorithm &algorithm) { *this = algorithm; }

void MyAlgorithm::setMaxSteps(std::size_t max_steps) { max_steps_ = max_steps; }

void MyAlgorithm::setWallsSensor(const WallsSensor &walls_sensor) {
  walls_sensor_ = &walls_sensor;
}

void MyAlgorithm::setDirtSensor(const DirtSensor &dirt_sensor) {
  dirt_sensor_ = &dirt_sensor;
}

void MyAlgorithm::setBatteryMeter(const BatteryMeter &battery_meter) {
  battery_meter_ = &battery_meter;
  max_battery_ = battery_meter_->getBatteryState();
}

void MyAlgorithm::updateNeighbors() {
  house_manager_.eraseUnexplored(current_position_);

  for (auto dir : dirPriority()) {
    house_manager_.updateNeighbor(dir, current_position_,
                                  walls_sensor_->isWall(dir));
  }
}

bool MyAlgorithm::needCharge() {
  if (current_position_ == DOCK_POS)
    return false;
  auto st = house_manager_.getShortestPath(current_position_, DOCK_POS);
  if (st.size() + BATTERY_BUFF > battery_meter_->getBatteryState())
    return true;
  return false;
}

bool MyAlgorithm::isSafeMove(Direction dir) {
  auto new_pos = getPosition(current_position_, dir);
  return !walls_sensor_->isWall(dir) && house_manager_.exists(new_pos);
}

Step MyAlgorithm::work() {
  if (house_manager_.dirt(current_position_) > 0)
    return Step::Stay;

  Direction best_dir = Direction::Stay;
  int max_dirt = -1;

  for (auto d : dirPriority()) {
    auto point = getPosition(current_position_, d);
    if (house_manager_.isUnexplored(point)) {
      if (isSafeMove(d)) {
        current_position_ = point;
        return static_cast<Step>(d);
      } else {
        house_manager_.markExplored(point);
      }
    } else if (house_manager_.exists(point) && house_manager_.dirt(point) > 0) {
      if (max_dirt < house_manager_.dirt(point)) {
        best_dir = d;
        max_dirt = house_manager_.dirt(point);
      }
    }
  }

  if (max_dirt > 0 && isSafeMove(best_dir)) {
    current_position_ = getPosition(current_position_, best_dir);
    return static_cast<Step>(best_dir);
  }

  state_ = AlgoState::TO_POS;
  stack_ = house_manager_.getShortestPath(current_position_, {}, true);
  
  if (stack_.size() * 2 > max_battery_)
    return Step::Finish;

  while (!stack_.empty()) {
    Direction dir = stack_.top();
    stack_.pop();
    if (isSafeMove(dir)) {
      current_position_ = getPosition(current_position_, dir);
      return static_cast<Step>(dir);
    }
  }

  return Step::Stay;
}

Step MyAlgorithm::nextStep() {
  steps_++;
  std::cout << "Step: " << steps_ << ", Position: (" << current_position_.first 
            << ", " << current_position_.second << "), State: " << static_cast<int>(state_) 
            << ", Battery: " << battery_meter_->getBatteryState() << std::endl;

  if (battery_meter_->getBatteryState() == 1 && steps_ == 0)
    return Step::Finish;

  if (steps_ != 1 && house_manager_.isUnexploredEmpty() &&
      house_manager_.total_dirt() == 0 && current_position_ == DOCK_POS)
    state_ = AlgoState::FINISH;

  if (state_ == AlgoState::FINISH)
    return Step::Finish;

  house_manager_.setDirt(current_position_, dirt_sensor_->dirtLevel());
  updateNeighbors();
  house_manager_.clean(current_position_, dirt_sensor_->dirtLevel());

  if (state_ == AlgoState::CHARGING) {
    if (battery_meter_->getBatteryState() != max_battery_)
      return Step::Stay;
    state_ = AlgoState::WORKING;
  }

  if (steps_ != 1 && (needCharge() || (house_manager_.isUnexploredEmpty() &&
                                       house_manager_.total_dirt() == 0))) {
    state_ = AlgoState::TO_DOCK;
    stack_ = house_manager_.getShortestPath(current_position_, DOCK_POS);
    
    while (!stack_.empty()) {
      Direction dir = stack_.top();
      stack_.pop();
      if (isSafeMove(dir)) {
        current_position_ = getPosition(current_position_, dir);
        return static_cast<Step>(dir);
      }
    }

    state_ = AlgoState::CHARGING;
    return Step::Stay;
  } else if (state_ == AlgoState::TO_DOCK || state_ == AlgoState::TO_POS) {
    while (!stack_.empty()) {
      Direction dir = stack_.top();
      stack_.pop();
      if (isSafeMove(dir)) {
        current_position_ = getPosition(current_position_, dir);
        return static_cast<Step>(dir);
      }
    }

    state_ = (current_position_ == DOCK_POS) ? AlgoState::CHARGING : AlgoState::WORKING;
    return Step::Stay;
  } else {
    return work();
  }
}