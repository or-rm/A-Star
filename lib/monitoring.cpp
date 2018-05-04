#include <nodestructures.h>

void set_Maximum_node_Descendant_N(unsigned long comp, unsigned long *counter){
	(*counter)= (comp > (*counter)) ? comp : (*counter);	
}
