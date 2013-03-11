#include <iostream>

#include <DynamicTrack.h>

using namespace std;

int main() {
    // Create a connection to the device.
    cDynamicTrack dynamicTrack("COM9", 230400);

    // Setup parameters to track. The app can define the type for
    //  the local storage via the template. Modifiers (e.g. Degrees vs Radians)
    //  aren't yet supported.
    cDTPODParameter<double> latitudeParam("Pos.Latitude", dynamicTrack);

    cDTPODParameter<double> yawParam("Orient.Yaw", dynamicTrack);
    cDTPODParameter<int> yawIntParam("Orient.Yaw", dynamicTrack);

    cDTPODParameter<double> pitchParam("Orient.Pitch", dynamicTrack);
    cDTPODParameter<double> rollParam("Orient.Roll", dynamicTrack);
    cDTPODParameter<unsigned> bogusParam("Bogus", dynamicTrack);
    cDTPODParameter<unsigned> gpsFixParam("Orient.GPS.Fix", dynamicTrack);

    for (int i = 0; i < 256; i++) {
        // Capture the data from the last device sync in the DT shadow area.
        dynamicTrack.CaptureDeviceParameters();
        double latitude, ypr;
        int yawInt;
        unsigned char gpsFix;

        if (nDTStatus::eSuccess == latitudeParam.GetValue(latitude)) {
            cout << " Latitude: " << latitude;
        }
        if (nDTStatus::eSuccess == gpsFixParam.GetValue(gpsFix)) {
            cout << " GPS Fix: " << (int)gpsFix;
        }
        if (nDTStatus::eSuccess == yawParam.GetValue(ypr)) {
            cout << " Yaw: " << ypr;
        }
        if (nDTStatus::eSuccess == pitchParam.GetValue(ypr)) {
            cout << " Pitch: " << ypr;
        }
        if (nDTStatus::eSuccess == rollParam.GetValue(ypr)) {
            cout << " Roll: " << ypr;
        }

        // Values can be retrieved in a POD that doesn't match the
        //  local storage type.
        double bogus;
        if (nDTStatus::eSuccess == bogusParam.GetValue(bogus)) {
            cout << " Bogus: " << bogus;
        }
        if (nDTStatus::eSuccess == yawIntParam.GetValue(yawInt)) {
            cout << " Yaw Int: " << yawInt;
        }

        cout << endl;

        Sleep(5000);
    }

    cout << "Hello World!" << endl;
    return 0;
}

