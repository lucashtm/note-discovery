#ifndef AUDIOANALYZER_H
#define AUDIOANALYZER_H

#include <QObject>
#include <fftw3.h>

class AudioAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit AudioAnalyzer(fftw_complex* spectrum, double length, int sampleRate, int samplesPerChannel, QObject *parent = nullptr);
    double getFrequency();
    QString getNoteName();

private:
    const QString notes[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    fftw_complex* spectrum;
    double calculatedFrequency = -1;
    double length;
    int sampleRate;
    double resolution;
    int samplesPerChannel;

    QString noteFromFreq(double freq);
    int keyFromFreq(double freq);
};

#endif // AUDIOANALYZER_H
