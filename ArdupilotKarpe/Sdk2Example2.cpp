#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#include "Sdk2Example2.h"

//#include <winsock2.h>

#define APP_VERSION "1.0"
#define BUF_SIZE 120
void ErrorHandling(char *message);

// Change these to zero if you don't want a file written for a particular data type
#define WRITE_IDENTIFIED_MKRS	1
#define WRITE_UNIDENTIFIED_MKRS	0
#define WRITE_SEGMENT_DATA		0

int cpt = 0;

// 우철 변수입니다.
double MySdk2Client::PosData[NQ][9] = { 0 };
//double DataSet::Mpos[NQ][9];

Sdk2Example2::Sdk2Example2()
{
}


Sdk2Example2::~Sdk2Example2()
{
}

// This is called for each frame of data received by the SDK2.
// The pointer to data is "hot", so if it needs to be stored
// for later use, use EvartSdk2Interface::CopyFrame() to make a copy.
void MySdk2Client::SdkDataHasArrived(sFrameOfData* data)
{
	WriteFrame(data);
	++m_frameCount;
}

// Initialize the EVaRT SDK2
// Query list of objects being tracked by EVaRT
// Write out data file header
bool MySdk2Client::Connect(const char* serverIP, const char* clientIP, int maxFrames)
{
	bool rc;
	sBodyDefs* evartObjects = NULL;

	m_maxFrames = maxFrames;
	m_frameCount = 0;

	// Initialize the EVaRT SDK2
	rc = m_sdk.Initialize(clientIP, serverIP);

	// Query the list of objects that EVaRT is currently tracking
	evartObjects = m_sdk.GetBodyDefinitions();

	if (rc && evartObjects)
	{
		// Register ourselves as a listener
		m_sdk.SetDataListener(this);

		// We are now ready to accept frames of data from EVaRT
		m_acceptData = true;
	}
	else
	{
		rc = false;
	}

	// Free memory for the EVaRT body definitions
	m_sdk.FreeBodyDefinitions(evartObjects);

	return rc;
}



void MySdk2Client::WriteFrame(sFrameOfData* data)
{
#if WRITE_IDENTIFIED_MKRS
	WriteIdentifiedFrame(data);
#endif
#if WRITE_UNIDENTIFIED_MKRS
	WriteUnidentifiedFrame(data);
#endif
#if WRITE_SEGMENT_DATA
	WriteSegmentFrame(data);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
// Write Identified Data =============================================================== //
///////////////////////////////////////////////////////////////////////////////////////////
void MySdk2Client::WriteIdentifiedFrame(sFrameOfData* data)
{
	char message[100];
	DWORD bytesWritten;
	int pos_data[NQ][12] = { 0 };
	//SEND data to UAVs 

	for (int i = 0; i < NQ; i++)
	{
		pos_data[i][0] = (data->BodyData[0].Markers[0][0]) * 1000;
		pos_data[i][1] = (data->BodyData[0].Markers[0][1]) * 1000;
		pos_data[i][2] = (data->BodyData[0].Markers[0][2]) * 1000;

		// quadrotor _ y_+
		pos_data[i][3] = (data->BodyData[0].Markers[1][0]) * 1000;
		pos_data[i][4] = (data->BodyData[0].Markers[1][1]) * 1000;
		pos_data[i][5] = (data->BodyData[0].Markers[1][2]) * 1000;

		// quadrotor _ x_+
		pos_data[i][6] = (data->BodyData[0].Markers[2][0]) * 1000;
		pos_data[i][7] = (data->BodyData[0].Markers[2][1]) * 1000;
		pos_data[i][8] = (data->BodyData[0].Markers[2][2]) * 1000;

		////M1 x,y,z
		//pos_data[i][9] = (data->BodyData[0].Segments[0][0]) * 1000;
		//pos_data[i][10] = (data->BodyData[0].Segments[0][1]) * 1000;
		//pos_data[i][11] = (data->BodyData[0].Segments[0][2]) * 1000;

		//M1 euler angle(x,y,z)
		pos_data[i][9] = (data->BodyData[0].Segments[0][3]) * 1000;
		pos_data[i][10] = (data->BodyData[0].Segments[0][4]) * 1000;
		pos_data[i][11] = (data->BodyData[0].Segments[0][5]) * 1000;
	}


	// DataSet::Mpos[x][i] : M1 pos(012) / M2 pos(345) / M3 pos(678) / M1 angle (9 10 11)
	for (int i = 0; i < NQ; i++){
		for (int j = 0; j < 12; j++){
			DataSet::Mpos[i][j] = pos_data[i][j];
			printf("%d ", pos_data[i][j]);
		}
	}

	printf("\n");
}

///////////////////////////////////////////////////////////////////////////////////////////
// Write Unidentified Data ============================================================= //
///////////////////////////////////////////////////////////////////////////////////////////
void MySdk2Client::WriteUnidentifiedFrame(sFrameOfData* data)
{

	// Write out X, Y, Z for each unnamed marker
	for (int i = 0; i < data->nUnidentifiedMarkers; i++)
	{
		printf("%.5f,%.5f,%.5f\n",
			data->UnidentifiedMarkers[i][0],
			data->UnidentifiedMarkers[i][1],
			data->UnidentifiedMarkers[i][2]);
	}
	printf("\n");
}

///////////////////////////////////////////////////////////////////////////////////////////
// Write Segment Data ================================================================== //
///////////////////////////////////////////////////////////////////////////////////////////
void MySdk2Client::WriteSegmentFrame(sFrameOfData* data)
{
	WSADATA wsaData;
	SOCKET eulerSock;

	double eulerData_1[7]; // Save the euler data which will be sended to AR_drone_1 

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	eulerSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (eulerSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	SOCKADDR_IN AR_drone[2];

	memset(&AR_drone[0], 0, sizeof(AR_drone[0]));
	AR_drone[0].sin_family = AF_INET;
	//AR_drone[0].sin_addr.s_addr = inet_addr("192.168.100.12");
	AR_drone[0].sin_port = htons(8101);

	int eulerStrLen[3];
	int ARD_AdrSz[3];
	int CBBA_AdrSz;
	int from_AdrSz;

	SOCKADDR_IN CBBA, recvAdr, fromAdr;

	//--------------------------------------------------------------------------
	//				data trasfer using UDP from SDK to client
	//--------------------------------------------------------------------------

	ARD_AdrSz[0] = sizeof(AR_drone[0]);
	ARD_AdrSz[1] = sizeof(AR_drone[1]);
	CBBA_AdrSz = sizeof(CBBA);

	// Assign the Euler data 

	eulerData_1[0] = data->BodyData[0].Segments[0][0];
	eulerData_1[1] = data->BodyData[0].Segments[0][1];
	eulerData_1[2] = data->BodyData[0].Segments[0][2];
	eulerData_1[3] = data->BodyData[0].Segments[0][3];
	eulerData_1[4] = data->BodyData[0].Segments[0][4];
	eulerData_1[5] = data->BodyData[0].Segments[0][5];
	eulerData_1[6] = data->BodyData[0].Segments[0][6];


	double Hz = 10; //200/Hz (Hz) 
	if (cpt > Hz)
		sendto(eulerSock, (const char*)eulerData_1, 56, 0, (SOCKADDR*)&AR_drone[0], sizeof(AR_drone[0]));


	// Write out TX, TY, TZ, RX, RY, RZ, Length for each segment
	for (int i = 0; i < data->nBodies; i++)
	{
		for (int j = 0; j < data->BodyData[i].nSegments; j++)
		{
			if (data->BodyData[i].Segments[j][0] < XEMPTY)
			{
				printf("%f,%f,%f,%f,%f,%f,%f",
					data->BodyData[i].Segments[j][0],
					data->BodyData[i].Segments[j][1],
					data->BodyData[i].Segments[j][2],
					data->BodyData[i].Segments[j][3],
					data->BodyData[i].Segments[j][4],
					data->BodyData[i].Segments[j][5],
					data->BodyData[i].Segments[j][6]);
			}
			else
			{
				printf("%f,%f,%f,%f,%f,%f,%f",
					0.0,
					0.0,
					0.0,
					0.0,
					0.0,
					0.0,
					0.0);
			}
		}
	}
	printf("\n");
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
