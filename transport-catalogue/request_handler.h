#pragma once
 
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "json.h"
#include <sstream>
 
using namespace transport_catalogue;
using namespace map_renderer;
using namespace transport_catalogue::detail::json;
 
namespace request_handler {
 
class RequestHandler {
public:
    RequestHandler() = default;
    std::vector<geo::Coordinates> GetStopsCoordinates(TransportCatalogue& catalogue_) const;
    std::vector<std::string_view> GetSortBusesNames(TransportCatalogue& catalogue_) const;
    
    BusInfoRoute BusQuery(TransportCatalogue& catalogue, std::string_view str);
    StopInfo StopQuery(TransportCatalogue& catalogue, std::string_view stop_name);
    
    Node ExecuteMakeNodeStop(int id_request, StopInfo query_result);
    Node ExecuteMakeNodeBus(int id_request, BusInfoRoute query_result);
    Node ExecuteMakeNodeMap(int id_request, TransportCatalogue& catalogue, RenderSettings render_settings);
    void ExecuteQueries(TransportCatalogue& catalogue, std::vector<StatRequest>& stat_requests, RenderSettings& render_settings);
    void ExecuteRenderMap(MapRenderer& map_catalogue, TransportCatalogue& catalogue_) const;
       
    const Document& GetDocument();
 
private:
    Document doc_out;
};
    
}//end namespace request_handler
