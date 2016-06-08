 
/* Need to do */

#include "render.h"

#include "math.h"





void Render::DataLock() {

	int error_code = MPI_SUCCESS;
	
	char error_message[MPI_MAX_ERROR_STRING];
	int message_length;
	
	#ifdef DEBUG
	my_rrr->time_temp = MPI_Wtime();
	#endif
	
	error_code = MPI_Irecv(&my_rrr->posRender->render, 1, my_rrr->posRender->renderDatatype, MAIN, RENDER_MOVEMENT, MPI_COMM_WORLD, &my_rrr->request);

	#ifdef DEBUG
	my_rrr->myMPI_Time = my_rrr->myMPI_Time + MPI_Wtime() - my_rrr->time_temp;
	#endif
	
	if(error_code != MPI_SUCCESS) {
		MPI_Error_string(error_code, error_message, &message_length);
		cerr << "ERRO Recv: " << error_message << endl;
		MPI_Abort(MPI_COMM_WORLD, -1);
		exit(0);
	}

}
	
	
	
	

// This rotine is only used for frame locking
// in this case, it forces all screens to swap at the same time
void Render::FrameLock() {
	
	int error_code = MPI_SUCCESS;
	
	char error_message[MPI_MAX_ERROR_STRING];
	int message_length;
	
	#ifdef DEBUG
	my_rrr->time_temp = MPI_Wtime();	
	#endif
	
	error_code = MPI_Barrier(my_rrr->graph_comm);
	
	#ifdef DEBUG
	my_rrr->myMPI_Time = my_rrr->myMPI_Time + MPI_Wtime() - my_rrr->time_temp;
	#endif
	
	if(error_code != MPI_SUCCESS) {
		MPI_Error_string(error_code, error_message, &message_length);
		cerr << "ERRO Allreduce: " << error_message << endl;
		MPI_Abort(MPI_COMM_WORLD, -1);
		exit(0);
	}
	
}


