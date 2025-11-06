#include "Elevator.h"
#include <iostream>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
    try {
        const std::string csvPath = "Mod10_Assignment_Elevators.csv";

        // FILE-ONLY LOGGER
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("simulation.log", true);
        auto logger = std::make_shared<spdlog::logger>("file_logger", file_sink);
        spdlog::set_default_logger(logger);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v"); 
        spdlog::set_level(spdlog::level::info); 

        // Simulations
        spdlog::info("Elevator Simulation (10s per floor)");
        Simulation sim10(10);
        sim10.loadCSV(csvPath);
        auto [avgWait10, avgTravel10] = sim10.run();
        spdlog::info("End of 10s Simulation");

        spdlog::info("Elevator Simulation (5s per floor)");
        Simulation sim5(5);
        sim5.loadCSV(csvPath);
        auto [avgWait5, avgTravel5] = sim5.run();
        spdlog::info("End of 5s Simulation");

        spdlog::info("Both simulations completed");
        spdlog::shutdown();

        // Print results
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nSimulation with 10 seconds per floor" << std::endl;
        std::cout << "\tAverage wait time:   " << avgWait10 << " s" << std::endl;
        std::cout << "\tAverage travel time: " << avgTravel10 << " s" << std::endl;

        // Simulation with 5s per floor
        std::cout << "\nSimulation with 5 seconds per floor" << std::endl;
        std::cout << "\tAverage wait time:   " << avgWait5 << " s" << std::endl;
        std::cout << "\tAverage travel time: " << avgTravel5 << " s" << std::endl;

        // Compute percentage improvement
        double avgWaitReduction = 100.0 * (avgWait10 - avgWait5) / avgWait10;
        double avgTravelReduction = 100.0 * (avgTravel10 - avgTravel5) / avgTravel10;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nReduction of average wait time: " << avgWaitReduction << "%\n";
        std::cout << "Reduction of average travel time: " << avgTravelReduction << "%\n";
    }
    catch (const std::exception& ex) {
        spdlog::error("Fatal error: {}", ex.what());
        std::cerr << "Error: " << ex.what() << std::endl;
        spdlog::shutdown();
        return 1;
    }

    return 0;
}
