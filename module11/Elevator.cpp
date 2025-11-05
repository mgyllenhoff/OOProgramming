#include "Elevator.h"

// ============================================================================
// Helper function: trims whitespace from both ends of a string
// Used while parsing CSV data (to clean up lines that have spaces)
// ============================================================================
static inline void trimInPlace(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
}

// ============================================================================
// CLASS: Passenger
// Represents a single building passenger requesting elevator transport
// ============================================================================
Passenger::Passenger(int i, int sTime, int sFloor, int eFloor)
    : id(i), startTime(sTime), startFloor(sFloor), endFloor(eFloor) {}
// Note: boardedTime and exitTime start as -1 until those events occur.

// ============================================================================
// CLASS: Floor
// Represents a single building floor, holding waiting passengers
// ============================================================================
Floor::Floor(int n) : number_(n) {}

// Adds a passenger to the waiting queue for this floor
void Floor::addWaiting(std::shared_ptr<Passenger> p) {
    waiting.push(p);
}

// ============================================================================
// CLASS: Elevator
// Each elevator acts autonomously — it decides what to do every simulation tick
// ============================================================================
Elevator::Elevator(int id, int moveTime)
    : id_(id),
      currentFloor_(1),            // All elevators start at floor 1
      state_(STOPPED),             // Initially idle
      moveTimer_(moveTime),        // Timer counts seconds until next floor move
      stopTimer_(0),
      moveTimePerFloor_(moveTime), // Movement speed (10s or 5s)
      targetFloor_(-1) {}          // No initial target

// ============================================================================
// dischargePassengers()
// Unloads passengers whose destination floor equals the current floor
// Updates their exit times and marks them complete
// ============================================================================
void Elevator::dischargePassengers(int time, std::vector<std::shared_ptr<Passenger>>& completed) {
    std::vector<std::shared_ptr<Passenger>>::iterator it = passengers_.begin();
    while (it != passengers_.end()) {
        if ((*it)->endFloor == currentFloor_) {
            // Passenger’s destination reached
            (*it)->exitTime = time;
            (*it)->completed = true;
            completed.push_back(*it);

            std::cout << "[t=" << time << "] Elevator " << id_
                      << ": Passenger " << (*it)->id << " exits at floor "
                      << currentFloor_ << "\n";
            it = passengers_.erase(it); // remove passenger from elevator list
        } else ++it;
    }
}

// ============================================================================
// boardPassengers()
// Boards new passengers waiting on the current floor (up to capacity)
// ============================================================================
void Elevator::boardPassengers(int time, std::shared_ptr<Floor> floor) {
    while ((int)passengers_.size() < ELEVATOR_CAPACITY && !floor->waiting.empty()) {
        std::shared_ptr<Passenger> p = floor->waiting.front(); // first waiting passenger
        floor->waiting.pop(); // remove from queue
        p->boardedTime = time; // record when they boarded
        passengers_.push_back(p); // add to elevator
        std::cout << "[t=" << time << "] Elevator " << id_
                  << ": Passenger " << p->id << " boards at floor "
                  << p->startFloor << " -> " << p->endFloor << "\n";
    }
}

// ============================================================================
// findNearestWaitingFloor()
// Searches all floors for one that currently has waiting passengers
// Returns the closest floor number, or -1 if none are waiting
// ============================================================================
int Elevator::findNearestWaitingFloor(const std::vector<std::shared_ptr<Floor>>& floors) {
    int nearest = -1;
    int minDist = MAX_FLOOR + 1;

    // Check all floors for waiting passengers
    for (std::vector<std::shared_ptr<Floor>>::const_iterator it = floors.begin(); it != floors.end(); ++it) { 
        std::shared_ptr<Floor> f = *it;
        if (f && !f->waiting.empty()) {
            int dist = std::abs(f->number_ - currentFloor_);
            if (dist < minDist) {
                minDist = dist;
                nearest = f->number_;
            }
        }
    }

    return nearest;
}

// ============================================================================
// tick()
// Called every simulated second
// Controls elevator movement, stopping, boarding, and discharging logic
// ============================================================================
void Elevator::tick(int currentTime,
                    std::vector<std::shared_ptr<Floor>>& floors,
                    std::vector<std::shared_ptr<Passenger>>& completed) {

    // ----------------- Case 1: Elevator is currently STOPPING -----------------
    if (state_ == STOPPING) {
        stopTimer_--; // count down the 2-second stopping delay

        if (stopTimer_ <= 0) {
            // Once stopping time is done, elevator becomes STOPPED
            state_ = STOPPED;
            std::cout << "[t=" << currentTime << "] Elevator " << id_
                      << " now STOPPED at floor " << currentFloor_ << "\n";
        }
        return;
    }

    // ----------------- Case 2: Elevator MOVING UP -----------------
    if (state_ == MOVING_UP) {
        moveTimer_--; // count down time to next floor

        // If the elevator reached the next floor
        if (moveTimer_ <= 0) {
            moveTimer_ = moveTimePerFloor_; // reset timer for next movement
            if (currentFloor_ < MAX_FLOOR) currentFloor_++;

            // Check if elevator should stop here (to pick up or drop off)
            bool stopHere = false;
            for (std::vector<std::shared_ptr<Passenger>>::iterator pit = passengers_.begin(); pit != passengers_.end(); ++pit)
                if ((*pit)->endFloor == currentFloor_) stopHere = true;
            if (!floors[currentFloor_]->waiting.empty()) stopHere = true;

            if (stopHere) {
                // Begin stopping sequence (2 seconds)
                state_ = STOPPING;
                stopTimer_ = 2;
                std::cout << "[t=" << currentTime << "] Elevator " << id_
                          << " stopping at floor " << currentFloor_ << "\n";
            } else if (currentFloor_ >= MAX_FLOOR) {
                // Reached top of building — reverse direction
                state_ = MOVING_DOWN;
                std::cout << "[t=" << currentTime << "] Elevator " << id_
                          << " reached top, reversing\n";
            }
        }
        return;
    }

    // ----------------- Case 3: Elevator MOVING DOWN -----------------
    if (state_ == MOVING_DOWN) {
        moveTimer_--; // countdown to next floor
        if (moveTimer_ <= 0) {
            moveTimer_ = moveTimePerFloor_;
            if (currentFloor_ > 1) currentFloor_--;

            bool stopHere = false;
            for (std::vector<std::shared_ptr<Passenger>>::iterator pit = passengers_.begin(); pit != passengers_.end(); ++pit)
                if ((*pit)->endFloor == currentFloor_) stopHere = true;
            if (!floors[currentFloor_]->waiting.empty()) stopHere = true;

            if (stopHere) {
                state_ = STOPPING;
                stopTimer_ = 2;
                std::cout << "[t=" << currentTime << "] Elevator " << id_
                          << " stopping at floor " << currentFloor_ << "\n";
            } else if (currentFloor_ <= 1) {
                // At bottom — reverse direction
                state_ = MOVING_UP;
                std::cout << "[t=" << currentTime << "] Elevator " << id_
                          << " reached bottom, reversing\n";
            }
        }
        return;
    }

    // ----------------- Case 4: Elevator STOPPED (idle or at floor) -----------------
    if (state_ == STOPPED) {
        // Step 1: Drop off any passengers whose destination is this floor
        dischargePassengers(currentTime, completed);

        // Step 2: Pick up any waiting passengers on this floor
        boardPassengers(currentTime, floors[currentFloor_]);

        // Step 3: Decide what to do next
        if (!passengers_.empty()) {
            // Have passengers onboard → go toward the first passenger’s destination
            targetFloor_ = passengers_.front()->endFloor;
            state_ = (targetFloor_ > currentFloor_) ? MOVING_UP : MOVING_DOWN;
            moveTimer_ = moveTimePerFloor_;
        } else {
            int nextFloor = findNearestWaitingFloor(floors); 
            if (nextFloor != -1) { 
                targetFloor_ = nextFloor; 
                state_ = (targetFloor_ > currentFloor_) ? MOVING_UP : MOVING_DOWN;
                moveTimer_ = moveTimePerFloor_;
            }
        }
    }
}

// ============================================================================
// CLASS: Simulation
// Controls the overall system, manages all elevators, passengers, and timing
// ============================================================================
Simulation::Simulation(int moveTime) {
    // Create 100 floors (1-indexed)
    floors.resize(MAX_FLOOR + 1);
    for (int i = 1; i <= MAX_FLOOR; ++i)
        floors[i] = std::make_shared<Floor>(i);

    // Create NUM_ELEVATORS elevator objects
    for (int i = 0; i < NUM_ELEVATORS; ++i)
        elevators.emplace_back(i + 1, moveTime);
}

// ============================================================================
// loadCSV()
// Reads passenger data from the CSV file and stores it by arrival time
// ============================================================================
void Simulation::loadCSV(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) throw std::runtime_error("Cannot open CSV " + path);

    std::string line;
    int id = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string a, b, c;

        // Parse 3 columns: Start Time, Start Floor, End Floor
        if (!std::getline(ss, a, ',')) continue;
        if (!std::getline(ss, b, ',')) continue;
        if (!std::getline(ss, c, ',')) continue;

        trimInPlace(a); trimInPlace(b); trimInPlace(c);
        if (a == "Start Time(s)") continue; // skip header line

        int startTime = std::stoi(a);
        int startFloor = std::stoi(b);
        int endFloor = std::stoi(c);

        if (startFloor < 1 || startFloor > MAX_FLOOR || endFloor < 1 || endFloor > MAX_FLOOR)
            continue; // ignore invalid data

        // Create passenger and store it globally and by start time
        std::shared_ptr<Passenger> p = std::make_shared<Passenger>(++id, startTime, startFloor, endFloor);
        allPassengers.push_back(p);
        arrivalsByTime.emplace(startTime, p);
    }

    totalPassengers = allPassengers.size();
    std::cout << "Loaded " << totalPassengers << " passengers from " << path << "\n";
}

// ============================================================================
// releaseArrivalsAtTime()
// Releases passengers whose startTime equals the current simulation time
// Adds them to the waiting queue on their starting floor
// ============================================================================
void Simulation::releaseArrivalsAtTime(int currentTime) {
    std::pair<std::multimap<int, std::shared_ptr<Passenger>>::iterator, std::multimap<int, std::shared_ptr<Passenger>>::iterator> range = arrivalsByTime.equal_range(currentTime);
    for (std::multimap<int, std::shared_ptr<Passenger>>::iterator it = range.first; it != range.second; ++it) { 
        std::shared_ptr<Passenger> p = it->second; 
        floors[p->startFloor]->addWaiting(p);

        std::cout << "[t=" << currentTime << "] Passenger " << p->id
                  << " arrives on floor " << p->startFloor
                  << " -> " << p->endFloor << "\n";
    }
    arrivalsByTime.erase(range.first, range.second);
}

// ============================================================================
// run()
// Executes the full simulation second by second until all passengers complete
// ============================================================================
std::pair<double, double> Simulation::run() {
    int currentTime = 0;
    const int IDLE_LIMIT = 60000; // safety limit to detect stalls
    int idleTicks = 0;
    std::vector<std::shared_ptr<Passenger>> completedNow;

    // Loop until every passenger has exited an elevator
    while (completedCount < totalPassengers) {
        // 1. Add new passengers who arrive at this time
        releaseArrivalsAtTime(currentTime);

        // 2. Let each elevator perform its own tick of logic
        int beforeCompleted = completedCount;
        completedNow.clear();
        for (std::vector<Elevator>::iterator eit = elevators.begin(); eit != elevators.end(); ++eit)
            eit->tick(currentTime, floors, completedNow);
        // 3. Update total completions
        completedCount += completedNow.size();

        // 4. Stall detection (if simulation not progressing)
        if (completedCount > beforeCompleted) idleTicks = 0;
        else idleTicks++;

        if (idleTicks > IDLE_LIMIT) {
            std::cerr << "Simulation stalled at t=" << currentTime
                      << " completed=" << completedCount << "/" << totalPassengers << "\n";
            break;
        }

        // 5. Periodic progress output every 600s (10 minutes)
        if (currentTime % 600 == 0)
            std::cout << "[t=" << currentTime << "] progress: "
                      << completedCount << "/" << totalPassengers << " done\n";

        ++currentTime; // move simulation clock forward
    }

    // ========================================================================
    // Compute results: average wait and travel times
    // ========================================================================
    double totalWait = 0, totalTravel = 0;
    int count = 0;
    for (std::vector<std::shared_ptr<Passenger>>::iterator pit = allPassengers.begin(); pit != allPassengers.end(); ++pit) {
        std::shared_ptr<Passenger> p = *pit;
        if (p->boardedTime >= 0 && p->exitTime >= 0) {
            totalWait += p->boardedTime - p->startTime;
            totalTravel += p->exitTime - p->boardedTime;
            count++;
        }
    }

    double avgWait = totalWait / count;
    double avgTravel = totalTravel / count;

    // Final output summary
    std::cout << "\n=== RESULTS ===\n";
    std::cout << "Total passengers completed: " << count << "/" << totalPassengers << "\n";
    std::cout << "Average wait time:   " << avgWait << " s\n";
    std::cout << "Average travel time: " << avgTravel << " s\n";

    return std::pair<double, double>(avgWait, avgTravel);
}
