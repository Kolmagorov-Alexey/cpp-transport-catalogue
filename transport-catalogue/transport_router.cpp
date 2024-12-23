#include "transport_router.h"
#include "graph.h"
 
namespace transport_catalogue {
namespace detail {
namespace router {

TransportRouter::TransportRouter(RoutingSettings routing_settings, const TransportCatalogue& transport_catalogue) 
  : routing_settings_{std::move(routing_settings)},
    transport_catalogue_{transport_catalogue}
{
  SetGraph();
  router_ = std::make_unique<Router<double>>(*graph_);
  router_->Build();
}
  
const std::variant<StopEdge, BusEdge>& TransportRouter::GetEdge(EdgeId id) const {return edge_id_to_edge_.at(id);}
 
std::optional<RouterByStop> TransportRouter::GetRouterByStop(Stop* stop) const {
    if (stop_to_router_.count(stop)) {
        return stop_to_router_.at(stop);
    } else {
        return std::nullopt;
    }
}
 
std::optional<RouteInfo> TransportRouter::GetRouteInfo(std::string_view start, std::string_view end) const {
    const auto startVertex = GetRouterByStop(transport_catalogue_.GetStop(start))->bus_wait_start;
    const auto endVertex = GetRouterByStop(transport_catalogue_.GetStop(end))->bus_wait_start;

    const auto route_info = router_->BuildRoute(startVertex, endVertex);
    if (route_info) {
        RouteInfo result;
        result.total_time = route_info->weight;
        
        for (const auto edge : route_info->edges) {
            result.edges.emplace_back(GetEdge(edge));
        }
        
        return result;
        
    } else {
        return std::nullopt;
    }
}   
    
const std::unordered_map<Stop*, RouterByStop>& TransportRouter::GetStopToVertex() const {return stop_to_router_;}
const std::unordered_map<EdgeId, std::variant<StopEdge, BusEdge>>& TransportRouter::GetEdgeIdToEdge() const {return edge_id_to_edge_;}
    
std::deque<Stop*> TransportRouter::GetStopsPtr() {
    std::deque<Stop*> stops_ptr;  
    
    for (auto [_, stop_ptr] : transport_catalogue_.GetStopnameToStop()) {
        stops_ptr.push_back(stop_ptr);
    }
 
    return stops_ptr;
}
    
std::deque<Bus*> TransportRouter::GetBusPtr() {
    std::deque<Bus*> buses_ptr;  
    
    for (auto [_, bus_ptr] : transport_catalogue_.GetBusnameToBus()) {
        buses_ptr.push_back(bus_ptr);
    }
 
    return buses_ptr;
}
    
void TransportRouter::SetStops(const std::deque<Stop*>& stops) {
    size_t i = 0;
    
    for (const auto stop : stops) {
        VertexId first = i++;
        VertexId second = i++;
        
        stop_to_router_[stop] = RouterByStop{first, second};
    }
}
 
void TransportRouter::AddEdgeToStop() {   
    
    for (const auto &[stop, num] : stop_to_router_) {
        EdgeId id = graph_->AddEdge(Edge<double>{num.bus_wait_start, 
                                                 num.bus_wait_end, 
                                                 routing_settings_.bus_wait_time});
        
        edge_id_to_edge_[id] = StopEdge{stop->name, routing_settings_.bus_wait_time};
    }
}
 
void TransportRouter::AddEdgeToBus() {
 
    for (auto bus : GetBusPtr()) {        
        ParseBusToEdges(bus->stops.begin(), 
                           bus->stops.end(), 
                           bus);
        
        if (!bus->is_roundtrip) {
            ParseBusToEdges(bus->stops.rbegin(), 
                               bus->stops.rend(), 
                               bus);
        }
    }
}
 
void TransportRouter::SetGraph() {
    const auto stops_ptr_size = GetStopsPtr().size();  
    
    graph_ = std::make_unique<DirectedWeightedGraph<double>>(2 * stops_ptr_size);    
    
    SetStops(GetStopsPtr());    
    AddEdgeToStop();
    AddEdgeToBus();
}
 
Edge<double> TransportRouter::MakeEdgeToBus(Stop* start, Stop* end, const double distance) const {
    Edge<double> result;
    
    result.from = stop_to_router_.at(start).bus_wait_end;
    result.to = stop_to_router_.at(end).bus_wait_start;
    result.weight = distance * 1.0 / (routing_settings_.bus_velocity * KILOMETER / HOUR);
    
    return result;
}
 
}//end namespace router
}//end namespace detail
}//end namespace transport_catalogue
