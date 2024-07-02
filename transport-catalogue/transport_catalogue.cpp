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
  
   
    if (busname_to_bus_.count(_bus_name)) {

        return busname_to_bus_.at(_bus_name);
    }
    else
        return nullptr;
}
 
Stop* TransportCatalogue::GetStop(std::string_view _stop_name) {
 
   
   if (stopname_to_stop_.count(_stop_name)) {

          return stopname_to_stop_.at(_stop_name);
   } else 
          return nullptr;
        
}
 
std::unordered_set<const Stop*> TransportCatalogue::GetUniqStops(const Bus* bus) {
    std::unordered_set<const Stop*> unique_stops_;
    
    unique_stops_.insert(bus->stops_.begin(), bus->stops_.end());
    
    return unique_stops_;
}
 
double TransportCatalogue::GetGeoLength(Bus* bus) {
   
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
        bus_info_route.length = TransportCatalogue::GetDistanceToBus(bus);
        bus_info_route.curvature = static_cast<double>(TransportCatalogue::GetDistanceToBus(bus) / GetGeoLength(bus));
        
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
void TransportCatalogue::AddDistance(const std::vector<Distance>& distances) {
    for (auto& distance : distances) {
        if (distance.A != nullptr && distance.B != nullptr) {
              auto pair_dist = std::make_pair(distance.A, distance.B);
              distance_to_stop_.insert(DistanceMap::value_type(pair_dist, distance.distance));
        }
    }
}
size_t TransportCatalogue::GetDistanceStop(const Stop* from, const Stop* to) {
    if (distance_to_stop_.count({ from,to })) {
        return distance_to_stop_.at({ from,to });
    }
    else if (distance_to_stop_.count({ to,from })) {
        return distance_to_stop_.at({ to, from });
    }
    else return 0;

}
size_t TransportCatalogue::GetDistanceToBus(const Bus* bus) {
    size_t distance = 0;
    auto stops_size = bus->stops_.size() - 1;
    for (int i = 0; i < stops_size; i++) {
        distance += GetDistanceStop(bus->stops_[i], bus->stops_[i + 1]);
    }
    return distance;
}
 }//end namespace transport_catalogue
