/* Red Pitaya C API example Generating continuous signal  
 * This application generates a specific signal */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "redpitaya/rp.h"

int main(int argc, char **argv){
	float freq;

	/* Print error, if rp_Init() function failed */
	if(rp_Init() != RP_OK){
		fprintf(stderr, "Rp api init failed!\n");
	}

	/* Generating frequency */
	printf("input freq in Hz: ");
	scanf("%f", &freq);
	rp_GenFreq(RP_CH_1, freq);

	/* Generating amplitude */
	rp_GenAmp(RP_CH_1, 0.1);

	/* Generating wave form */
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);

	/* Enable channel */
	rp_GenOutEnable(RP_CH_1);

	/* Releasing resources */
	rp_Release();

	return 0;
}
