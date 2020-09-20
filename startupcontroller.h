#ifndef STARTUPCONTROLLER_H
#define STARTUPCONTROLLER_H

#include <QObject>
#include <QAudioDeviceInfo>
#include "wavfile.h"
#include <QAudioInput>
#include <QBuffer>

class StartupController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString frequency MEMBER frequencyText NOTIFY frequencyChanged)
    Q_PROPERTY(QString note MEMBER noteName NOTIFY noteChanged)
    Q_PROPERTY(QList<QString> inputDevices READ inputDevicesNames NOTIFY inputDevicesChanged)
    Q_PROPERTY(int selectedInputDevice MEMBER selectedInputDeviceIndex NOTIFY selectedInputDeviceChanged)
public:
    explicit StartupController(QObject *parent = nullptr);
    QString frequencyText;
    QString noteName;
    Q_INVOKABLE void read(const QString &fileName);
    Q_INVOKABLE void setSelectedInputDevice(int index);
    Q_INVOKABLE void startRecording();
Q_SIGNALS:
    void frequencyChanged();
    void noteChanged();
    void inputDevicesChanged();
    void selectedInputDeviceChanged();
public Q_SLOTS:
    void stopRecording();
private:
    QList<QAudioDeviceInfo> inputDevices;
    QAudioDeviceInfo selectedInputDevice;
    QAudioInput* audioInput;
    QAudioFormat format;
    QBuffer tmpFile;
    int selectedInputDeviceIndex;

    QList<QString> inputDevicesNames(){
        QList<QString> names;
        foreach(QAudioDeviceInfo device, inputDevices){
            names.append(device.deviceName());
        }
        return names;
    }

    void setFrequency(QString frequencyText){
        if(frequencyText != this->frequencyText){
            this->frequencyText = frequencyText;
            Q_EMIT frequencyChanged();
        }
    };

    void setNote(QString noteName){
        if(noteName != this->noteName){
            this->noteName = noteName;
            Q_EMIT noteChanged();
        }
    };
};

#endif // STARTUPCONTROLLER_H
