
#include "wand.h"
#include "../general.h"
#include "../communication.h"

#include "../isense.h"

#include <iostream>

#define CAMERA_TRACKER  0
#define GET_AUX_INPUTS  0

using namespace std;

const char *systemType(int Type) {
	switch (Type) {
		case ISD_NONE:
			return "Unknown";
		case ISD_PRECISION_SERIES:
			return "IS Precision Series";
		case ISD_INTERTRAX_SERIES:
			return "InterTrax Series";
	}
	return("Unknown");
}

const char *systemName (int Model) {
	switch (Model) {
		case ISD_IS300:			return "IS-300 Series";
		case ISD_IS600:			return "IS-600 Series";
		case ISD_IS900:			return "IS-900 Series";
		case ISD_INTERTRAX:			return "InterTrax 30";
		case ISD_INTERTRAX_2:			return "InterTrax2";
		case ISD_INTERTRAX_LS:			return "InterTraxLS";
		case ISD_INTERTRAX_LC:			return "InterTraxLC";
		case ISD_ICUBE2:			return "InertiaCube2";
		case ISD_ICUBE2_PRO:			return "InertiaCube2 Pro";
		case ISD_ICUBE3:			return "InertiaCube3";
		case ISD_IS1200:			return "IS-1200 Series";
	}
	return("Unknown");
}



void showTrackerStats (ISD_TRACKER_HANDLE handle) {
	
	ISD_TRACKER_INFO_TYPE Tracker;
	ISD_STATION_INFO_TYPE Station;
	
	WORD i, numStations = 4;
	
	char buf[20];

	if (ISD_GetTrackerConfig (handle, &Tracker, TRUE))	{
		cout << "\n\n********** InterSense Tracker Information ***********\n\n";
		cout << "Type:     " << systemType (Tracker.TrackerType) << " device on port " << Tracker.Port << endl;
		cout << "Model:    " << systemName (Tracker.TrackerModel) << endl;
		
		switch (Tracker.TrackerModel) {
			case ISD_IS300:
			case ISD_IS1200:
				numStations = 4;
				break;
			case ISD_IS600:
			case ISD_IS900:
				numStations = ISD_MAX_STATIONS;
				break;
			default:
				numStations = 1;
				break;
		}

		printf ("\nStation\tTime\tState\tCube  Enhancement  Sensitivity  Compass  Prediction\n");
	
		for (i = 1; i <= numStations; i++) {
			printf ("%d\t", i);
		
			if (ISD_GetStationConfig(handle, &Station, i, FALSE)) {
				sprintf (buf, "%d", Station.InertiaCube);
				
				/*
				printf ("%s\t%s\t%s\t   %u\t\t%u\t  %u\t  %u\n", 
				Station.TimeStamped ? "ON" : "OFF", 
				Station.State ? "ON" : "OFF", 
				Station.InertiaCube == -1 ? "None" : buf, 
				Station.Enhancement, 
				Station.Sensitivity, 
				Station.Compass, 
				Station.Prediction);
				*/

				cout << 
				(Station.TimeStamped ? "ON" : "OFF") << "\t" <<
				(Station.State ? "ON" : "OFF") << "\t" <<
                                (Station.InertiaCube == -1 ? "None" : buf) << "\t" <<
                                (Station.Enhancement) << "\t" <<
                                (Station.Sensitivity) << "\t" <<
                                (Station.Compass) << "\t" <<
                                (Station.Prediction) << "\t" <<
				endl;


			} else {
				printf ("ISD_GetStationConfig failed\n");
				break;
			}
		}
		
		printf ("\n");
	} else {
		printf ("ISD_GetTrackerConfig failed\n");
	}

}



/* Just read an intertrax device */
void Wand::readheadtracking() {
	
	posWand[0] = new PosWand(); 
	
	ISD_TRACKER_HANDLE handle;
	ISD_TRACKER_DATA_TYPE data;
	ISD_STATION_INFO_TYPE Station[ISD_MAX_STATIONS];
	ISD_CAMERA_DATA_TYPE cameraData;
	ISD_TRACKER_INFO_TYPE Tracker;
	
	WORD station = 1;
	
	Bool verbose = TRUE;
	
	float lastTime;
	
	// Detect first tracker. If you have more than one InterSense device and
	// would like to have a specific tracker, connected to a known port, 
	// initialized first, then enter the port number instead of 0. Otherwise, 
	// tracker connected to the rs232 port with lower number is found first 
		
	handle = ISD_OpenTracker ((Hwnd) NULL, 0, FALSE, verbose);
	
	// Check value of handle to see if tracker was located 
	if (handle < 1) {
		cout << "Failed to detect InterSense tracking device" << endl;
	} else {
		// Get tracker configuration info 
		ISD_GetTrackerConfig (handle, &Tracker, verbose);
		
		// Clear station configuration info to make sure GetAnalogData and other flags are FALSE 
		memset ((void *) Station, 0, sizeof (Station));
		
		// General procedure for changing any setting is to first retrieve current 
		// configuration, make the change, and then apply it. Calling 
		// ISD_GetStationConfig is important because you only want to change 
		// some of the settings, leaving the rest unchanged. 
			
		if (Tracker.TrackerType == ISD_PRECISION_SERIES) {
			for (station = 1; station <= 4; station++) {
				// fill ISD_STATION_INFO_TYPE structure with current station configuration 
				if (!ISD_GetStationConfig(handle, &Station[station - 1], station, verbose))
					break;
				if (GET_AUX_INPUTS && Tracker.TrackerModel == ISD_IS900) {
					Station[station - 1].GetAuxInputs =	TRUE;
					
					// apply new configuration 
					if (!ISD_SetStationConfig(handle,&Station[station - 1],station, verbose))
						break;
				}
		
				if (CAMERA_TRACKER && Tracker.TrackerModel == ISD_IS900) {
					Station[station - 1].GetCameraData = TRUE;
					
					// apply new configuration 
					if (!ISD_SetStationConfig(handle,&Station[station - 1],station, verbose))
						break;
				
				}

			}
		}
	}
	
	station = 1;
	
	lastTime = ISD_GetTime ();
	
	while (true) {
		
			
		breath();	// just a sleep	

		
		// must be called at a reasonable rate
		if (handle > 0)	{
			ISD_GetData (handle, &data);
			ISD_GetCameraData (handle, &cameraData);
		
			if (ISD_GetTime () - lastTime > 0.01f) {
				lastTime = ISD_GetTime ();
				
				if (handle > 0) {
					/*
					showStationData (handle, &Tracker,&Station[0], &data.Station[0], &cameraData.Camera[0]);
					
					void showStationData (ISD_TRACKER_HANDLE handle, 
					ISD_TRACKER_INFO_TYPE * Tracker, 
					ISD_STATION_INFO_TYPE * Station, 
					ISD_STATION_STATE_TYPE * data,
					ISD_CAMERA_ENCODER_DATA_TYPE * cameraData);
					*/
					// Get comm port statistics for display with tracker data 
					if (ISD_GetCommInfo (handle, &Tracker)) {
					#ifdef DEBUG
						printf ("%3.0fKbps %d R/s ", Tracker.KBitsPerSec, Tracker.RecordsPerSec);
					#endif
		
						// display position only if system supports it 
						if (Tracker.TrackerModel == ISD_IS600 || Tracker.TrackerModel == ISD_IS900 || Tracker.TrackerModel == ISD_IS1200) {
							#ifdef DEBUG
								printf ("%6.2f %6.2f %6.2f \n", data->Position[0], data->Position[1], data->Position[2]);
							#endif
						}
		
						// all products can return orientation 
						if (Station[0].AngleFormat == ISD_QUATERNION) {
			
							#ifdef DEBUG
								printf ("%5.2f %5.2f %5.2f %5.2f ", data->Orientation[0], data->Orientation[1], data->Orientation[2], data->Orientation[3]);
							#endif
			
						} else {	// Euler angles
			
							#ifdef DEBUG
								cout << data->Orientation[0] << " " << data->Orientation[1] << " " << data->Orientation[2];
							#endif
			
							MPI_Status status;
							int received=1;

							MPI_Test(&request, &received, &status);
							if(received) {
								MPI_Wait(&request, &status);
								MPI_Irecv(&this->input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &this->request);

								posWand[0]->wand.trans[0]=0;
								posWand[0]->wand.trans[1]=-1;
								posWand[0]->wand.trans[2]=-2;
								posWand[0]->wand.rotat[0]=0;
								posWand[0]->wand.rotat[1]=0;
								posWand[0]->wand.rotat[2]=0;
			
								posWand[0]->head.rotat[0]=data.Station[0].Orientation[1];
								posWand[0]->head.rotat[1]=-data.Station[0].Orientation[0];
								posWand[0]->head.rotat[2]=data.Station[0].Orientation[2];
			
								posWand[0]->wand.buttons=0;
								posWand[0]->wand.wonoff=0;
								posWand[0]->wand.lixo=0;
			
								MPI_Ssend(&posWand[0]->wand, 1, posWand[0]->wandDatatype, WAND, INPUT_WAND, MPI_COMM_WORLD);
							}
				
						}
		
						if (Station[0].GetAuxInputs) {
							#ifdef DEBUG
								printf ("%d %d %d %d ", (int) data->AuxInputs[0], (int) data->AuxInputs[1], (int) data->AuxInputs[2], (int) data->AuxInputs[3]);
							#endif
						}
		
						// if system is configured to read stylus or wand buttons 
						if (Station[0].GetInputs) {
							// Currently available products have at most 5 buttons,
							// stylus has 2, wand has 5
							#ifdef DEBUG
								printf ("%d%d%d%d%d ", 
										(int) data.Station[0].ButtonState[0], 
										(int) data.Station[0].ButtonState[1], 
										(int) data.Station[0].ButtonState[2], 
										(int) data.Station[0].ButtonState[3], 
										(int) data.Station[0].ButtonState[4]);
			
								printf ("%d %d ", data.Station[0].AnalogData[0],
			  							data.Station[0].AnalogData[1]);
							#endif
						}
		
						// if system is configured to read encoder data 
						if (Station[0].GetCameraData) {
							#ifdef DEBUG
								printf ("\n\tTime: %.3f  Camera data: %-ld %-ld %-ld \n", 
									data->TimeStamp, 
									cameraData->ApertureEncoder, 
									cameraData->FocusEncoder, 
									cameraData->ZoomEncoder);
							#endif
						} else {
							#ifdef DEBUG
								printf ("\r");
							#endif
						}

						//fflush (0);

					}
								
				} 					
			}
		
		} else {
					
		
			MPI_Status status;
			int received=1;

			MPI_Test(&request, &received, &status);
			if(received) {
				MPI_Wait(&request, &status);
				MPI_Irecv(&this->input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &this->request);

				posWand[0]->wand.trans[0]=0;
				posWand[0]->wand.trans[1]=-1;
				posWand[0]->wand.trans[2]=-2;
				posWand[0]->wand.rotat[0]=0;
				posWand[0]->wand.rotat[1]=0;
				posWand[0]->wand.rotat[2]=0;
			
				posWand[0]->head.rotat[0]=0.0;
				posWand[0]->head.rotat[1]=0.0;
				posWand[0]->head.rotat[2]=0.0;
			
				posWand[0]->wand.buttons=0;
				posWand[0]->wand.wonoff=0;
				posWand[0]->wand.lixo=0;
				
				MPI_Ssend(&posWand[0]->wand, 1, posWand[0]->wandDatatype, WAND, INPUT_WAND, MPI_COMM_WORLD);

			}
						
		}
		
	}
	
	ISD_CloseTracker (handle);

}
