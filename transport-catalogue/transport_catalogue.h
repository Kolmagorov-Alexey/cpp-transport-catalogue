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
    double latitude_;
    double longitude_;
    
    std::vector<Bus*> buses_;
};
 
struct Bus { 
    std::string name_;
    std::vector<Stop*> stops_;
};
 
typedef std::unordered_map<std::string_view , Stop*> StopMap;
typedef std::unordered_map<std::string_view , Bus*> BusMap;
 
 
class TransportCatalogue {
public:   
    void Add_bus(Bus&& bus);
    void Add_stop(Stop&& stop);
    
    Bus* Get_bus(std::string_view _bus_name);
    Stop* Get_stop(std::string_view _stop_name);
 
    std::unordered_set<const Bus*> Stop_get_uniq_buses(Stop* stop);    
    std::unordered_set<const Stop*> Get_uniq_stops(Bus* bus);
    double Get_length(Bus* bus);
private:
 
    std::deque<Stop> stops_;
    StopMap stopname_to_stop;
    
    std::deque<Bus> buses_;
    BusMap busname_to_bus;
};
}//end namespace transport_catalogue