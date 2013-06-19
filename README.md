DynamicTrack
============

<a href="https://github.com/XenonofArcticus/DynamicTrack">DynamicTrack</a> is an Open Source library for interfacing to devices that provide dyanmic position, orientation and/or other data for one or more entities like GPSes, INSes (Inertial navigation Systems), gyros, compasses, or datastreams derived from real-time tracking like <a href="http://en.wikipedia.org/wiki/Automatic_dependent_surveillance-broadcast">ADS-B</a>, <a href="http://en.wikipedia.org/wiki/Automatic_Identification_System">AIS</a>, <a href="http://en.wikipedia.org/wiki/Blue_Force_Tracking">Blue Force</a>, <a href="http://www.aprs.org/">APRS</a> or other dynamic data sources.

Currently device support is built in for the <a href="http://www.vectornav.com/products/vn200-rug?id=80">VectorNav VN-200</a> on Linux or Windows. Support for additional devices and for querying devices and device datatypes is planned and contributions are welcomed.

DynamicTrack exposes a very simple, extensible string-based interface for connecting to available devices and available data channels provided by those devices:

<code>
    cDynamicTrack dynamicTrack("COM9", 230400);
    cDTPODParameter<double> latitudeParam("Pos.Latitude", dynamicTrack);
    cDTPODParameter<double> yawParam("Orient.Yaw", dynamicTrack);
    cDTPODParameter<double> pitchParam("Orient.Pitch", dynamicTrack);
    cDTPODParameter<double> rollParam("Orient.Roll", dynamicTrack);
    cDTPODParameter<unsigned> gpsFixParam("Orient.GPS.Fix", dynamicTrack);
</code>

DynamicTrack was commissioned by a confidential aircraft navigation startup and is provided under the MIT License ( http://opensource.org/licenses/MIT ).

GPS/INS/Navigation/Communication
