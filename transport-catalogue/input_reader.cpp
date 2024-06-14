#include "input_reader.h"
#include "stat_reader.h"
#include <algorithm>

namespace transport_catalogue{
	namespace detail{
		namespace stop{
Stop Split_stop(std::string str) {
    auto twopoint_pos = str.find(':');
    auto comma_pos = str.find(',');
    auto entry_length = 5;
    auto distance = 2;
    Stop _stop;
 
    _stop.name_ = str.substr(entry_length, 
                                  twopoint_pos - entry_length);
    _stop.latitude_ = stod(str.substr(twopoint_pos + distance, 
                                      comma_pos - twopoint_pos - distance));
    _stop.longitude_ = stod(str.substr(comma_pos + distance));
    
    return _stop;
}
		}//ens namespace stop
        
		namespace bus{
Bus Split_bus(TransportCatalogue& catalogue, std::string_view str) {
    auto entry_length = 4;
    auto distance = 2;
    auto twopoint_pos = str.find(':');
    Bus bus;
    bus.name_ = str.substr(entry_length, 
                           twopoint_pos - entry_length);
    
    str = str.substr(twopoint_pos + distance);
    
    auto more_pos = str.find('>');
    if (more_pos == std::string_view::npos) {
        auto tire_pos = str.find('-');
        
        while (tire_pos != std::string_view::npos) {
            bus.stops_.push_back(catalogue.Get_stop(str.substr(0, tire_pos - 1)));
            
            str = str.substr(tire_pos + distance);
            tire_pos = str.find('-');
        }
        
        bus.stops_.push_back(catalogue.Get_stop(str.substr(0, tire_pos - 1)));
        size_t size_ = bus.stops_.size() - 1;
        
        for (size_t i = size_; i > 0; i--) {
            bus.stops_.push_back(bus.stops_[i-1]);
        }
        
    } else {
        while (more_pos != std::string_view::npos) {
            bus.stops_.push_back(catalogue.Get_stop(str.substr(0, more_pos - 1)));
            
            str = str.substr(more_pos + distance);
            more_pos = str.find('>');
        }
        
        bus.stops_.push_back(catalogue.Get_stop(str.substr(0, more_pos - 1)));
    }
    return bus;
}
		}//end namespace bus
void FillTransportCatalogue(std::istream& input,TransportCatalogue& catalogue) {
    
    std::string count;
    std::getline(input, count);
    
    if (count != "") {
        std::string str;
        std::vector<std::string> buses;
        int amount = stoi(count);
              
        for (int i = 0; i < amount; ++i) {
            std::getline(std::cin, str);
            
            if (str != "") {
                auto space_pos = str.find_first_not_of(' ');
                str = str.substr(space_pos);
 
                if (str.find("Bus") == str.npos) {
                    catalogue.Add_stop(stop::Split_stop(str));
                } else {
                    buses.push_back(str);
                }
            }
        }
        
        for (auto &bus : buses) {
            catalogue.Add_bus(bus::Split_bus(catalogue, bus));
        }
    }
}
	}//end namespace detail
}//end namespace transport_catalogue