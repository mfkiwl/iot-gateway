#include "adc.h"

void adc_init()
{
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_ADC0;
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_ADC1;
    
    CMU->ADCCTRL = CMU_ADCCTRL_ADC0CLKINV | CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO | (3 << _CMU_ADCCTRL_ADC0CLKDIV_SHIFT) | CMU_ADCCTRL_ADC1CLKINV | CMU_ADCCTRL_ADC1CLKSEL_AUXHFRCO | (3 << _CMU_ADCCTRL_ADC1CLKDIV_SHIFT);
    
    cmu_update_clocks();

    // ADC_CLK is 8 MHz
    // adc_sar_clk is 100 kHz (ADC_CLK / (PRESC + 1)) PRESC = 79
    // TIMEBASE period is 1 us (1 MHz) (ADC_CLK / (TIMEBASE + 1)) TIMEBASE = 7
    ADC0->CTRL = ADC_CTRL_CHCONREFWARMIDLE_KEEPPREV | ADC_CTRL_CHCONMODE_MAXSETTLE | ADC_CTRL_OVSRSEL_X16 | (7 << _ADC_CTRL_TIMEBASE_SHIFT) | (79 << _ADC_CTRL_PRESC_SHIFT) | ADC_CTRL_ASYNCCLKEN_ALWAYSON | ADC_CTRL_ADCCLKMODE_ASYNC | ADC_CTRL_WARMUPMODE_NORMAL;
}
float adc_get_avdd()
{
    while(ADC0->STATUS & (ADC_STATUS_WARM | ADC_STATUS_SCANREFWARM | ADC_STATUS_SINGLEREFWARM)); // Wait for ADC to cool down

    ADC0->SINGLECTRL = ADC_SINGLECTRL_AT_64CYCLES | ADC_SINGLECTRL_NEGSEL_VSS | ADC_SINGLECTRL_POSSEL_AVDD | ADC_SINGLECTRL_REF_5V | ADC_SINGLECTRL_RES_OVS;
    ADC0->SINGLECTRLX = ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE | (0 << _ADC_SINGLECTRLX_DVL_SHIFT);
    ADC0->CAL &= ~(_ADC_CAL_SINGLEGAIN_MASK | _ADC_CAL_SINGLEOFFSET_MASK | _ADC_CAL_SINGLEOFFSETINV_MASK);
    ADC0->CAL |= (DEVINFO->ADC0CAL1 & 0x7FFF0000) >> 16; // Calibration for 5V reference
    ADC0->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;
    ADC1->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;

    ADC0->CMD |= ADC_CMD_SINGLESTART;

    while(!(ADC0->IF & ADC_IF_SINGLE));

    float fADCCode = ADC0->SINGLEDATA >> 4;
    float fAVDD = fADCCode * 5000.f / 4096.f;

    return fAVDD;
}
float adc_get_dvdd()
{
    while(ADC0->STATUS & (ADC_STATUS_WARM | ADC_STATUS_SCANREFWARM | ADC_STATUS_SINGLEREFWARM)); // Wait for ADC to cool down

    ADC0->SINGLECTRL = ADC_SINGLECTRL_AT_64CYCLES | ADC_SINGLECTRL_NEGSEL_VSS | ADC_SINGLECTRL_POSSEL_DVDD | ADC_SINGLECTRL_REF_5V | ADC_SINGLECTRL_RES_OVS;
    ADC0->SINGLECTRLX = ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE | (0 << _ADC_SINGLECTRLX_DVL_SHIFT);
    ADC0->CAL &= ~(_ADC_CAL_SINGLEGAIN_MASK | _ADC_CAL_SINGLEOFFSET_MASK | _ADC_CAL_SINGLEOFFSETINV_MASK);
    ADC0->CAL |= (DEVINFO->ADC0CAL1 & 0x7FFF0000) >> 16; // Calibration for 5V reference
    ADC0->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;
    ADC1->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;

    ADC0->CMD |= ADC_CMD_SINGLESTART;

    while(!(ADC0->IF & ADC_IF_SINGLE));

    float fADCCode = ADC0->SINGLEDATA >> 4;
    float fDVDD = fADCCode * 5000.f / 4096.f;

    return fDVDD;
}
float adc_get_iovdd()
{
    while(ADC0->STATUS & (ADC_STATUS_WARM | ADC_STATUS_SCANREFWARM | ADC_STATUS_SINGLEREFWARM)); // Wait for ADC to cool down

    ADC0->SINGLECTRL = ADC_SINGLECTRL_AT_64CYCLES | ADC_SINGLECTRL_NEGSEL_VSS | ADC_SINGLECTRL_POSSEL_IOVDD | ADC_SINGLECTRL_REF_5V | ADC_SINGLECTRL_RES_OVS;
    ADC0->SINGLECTRLX = ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE | (0 << _ADC_SINGLECTRLX_DVL_SHIFT);
    ADC0->CAL &= ~(_ADC_CAL_SINGLEGAIN_MASK | _ADC_CAL_SINGLEOFFSET_MASK | _ADC_CAL_SINGLEOFFSETINV_MASK);
    ADC0->CAL |= (DEVINFO->ADC0CAL1 & 0x7FFF0000) >> 16; // Calibration for 5V reference
    ADC0->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;
    ADC1->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;

    ADC0->CMD |= ADC_CMD_SINGLESTART;

    while(!(ADC0->IF & ADC_IF_SINGLE));

    float fADCCode = ADC0->SINGLEDATA >> 4;
    float fIOVDD = fADCCode * 5000.f / 4096.f;

    return fIOVDD;
}
float adc_get_corevdd()
{
    while(ADC0->STATUS & (ADC_STATUS_WARM | ADC_STATUS_SCANREFWARM | ADC_STATUS_SINGLEREFWARM)); // Wait for ADC to cool down

    ADC0->SINGLECTRL = ADC_SINGLECTRL_AT_64CYCLES | ADC_SINGLECTRL_NEGSEL_VSS | ADC_SINGLECTRL_POSSEL_DECOUPLE | ADC_SINGLECTRL_REF_2V5 | ADC_SINGLECTRL_RES_OVS;
    ADC0->SINGLECTRLX = ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE | (0 << _ADC_SINGLECTRLX_DVL_SHIFT);
    ADC0->CAL &= ~(_ADC_CAL_SINGLEGAIN_MASK | _ADC_CAL_SINGLEOFFSET_MASK | _ADC_CAL_SINGLEOFFSETINV_MASK);
    ADC0->CAL |= (DEVINFO->ADC0CAL0 & 0x7FFF0000) >> 16; // Calibration for 2V5 reference
    ADC0->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;
    ADC1->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_HIGHACC;

    ADC0->CMD |= ADC_CMD_SINGLESTART;

    while(!(ADC0->IF & ADC_IF_SINGLE));

    float fADCCode = ADC0->SINGLEDATA >> 4;
    float fDECOUPLE = fADCCode * 2500.f / 4096.f;

    return fDECOUPLE;
}
float adc_get_temperature()
{
    while(ADC0->STATUS & (ADC_STATUS_WARM | ADC_STATUS_SCANREFWARM | ADC_STATUS_SINGLEREFWARM)); // Wait for ADC to cool down

    ADC0->SINGLECTRL = ADC_SINGLECTRL_AT_256CYCLES | ADC_SINGLECTRL_NEGSEL_VSS | ADC_SINGLECTRL_POSSEL_TEMP | ADC_SINGLECTRL_REF_1V25 | ADC_SINGLECTRL_RES_OVS;
    ADC0->SINGLECTRLX = ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE | (0 << _ADC_SINGLECTRLX_DVL_SHIFT);
    ADC0->CAL &= ~(_ADC_CAL_SINGLEGAIN_MASK | _ADC_CAL_SINGLEOFFSET_MASK | _ADC_CAL_SINGLEOFFSETINV_MASK);
    ADC0->CAL |= (DEVINFO->ADC0CAL0 & 0x00007FFF) >> 0; // Calibration for 1V25 reference
    ADC0->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_LOWACC;
    ADC1->BIASPROG = (ADC0->BIASPROG & ~_ADC_BIASPROG_ADCBIASPROG_MASK) | ADC_BIASPROG_GPBIASACC_LOWACC;

    ADC0->CMD |= ADC_CMD_SINGLESTART;

    while(!(ADC0->IF & ADC_IF_SINGLE));

    float fADCCode = ADC0->SINGLEDATA >> 4;
    float fCalibrationTemp = (DEVINFO->CAL & _DEVINFO_CAL_TEMP_MASK) >> _DEVINFO_CAL_TEMP_SHIFT;
    float fADCCalibrationTemp = (DEVINFO->ADC0CAL3 & _DEVINFO_ADC0CAL3_TEMPREAD1V25_MASK) >> _DEVINFO_ADC0CAL3_TEMPREAD1V25_SHIFT;
    float fADCTemp = fCalibrationTemp - (fADCCalibrationTemp - fADCCode) * 1250.f / (4096.f * -1.84f);

    return fADCTemp;
}

