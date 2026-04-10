#pragma once
#include "city_graph.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class EmergencyRouting {
public:
    CityGraph& city;

    explicit EmergencyRouting(CityGraph& g) : city(g) {}

    vector<int> findShortestPathDijkstra(int src, int dest) {
        unordered_map<int, double> dist;
        unordered_map<int, int> parent;

        for (auto& p : city.nodes) {
            dist[p.first] = 1e9;
        }

        dist[src] = 0;
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            
            int u = top.second;
            double d = top.first;

            if (d > dist[u]) continue;

            for (auto& edge : city.adjList[u]) {
                int v = edge.to;
                double w = edge.weight;

                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        vector<int> path;
        int cur = dest;
        while (cur != src) {
            path.push_back(cur);
            if (parent.find(cur) == parent.end()) {
                return {}; // No path found
            }
            cur = parent[cur];
        }
        path.push_back(src);
        reverse(path.begin(), path.end());
        
        return path;
    }

    vector<int> findShortestPathBFS(int src, int dest) {
        unordered_map<int, int> dist;
        unordered_map<int, int> parent;
        
        for (auto& p : city.nodes) {
            dist[p.first] = 1e9;
        }

        queue<int> q;
        dist[src] = 0;
        q.push(src);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (u == dest) break;

            for (auto& edge : city.adjList[u]) {
                int v = edge.to;
                if (dist[v] == 1e9) { 
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        vector<int> path;
        int cur = dest;
        while (cur != src) {
            path.push_back(cur);
            if (parent.find(cur) == parent.end()) {
                return {};
            }
            cur = parent[cur];
        }
        path.push_back(src);
        reverse(path.begin(), path.end());
        
        return path;
    }
};