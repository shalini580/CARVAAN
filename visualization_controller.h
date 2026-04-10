#pragma once
#include "city_graph.h"
#include "signal_controller.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

class VisualizationController {
public:
    CityGraph& city;
    SignalController& signalCtrl;

    VisualizationController(CityGraph& cg, SignalController& sc) 
        : city(cg), signalCtrl(sc) {}

    void printCityGraph() const {
        cout << "\n================ CITY GRAPH ================\n";
        for (auto& pair : city.nodes) {
            int id = pair.first;
            auto& node = pair.second;
            cout << "Junction [" << id << "]: " << left << setw(14) << node.name 
                 << " | Vehicles Mapped: " << setw(3) << node.vehicleCount 
                 << " | Condition: " << node.congestionStr() << "\n";
                 
            for (auto& pair2 : city.adjList) {
                for (auto& edge : pair2.second) {
                    if (edge.to == id) {
                        cout << "   <- Incoming from " << left << setw(14) << city.nodes.at(edge.from).name
                             << " | dist: " << setw(4) << edge.distance
                             << " | traffic: " << setw(3) << edge.trafficDensity
                             << " | wt: " << fixed << setprecision(1) << setw(4) << edge.weight
                             << " | signal: " << (edge.isGreen ? "GREEN" : "RED  ") << "\n";
                    }
                }
            }
            cout << "\n";
        }
    }

    void printTopCongestedJunctions() const {
        vector<pair<int, Node>> sortedNodes(city.nodes.begin(), city.nodes.end());
        
        sort(sortedNodes.begin(), sortedNodes.end(), [](const pair<int, Node>& a, const pair<int, Node>& b) {
            return a.second.vehicleCount > b.second.vehicleCount;
        });

        cout << "\n======== MOST CONGESTED JUNCTIONS ========\n";
        for (const auto& p : sortedNodes) {
             cout << left << setw(15) << p.second.name 
                  << " | Total Incoming Vehicles: " << setw(3) << p.second.vehicleCount 
                  << " | Congestion: " << p.second.congestionStr() << "\n";
        }
        cout << "==========================================\n";
    }

    void printPath(const string& algo, const vector<int>& path) const {
        cout << "\nShortest Path (" << algo << "):\n";
        if (path.empty()) {
            cout << "No path found.\n";
            return;
        }
        for (size_t i = 0; i < path.size(); ++i) {
            cout << city.nodes.at(path[i]).name;
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }
};