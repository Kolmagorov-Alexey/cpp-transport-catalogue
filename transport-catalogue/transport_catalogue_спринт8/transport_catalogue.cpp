#include "transport_catalogue.h"
 
#include <execution>
 
 namespace transport_catalogue{
	 
void TransportCatalogue::AddStop(const Stop& stop_) {
    stops_.push_back(stop_);
    Stop* stop_buf = &stops_.back();
    stopname_to_stop_.insert(StopMap::value_type(stop_buf->name_, stop_buf));
}
 
void TransportCatalogue::AddBus(const Bus& bus_) {
    Bus* bus_buf;
    buses_.push_back(bus_); 
    bus_buf = &buses_.back();
    busname_to_bus_.insert(BusMap::value_type(bus_buf->name_, bus_buf));
 
    for (Stop* _stop : bus_buf->stops_) {
        _stop->buses_.push_back(bus_buf);
    }    
}
 
Bus* TransportCatalogue::GetBus(std::string_view _bus_name) {   
  
    return  busname_to_bus_.find(_bus_name) != busname_to_bus_.end() ? busname_to_bus_.at(_bus_name): nullptr;

}
 
Stop* TransportCatalogue::GetStop(std::string_view _stop_name) {
 
   return  stopname_to_stop_.find(_stop_name)!= stopname_to_stop_.end() ? stopname_to_stop_.at(_stop_name): nullptr;
}
 
std::unordered_set<const Stop*> TransportCatalogue::GetUniqStops(const Bus* bus) {
    std::unordered_set<const Stop*> unique_stops_;
    
    unique_stops_.insert(bus->stops_.begin(), bus->stops_.end());
    
    return unique_stops_;
}
 
double TransportCatalogue::GetLength(Bus* bus) {
   
    double length = 0.0;
    for (size_t i = 0; i < bus->stops_.size() - 1; ++i) {
        auto from = bus->stops_[i];
        auto to = bus->stops_[i + 1];
        length += detail::geo::ComputeDistance({ from->coordinates.latitude, from->coordinates.longitude }, { to->coordinates.latitude, to->coordinates.longitude });
    }
        
    return length;
}
 BusInfoRoute TransportCatalogue::GetBusInfo(std::string_view _bus_name) {
    BusInfoRoute bus_info_route;
    auto entry = 4;
    _bus_name = _bus_name.substr(entry);
    Bus* bus = TransportCatalogue::GetBus(_bus_name);
    if (bus != nullptr) {
        bus_info_route.name = bus->name_;
        bus_info_route.not_found = false;
        bus_info_route.unique_stops_count = TransportCatalogue::GetUniqStops(bus).size();
        bus_info_route.stops_count = bus->stops_.size();
        bus_info_route.length = TransportCatalogue::GetLength(bus);
        
    }
    else {
        bus_info_route.name = _bus_name;
        bus_info_route.not_found = true;
    }
    return bus_info_route;
};
     
std::unordered_set<const Bus*> TransportCatalogue::StopGetUniqBuses(const Stop* stop){    
    std::unordered_set<const Bus*> unique_stops_;    
      
    unique_stops_.insert(stop->buses_.begin(), stop->buses_.end());   
    
    return unique_stops_;
}
 }//end namespace transport_catalogue
