#include "stat_reader.h"
 
 namespace transport_catalogue{
	namespace detail{
		namespace bus{
void Query_bus(TransportCatalogue& catalogue, std::string_view str) {
    auto entry = 4;
    str = str.substr(entry);
    
    Bus* bus = catalogue.Get_bus(str);
    if (bus != nullptr) {
        std::cout << "Bus " << bus->name_ << ": "
                  << bus->stops_.size() << " stops on route, "
                  << (catalogue.Get_uniq_stops(bus)).size() << " unique stops, "
                  << std::setprecision(6) << catalogue.Get_length(bus) << " route length" << std::endl;
    } else {      
        std::cout << "Bus " << str << ": not found" << std::endl;
    }  
}
		}//end namespace bus
       namespace stop{
void Query_stop(TransportCatalogue& catalogue, std::string_view stop_name) {
    auto entry = 5;
    stop_name = stop_name.substr(entry);
    std::unordered_set<const Bus*> unique_buses;      
    std::unordered_set<std::string_view> unique_buses_name;   
    std::vector <std::string> bus_name_v;
    
    Stop* stop = catalogue.Get_stop(stop_name);
    
    if (stop != NULL) {
        unique_buses = catalogue.Stop_get_uniq_buses(stop);
        
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
        bus::Query_bus(catalogue, str);
    } else if (str.find("Stop") != str.npos) {
        stop::Query_stop(catalogue, str);
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