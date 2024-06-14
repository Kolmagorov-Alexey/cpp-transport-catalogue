#include "transport_catalogue.h"
 
#include <execution>
 
 namespace transport_catalogue{
	 
void TransportCatalogue::Add_stop(Stop&& stop_) {
    stops_.push_back(std::move(stop_));
    Stop* stop_buf = &stops_.back();
    stopname_to_stop.insert(StopMap::value_type(stop_buf->name_, stop_buf));
}
 
void TransportCatalogue::Add_bus(Bus&& bus_) {
    Bus* bus_buf;
    buses_.push_back(std::move(bus_)); 
    bus_buf = &buses_.back();
    busname_to_bus.insert(BusMap::value_type(bus_buf->name_, bus_buf));
 
    for (Stop* _stop : bus_buf->stops_) {
        _stop->buses_.push_back(bus_buf);
    }    
}
 
Bus* TransportCatalogue::Get_bus(std::string_view _bus_name) {   
    if(busname_to_bus.empty()){
        return nullptr;
    }
 
    try {
        return busname_to_bus.at(_bus_name);
    } catch(const std::out_of_range &e) {
        return nullptr;
    }
}
 
Stop* TransportCatalogue::Get_stop(std::string_view _stop_name) {
    if(stopname_to_stop.empty()){
        return nullptr;
    }
 
    try {
        return stopname_to_stop.at(_stop_name);
    } catch(const std::out_of_range &e) {
        return nullptr;
    }
}
 
std::unordered_set<const Stop*> TransportCatalogue::Get_uniq_stops(Bus* bus) {
    std::unordered_set<const Stop*> unique_stops_;
    
    unique_stops_.insert(bus->stops_.begin(), bus->stops_.end());
    
    return unique_stops_;
}
 
double TransportCatalogue::Get_length(Bus* bus) {
   
    double length =0.0;
    for (size_t i = 0; i < bus->stops_.size() - 1; ++i) {
        auto from = bus->stops_[i];
        auto to = bus->stops_[i + 1];
        length += detail::geo::ComputeDistance({ from->latitude_,from->longitude_ }, { to->latitude_,to->longitude_ });
    }
        
    return length;
}
 
std::unordered_set<const Bus*> TransportCatalogue::Stop_get_uniq_buses(Stop* stop){    
    std::unordered_set<const Bus*> unique_stops_;    
      
    unique_stops_.insert(stop->buses_.begin(), stop->buses_.end());   
    
    return unique_stops_;
}
 }//end namespace transport_catalogue
