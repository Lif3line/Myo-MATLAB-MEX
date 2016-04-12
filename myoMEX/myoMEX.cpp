// myoMEX.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <Windows.h>
#include <inttypes.h>
#include "myo\myo.hpp"
#include "mex.h"
#include "matrix.h"

#define RUN_TIME_OUT 10 // Timeout for data gather in ms
#define BUFFER_SAMPLES 40 // How many samples we want to keep

#define NUM_CHANNELS 8 // Number of channels on the myo

#define BUFFER_SIZE NUM_CHANNELS*BUFFER_SAMPLES // Total buffer size 

using namespace std;

myo::Hub* curHub = NULL;
FILETIME ft_now;

class DataCollector : public myo::DeviceListener {
public:
	DataCollector() : emgSamples() {}

	/* On user disconnect */
	void onUnpair(myo::Myo* myo, uint64_t timestamp) {
		emgSamples.fill(0);
		timestampArray.fill(0);
	}

	/* EMG data interrupt - save data and print to text file */
	void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) {
		uint64_t winTimestamp;
		int j;

		/* EMG Handling */
		for (int i = NUM_CHANNELS; i < BUFFER_SIZE; i++) { // Left shift by number of channels (keep constant buffer)
			emgSamples[i- NUM_CHANNELS] = emgSamples[i];
		}

		j = 0;
		for (int i = BUFFER_SIZE - NUM_CHANNELS; i < BUFFER_SIZE; i++) { // Add new EMG data to buffer
			emgSamples[i] = emg[j];
			j++;
		}


		/* Timestamp Handling*/
		GetSystemTimeAsFileTime(&ft_now); // Time grabbed from windows since myo SDK specifies timestamp as from (unspecified) time 
		winTimestamp = ((uint64_t)ft_now.dwLowDateTime + ((uint64_t)(ft_now.dwHighDateTime) << 32LL) - 116444736000000000LL); // time since UNIX epoch in 100ns resolution

		for (int i = 1; i < BUFFER_SAMPLES; i++) { // Left shift by number of channels (keep constant buffer)
			timestampArray[i - 1] = timestampArray[i];
		}
		timestampArray[BUFFER_SAMPLES - 1] = winTimestamp;

	}

	void getEmgData(double *outMatrix, uint64_t *outTime) {
		for (int i = 0; i < BUFFER_SIZE; i++) {
			outMatrix[i] = (double)emgSamples[i];
		}

		for (int i = 0; i < BUFFER_SAMPLES; i++) {
			outTime[i] = timestampArray[i];
		}
	}

private:
	std::array<int8_t, BUFFER_SIZE > emgSamples;
	std::array<uint64_t, BUFFER_SAMPLES > timestampArray;
};
DataCollector collector;


void openMyoLink(void) {
	curHub = new myo::Hub("com.adam-hartwell.myoMATLAB");

	myo::Myo* curMyo = curHub->waitForMyo(10000);
	if (!curMyo) {
		throw std::runtime_error("Unable to find a Myo!");
	}
	
	curMyo->setStreamEmg(myo::Myo::streamEmgEnabled); // Set up EMG stream

	curHub->addListener(&collector);
	return;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	double *outMatrix, in;
	uint64_t *outTime;

	if (nrhs != 1) {
		mexErrMsgIdAndTxt("MyoDataAcquisition:mexFunction:nrhs","Single input required.");
	}

	in = mxGetScalar(prhs[0]);

	plhs[0] = mxCreateDoubleMatrix(1, BUFFER_SIZE, mxREAL);
	plhs[1] = mxCreateNumericMatrix(1, BUFFER_SAMPLES, mxUINT64_CLASS, mxREAL);
	outMatrix = mxGetPr(plhs[0]);
	outTime = (uint64_t*)mxGetData(plhs[1]);

	if (in == 0) {
		curHub->runOnce(RUN_TIME_OUT);
		collector.getEmgData(outMatrix, outTime);
	} else if (in == 1) {
		openMyoLink();
	} 
}