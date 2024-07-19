#include "MyAlgorithm.h"

#include "AlgoUtils.h"

#include <queue>

MyAlgorithm::MyAlgorithm()
    : steps_(0), state_(AlgoState::CHARGING),  current_position_(DOCK_POS),
       house_manager_(){
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
    if (!walls_sensor_->isWall(dir)) {
      house_manager_.updateNeighbor(dir, current_position_, false);
    } else {
      house_manager_.updateNeighbor(dir, current_position_, true);
    }
  }
}

bool MyAlgorithm::needCharge() {
  if (current_position_ == DOCK_POS)
    return false;
  
  auto path_to_dock = house_manager_.getShortestPath(current_position_, DOCK_POS);
  int battery_needed = path_to_dock.size() + BATTERY_BUFF;
  
  // Check if we have enough battery to reach the dock and perform some work
  if (battery_needed > battery_meter_->getBatteryState() * 0.7)  // Return when 70% of battery is needed to reach dock
    return true;
  
  return false;
}

Step MyAlgorithm::work() {
  if (house_manager_.dirt(current_position_) > 0)
    return Step::Stay;

  Direction dir;
  int max_dirt = -1;
  
  for (auto d : dirPriority()) {
    auto point = getPosition(current_position_, d);
    // Add wall check here
    if (walls_sensor_->isWall(d)) {
      continue;  // Skip this direction if there's a wall
    }
    if (house_manager_.isUnexplored(point)) {
      current_position_ = point;
      return static_cast<Step>(d);
    } else if (house_manager_.exists(point) && house_manager_.dirt(point) > 0) {
      if (max_dirt < house_manager_.dirt(point)) {
        dir = d;
        max_dirt = house_manager_.dirt(point);
      }
    }
  }
  
  if (max_dirt > 0) {
    current_position_ = getPosition(current_position_, dir);
    return static_cast<Step>(dir);
  }

  state_ = AlgoState::TO_POS;
  stack_ = house_manager_.getShortestPath(current_position_, {}, true);
  if (stack_.size() * 2 > max_battery_)
    return Step::Finish;
  if(!stack_.empty()){
    dir = stack_.top();
    stack_.pop();
  current_position_ = getPosition(current_position_, dir);
  return static_cast<Step>(dir);
    }
  return Step::Stay;
}

/**
 * @todo
 * 1. handle total dirt
 */
Step MyAlgorithm::nextStep() {
  if (battery_meter_->getBatteryState() == 1 && steps_ == 0) // DEAD case
    return Step::Finish;
  steps_++;
  // std::cout << __PRETTY_FUNCTION__ << " currentpos: " <<
  // current_position_.first
  //           << " " << current_position_.second
  //           << " totaldirt: " << house_manager_.total_dirt() << " " << state_
  //           << " unexplored " << house_manager_.isUnexploredEmpty()
  //           << std::endl;

  if (steps_ != 1 && house_manager_.isUnexploredEmpty() &&
      house_manager_.total_dirt() == 0 && current_position_ == DOCK_POS)
    state_ = AlgoState::FINISH;

  if (state_ == AlgoState::FINISH) {
    return Step::Finish;
  }

  house_manager_.setDirt(current_position_, dirt_sensor_->dirtLevel());

  updateNeighbors();
  house_manager_.clean(current_position_, dirt_sensor_->dirtLevel());

  if (state_ == AlgoState::CHARGING) {
    // std::cout << __FUNCTION__ << "CHARGING" << std::endl;
    if (battery_meter_->getBatteryState() != max_battery_) {
      return Step::Stay;
    }
    state_ = AlgoState::WORKING;
  }

  if (steps_ != 1 && (needCharge() || (house_manager_.isUnexploredEmpty() &&
                                       house_manager_.total_dirt() == 0))) {
    // std::cout << __FUNCTION__ << "NEED-CHARGE" << std::endl;
    state_ = AlgoState::TO_DOCK;
    // populate stack
    stack_ = house_manager_.getShortestPath(current_position_, DOCK_POS);
    if(!stack_.empty()){
      auto dir = stack_.top();
      stack_.pop();
      current_position_ = getPosition(current_position_, dir);
      return static_cast<Step>(dir);
    }
    if (stack_.empty())
      state_ = AlgoState::CHARGING;
      return Step::Stay;
  } else if (state_ == AlgoState::TO_DOCK || state_ == AlgoState::TO_POS) {
    // std::cout << __FUNCTION__ << "TO_DOCK/POS" << std::endl;
    // if (state_ == AlgoState::TO_POS && needCharge()) {
    //   stack_ = std::stack<Direction>(); // clear stack
    //   state_ = AlgoState::TO_DOCK;
    // }
    if(!stack_.empty()){
      auto dir = stack_.top();
      stack_.pop();
      current_position_ = getPosition(current_position_, dir);
      return static_cast<Step>(dir);

    }
    // @todo check for correctness
    // if position is valid i.e unexplored or perceived
    if (stack_.empty())
      state_ = DOCK_POS == current_position_ ? AlgoState::CHARGING
                                             : AlgoState::WORKING;
    return Step::Stay;
  } else {
    return work();
  }
}
