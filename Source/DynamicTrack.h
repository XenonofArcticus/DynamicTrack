
/*
The MIT License (MIT)

Copyright (c) 2013 AlphaPixel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef DYNAMICTRACK_H
#define DYNAMICTRACK_H

#include <fstream>

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/signals2.hpp>

#include <iostream>

#include <vectornav.h>

class cDynamicTrack;

namespace nDTStatus {
enum eDTStatus {
    eSuccess = 0,
    eDeviceFailure,
    eDeviceMissing,
    eUnknownParameter,
    eValueInvalid,
    eValueOutofRange,
};
} // nDTStatus

class cDeviceDataInterface {
public:
    virtual void RetrieveData(void *aDataStructure, unsigned aFieldByteOffset,
                              double &aData, nDTStatus::eDTStatus &aDTStatus) = 0;
    virtual void RetrieveData(void *aDataStructure, unsigned aFieldByteOffset,
                              std::string &aString, nDTStatus::eDTStatus &aDTStatus) = 0;
};

template<typename tType> class cRetrieveDeviceType : public cDeviceDataInterface {
    public:
        void RetrieveData(void *aDataStructure, unsigned aFieldByteOffset,
                          double &aData, nDTStatus::eDTStatus &aDTStatus) {
            aData = *(tType *)&((char *)aDataStructure)[aFieldByteOffset];
            aDTStatus = nDTStatus::eSuccess;
        }
        void RetrieveData(void *aDataStructure, unsigned aFieldByteOffset,
                          std::string &aData, nDTStatus::eDTStatus &aDTStatus) {
            aData = boost::lexical_cast<std::string>(*(tType *)&(((char *)aDataStructure)[aFieldByteOffset]));
            aDTStatus = nDTStatus::eSuccess;
        }
};

class cRetrieveDeviceString : public cDeviceDataInterface {
public:
    void RetrieveData(void *aDataStructure, unsigned aFieldByteOffset,
                      double &aData, nDTStatus::eDTStatus &aDTStatus) {
        aData = atof(&((char *)aDataStructure)[aFieldByteOffset]);
        aDTStatus = nDTStatus::eSuccess;
    }
    void RetrieveData(void *aDataStructure, unsigned aFieldByteOffset,
                      std::string &aData, nDTStatus::eDTStatus &aDTStatus) {
        aData = &((char *)aDataStructure)[aFieldByteOffset];
        aDTStatus = nDTStatus::eSuccess;
    }
};

class cDTParameter {
public:
    friend class cDynamicTrack;
    cDTParameter(cDynamicTrack *aDynamicTrack);

    void AlignParameter(cDeviceDataInterface *aDataInterface, unsigned aFieldOffset) {
        DataFieldInterface = aDataInterface;
        DataFieldOffset = aFieldOffset;
    }

    virtual nDTStatus::eDTStatus GetValue(char &aValue) = 0;
    virtual nDTStatus::eDTStatus GetValue(unsigned char &aValue) = 0;
    virtual nDTStatus::eDTStatus GetValue(double &aValue) = 0;
    virtual nDTStatus::eDTStatus GetValue(float &aValue) = 0;
    virtual nDTStatus::eDTStatus GetValue(int &aValue) = 0;
    virtual nDTStatus::eDTStatus GetValue(unsigned &aValue) = 0;
    virtual nDTStatus::eDTStatus GetValue(std::string &aString) = 0;
    virtual void LoadData(void *aProprietaryDataStructure) = 0;

protected:
    cDeviceDataInterface *DataFieldInterface;
    unsigned DataFieldOffset;
    cDynamicTrack *DynamicTrack;
    nDTStatus::eDTStatus Status;
};

class cDTStringParameter : public cDTParameter {
public:

    cDTStringParameter(std::string aParameterName, cDynamicTrack &aDynamicTrack);
    ~cDTStringParameter();

    nDTStatus::eDTStatus GetValue(std::string &aString) {
        aString = DeviceString;
        return Status;
    }

    // MKM March 2012: TODO - add bounds checking
    nDTStatus::eDTStatus GetValue(char &aValue) {
        aValue = (char)atoi(DeviceString.c_str());
        return Status;
    }
    nDTStatus::eDTStatus GetValue(unsigned char &aValue) {
        aValue = (unsigned char)atoi(DeviceString.c_str());
        return Status;
    }
    nDTStatus::eDTStatus GetValue(short &aValue) {
        aValue = (short)atoi(DeviceString.c_str());
        return Status;
    }
    nDTStatus::eDTStatus GetValue(unsigned short &aValue) {
        aValue = (unsigned short)atoi(DeviceString.c_str());
        return Status;
    }
    nDTStatus::eDTStatus GetValue(double &aValue) {
        aValue = atof(DeviceString.c_str());
        return Status;
    }
    nDTStatus::eDTStatus GetValue(float &aValue) {
        aValue = (float)atof(DeviceString.c_str());
        return Status;
    }
    nDTStatus::eDTStatus GetValue(int &aValue) {
        aValue = (int)atoi(DeviceString.c_str());
        return Status;
    }
    nDTStatus::eDTStatus GetValue(unsigned &aValue) {
        aValue = (unsigned)atoi(DeviceString.c_str());
        return Status;
    }

    void LoadData(void *aProprietaryDataStructure) {
        if (DataFieldInterface) {
            DataFieldInterface->RetrieveData(aProprietaryDataStructure, DataFieldOffset,
                                             DeviceString, Status);
        }
    }

protected:
    std::string DeviceString;
};

template <typename tType> class cDTPODParameter : public cDTParameter {
public:
    cDTPODParameter(std::string aParameterName, cDynamicTrack &aDynamicTrack)
        : cDTParameter(&aDynamicTrack) {
        DynamicTrack->AddParameter(aParameterName, this);
    }

    virtual ~cDTPODParameter() {
        DynamicTrack->RemoveParameter(this);
    }

    nDTStatus::eDTStatus GetValue(std::string &aValue) {
        aValue = boost::lexical_cast<std::string>(DeviceValue);
        return Status;
    }

    // MKM March 2012: TODO - add bounds checking
    nDTStatus::eDTStatus GetValue(char &aValue) {
        aValue = (char)DeviceValue;
        return Status;
    }
    nDTStatus::eDTStatus GetValue(unsigned char &aValue) {
        aValue = (unsigned char)DeviceValue;
        return Status;
    }
    nDTStatus::eDTStatus GetValue(short &aValue) {
        aValue = (short)DeviceValue;
        return Status;
    }
    nDTStatus::eDTStatus GetValue(unsigned short &aValue) {
        aValue = (unsigned short)DeviceValue;
        return Status;
    }
    nDTStatus::eDTStatus GetValue(double &aValue) {
        aValue = (double)DeviceValue;
        return Status;
    }
    nDTStatus::eDTStatus GetValue(float &aValue) {
        aValue = (float)DeviceValue;
        return Status;
    }
    nDTStatus::eDTStatus GetValue(int &aValue) {
        aValue = (int)DeviceValue;
        return Status;
    }
    nDTStatus::eDTStatus GetValue(unsigned &aValue) {
        aValue = (unsigned)DeviceValue;
        return Status;
    }

    void LoadData(void *aProprietaryDataStructure) {
        if (DataFieldInterface) {
            double dataValue;
            DataFieldInterface->RetrieveData(aProprietaryDataStructure, DataFieldOffset,
                                             dataValue, Status);
            DeviceValue = (tType)dataValue;
        }
    }
protected:
    tType DeviceValue;
};

class cINSLoggedData {
public:
    cINSLoggedData() {}
    cINSLoggedData(Vn200CompositeData &aAllData, double aElapsedTime,
                   bool aLastDataPoint = false)
        : ElapsedTime(aElapsedTime),
          LastDataPoint(aLastDataPoint),
          LatitudeLongitudeAltitude(aAllData.latitudeLongitudeAltitude),
          NEDVelocity(aAllData.velocity),
          YawPitchRoll(aAllData.ypr) {
    }

    double GetElapsedTime() {
        return ElapsedTime;
    }

    void GetINSData(Vn200CompositeData &aDataStruct) {
        aDataStruct.latitudeLongitudeAltitude = LatitudeLongitudeAltitude;
        aDataStruct.velocity = NEDVelocity;
        aDataStruct.ypr = YawPitchRoll;
    }

    bool Interpolate(cINSLoggedData &aLastCapture, double aElapsedTime) {
        if (aElapsedTime < ElapsedTime || ElapsedTime > aLastCapture.ElapsedTime) {
            return false;
        }
        double slope = (aElapsedTime - ElapsedTime) / (aLastCapture.ElapsedTime - ElapsedTime);

        LatitudeLongitudeAltitude.c0 = InterpolateCalculation(slope, LatitudeLongitudeAltitude.c0, aLastCapture.LatitudeLongitudeAltitude.c0);
        LatitudeLongitudeAltitude.c1 = InterpolateCalculation(slope, LatitudeLongitudeAltitude.c1, aLastCapture.LatitudeLongitudeAltitude.c1);
        LatitudeLongitudeAltitude.c2 = InterpolateCalculation(slope, LatitudeLongitudeAltitude.c2, aLastCapture.LatitudeLongitudeAltitude.c2);
        NEDVelocity.c0 = InterpolateCalculation(slope, NEDVelocity.c0, aLastCapture.NEDVelocity.c0);
        NEDVelocity.c1 = InterpolateCalculation(slope, NEDVelocity.c1, aLastCapture.NEDVelocity.c1);
        NEDVelocity.c2 = InterpolateCalculation(slope, NEDVelocity.c2, aLastCapture.NEDVelocity.c2);
        YawPitchRoll.yaw = InterpolateCalculation(slope, YawPitchRoll.yaw, aLastCapture.YawPitchRoll.yaw);
        YawPitchRoll.pitch = InterpolateCalculation(slope, YawPitchRoll.pitch, aLastCapture.YawPitchRoll.pitch);
        YawPitchRoll.roll = InterpolateCalculation(slope, YawPitchRoll.roll, aLastCapture.YawPitchRoll.roll);

        return true;
    }

    bool IsLastDatapoint() {
        return LastDataPoint;
    }
    void SetLastDatapoint() {
        LastDataPoint = true;
    }

protected:
    friend class boost::serialization::access;

    double InterpolateCalculation(double aSlope, double aFirstValue, double aSecondValue) {
        return (aSlope * (aSecondValue - aFirstValue)) + aFirstValue;
    }

    template <class Archive> void serialize(Archive &aArchive, const unsigned int) {
        aArchive & LastDataPoint;
        aArchive & ElapsedTime;
        aArchive & LatitudeLongitudeAltitude.c0;
        aArchive & LatitudeLongitudeAltitude.c1;
        aArchive & LatitudeLongitudeAltitude.c2;
        aArchive & NEDVelocity.c0;
        aArchive & NEDVelocity.c1;
        aArchive & NEDVelocity.c2;
        aArchive & YawPitchRoll.yaw;
        aArchive & YawPitchRoll.pitch;
        aArchive & YawPitchRoll.roll;
    }

    bool LastDataPoint;
    VnYpr YawPitchRoll;
    VnVector3 LatitudeLongitudeAltitude;
    VnVector3 NEDVelocity;
    double ElapsedTime;
};

class cDynamicTrack {
public:
    cDynamicTrack(const char *aPort, unsigned aBaudRate)
        : ArchiveInput(0), ArchiveOutput(0), DataLogInEn(false),
          DataLogOutEn(false), LastInputCaptureValid (false) {
        ValidConnection = VNERR_NO_ERROR == vn200_connect(&VectorNav200, aPort, aBaudRate);
    }

    ~cDynamicTrack() {
        ParameterList.clear();
        vn200_disconnect(&VectorNav200);
        DisableInputLogging();
        DisableOutputLogging();
    }

    nDTStatus::eDTStatus AddParameter(std::string &aParameterName, cDTParameter *aParameter) {
        while (1) {
            if ("Pos.Altitude" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, latitudeLongitudeAltitude.c2));
                break;
            }
            if ("Pos.Latitude" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, latitudeLongitudeAltitude.c0));
                break;
            }
            if ("Pos.Longitude" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, latitudeLongitudeAltitude.c1));
                break;
            }
            if ("Orient.Yaw" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, ypr.yaw));
                break;
            }
            if ("Orient.Pitch" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, ypr.pitch));
                break;
            }
            if ("Orient.Roll" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, ypr.roll));
                break;
            }
            if ("Orient.GPS.Fix" == aParameterName) {
                aParameter->AlignParameter(&RetrieveUCharField, offsetof(Vn200CompositeData, gpsFix));
                break;
            }
            if ("Motion.Velocity.NEDVelN" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, velocity.c0));
                break;
            }
            if ("Motion.Velocity.NEDVelE" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, velocity.c1));
                break;
            }
            if ("Motion.Velocity.NEDVelD" == aParameterName) {
                aParameter->AlignParameter(&RetrieveDoubleField, offsetof(Vn200CompositeData, velocity.c2));
                break;
            }
            return nDTStatus::eUnknownParameter;
        };

        ParameterList.push_back(aParameter);

        if (!ValidConnection) {
            return nDTStatus::eDeviceMissing;
        }
        return nDTStatus::eSuccess;
    }

    void DisableInputLogging() {
        if (ArchiveInput) {
            delete ArchiveInput;
            ArchiveInput = 0;
            InputStream.close();
        }
        DataLogInEn = false;
    }
    bool SetInputLogging(std::string &aFilePath) {
        if (DataLogOutEn) {
            return false;
        }

        InputStream.open(aFilePath.c_str());
        if (InputStream.is_open()) {
            ArchiveInput = new boost::archive::text_iarchive(InputStream);
            ValidConnection = true;
            DataLogInEn = true;
            ElapsedTimer.restart();
            LastInputCaptureValid = false;
            return true;
        }
        return false;
    }

    void DisableOutputLogging() {
        if (ArchiveOutput) {
            const cINSLoggedData endSentinel(CurrentData, ElapsedTimer.elapsed(), true);
            *ArchiveOutput << endSentinel;
            delete ArchiveOutput;
            ArchiveOutput = 0;
            OutputStream.close();
        }
        DataLogOutEn = false;
    }
    bool SetOutputLogging(std::string &aFilePath) {
        if (DataLogInEn) {
            return false;
        }

        OutputStream.open(aFilePath.c_str());
        if (OutputStream.is_open()) {
            ArchiveOutput = new boost::archive::text_oarchive(OutputStream);
            DataLogOutEn = true;
            ElapsedTimer.restart();
            return true;
        }
        return false;
    }

    void RemoveParameter(cDTParameter *aDTParamter) {
        ParameterList.remove(aDTParamter);
    }

    bool CaptureDeviceParameters() {
        if (DataLogInEn) {
            // New data is input via a log file;
            const double elapsedTime = ElapsedTimer.elapsed();

            cINSLoggedData stashedCapture;

            if (!LastInputCaptureValid) {
                *ArchiveInput >> stashedCapture;

                if (stashedCapture.GetElapsedTime() > elapsedTime ||
                        stashedCapture.IsLastDatapoint()) {
                    // The current elapsed time is before the first data
                    //  capture occurred, thus return the first capture.
                    stashedCapture.GetINSData(CurrentData);
                    std::list<cDTParameter *>::iterator parameter = ParameterList.begin();
                    for (; ParameterList.end() != parameter; parameter++) {
                        (*parameter)->LoadData((void *)&CurrentData);
                    }

                    LastInputCapture = stashedCapture;
                    LastInputCaptureValid = true;
                    return false;
                }
                *ArchiveInput >> LastInputCapture;
                LastInputCaptureValid = true;
            }
            else if (LastInputCapture.GetElapsedTime() < elapsedTime &&
                     !LastInputCapture.IsLastDatapoint()) {
                stashedCapture = LastInputCapture;
                *ArchiveInput >> LastInputCapture;
            }
            else {
                // The current elapsed time is before the first data
                //  capture occurred, or the end of the log has been
				//	reached. Use the last known good data.
                LastInputCapture.GetINSData(CurrentData);
                std::list<cDTParameter *>::iterator parameter = ParameterList.begin();
                for (; ParameterList.end() != parameter; parameter++) {
                    (*parameter)->LoadData((void *)&CurrentData);
                }
                return false;
            }

            while (LastInputCapture.GetElapsedTime() < elapsedTime) {
                if (LastInputCapture.IsLastDatapoint()) {
                    // The end of the stream was reached, return the last data captured
                    LastInputCapture.GetINSData(CurrentData);
                    std::list<cDTParameter *>::iterator parameter = ParameterList.begin();
                    for (; ParameterList.end() != parameter; parameter++) {
                        (*parameter)->LoadData((void *)&CurrentData);
                    }
                    return false;
                }
                stashedCapture = LastInputCapture;
                *ArchiveInput >> LastInputCapture;
            }

            // Stashed capture should now point to data prior to elapsedTime,
            //  and LastInputCapture should point to after. Allowing the
            //  actual to be interpolated, assuming an adequate sample rate.
            stashedCapture.Interpolate(LastInputCapture, elapsedTime);

            stashedCapture.GetINSData(CurrentData);
            std::list<cDTParameter *>::iterator parameter = ParameterList.begin();
            for (; ParameterList.end() != parameter; parameter++) {
                (*parameter)->LoadData((void *)&CurrentData);
            }
        }

        else if (ValidConnection) {
            // New data is retrieved from the INS device.
            vn200_getCurrentAsyncData(&VectorNav200, &CurrentData);

            std::list<cDTParameter *>::iterator parameter = ParameterList.begin();
            for (; ParameterList.end() != parameter; parameter++) {
                (*parameter)->LoadData((void *)&CurrentData);
            }

            if (DataLogOutEn) {
                const cINSLoggedData loggedData(CurrentData, ElapsedTimer.elapsed());
                *ArchiveOutput << loggedData;
            }
        }
        return true;
    }

    bool ConnectionValid () {
        return ValidConnection;
    }

protected:
    bool ValidConnection;
    bool DataLogInEn;
    bool DataLogOutEn;
    bool LastInputCaptureValid;

    boost::archive::text_iarchive *ArchiveInput;
    boost::archive::text_oarchive *ArchiveOutput;
    Vn200CompositeData CurrentData;
    boost::timer ElapsedTimer;
    std::ifstream InputStream;
    cINSLoggedData LastInputCapture;
    std::ofstream OutputStream;

    Vn200 VectorNav200;
    std::list<cDTParameter *> ParameterList;

    // Templates to support each field type in the devices proprietarty data structure.
    cRetrieveDeviceType<char> RetrieveCharField;
    cRetrieveDeviceType<unsigned char> RetrieveUCharField;
    cRetrieveDeviceType<double> RetrieveDoubleField;
    cRetrieveDeviceType<float> RetrieveFloatField;
    cRetrieveDeviceType<int> RetrieveIntField;
    cRetrieveDeviceType<unsigned> RetrieveUnsignedField;
};


#endif // DYNAMICTRACK_H
