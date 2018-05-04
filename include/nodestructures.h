/*Declarations for basic structures*/
#define EarthDiam 12742000.0;
typedef struct {
	unsigned long id;          // Node identification
	double lat, lon;           // Node position
	unsigned long nsucc=0;     // Number of node successors; i. e. length of successors
	unsigned long offset;
} node;

typedef struct {
	unsigned long source;
	unsigned long destination;
} connection;

typedef char Queue;
enum whichQueue {NONE, OPEN, CLOSED};

typedef struct {
	double g, h, f;
	unsigned long parent; //Vector Position
	unsigned long PosInQueue;
	Queue whq;
} AStarNode;

typedef struct{
	unsigned long* base; //Pointer to the vector containing the vector positions of the AStarNodes
	unsigned long QSize; //Points to the next free storage of the Queue
	unsigned long MaxQSize; //Maximum size of the Queue
	double MinF= (double) EarthDiam;
	unsigned long PointsMinF=0;
	Queue whq; // Type of Queue CLOSED OR OPEN
}QueueStar;
