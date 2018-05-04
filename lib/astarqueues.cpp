#include <nodestructures.h>
#include <cfloat>
#include <omp.h>


struct Compare { double val; unsigned long index; };    
#pragma omp declare reduction(minimum : struct Compare : omp_out = omp_in.val < omp_out.val ? omp_in : omp_out)


unsigned short push2Queue (QueueStar* Que, AStarNode* Member, unsigned long PosArray){
	
	if (Que->QSize < Que->MaxQSize){ //We check that the Queue has space

		if(((Member+PosArray)->whq)!=Que->whq){ //We check that the Member does not belong already to Queue

			(Member+PosArray)->whq=Que->whq; //Set The AStarNode to the List Type. Equivalent to Assign to the list
			(Member+PosArray)->PosInQueue=Que->QSize; //KeepTrack of the PosInQueue for future Extraction
			(*(Que->base +Que->QSize))=PosArray; //Setting the member of the Position to the Members of Queue

			/***************UPDATE OF MINIMUM AFTER INSERTION******************************/
			if(((Member+PosArray)->f)<(Que->MinF)){

				Que->PointsMinF=Que->QSize;
				Que->MinF=(Member+PosArray)->f;
			}
			/***************UPDATE OF MINIMUM AFTER INSERTION******************************/

			Que->QSize=Que->QSize+1; // Incrementing the pointer of the Queue

		}

		else{
			return 2; //Return we are trying to insert a node already inserted
		}
	}

	else{
		return 1; //Return we are trying to insert in a full list
	}
	return 0;
}


unsigned long UpdateMinimum( QueueStar* Que, AStarNode* Members){
	
	double tmpMin;
	unsigned long tmpposMinF;
	Que->MinF=DBL_MAX; //Comparison in Worst Case;
	for (unsigned long j=0; j < (Que->QSize); j++){
		
		tmpposMinF=(*(Que->base+j));

		if(((Members+tmpposMinF)->f)<(Que->MinF)){
			Que->MinF=(Members+tmpposMinF)->f;
			Que->PointsMinF=j;
		}
	}
	if(Que->QSize==0){
		Que->MinF=DBL_MAX;
		Que->PointsMinF=0;
		(*(Que->base))=0;
		return 0;	
	}
	return *(Que->base+Que->PointsMinF);
}



struct Compare MinimizerFind(QueueStar* Que, AStarNode* Members){

	struct Compare min_struct;
	

	unsigned long index = 0;
	double mini = (Members+(*(Que->base)))->f;
	min_struct.val=mini;
	min_struct.index = index;

	#pragma omp parallel
	{
		unsigned long index_local = index;
		double min_local = mini;  
		#pragma omp for nowait
		for (unsigned long i = 1; i < Que->QSize; i++) {        
		    if ((Members+(*(Que->base+i)))->f < min_local) {
		        min_local = (Members+(*(Que->base+i)))->f;
		        index_local = i;
		    }
		}
		#pragma omp critical 
		{
		    if (min_local < mini) {
		        mini = min_local;
		        index = index_local;
				min_struct.val=mini;
				min_struct.index = index;

		    }
		}
	}
		
	return min_struct;

}



unsigned long ExtractMinimum( QueueStar* Que){
	
	return (*(Que->base+Que->PointsMinF));
}




unsigned short extract4Closed( QueueStar* Que, AStarNode* Members, unsigned long PosArray){
	
	unsigned long value;
	unsigned long OldPosInQue;
	struct Compare FindMIN;

	if(Que->QSize != 0){

		if ((Members+PosArray)->whq ==CLOSED){
							
			value=(*(Que->base+Que->QSize-1));// We take the value of the ID on the top member of Queue
			//This member should have set the PosInQueue value to QSize-1
			(*(Que->base+(Members+PosArray)->PosInQueue))=value;//We substitute the extracting by the top element in the Queue of ID's							
			(Members+value)->PosInQueue=(Members+PosArray)->PosInQueue; //We update the position for the top not anymore to the top

			(Members+PosArray)->whq=NONE; //The extracted member we set to NONE
			Que->QSize=Que->QSize-1; //We decrease the value of the Queue in one;		
			
			if(((Members+PosArray)->PosInQueue)==(Que->PointsMinF)){
				if (Que->QSize>8000){
					omp_set_num_threads(4);
					FindMIN=MinimizerFind(Que, Members);
					Que->MinF=FindMIN.val;
					Que->PointsMinF=FindMIN.index;
				}
				else if (Que->QSize>100) {
					omp_set_num_threads(2);
					FindMIN=MinimizerFind(Que, Members);
					Que->MinF=FindMIN.val;
					Que->PointsMinF=FindMIN.index;
				}
				else{
					UpdateMinimum(Que, Members);
				}
				
				//UpdateMinimum(Que, Members);
			}

			(Members+PosArray)->PosInQueue=0; // The extracted member has no position in array. 
			
			
		}
		else{
			
			return 2; //Extracting from different Queue
		}		
	}
	else{
		return 1; //Extracting From Empty List
	}	
	return 0;
}




unsigned short extract4Open( QueueStar* Que, AStarNode* Members, unsigned long PosArray){

	unsigned long value;
	unsigned long OldPosInQue;
	struct Compare FindMIN;

	if(Que->QSize != 0){

		if ((Members+PosArray)->whq ==OPEN){

			// We take the value of the ID on the top member of Queue
			value=(*(Que->base+Que->QSize-1));
			
			//This member should have set the PosInQueue value to QSize-1
			(*(Que->base+(Members+PosArray)->PosInQueue))=value;//We substitute the extracting by the top element in the Queue of ID's							
			(Members+value)->PosInQueue=(Members+PosArray)->PosInQueue; //We update the position for the top not anymore to the top
			
			(Members+PosArray)->whq=NONE; //The extracted member we set to NONE
			Que->QSize=Que->QSize-1; //We decrease the value of the Queue in one;

			if(((Members+PosArray)->PosInQueue)==(Que->PointsMinF)){
				//UpdateMinimum(Que, Members);
				if (Que->QSize>200){
					omp_set_num_threads(4);
					FindMIN=MinimizerFind(Que, Members);
					Que->MinF=FindMIN.val;
					Que->PointsMinF=FindMIN.index;

				}
				else if (Que->QSize>24) {
					omp_set_num_threads(2);
					FindMIN=MinimizerFind(Que, Members);
					Que->MinF=FindMIN.val;
					Que->PointsMinF=FindMIN.index;
				}
				else{
					UpdateMinimum(Que, Members);
				}

			}

			(Members+PosArray)->PosInQueue=0; // The extracted member has no position in array. 

		}
		else{
			
			return 2; //Extracting from different Queue
		}		
	}
	else{
		return 1; //Extracting From Empty List
	}	
	return 0;
}
