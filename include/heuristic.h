const double PI_C= (double) 4.0*atan(1.0);
const double EarthR= (double) 6371000.0;

double d2r (double d);

double sind(double x);

volatile double HarvesineD (double lat1, double lon1, double lat2, double lon2);
volatile double HarvesineRad (double lat1, double lon1, double lat2, double lon2);
volatile double PitagorasD (double lat1, double lon1, double lat2, double lon2);
volatile double PitagorasRad (double lat1, double lon1, double lat2, double lon2);
