#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GpsModule {
private:
  TinyGPSPlus gps;
  HardwareSerial* gpsSerial;

  int rxPin;
  int txPin;
  double latitude;
  double longitude;
  int satellites;
  bool hasValidLocation;

public:
  GpsModule(HardwareSerial* serialPort, int rx, int tx)
    : gpsSerial(serialPort),
      rxPin(rx),
      txPin(tx),
      latitude(0.0),
      longitude(0.0),
      satellites(0),
      hasValidLocation(false) {}

  void begin() {
    gpsSerial->begin(9600, SERIAL_8N1, rxPin, txPin);
  }

  void update() {
    while (gpsSerial->available()) {
      gps.encode(gpsSerial->read());
    }

    satellites = gps.satellites.value();

    if (gps.location.isValid()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      hasValidLocation = true;
    }
  }

  bool hasLocation() {
    return hasValidLocation;
  }

  double getLatitude() {
    return latitude;
  }

  double getLongitude() {
    return longitude;
  }

  int getSatellites() {
    return satellites;
  }

  void printStatus() {
    Serial.print("Satellites: ");
    Serial.println(satellites);

    if (hasValidLocation) {
      Serial.print("Latitude: ");
      Serial.println(latitude, 6);

      Serial.print("Longitude: ");
      Serial.println(longitude, 6);
    } else {
      Serial.println("No valid GPS location yet");
    }

    Serial.println();
  }
};

#endif
