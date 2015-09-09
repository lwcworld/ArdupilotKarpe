#pragma comment (lib, "EVaComm2")
#pragma comment(lib, "ws2_32.lib")

#define APP_VERSION "1.0"
#define BUF_SIZE 120
void ErrorHandling(char *message);

// Change these to zero if you don't want a file written for a particular data type
#define WRITE_IDENTIFIED_MKRS	1
#define WRITE_UNIDENTIFIED_MKRS	0
#define WRITE_SEGMENT_DATA		0


class Sdk2Example2
{
public:
	Sdk2Example2();
	~Sdk2Example2();
};

// 우철 declare
#define NQ (1)

//#include "DataSet.h"
#include <windows.h>				// for Sleep() function
#include "EvartSdk2Interface.h"		// wrapper around EVaRT SDK2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// This object has the ability to receive data from the
// EvartSdk2Interface object becuase it implements the
// ISdkDataListener interface. The two required methods
// for this interface are SdkDataHasArrived() and AcceptingSdkData()
class MySdk2Client : public ISdkDataListener
{

public:

	// Constructor
	MySdk2Client() : m_acceptData(false)
	{
	}

	// Destructor, must close the data files
	~MySdk2Client()
	{
	}

	// Required method to implement the ISdkDataListener interface.
	// Once we register ourselves with an EvartSdk2Interface object,
	// this method will be called whenever a frame of data arrives from
	// EVaRT.
	virtual void SdkDataHasArrived(sFrameOfData* data);

	// Required method to implement the ISdkDataListener interface.
	// This should return true if frames of data are currently being
	// accepted, and false if you do not want to receive frames of data.
	virtual bool AcceptingSdkData() const { return m_acceptData; }

	// Connect to the EVaRT SDK2 running on "serverIP" using the NIC in
	// our client machine with IP address "clientIP"; maxFrames is the number
	// of frames to write to the data file before exiting.
	bool Connect(const char* serverIP, const char* clientIP, int maxFrames);

	// Have we received all the frames required
	bool IsFinished() const		{ return m_frameCount >= m_maxFrames; }

	// Unregister ourselves from the EvartSdk2Interface object
	void Disconnect()
	{
		m_sdk.SetDataListener(NULL);
		m_sdk.Uninitialize();
	}


	// woocheol variable
	static double PosData[NQ][9]; // ball 3개 x,y,z 데이터 

private:

	bool m_acceptData;
	int m_frameCount;
	int m_maxFrames;
	EvartSdk2Interface m_sdk;

	void WriteFrame(sFrameOfData* data);

	void WriteIdentifiedFrame(sFrameOfData* data);
	void WriteUnidentifiedFrame(sFrameOfData* data);
	void WriteSegmentFrame(sFrameOfData* data);
};