#pragma once
#include "transport_catalogue.h"
#include <algorithm>
#include <vector>

namespace transport_catalogue {
    namespace detail {
        namespace stop {
            void QueryStop(TransportCatalogue& catalogue, std::string_view stop_name);
        }//end namespace stop
        namespace bus{
        void QueryBus(TransportCatalogue& catalogue, std::string_view str);
        }//end namespace bus
        void Query_(TransportCatalogue& catalogue, std::string_view str);
        void ProcessСatalogRequests(std::istream& input,TransportCatalogue& catalogue);
    }//end namespace detail
}//end namespace transport_catalogue