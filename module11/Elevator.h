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

// ---------- Passenger definition ----------
struct Passenger {
    int id;
    int startTime;
    int startFloor;
    int endFloor;
    int boardedTime = -1;
    int exitTime = -1;
    bool completed = false;

    Passenger(int i, int sTime, int sFloor, int eFloor);
};

// ---------- Floor definition ----------
struct Floor {
    int number;
    std::queue<std::shared_ptr<Passenger>> waiting;

    Floor(int n);
    void addWaiting(std::shared_ptr<Passenger> p);
    std::vector<std::shared_ptr<Passenger>> getWaitingUpToCapacity(int capacity);
    size_t waitingCount() const;
};

// ---------- Elevator definition ----------
class Elevator {
private:
    int id_;
    int currentFloor_;
    ElevatorState state_;
    int moveTimer_;             // countdown for moving between floors
    int stopTimer_;             // countdown for stopping
    int moveTimePerFloor_;      // movement time (10s or 5s)
    std::vector<std::shared_ptr<Passenger>> passengers_;
    int targetFloor_;           // next destination

public:
    Elevator(int id, int moveTime);

    // getters
    int id() const;
    int currentFloor() const;
    ElevatorState state() const;
    int passengerCount() const;
    bool isIdle() const;

    // core actions
    void dischargePassengers(int time, std::vector<std::shared_ptr<Passenger>>& completed);
    void boardPassengers(int time, std::shared_ptr<Floor> floor);
    int findNearestWaitingFloor(const std::vector<std::shared_ptr<Floor>>& floors);
    void tick(int currentTime, std::vector<std::shared_ptr<Floor>>& floors,
              std::vector<std::shared_ptr<Passenger>>& completed);
};

// ---------- Simulation controller ----------
class Simulation {
private:
    std::vector<std::shared_ptr<Floor>> floors;
    std::vector<Elevator> elevators;
    std::vector<std::shared_ptr<Passenger>> allPassengers;
    std::multimap<int, std::shared_ptr<Passenger>> arrivalsByTime;
    int totalPassengers = 0;
    int completedCount = 0;

public:
    Simulation(int moveTime);

    void loadCSV(const std::string& path);
    void releaseArrivalsAtTime(int currentTime);
    std::pair<double, double> run(); // returns avgWait, avgTravel
};

#endif
