#include "startupcontroller.h"
#include "AudioFile.h"
#include <fftw3.h>
#include <QUrl>
#include "audioanalyzer.h"
#include <QTimer>

#define REAL 0
#define IMAG 1

StartupController::StartupController(QObject *parent)
    : QObject(parent)
    , inputDevices(QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    , selectedInputDevice(QAudioDeviceInfo::defaultInputDevice())
{
    for(int i = 0; i < inputDevices.length(); i++){
        if(QString::compare(inputDevices[i].deviceName(), selectedInputDevice.deviceName())){
            selectedInputDeviceIndex = i;
            break;
        }
    }
}

void StartupController::read(const QString &fileName)
{
    AudioFile<double> audioFile;
    audioFile.load(QUrl(fileName).toLocalFile().toStdString());
    int nsamples = audioFile.getNumSamplesPerChannel();
    fftw_complex spectrum[nsamples];
    for(int i = 0; i < nsamples; i++){
        spectrum[i][REAL] = audioFile.samples[0][i];
        spectrum[i][IMAG] = 0;
    }

    AudioAnalyzer analyzer(spectrum, audioFile.getLengthInSeconds(), audioFile.getSampleRate(), audioFile.getNumSamplesPerChannel());
    setFrequency(QString::number(analyzer.getFrequency()));
    setNote(analyzer.getNoteName());
}

void StartupController::setSelectedInputDevice(int index){
    if(index >= inputDevices.length()) return;
    this->selectedInputDeviceIndex = index;
    this->selectedInputDevice = inputDevices[index];
}

void StartupController::startRecording()
{
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(44100);
    format.setChannelCount(1);
    tmpFile.open( QIODevice::ReadWrite | QIODevice::Truncate );
    this->audioInput = new QAudioInput(selectedInputDevice, format, this);
    QTimer::singleShot(3000, this, SLOT(stopRecording()));
    audioInput->start(&tmpFile);
}

void StartupController::stopRecording()
{
    double length = audioInput->elapsedUSecs()/1000000.0;
    int sampleRate = format.sampleRate();
    int samplesPerChannel = tmpFile.size();
    QByteArray data = tmpFile.data();
    fftw_complex* spectrum = (fftw_complex*)malloc(samplesPerChannel*sizeof(fftw_complex));
    for(int i = 0; i < samplesPerChannel; i++){
        spectrum[i][REAL] = data[i] / 32768.;
        spectrum[i][IMAG] = 0;
    }
    AudioAnalyzer analyzer(spectrum, length, sampleRate, samplesPerChannel);
    setFrequency(QString::number(analyzer.getFrequency()));
    setNote(analyzer.getNoteName());
    audioInput->stop();
    data.clear();
    free(spectrum);
    tmpFile.close();
    audioInput->start();
    delete audioInput;
    startRecording();
}
