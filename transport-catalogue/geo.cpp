#define _USE_MATH_DEFINES
#include "geo.h"
 
#include <cmath>
 
namespace geo {
    
double ComputeDistance(Coordinates A, Coordinates end) {
    using namespace std;
    
    if (!(A == end)) {
        const double dr = PI / 180.;
        return acos(sin(A.latitude * dr) * sin(end.latitude * dr)
                    + cos(A.latitude * dr) * cos(end.latitude * dr) 
                    * cos(abs(A.longitude - end.longitude) * dr)) * EARTH_RADIUS;
    } else {
        return 0.0;
    }
}
    
}//end namespace geo
