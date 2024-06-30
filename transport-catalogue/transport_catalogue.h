#pragma once
#include <deque>
#include <string>
#include <vector>
 
#include <iomanip>
#include <iostream>
 
#include <unordered_set>
#include <unordered_map>
 
#include "geo.h"

namespace transport_catalogue{
	
struct Bus;
    
struct Stop {    
    std::string name_;
    detail::geo::Coordinates coordinates;   
    std::vector<Bus*> buses_;
};
 
struct Bus { 
    std::string name_;
    std::vector<Stop*> stops_;
};
    
 struct BusInfoRoute {
    std::string_view name;
    bool not_found;
    size_t stops_count;
    size_t unique_stops_count;
    size_t length;
    double curvature;
 };

 struct Distance {
     const Stop* A;
     const Stop* B;
     int distance;
 };
    
 struct DistanceHasher {
     std::hash<const void*> hasher;
     std::size_t operator()(const std::pair<const Stop*, const Stop*>pair_stops) const noexcept {
         auto hash_first = std::hash<const void*>{}(pair_stops.first);
         auto hash_second = std::hash<const void*>{}(pair_stops.second);
         return hash_first + hash_second * 37;
     }

 };
 using StopMap = std :: unordered_map<std::string_view , Stop*>;
 using BusMap = std :: unordered_map<std::string_view , Bus*>;
 using DistanceMap = std::unordered_map<std::pair<const Stop*, const Stop*>, int, DistanceHasher>;
 
 
class TransportCatalogue {
public:   
    
    void AddBus(const Bus& bus);
    void AddStop(const Stop& stop);
    void AddDistance(const std::vector<Distance>& distances);
    BusInfoRoute  GetBusInfo( std::string_view _bus_name);
    Bus* GetBus(std::string_view _bus_name);
    Stop* GetStop(std::string_view _stop_name);
    std::unordered_set<const Bus*> StopGetUniqBuses(const Stop* stop);    
    std::unordered_set<const Stop*> GetUniqStops(const Bus* bus);
    double GetGeoLength(Bus* bus);
    size_t GetDistanceStop(const Stop* from, const Stop* to);
    size_t GetDistanceToBus(const Bus* bus);

private:
 
    std::deque<Stop> stops_;
    StopMap stopname_to_stop_;   
    std::deque<Bus> buses_;
    BusMap busname_to_bus_;
    DistanceMap distance_to_stop_;
};
}//end namespace transport_catalogue