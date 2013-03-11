#ifndef DYNAMICTRACK_H
#define DYNAMICTRACK_H

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

class cDynamicTrack {
public:
    cDynamicTrack(char *aPort, unsigned aBaudRate) {
        vn200_connect(&VectorNav200, aPort, aBaudRate);
    }

    ~cDynamicTrack() {
        ParameterList.clear();
        vn200_disconnect(&VectorNav200);
    }

    nDTStatus::eDTStatus AddParameter(std::string &aParameterName, cDTParameter *aParameter) {
        do {
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
            return nDTStatus::eUnknownParameter;
        } while (0);

        ParameterList.push_back(aParameter);
        return nDTStatus::eSuccess;
    }

    void RemoveParameter(cDTParameter *aDTParamter) {
        ParameterList.remove(aDTParamter);
    }

    void CaptureDeviceParameters() {
        Vn200CompositeData currentData;
        vn200_getCurrentAsyncData(&VectorNav200, &currentData);

        std::list<cDTParameter *>::iterator parameter = ParameterList.begin();
        for (; ParameterList.end() != parameter; parameter++) {
            (*parameter)->LoadData((void *)&currentData);
        }
    }

protected:
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
