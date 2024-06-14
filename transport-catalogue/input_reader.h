#pragma once
#include "transport_catalogue.h"

 namespace transport_catalogue{
	namespace detail{
         namespace stop{		
Stop Split_stop(std::string_view str);
		 }//end namespace stop
		 namespace bus{
Bus Split_bus(TransportCatalogue& catalogue, std::string_view str);
		 }//end namespace bus
void FillTransportCatalogue(std::istream& input,TransportCatalogue& catalogue);
	}//end namespace detail
 }//end namespace transport_catalogue