#include "Elevator.h"

static inline void trimInPlace(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
}

/* ---------------- Passenger ---------------- */
// Constructor for a passenger
Passenger::Passenger(int i, int sTime, int sFloor, int eFloor)
    : id(i), startTime(sTime), startFloor(sFloor), endFloor(eFloor) {}

/* ---------------- Floor ---------------- */
// Constructor for a floor
Floor::Floor(int n) : number(n) {}

// Add a passenger to the floor's waiting queue
void Floor::addWaiting(std::shared_ptr<Passenger> p) { 
    waiting.push(p); 
}

// Get passengers up to the elevator's capacity
std::vector<std::shared_ptr<Passenger>> Floor::getWaitingUpToCapacity(int capacity) {
    std::vector<std::shared_ptr<Passenger>> boarding;
    while (!waiting.empty() && (int)boarding.size() < capacity) {
        boarding.push_back(waiting.front());
        waiting.pop();
    }
    return boarding;
}

// Return how many passengers are waiting on this floor
size_t Floor::waitingCount() const { 
    return waiting.size(); 
}

/* ---------------- Elevator ---------------- */
// Constructor for an elevator
Elevator::Elevator(int id, int moveTime)
    : id_(id), currentFloor_(1), state_(STOPPED),
      moveTimer_(moveTime), stopTimer_(0),
      moveTimePerFloor_(moveTime), targetFloor_(-1) {}

// Elevator info getters
int Elevator::id() const { return id_; }
int Elevator::currentFloor() const { return currentFloor_; }
ElevatorState Elevator::state() const { return state_; }
int Elevator::passengerCount() const { return passengers_.size(); }
bool Elevator::isIdle() const { return state_ == STOPPED && passengers_.empty(); }

// Unload passengers whose destination is the current floor
void Elevator::dischargePassengers(int time, std::vector<std::shared_ptr<Passenger>>& completed) {
    auto it = passengers_.begin();
    while (it != passengers_.end()) {
        if ((*it)->endFloor == currentFloor_) { // passenger reached destination
            (*it)->exitTime = time;             // record exit time
            (*it)->completed = true;            // mark as completed
            completed.push_back(*it);           // add to completed list
            it = passengers_.erase(it);         // remove from elevator
        } else ++it;
    }
}

// Board passengers from a floor up to elevator capacity
void Elevator::boardPassengers(int time, std::shared_ptr<Floor> floor) {
    while ((int)passengers_.size() < ELEVATOR_CAPACITY && !floor->waiting.empty()) {
        auto p = floor->waiting.front();
        floor->waiting.pop();
        p->boardedTime = time; // record boarding time
        passengers_.push_back(p); // add to elevator
    }
}

// Find the nearest floor with waiting passengers
std::optional<int> Elevator::findNearestWaitingFloor(const std::vector<std::shared_ptr<Floor>>& floors) {
    int nearest = -1;
    int minDist = MAX_FLOOR + 1;
    for (auto& f : floors) {
        if (f && !f->waiting.empty()) {
            int dist = std::abs(f->number - currentFloor_); // distance from current floor
            if (dist < minDist) {
                minDist = dist;
                nearest = f->number;
            }
        }
    }
    if (nearest == -1) return std::nullopt; // no waiting passengers
    return nearest;
}

// Advance the elevator by one tick (simulation step)
void Elevator::tick(int currentTime, std::vector<std::shared_ptr<Floor>>& floors,
                    std::vector<std::shared_ptr<Passenger>>& completed) {
    // STOPPING → STOPPED
    if (state_ == STOPPING) {
        stopTimer_--;
        if (stopTimer_ <= 0) state_ = STOPPED;
        return;
    }

    // MOVING UP
    if (state_ == MOVING_UP) {
        moveTimer_--; // decrement movement timer
        if (moveTimer_ <= 0) { // time to move to next floor
            moveTimer_ = moveTimePerFloor_;
            if (currentFloor_ < MAX_FLOOR) currentFloor_++; // move up

            // Check if elevator should stop at this floor
            bool stopHere = false;
            for (auto& p : passengers_)
                if (p->endFloor == currentFloor_) stopHere = true; // passenger needs to get off
            if (!floors[currentFloor_]->waiting.empty()) stopHere = true; // passengers waiting to board
            if (stopHere) {
                state_ = STOPPING;
                stopTimer_ = 2; // stop for 2 ticks
            } else if (currentFloor_ >= MAX_FLOOR) {
                state_ = MOVING_DOWN; // bounce down if reached top
            }
        }
        return;
    }

    // MOVING DOWN
    if (state_ == MOVING_DOWN) {
        moveTimer_--;
        if (moveTimer_ <= 0) { // time to move to next floor
            moveTimer_ = moveTimePerFloor_;
            if (currentFloor_ > 1) currentFloor_--; // move down

            // Check if elevator should stop at this floor
            bool stopHere = false;
            for (auto& p : passengers_)
                if (p->endFloor == currentFloor_) stopHere = true;  // passenger needs to get off
            if (!floors[currentFloor_]->waiting.empty()) stopHere = true; // passengers waiting to board
            if (stopHere) {
                state_ = STOPPING;
                stopTimer_ = 2; // stop for 2 ticks
            } else if (currentFloor_ <= 1) {
                state_ = MOVING_UP; // bounce up if reached bottom
            }
        }
        return;
    }

    // STOPPED
    if (state_ == STOPPED) {
        dischargePassengers(currentTime, completed); // unload passengers
        boardPassengers(currentTime, floors[currentFloor_]); // board waiting passengers

        if (!passengers_.empty()) {
            // If there are passengers, set target floor based on first passenger
            targetFloor_ = passengers_.front()->endFloor;
            state_ = (targetFloor_ > currentFloor_) ? MOVING_UP : MOVING_DOWN;
            moveTimer_ = moveTimePerFloor_;
        } else {
            // If no passengers, find nearest floor with waiting passengers
            auto nextFloor = findNearestWaitingFloor(floors);
            if (nextFloor) {
                targetFloor_ = *nextFloor;
                state_ = (targetFloor_ > currentFloor_) ? MOVING_UP : MOVING_DOWN;
                moveTimer_ = moveTimePerFloor_;
            }
        }
    }
}

/* ---------------- Simulation ---------------- */
// Constructor initializes floors and elevators
Simulation::Simulation(int moveTime) {
    floors.resize(MAX_FLOOR + 1);
    for (int i = 1; i <= MAX_FLOOR; ++i) // floor 0 unused
        floors[i] = std::make_shared<Floor>(i);
    for (int i = 0; i < NUM_ELEVATORS; ++i)
        elevators.emplace_back(i + 1, moveTime);
}

// Load passenger requests from a CSV file
void Simulation::loadCSV(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) throw std::runtime_error("Cannot open CSV " + path);

    std::string line;
    int id = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string a, b, c;
        if (!std::getline(ss, a, ',')) continue;
        if (!std::getline(ss, b, ',')) continue;
        if (!std::getline(ss, c, ',')) continue;
        trimInPlace(a); trimInPlace(b); trimInPlace(c);
        if (a == "Start Time(s)") continue;

        int startTime = std::stoi(a);
        int startFloor = std::stoi(b);
        int endFloor = std::stoi(c);

        // Ignore invalid floors
        if (startFloor < 1 || startFloor > MAX_FLOOR || endFloor < 1 || endFloor > MAX_FLOOR)
            continue;

        auto p = std::make_shared<Passenger>(++id, startTime, startFloor, endFloor);

        // If start==end, passenger is already done
        if (startFloor == endFloor) {
            p->boardedTime = startTime;
            p->exitTime = startTime;
            p->completed = true;
            allPassengers.push_back(p);
            completedCount++;
        } else {
            allPassengers.push_back(p);
            arrivalsByTime.emplace(startTime, p);
        }
    }
    totalPassengers = allPassengers.size();
}

// Release passengers that arrive at the current simulation time
void Simulation::releaseArrivalsAtTime(int currentTime) {
    auto range = arrivalsByTime.equal_range(currentTime);
    for (auto it = range.first; it != range.second; ++it) {
        auto p = it->second;
        if (p->startFloor >= 1 && p->startFloor <= MAX_FLOOR)
            floors[p->startFloor]->addWaiting(p);
    }
    arrivalsByTime.erase(range.first, range.second);
}

// Run the full simulation
std::pair<double, double> Simulation::run() {
    int currentTime = 0;
    const int IDLE_LIMIT = 10000; // prevent infinite stalling
    int idleTicks = 0;
    std::vector<std::shared_ptr<Passenger>> completedNow;

    while (completedCount < totalPassengers) {
        releaseArrivalsAtTime(currentTime); // release new passengers

        int beforeCompleted = completedCount;
        std::vector<int> beforeFloors, beforeStates;
        beforeFloors.reserve(elevators.size());
        beforeStates.reserve(elevators.size());
        for (auto& e : elevators) {
            beforeFloors.push_back(e.currentFloor());
            beforeStates.push_back(static_cast<int>(e.state()));
        }

        completedNow.clear();
        for (auto& e : elevators) {
            e.tick(currentTime, floors, completedNow); // move each elevator one tick
        }
        completedCount += completedNow.size();

        // Detect if simulation is making progress
        bool madeProgress = (completedCount > beforeCompleted);
        for (size_t i = 0; i < elevators.size(); ++i) {
            if (elevators[i].currentFloor() != beforeFloors[i] ||
                static_cast<int>(elevators[i].state()) != beforeStates[i]) {
                madeProgress = true;
                break;
            }
        }

        if (!madeProgress) idleTicks++; else idleTicks = 0;

        // Stall detection
        if (idleTicks > IDLE_LIMIT) {
            std::cerr << "Simulation stalled at t=" << currentTime
                      << " completed=" << completedCount << "/" << totalPassengers << "\n";
            throw std::runtime_error("Simulation stalled");
        }

        currentTime++;
    }

    // Calculate average wait and travel times
    double totalWait = 0, totalTravel = 0;
    int count = 0;
    for (auto& p : allPassengers) {
        if (!p->completed) continue;
        int wait = (p->boardedTime == -1) ? 0 : (p->boardedTime - p->startTime);
        int travel = (p->exitTime == -1 || p->boardedTime == -1) ? 0 : (p->exitTime - p->boardedTime);
        totalWait += wait;
        totalTravel += travel;
        count++;
    }

    double avgWait = totalWait / count;
    double avgTravel = totalTravel / count;

    std::cout << "Average wait time: " << avgWait << " s\n";
    std::cout << "Average travel time: " << avgTravel << " s\n";

    return {avgWait, avgTravel};
}
