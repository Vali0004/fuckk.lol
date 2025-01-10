#pragma once
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PERIOD0 CLOCK_RATIO_DEFAULT // max(round(clock_ratio),9)
#define STEP0 3 // ceil(PERIOD0/9.0)

#define SIDMODEL_8580 8580
#define SIDMODEL_6581 6581

#define DEFAULT_SAMPLERATE 44100  //(Soldier of Fortune, 2nd Reality, Alliance, X-tra energy, Jackal, Sanxion, Ultravox, Hard Track, Swing, Myth, LN3, etc.)
#define DEFAULT_SIDMODEL SIDMODEL_6581

#define C64_PAL_CPUCLK 985248.0
#define SID_CHANNEL_AMOUNT 3
#define MAX_FILENAME_LEN 512
#define MAX_DATA_LEN 65536
#define MAX_FILE_SIZE 20000
#define PAL_FRAMERATE 50.06 // 50.0443427 //50.1245419 //(C64_PAL_CPUCLK/63/312.5), selected carefully otherwise some ADSR-sensitive tunes may suffer more:
#define CLOCK_RATIO_DEFAULT C64_PAL_CPUCLK / DEFAULT_SAMPLERATE //(50.0567520: lowest framerate where Sanxion is fine, and highest where Myth is almost fine)
#define VCR_SHUNT_6581 1500 // kOhm //cca 1.5 MOhm Rshunt across VCR FET drain and source (causing 220Hz bottom cutoff with 470pF integrator capacitors in old C64)
#define VCR_FET_TRESHOLD 192 // Vth (on cutoff numeric range 0..2048) for the VCR cutoff-frequency control FET below which it doesn't conduct
#define CAP_6581 0.470 // nF //filter capacitor value for 6581
#define FILTER_DARKNESS_6581 22.0 // the bigger the value, the darker the filter control is (that is, cutoff frequency increases less with the same cutoff-value)
#define FILTER_DISTORTION_6581 0.0016 // the bigger the value the more of resistance-modulation (filter distortion) is applied for 6581 cutoff-control

class libcsid
{
    enum
    {
        GATE_BITMASK = 0x01,
        SYNC_BITMASK = 0x02,
        RING_BITMASK = 0x04,
        TEST_BITMASK = 0x08,
        TRI_BITMASK = 0x10,
        SAW_BITMASK = 0x20,
        PULSE_BITMASK = 0x40,
        NOISE_BITMASK = 0x80,
        HOLDZERO_BITMASK = 0x10,
        DECAYSUSTAIN_BITMASK = 0x40,
        ATTACK_BITMASK = 0x80,
        LOWPASS_BITMASK = 0x10,
        BANDPASS_BITMASK = 0x20,
        HIGHPASS_BITMASK = 0x40,
        OFF3_BITMASK = 0x80
    };
public:
    libcsid()
    {}
    void init_helper(int _samplerate, int _sidmodel);
    int load(uint8_t *_buffer, int _bufferlen, int _subtune);
    void play(int tune_index);
    void set_volume(float level)
    {
        volume = level;
    }
    void init(uint8_t subt);
    void render(uint8_t* stream, int len);
    void initCPU(uint32_t mempos)
    {
        PC = mempos;
        A = 0;
        X = 0;
        Y = 0;
        ST = 0;
        SP = 0xFF;
    }
    // My CPU implementation is based on the instruction table by Graham at codebase64.
    // After some examination of the table it was clearly seen that columns of the table (instructions' 2nd nybbles)
    //  mainly correspond to addressing modes, and double-rows usually have the same instructions.
    // The code below is laid out like this, with some exceptions present.
    // Thanks to the hardware being in my mind when coding this, the illegal instructions could be added fairly easily...
    uint8_t CPU();
    void cSID_init(int samplerate)
    {
        int i;
        clock_ratio = C64_PAL_CPUCLK / samplerate;
        if (clock_ratio > 9)
        {
            ADSRperiods[0] = clock_ratio;
            ADSRstep[0] = ceil(clock_ratio / 9.0);
        }
        else
        {
            ADSRperiods[0] = 9.0;
            ADSRstep[0] = 1;
        }
        cutoff_ratio_8580 = -2 * 3.14 * (12500 / 2048) / samplerate;                // -2 * 3.14 * ((82000/6.8) / 2048) / samplerate; //approx. 30Hz..12kHz according to datasheet, but only for 6.8nF value, 22nF makes 9Hz...3.7kHz? wrong
        cap_6581_reciprocal = -1000000 / CAP_6581;                                  // lighten CPU-load in sample-callback
        cutoff_steepness_6581 = FILTER_DARKNESS_6581 * (2048.0 - VCR_FET_TRESHOLD); // pre-scale for 0...2048 cutoff-value range //lighten CPU-load in sample-callback
        // cutoff_bottom_6581 = 1 - exp( -1 / (0.000000000470*1500000) / samplerate ); // 1 - exp( -2 * 3.14 * (26000/pow(2,9)/0.47) / samplerate ); //around 140..220Hz cutoff is set by VCR-MOSFET limiter/shunt-resistor (1.5MOhm)
        // cutoff_top_6581 = 20000; //Hz // (26000/0.47);  // 1 - exp( -2 * 3.14 * (26000/0.47) / samplerate);   //cutoff range is 9 octaves stated by datasheet, but process variation might eliminate any filter spec.
        // cutoff_ratio_6581 = -2 * 3.14 * (cutoff_top_6581 / 2048) / samplerate; //(cutoff_top_6581-cutoff_bottom_6581)/(2048.0-192.0); //datasheet: 30Hz..12kHz with 2.2pF -> 140Hz..56kHz with 470pF?

        // createCombinedWF(TriSaw_8580, 0.8, 2.4, 0.64);
        // createCombinedWF(PulseSaw_8580, 1.4, 1.9, 0.68);
        // createCombinedWF(PulseTriSaw_8580, 0.8, 2.5, 0.64);

        for (i = 0; i < 9; i++)
        {
            ADSRstate[i] = HOLDZERO_BITMASK;
            envcnt[i] = 0;
            ratecnt[i] = 0;
            phaseaccu[i] = 0;
            prevaccu[i] = 0;
            expcnt[i] = 0;
            prevSR[i] = 0;
            noise_LFSR[i] = 0x7FFFFF;
            prevwfout[i] = 0;
        }

        for (i = 0; i < 3; i++)
        {
            sourceMSBrise[i] = 0;
            sourceMSB[i] = 0;
            prevlowpass[i] = 0;
            prevbandpass[i] = 0;
        }

        initSID();
    }
    // registers: 0:freql1  1:freqh1  2:pwml1  3:pwmh1  4:ctrl1  5:ad1   6:sr1  7:freql2  8:freqh2  9:pwml2 10:pwmh2 11:ctrl2 12:ad2  13:sr 14:freql3 15:freqh3 16:pwml3 17:pwmh3 18:ctrl3 19:ad3  20:sr3
    //            21:cutoffl 22:cutoffh 23:flsw_reso 24:vol_ftype 25:potX 26:potY 27:OSC3 28:ENV3
    void initSID()
    {
        int i;
        for (i = 0xD400; i <= 0xD7FF; i++)
            memory[i] = 0;
        for (i = 0xDE00; i <= 0xDFFF; i++)
            memory[i] = 0;
        for (i = 0; i < 9; i++)
        {
            ADSRstate[i] = HOLDZERO_BITMASK;
            ratecnt[i] = envcnt[i] = expcnt[i] = 0;
        }
    }
    int SID(uint8_t num, uint32_t baseaddr);
    

    // in case you don't like these calculated combined waveforms it's easy to substitute the generated tables by pre-sampled 'exact' versions
    uint32_t combinedWF(uint8_t num, uint8_t channel, const uint32_t* wfarray, int index, uint8_t differ6581, uint8_t freqh)
    {
        static float addf;
        addf = 0.6 + 0.4 / freqh;
        if (differ6581 && SID_model[num] == 6581)
            index &= 0x7FF;
        prevwavdata[channel] = wfarray[index] * addf + prevwavdata[channel] * (1.0 - addf);
        return prevwavdata[channel];
    }
    void reset()
    {
        OUTPUT_SCALEDOWN = SID_CHANNEL_AMOUNT * 16 + 26;
        clock_ratio = CLOCK_RATIO_DEFAULT;
        memset(ADSRstate, 0, sizeof(ADSRstate));
        memset(expcnt, 0, sizeof(expcnt));
        memset(prevSR, 0, sizeof(prevSR));
        memset(sourceMSBrise, 0, sizeof(sourceMSBrise));

        memset(envcnt, 0, sizeof(envcnt));

        memset(prevwfout, 0, sizeof(prevwfout));
        memset(prevwavdata, 0, sizeof(prevwavdata));
        memset(sourceMSB, 0, sizeof(sourceMSB));
        memset(noise_LFSR, 0, sizeof(noise_LFSR));

        memset(phaseaccu, 0, sizeof(phaseaccu));
        memset(prevaccu, 0, sizeof(prevaccu));
        memset(prevlowpass, 0, sizeof(prevlowpass));
        memset(prevbandpass, 0, sizeof(prevbandpass));
        memset(ratecnt, 0, sizeof(ratecnt));

        filedata = nullptr;
        memset(memory, 0, sizeof(memory));
        memset(timermode, 0, sizeof(timermode));
        memset(SIDtitle, 0, sizeof(SIDtitle));
        memset(SIDauthor, 0, sizeof(SIDauthor));
        memset(SIDinfo, 0, sizeof(SIDinfo));
        PC = pPC = addr = storadd = 0;
        A = T = SP;
        X = Y = IR = ST;
        CPUtime = 0.f;
        cycles = finished = dynCIA = 0;
        set_volume(100);
    }
    float volume = 1.f;
    int OUTPUT_SCALEDOWN = SID_CHANNEL_AMOUNT * 16 + 26;
    float clock_ratio = CLOCK_RATIO_DEFAULT;
    // SID-emulation variables:
    const uint8_t FILTSW[9] = { 1, 2, 4, 1, 2, 4, 1, 2, 4 };
    uint8_t ADSRstate[9], expcnt[9], prevSR[9], sourceMSBrise[9];
    int16_t envcnt[9];
    uint32_t prevwfout[9], prevwavdata[9], sourceMSB[3], noise_LFSR[9];
    int phaseaccu[9], prevaccu[9], prevlowpass[3], prevbandpass[3];
    float ratecnt[9], cutoff_ratio_8580, cutoff_steepness_6581, cap_6581_reciprocal; //, cutoff_ratio_6581, cutoff_bottom_6581, cutoff_top_6581;
    // player-related variables:
    int SIDamount = 1, SID_model[3] = { 8580, 8580, 8580 }, requested_SID_model = -1, sampleratio;
    uint8_t* filedata; // pointer to incoming psid data
    uint8_t memory[MAX_DATA_LEN];
    uint8_t timermode[0x20], SIDtitle[0x20], SIDauthor[0x20], SIDinfo[0x20];
    int subtune = 0;
    int default_tune = 0;
    int total_tunes = 0;
    int tunelength = -1, default_tunelength = 300, minutes = -1, seconds = -1;
    uint32_t initaddr, playaddr, playaddf, SID_address[3] = { 0xD400, 0, 0 };
    int samplerate = DEFAULT_SAMPLERATE;
    float framecnt = 0, frame_sampleperiod = DEFAULT_SAMPLERATE / PAL_FRAMERATE;
    // CPU (and CIA/VIC-IRQ) emulation constants and variables - avoiding internal/automatic variables to retain speed
    uint8_t flagsw[8] = { 0x01, 0x21, 0x04, 0x24, 0x00, 0x40, 0x08, 0x28 };
    uint8_t branchflag[4] = { 0x80, 0x40, 0x01, 0x02 };
    uint32_t PC = 0, pPC = 0, addr = 0, storadd = 0;
    int16_t A = 0, T = 0, SP = 0xFF;
    uint8_t X = 0, Y = 0, IR = 0, ST = 0x00; // STATUS-flags: N V - B D I Z C
    float CPUtime = 0.f;
    uint8_t cycles = 0, finished = 0, dynCIA = 0;
    float ADSRperiods[16] = { PERIOD0, 32, 63, 95, 149, 220, 267, 313, 392, 977, 1954, 3126, 3907, 11720, 19532, 31251 };
    uint8_t ADSRstep[16] = { STEP0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    const uint8_t ADSR_exptable[256] = {
        1, 30, 30, 30, 30, 30, 30, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4,                                                                                   // pos0:1  pos6:30  pos14:16  pos26:8
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
};