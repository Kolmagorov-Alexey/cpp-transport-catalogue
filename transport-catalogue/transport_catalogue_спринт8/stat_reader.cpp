#include "stat_reader.h"
 
 namespace transport_catalogue{
	namespace detail{
		namespace bus{
            
void QueryBus(TransportCatalogue& catalogue, std::string_view str) {
    
    BusInfoRoute bus_info_route = catalogue.GetBusInfo(str);
    if (bus_info_route.not_found) { std::cout << "Bus " << bus_info_route.name << ": not found" << std::endl; }
      else {
          std::cout << "Bus " << bus_info_route.name << ": "
           << bus_info_route.stops_count << " stops on route, "
           << bus_info_route.unique_stops_count << " unique stops, "
           << std::setprecision(6) << bus_info_route.length << " route length" << std::endl;
    }
}
                    }//end namespace bus
        
       namespace stop{
void QueryStop(TransportCatalogue& catalogue, std::string_view stop_name) {
    
    auto entry = 5;
    stop_name = stop_name.substr(entry);
    std::unordered_set<const Bus*> unique_buses;      
    std::unordered_set<std::string_view> unique_buses_name;   
    std::vector <std::string> bus_name_v;
    
    Stop* stop = catalogue.GetStop(stop_name);
    
    if (stop != NULL) {
        unique_buses = catalogue.StopGetUniqBuses(stop);
        
        if (unique_buses.size() == 0) {
            std::cout << "Stop " << stop_name << ": no buses" << std::endl;
        } else {
            std::cout << "Stop " << stop_name << ": buses ";
 
            for (const Bus* _bus : unique_buses) {
                bus_name_v.push_back(_bus->name_);
            }
 
            std::sort(bus_name_v.begin(), bus_name_v.end());         
 
            for (std::string_view _bus_name : bus_name_v) {
                std::cout << _bus_name;
                std::cout << " ";
            }
            std::cout << std::endl;
        }        
    } 
    else {      
        std::cout << "Stop " << stop_name << ": not found" << std::endl;
    }
}
	   }//end namespace stop
        
void Query_(TransportCatalogue& catalogue, std::string_view str) {
    if (str.find("Bus") != str.npos) {
        bus::QueryBus(catalogue, str);
    } else if (str.find("Stop") != str.npos) {
        stop::QueryStop(catalogue, str);
    } else {
        std::cout << "Error query" << std::endl;
    }
}
 
void ProcessСatalogRequests(std::istream& input,TransportCatalogue& catalogue) {
    std::string count;
    std::getline(input, count);
    
    std::string str;
    std::vector<std::string> query;
    auto amount = stoi(count);
    
    for (int i = 0; i < amount; ++i) {
        std::getline(std::cin, str);
        query.push_back(str);
    }
    
    for (auto& strr : query) {
        Query_(catalogue, strr);
    }
}
	}//end namespace detail
 }// end namespace transport_catalogue