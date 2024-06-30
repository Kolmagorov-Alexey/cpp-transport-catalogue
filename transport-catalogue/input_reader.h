#pragma once
#include "transport_catalogue.h"
#include <vector>

 namespace transport_catalogue{
	namespace detail{
        namespace stop{		
Stop SplitStop(std::string_view str);
		 }//end namespace stop
		 namespace bus{
Bus SplitBus(TransportCatalogue& catalogue, std::string_view str);
		 }//end namespace bus
        namespace distance {
std::vector<Distance> SplitDistances(TransportCatalogue& catalogue,const std::string& str);
		 }//end namespace distance
void FillTransportCatalogue(std::istream& input,TransportCatalogue& catalogue);
	}//end namespace detail
 }//end namespace transport_catalogue