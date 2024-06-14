#include "input_reader.h"
#include "stat_reader.h"
 
using namespace transport_catalogue;
using namespace transport_catalogue::detail;
 
int main() {     
    TransportCatalogue catalogue;
    FillTransportCatalogue(std::cin,catalogue);
    ProcessСatalogRequests(std::cin,catalogue);
} 