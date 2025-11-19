#include "Elevator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Helper function for loadCSV() to trimp whitespace from both ends of a string
static inline void trimInPlace(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ 
        return !std::isspace(ch); 
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ 
        return !std::isspace(ch); 
    }).base(), s.end());
}

/*
Passenger class functions
*/
// Passenger onstructor
Passenger::Passenger(int i, int sTime, int sFloor, int eFloor)
    : passengerID(i), startTime(sTime), startFloor(sFloor), endFloor(eFloor) {}
// Note: boardedTime and exitTime start as -1 until those events occur.

/*
Floor class functions
*/
// Floor constructor
Floor::Floor(int n) : floorNumber(n) {}

// Adds a passenger to the waiting queue for this floor
void Floor::addWaiting(std::shared_ptr<Passenger> p) {
    waiting.push(p);
}

/*
Elevator class functions
*/
// Elevator constructor
Elevator::Elevator(int passengerID, int moveTime)
    : elevatorID(passengerID),
      currentFloor(1),            // All elevators start at floor 1
      state(STOPPED),             // Initially idle
      moveTimer(moveTime),        // Timer counts seconds until next floor move
      stopTimer(0),
      moveTimePerFloor(moveTime), // Movement speed (10s or 5s)
      targetFloor(-1) {           // No initial target
    spdlog::info("Elevator {} initialized at floor {}", elevatorID, currentFloor);
}

// Unloads passengers if the current floor is their destination floor
void Elevator::exitPassengers(int time, std::vector<std::shared_ptr<Passenger>>& completed) {
    std::vector<std::shared_ptr<Passenger>>::iterator it = passengers.begin();
    while (it != passengers.end()) {
        if ((*it)->endFloor == currentFloor) {
            // Passenger’s destination reached
            (*it)->exitTime = time; // Record when the passenger exits
            (*it)->completed = true; // Mark complete
            completed.push_back(*it);
            spdlog::info("[t={}] Elevator {}: Passenger {} exited at floor {}", 
                time, elevatorID, (*it)->passengerID, currentFloor);
            it = passengers.erase(it); // Remove passenger from elevator list
        } 
        else {
            ++it;
        }
    }
}

// Boards passengers who are waiting on the current floor (up to elevator's capacity)
void Elevator::boardPassengers(int time, std::shared_ptr<Floor> floor) {
    while ((int)passengers.size() < ELEVATOR_CAPACITY && !floor->waiting.empty()) {
        std::shared_ptr<Passenger> p = floor->waiting.front(); // First waiting passenger
        floor->waiting.pop(); // Remove passenger from queue
        p->boardedTime = time; // Record when passenger boards
        passengers.push_back(p); // Add passenger to elevator
        spdlog::info("[t={}] Elevator {}: Passenger {} boarded at floor {}", 
            time, elevatorID, p->passengerID, p->startFloor);
    }
}

// Searches all floors for one that currently has waiting passengers
int Elevator::findNearestWaitingFloor(const std::vector<std::shared_ptr<Floor>>& floors) {
    int nearest = -1;
    int minDist = MAX_FLOOR + 1;

    // Check all floors for waiting passengers
    for (std::vector<std::shared_ptr<Floor>>::const_iterator it = floors.begin(); 
        it != floors.end(); ++it) { 
        std::shared_ptr<Floor> f = *it;
        if (f && !f->waiting.empty()) {
            int dist = std::abs(f->floorNumber - currentFloor);
            if (dist < minDist) {
                minDist = dist;
                nearest = f->floorNumber;
            }
        }
    }

    // Returns the closest floor floorNumber, or -1 if none are waiting
    return nearest;
}

// Controls elevator movement, stopping, boarding, and discharging logic
// Called every simulated second
void Elevator::tick(int currentTime,
                    std::vector<std::shared_ptr<Floor>>& floors,
                    std::vector<std::shared_ptr<Passenger>>& completed) {
    // Elevator is STOPPING
    if (state == STOPPING) {
        stopTimer--; // Decrement the stop timer as elevator is stopping

        // If timer is done STOPPING, elevator becomes STOPPED
        if (stopTimer <= 0) {
            state = STOPPED;
        }
        return;
    }

    // Elevator is MOVING_UP 
    if (state == MOVING_UP) {
        moveTimer--; // Decrement the move timer to the next floor

        // If timer is done MOVING, elevator either starts STOPPING or moves to next floor
        if (moveTimer <= 0) {
            moveTimer = moveTimePerFloor; // Reset timer for next movement between floors
            if (currentFloor < MAX_FLOOR) {
                currentFloor++;
                spdlog::debug("[t={}] Elevator {} reached floor {}", 
                    currentTime, elevatorID, currentFloor);
            }

            // Check if elevator should stop here (to pick up or drop off)
            bool stopHere = false;
            for (std::vector<std::shared_ptr<Passenger>>::iterator pit = passengers.begin(); 
                pit != passengers.end(); ++pit) {
                if ((*pit)->endFloor == currentFloor) stopHere = true;
            }
            if (!floors[currentFloor]->waiting.empty()) {
                stopHere = true;
            }

            if (stopHere) {
                // Begin stopping (2s stopping delay)
                state = STOPPING;
                stopTimer = 2;
            } 
            else if (currentFloor >= MAX_FLOOR) {
                // Reached top floor so reverse direction
                state = MOVING_DOWN;
            }
        }
        return;
    }

    // Elevator is MOVING_DOWN
    if (state == MOVING_DOWN) {
        moveTimer--; // Decrement the move timer to the next floor

        // If timer is done MOVING, elevator either starts STOPPING or moves to next floor
        if (moveTimer <= 0) {
            moveTimer = moveTimePerFloor; // Reset timer for next movement between floors
            if (currentFloor > 1) {
                currentFloor--;
                spdlog::debug("[t={}] Elevator {} reached floor {}", 
                    currentTime, elevatorID, currentFloor); 
            }

            // Check if elevator should stop here (to pick up or drop off)
            bool stopHere = false;
            for (std::vector<std::shared_ptr<Passenger>>::iterator pit = passengers.begin(); 
                pit != passengers.end(); ++pit) {
                if ((*pit)->endFloor == currentFloor) stopHere = true;
            }
            if (!floors[currentFloor]->waiting.empty()) {
                stopHere = true;
            }

            if (stopHere) {
                // Begin stopping (2s stopping delay)
                state = STOPPING;
                stopTimer = 2;
            } 
            else if (currentFloor <= 1) {
                // Reached bottom floor so reverse direction
                state = MOVING_UP;
            }
        }
        return;
    }

    // Elevator is STOPPED (idle or at floor)
    if (state == STOPPED) {
        // Drop off passengers whose destination is this floor
        exitPassengers(currentTime, completed);

        // Pick up waiting passengers on this floor
        boardPassengers(currentTime, floors[currentFloor]);

        // If passengers are on board, go towards the first passenger's destination
        if (!passengers.empty()) {
            targetFloor = passengers.front()->endFloor;
            state = (targetFloor > currentFloor) ? MOVING_UP : MOVING_DOWN;
            moveTimer = moveTimePerFloor;
            spdlog::debug("[t={}] Elevator {} departing floor {} toward {} ({} passengers)", 
                currentTime, elevatorID, currentFloor, targetFloor, passengers.size()); 
        } 
        // If no passengers, search for the nearest waiting passenger
        else {
            int nextFloor = findNearestWaitingFloor(floors); 
            if (nextFloor != -1) { 
                targetFloor = nextFloor; 
                state = (targetFloor > currentFloor) ? MOVING_UP : MOVING_DOWN;
                moveTimer = moveTimePerFloor;
            }
        }
    }
}

/*
Simulation class functions
*/
// Simulator constructor (must enter time it takes to move between floors)
Simulation::Simulation(int moveTime) {
    // Create 100 floors (1-indexed)
    floors.resize(MAX_FLOOR + 1);
    for (int i = 1; i <= MAX_FLOOR; ++i) {
        floors[i] = std::make_shared<Floor>(i);
    }

    // Create NUM_ELEVATORS of elevators
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        elevators.emplace_back(i + 1, moveTime);
    }
    spdlog::info("Simulation initialized with {} elevators", NUM_ELEVATORS);
}

// Reads passenger data from the CSV file and stores it by arrival time
void Simulation::loadCSV(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open CSV " + path);
    }

    std::string line;
    int passengerID = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string a, b, c;

        // Parse 3 columns: Start Time, Start Floor, End Floor
        if (!std::getline(ss, a, ',')) continue;
        if (!std::getline(ss, b, ',')) continue;
        if (!std::getline(ss, c, ',')) continue;

        // Clean data from 3 columns
        trimInPlace(a); 
        trimInPlace(b); 
        trimInPlace(c);

        // Skip header line
        if (a == "Start Time(s)") {
            continue; 
        }

        int startTime = std::stoi(a);
        int startFloor = std::stoi(b);
        int endFloor = std::stoi(c);

        // Ignore invalid data
        if (startFloor < 1 || 
            startFloor > MAX_FLOOR || 
            endFloor < 1 || 
            endFloor > MAX_FLOOR) {
            continue; 
        }

        // Create passenger and store it globally
        std::shared_ptr<Passenger> p = 
            std::make_shared<Passenger>(++passengerID, startTime, startFloor, endFloor);
        allPassengers.push_back(p);
        arrivalsByTime.emplace(startTime, p);
    }

    totalPassengers = allPassengers.size();
    spdlog::info("Loaded {} passengers from {}", totalPassengers, path); 
}

// Releases passengers whose startTime equals the current simulation time
// Adds them to their starting floor's waiting queue
void Simulation::releaseArrivalsAtTime(int currentTime) {
    std::pair<std::multimap<int, std::shared_ptr<Passenger>>::iterator, 
        std::multimap<int, std::shared_ptr<Passenger>>::iterator> range = 
        arrivalsByTime.equal_range(currentTime);
    for (std::multimap<int, std::shared_ptr<Passenger>>::iterator it = range.first; 
        it != range.second; ++it) { 
        std::shared_ptr<Passenger> p = it->second; 
        floors[p->startFloor]->addWaiting(p);
        spdlog::info("[t={}] Passenger {} arrived on floor {} going to {}", 
            currentTime, p->passengerID, p->startFloor, p->endFloor); 
    }
    arrivalsByTime.erase(range.first, range.second);
}

// Executes the full simulation second by second until all passengers complete
std::pair<double, double> Simulation::run() {
    int currentTime = 0;
    const int IDLE_LIMIT = 60000; // Safety limit to detect stalls
    int idleTicks = 0;
    std::vector<std::shared_ptr<Passenger>> completedNow;

    spdlog::info("Simulation started"); 

    // Loop until every passenger has exited an elevator
    while (completedCount < totalPassengers) {
        // Add new passengers who arrive at this time
        releaseArrivalsAtTime(currentTime);

        // Let each elevator perform its own logic for this tick
        int beforeCompleted = completedCount;
        completedNow.clear();
        for (std::vector<Elevator>::iterator eit = elevators.begin(); eit != elevators.end(); ++eit) {
            eit->tick(currentTime, floors, completedNow);
        }

        // Update total completed elevator rides
        completedCount += completedNow.size();

        // Checks if simulation is not progressing
        if (completedCount > beforeCompleted) {
            idleTicks = 0;
        }
        else {
            idleTicks++;
        }
        if (idleTicks > IDLE_LIMIT) {
            spdlog::error("Simulation stalled at t={} completed={}/{}", 
                currentTime, completedCount, totalPassengers); 
            break;
        }

        // Move simulation clock forward a second
        ++currentTime;
    }

    // Compute results (average wait and travel times of passengers)
    double totalWait = 0, totalTravel = 0;
    int count = 0;
    for (std::vector<std::shared_ptr<Passenger>>::iterator pit = allPassengers.begin(); 
        pit != allPassengers.end(); ++pit) {
        std::shared_ptr<Passenger> p = *pit;
        if (p->boardedTime >= 0 && p->exitTime >= 0) {
            totalWait += p->boardedTime - p->startTime;
            totalTravel += p->exitTime - p->boardedTime;
            count++;
        }
    }
    double avgWait = totalWait / count;
    double avgTravel = totalTravel / count;

    spdlog::info("Simulation complete: avgWait={:.2f}s avgTravel={:.2f}s", avgWait, avgTravel); 
    return std::pair<double, double>(avgWait, avgTravel);
}
