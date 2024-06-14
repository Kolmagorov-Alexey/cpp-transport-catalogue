#pragma once
#include "transport_catalogue.h"
#include <algorithm>
#include <vector>

namespace transport_catalogue {
    namespace detail {
        namespace stop {
            void Query_stop(TransportCatalogue& catalogue, std::string_view stop_name);
        }//end namespace stop
        void Query_(TransportCatalogue& catalogue, std::string_view str);
        void ProcessСatalogRequests(std::istream& input,TransportCatalogue& catalogue);
    }//end namespace detail
}//end namespace transport_catalogue