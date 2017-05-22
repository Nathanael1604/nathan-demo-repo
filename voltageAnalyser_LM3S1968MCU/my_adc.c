// Author: Nathanael Park and Hayden Becker
// my_adc for ENCE361 Project 1
// Contains ADC initialization, interrupt, and main functions

#include "my_adc.h"  // header file for my_adc.c

// Global variable declaration
uint16_t dataArray1[DATA_BUFF_SIZE];
uint16_t dataArray2[DATA_BUFF_SIZE];
uint16_t uiValue = 0;
static volatile uint16_t dataIndex = 0;

void adcInit(void)
{
	// Sets the system clock
	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	// Enables pins B1 and A1 to allow input for ADC0 and ADC1 respectively
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTA_BASE, GPIO_PIN_1);

	// Enable ADC0 and ADC1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

	// Sample rate is increased from default to avoid conversion speed being too slow,
	// which could impact results. The highest setting was chosen to be as safe as possible.
	SysCtlADCSpeedSet(SYSCTL_ADCSPEED_1MSPS);

	// Configure ADC to be triggered by the timer and put input from ADC0 and
	// ADC1 in the FIFO using sample sequence 2.
	ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_TIMER, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

	// Enable sample sequence 2
	ADCSequenceEnable(ADC0_BASE, 2);

	// Register the function ADCISR to be called when the timer triggers the ADC
	ADCIntRegister(ADC0_BASE, 2, ADCISR);
	ADCIntEnable(ADC0_BASE, 2);
    IntEnable(INT_ADC0);
}

// This function runs when the timer interrupts
void ADCISR(void)
{
	uint32_t ulValue[2];
	//Since an interrupt has caused this function to be run, clear the interrupt flag
    ADCIntClear(ADC_BASE, 2);

    // Collecting data into the buffer with data from ADC
	// The timer trigger has already initiated an ADC conversion. All that is required is to
	// store the ADC values in ulValue, convert to unsigned shorts, then store in dataArray1 and dataArray2
    ADCSequenceDataGet(ADC0_BASE, 2, ulValue);
	uiValue = (unsigned int) ulValue[0];
	dataArray1[dataIndex] = uiValue;
	uiValue = (unsigned int) ulValue[1];
	dataArray2[dataIndex] = uiValue;

	// dataIndex is used to implement circular buffers for dataArray1 and dataArray2
    if (dataIndex < DATA_BUFF_SIZE) {
    	dataIndex++;
    }
    else if (dataIndex >= DATA_BUFF_SIZE) {
    	dataIndex = 0;
    }
}

int main(void)
{
	uint16_t MSEVal = 0;
	uint16_t minV1 = 1023, maxV1 = 0, maxVSteadied1 = 0, minVSteadied1 = 1023;
	uint16_t vAve1 = 0, vrmsVal1 = 0;
	uint16_t peak1 = 0, peakCalib1 = 0, peakToPeak1 = 0, peakToPeakSteadied1 = 0;
	uint16_t waveFlag1 = DEFAULT_WAVE, frequencyVal1 = 0;
	uint16_t minV2 = 1023, maxV2 = 0, maxVSteadied2 = 0, minVSteadied2 = 1023;
	uint16_t vAve2 = 0, vrmsVal2 = 0;
	uint16_t peak2 = 0, peakCalib2 = 0, peakToPeak2 = 0, peakToPeakSteadied2 = 0;
	uint16_t waveFlag2 = DEFAULT_WAVE, frequencyVal2 = 0;

    // Initialization functions
    adcInit();
    buttonsInit();
    displayInit();
    timerInit();
    // Clearing the screen to start executing tasks
    displayClear();
	// Starting the program
    schedulerStart();
    // Clear the interrupt before we get started
    ADCIntClear(ADC0_BASE, 2);

    while(1)
    {
    	// Checks button input and only progresses if there has been
    	// input at least once
        if (schedulerRun())
        {
        	// Checking for maximum values in dataArray
        	maxV1 = getMaxV(dataArray1);
        	maxV2 = getMaxV(dataArray2);
       		// Checking for mininum values in dataArray
     		minV1 = getMinV(dataArray1);
     		minV2 = getMinV(dataArray2);
     		// Calculating peak to peak voltage
        	peakToPeak1 = getPeakToPeakV(maxV1, minV1);
        	peakToPeak2 = getPeakToPeakV(maxV2, minV2);
 			// Calculating the average voltage (i.e. DC offset)
        	vAve1 = getVAve(peakToPeak1, minV1);
        	vAve2 = getVAve(peakToPeak2, minV2);
        	// Amplitude of the input wave
        	peak1 = maxV1 - vAve1;
        	peak2 = maxV2 - vAve2;
        	// Calculating the frequency
        	frequencyVal1 = getFrequency(dataArray1, vAve1);
        	frequencyVal2 = getFrequency(dataArray2, vAve2);
        	// Calculating the room mean square voltage
        	vrmsVal1 = getVRMS(dataArray1);
        	vrmsVal2 = getVRMS(dataArray2);
        	// Finding the mean square error
        	MSEVal = getMSE(dataArray1, dataArray2);
        	// Steadying values to prevent tiny fluctuations
        	maxVSteadied1 = steady(maxV1, maxVSteadied1);
        	maxVSteadied2 = steady(maxV2, maxVSteadied2);
        	minVSteadied1 = steady(minV1, minVSteadied1);
        	minVSteadied2 = steady(minV2, minVSteadied2);
        	peakToPeakSteadied1 = steady(peakToPeak1, peakToPeakSteadied1);
        	peakToPeakSteadied2 = steady(peakToPeak2, peakToPeakSteadied2);
        	// Calibrating wave amplitude for waveform calculations
        	peakCalib1 = peak1 - peak1/10;
        	peakCalib2 = peak2 - peak2/10;
        	// Calibrating peak-to-peak values for lab equipment
        	peakToPeakSteadied1 = peakToPeakSteadied1 + peakToPeakSteadied1/21;
        	peakToPeakSteadied2 = peakToPeakSteadied2 + peakToPeakSteadied2/21;
        	// Finding the type of wave
        	waveFlag1 = getWaveform(peakToPeak1, dataArray1, vAve1, peakCalib1);
        	waveFlag2 = getWaveform(peakToPeak2, dataArray2, vAve2, peakCalib2);
       	 	// Scheduling processes depending on button_events
       	 	schedulerTask(vrmsVal1, peakToPeakSteadied1, frequencyVal1, waveFlag1, vrmsVal2, peakToPeakSteadied2, frequencyVal2, waveFlag2, MSEVal);
        } 	 	
    }
}
