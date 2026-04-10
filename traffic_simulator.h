#pragma once
#include "city_graph.h"
#include <random>
#include <chrono>

using namespace std;

class TrafficSimulator {
public:
    CityGraph& city;
    mt19937 rng;
    int tickCount = 0;

    explicit TrafficSimulator(CityGraph& g)
        : city(g),
          rng(chrono::steady_clock::now().time_since_epoch().count()) {}

    // Adds a vehicle to a specific edge
    void addVehicleToEdge(int from, int to, int count = 1) {
        for (auto& edge : city.adjList[from]) {
            if (edge.to == to) {
                edge.trafficDensity += count;
                city.updateNodeVehicleCounts();
                return;
            }
        }
    }

    // Random traffic generation
    void generateRandomTraffic(int count) {
        if (city.nodes.empty()) return;
        
        vector<pair<int, int>> allEdges;
        for (auto& pair : city.adjList) {
            for (auto& edge : pair.second) {
                allEdges.push_back({edge.from, edge.to});
            }
        }
        if (allEdges.empty()) return;

        uniform_int_distribution<int> edgeDist(0, allEdges.size() - 1);
        
        for (int i = 0; i < count; ++i) {
            auto idx = edgeDist(rng);
            int from = allEdges[idx].first;
            int to = allEdges[idx].second;
            addVehicleToEdge(from, to, 1);
        }
    }

    // Tick simulation: vehicles on GREEN edges move/pass
    void tick() {
        tickCount++;
        for (auto& pair : city.adjList) {
            for (auto& edge : pair.second) {
                if (edge.isGreen && edge.trafficDensity > 0) {
                    // Let 1 vehicle pass per tick
                    edge.trafficDensity--;
                }
            }
        }
        city.updateNodeVehicleCounts();
    }
};