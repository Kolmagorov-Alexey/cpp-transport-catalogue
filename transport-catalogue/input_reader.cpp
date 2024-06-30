#include "input_reader.h"
#include "stat_reader.h"
#include <algorithm>

namespace transport_catalogue{
	namespace detail{
		namespace stop{
Stop SplitStop(std::string str) {
    auto twopoint_pos = str.find(':');
    auto comma_pos = str.find(',');
    auto entry_length = 5;
    auto distance = 2;
    Stop _stop;
 
    _stop.name_ = str.substr(entry_length, 
                                  twopoint_pos - entry_length);
    _stop.coordinates.latitude = stod(str.substr(twopoint_pos + distance, 
                                      comma_pos - twopoint_pos - distance));
    _stop.coordinates.longitude = stod(str.substr(comma_pos + distance));
    
    return _stop;
}
		}//ens namespace stop
        
		namespace bus{
Bus SplitBus(TransportCatalogue& catalogue, std::string_view str) {
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
            bus.stops_.push_back(catalogue.GetStop(str.substr(0, tire_pos - 1)));
            
            str = str.substr(tire_pos + distance);
            tire_pos = str.find('-');
        }
        
        bus.stops_.push_back(catalogue.GetStop(str.substr(0, tire_pos - 1)));
        size_t size_ = bus.stops_.size() - 1;
        
        for (size_t i = size_; i > 0; i--) {
            bus.stops_.push_back(bus.stops_[i-1]);
        }
        
    } else {
        while (more_pos != std::string_view::npos) {
            bus.stops_.push_back(catalogue.GetStop(str.substr(0, more_pos - 1)));
            
            str = str.substr(more_pos + distance);
            more_pos = str.find('>');
        }
        
        bus.stops_.push_back(catalogue.GetStop(str.substr(0, more_pos - 1)));
    }
    return bus;
}
		}//end namespace bus
        namespace distance {
            std::vector<Distance> SplitDistances(TransportCatalogue& catalogue, const std::string& str) {
                std::vector<Distance> distances;
                auto entry_length = 5;
                auto twopoint_pos = str.find(':');
                auto space = 2;

                std::string str_copy = str;
                std::string name = str_copy.substr(entry_length,
                    twopoint_pos - entry_length);
                str_copy = str_copy.substr(str_copy.find(',') + 1);
                str_copy = str_copy.substr(str_copy.find(',') + space);

                while (str_copy.find(',') != std::string::npos) {
                    int distance = stoi(str_copy.substr(0, str_copy.find('m')));
                    std::string stop_dist_name = str_copy.substr(str_copy.find('m') + entry_length);
                    stop_dist_name = stop_dist_name.substr(0, stop_dist_name.find(','));
                    distances.push_back({ catalogue.GetStop(name),
                                          catalogue.GetStop(stop_dist_name), distance });
                    str_copy = str_copy.substr(str_copy.find(',') + space);
                }
                std::string last_name = str_copy.substr(str_copy.find('m') + entry_length);
                int distance = stoi(str_copy.substr(0, str_copy.find('m')));
                distances.push_back({ catalogue.GetStop(name), catalogue.GetStop(last_name), distance });

                return distances;
            }
        }//end namespace distance
void FillTransportCatalogue(std::istream& input,TransportCatalogue& catalogue) {
    
    std::string count;
    std::getline(input, count);

    if (count != "") {
        std::string str;
        std::vector<std::string> buses;
        std::vector<std::string> stops;
        int amount = stoi(count);
        auto bus_distance = 3;

        for (int i = 0; i < amount; ++i) {
            std::getline(input, str);

            if (str != "") {
                auto space_pos = str.find_first_not_of(' ');
                str = str.substr(space_pos);

                if (str.substr(0, bus_distance) != "Bus") {
                    stops.push_back(str);
                }
                else {
                    buses.push_back(str);
                }
            }
        }

        for (auto &stop : stops) {
            catalogue.AddStop(stop::SplitStop(stop));
        }

        for (auto &stop : stops) {
            catalogue.AddDistance(distance::SplitDistances(catalogue, stop));
        }

        for (auto &bus : buses) {
            catalogue.AddBus(bus::SplitBus( catalogue, bus));
        }
    }
}
	}//end namespace detail
}//end namespace transport_catalogue