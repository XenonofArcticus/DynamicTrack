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
        // Capture the data from the last device sync in the DTs shadow area.
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

