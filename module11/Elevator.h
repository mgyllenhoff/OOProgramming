#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <optional>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>

constexpr int MAX_FLOOR = 100;
constexpr int ELEVATOR_CAPACITY = 8;
constexpr int NUM_ELEVATORS = 4;

enum ElevatorState {
    STOPPED,
    STOPPING,
    MOVING_UP,
    MOVING_DOWN
};

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

struct Floor {
    int number;
    std::queue<std::shared_ptr<Passenger>> waiting;

    Floor(int n);
    void addWaiting(std::shared_ptr<Passenger> p);
    std::vector<std::shared_ptr<Passenger>> getWaitingUpToCapacity(int capacity);
    size_t waitingCount() const;
};

class Elevator {
private:
    int id_;
    int currentFloor_;
    ElevatorState state_;
    int moveTimer_;
    int stopTimer_;
    int moveTimePerFloor_;
    std::vector<std::shared_ptr<Passenger>> passengers_;
    int targetFloor_;

public:
    Elevator(int id, int moveTime);

    int id() const;
    int currentFloor() const;
    ElevatorState state() const;
    int passengerCount() const;
    bool isIdle() const;

    void dischargePassengers(int time, std::vector<std::shared_ptr<Passenger>>& completed);
    void boardPassengers(int time, std::shared_ptr<Floor> floor);
    std::optional<int> findNearestWaitingFloor(const std::vector<std::shared_ptr<Floor>>& floors);
    void tick(int currentTime, std::vector<std::shared_ptr<Floor>>& floors,
              std::vector<std::shared_ptr<Passenger>>& completed);
};

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
