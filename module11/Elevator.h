#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>

// ---------- Simulation constants ----------
constexpr int MAX_FLOOR = 100;          // building height
constexpr int ELEVATOR_CAPACITY = 8;    // max passengers per elevator
constexpr int NUM_ELEVATORS = 4;        // total elevators

// ---------- Elevator movement states ----------
enum ElevatorState {
    STOPPED,    // idle or waiting at a floor
    STOPPING,   // transitioning to STOPPED (2 seconds)
    MOVING_UP,
    MOVING_DOWN
};

// ============================================================================
// CLASS: Passenger
// Represents a single person traveling between floors
// ============================================================================
class Passenger {
public:
    Passenger(int i, int sTime, int sFloor, int eFloor);

    int id;
    int startTime;
    int startFloor;
    int endFloor;
    int boardedTime = -1;
    int exitTime = -1;
    bool completed = false;
};

// ============================================================================
// CLASS: Floor
// Represents a single building floor, holding waiting passengers
// ============================================================================
class Floor {
public:
    explicit Floor(int n);

    void addWaiting(std::shared_ptr<Passenger> p);

private:
    std::queue<std::shared_ptr<Passenger>> waiting;
    int number_;
    friend class Elevator; 
};

// ============================================================================
// CLASS: Elevator
// Represents an individual elevator operating in the simulation
// ============================================================================
class Elevator {
public:
    Elevator(int id, int moveTime);

    // Called once per simulation tick (main external control point)
    void tick(int currentTime,
              std::vector<std::shared_ptr<Floor>>& floors,
              std::vector<std::shared_ptr<Passenger>>& completed);

private:
    // Internal logic (hidden from outside users)
    void dischargePassengers(int time, std::vector<std::shared_ptr<Passenger>>& completed);
    void boardPassengers(int time, std::shared_ptr<Floor> floor);
    int findNearestWaitingFloor(const std::vector<std::shared_ptr<Floor>>& floors);

    // State data
    int id_;
    int currentFloor_;
    ElevatorState state_;
    int moveTimer_;
    int stopTimer_;
    int moveTimePerFloor_;
    std::vector<std::shared_ptr<Passenger>> passengers_;
    int targetFloor_;
};

// ============================================================================
// CLASS: Simulation
// Controls all elevators and manages time progression
// ============================================================================
class Simulation {
public:
    explicit Simulation(int moveTime);

    void loadCSV(const std::string& path);
    void releaseArrivalsAtTime(int currentTime);
    std::pair<double, double> run(); // returns avgWait, avgTravel

private:
    std::vector<std::shared_ptr<Floor>> floors;
    std::vector<Elevator> elevators;
    std::vector<std::shared_ptr<Passenger>> allPassengers;
    std::multimap<int, std::shared_ptr<Passenger>> arrivalsByTime;
    int totalPassengers = 0;
    int completedCount = 0;
};

#endif
