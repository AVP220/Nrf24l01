#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

struct DeviceStatus {
  bool connect;
  int battery;
  bool flag;
  int ledsNum;
  int myColor[3];
  char* address;
};

extern DeviceStatus MyData[30];

#endif // DEVICESTATUS_H
