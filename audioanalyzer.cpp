#include "audioanalyzer.h"
#include <cmath>

#define REAL 0
#define IMAG 1
#define PREC 0.25
#define SR 44100

AudioAnalyzer::AudioAnalyzer(fftw_complex* spectrum, double length, int sampleRate, int samplesPerChannel, QObject *parent)
    : QObject(parent)
    , spectrum(spectrum)
    , length(length)
    , sampleRate(sampleRate)
    , samplesPerChannel(samplesPerChannel)
{
    this->resolution = sampleRate/(length*sampleRate);
}

double AudioAnalyzer::getFrequency(){
    if(this->calculatedFrequency > 0){
        return this->calculatedFrequency;
    }
    fftw_complex* out = (fftw_complex*)malloc(samplesPerChannel*sizeof(fftw_complex));
    fftw_plan plan = fftw_plan_dft_1d(samplesPerChannel, spectrum, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    fftw_cleanup();
    double max = -1;
    int bin = -1;
    int nyq_limit = samplesPerChannel/2;
    for(int i = 0; i < nyq_limit; i++){
        double current = sqrt(pow(out[i][REAL], 2) + pow(out[i][IMAG], 2))*2;
        if(current > max){
            max = current;
            bin = i;
        }
    }
    free(out);
    this->calculatedFrequency = bin*resolution;
    return this->calculatedFrequency;
}

QString AudioAnalyzer::getNoteName(){
    int key = keyFromFreq(getFrequency());
    if(key == -1) return " ";
    return notes[((keyFromFreq(getFrequency())-1)+9)%12];
}

int AudioAnalyzer::keyFromFreq(double freq){
    double a = 440.00;
    if(freq < 27.5 || freq > 4186.009)
        return -1;
    return round(12*log2(freq/a)+49);
}
