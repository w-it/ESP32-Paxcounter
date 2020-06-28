#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_

#include "sds011read.h"



class PayloadConvert {

public:
  PayloadConvert(uint8_t size);
  ~PayloadConvert();

  void reset(void);
  uint8_t getSize(void);
  uint8_t *getBuffer(void);
  void addByte(uint8_t value);
  void addCount(uint16_t value, uint8_t sniffytpe);

  void addStartButttonCount(uint16_t value, uint16_t interval);
  
  void addConfig(configData_t value);
  void addStatus(uint16_t voltage, uint64_t uptime, float cputemp, uint32_t mem,
                 uint8_t reset1, uint8_t reset2);
  void addAlarm(int8_t rssi, uint8_t message);
  void addVoltage(uint16_t value);
  void addGPS(gpsStatus_t value);
  void addBME(bmeStatus_t value);
  void addButton(uint8_t value);
  void addSensor(uint8_t[]);
  void addTime(time_t value);
  void addSDS(sdsStatus_t value);
private:
  uint8_t *buffer;
  uint8_t cursor;
  void addChars( char* string, int len);
  void uintToBytes(uint64_t i, uint8_t byteSize);
  void writeUptime(uint64_t unixtime);
  void writeLatLng(double latitude, double longitude);
  void writeUint64(uint64_t i);
  void writeUint32(uint32_t i);
  void writeUint16(uint16_t i);
  void writeUint8(uint8_t i);
  void writeFloat(float value);
  void writeUFloat(float value);
  void writePressure(float value);
  void writeVersion(char *version);
  void writeBitmap(bool a, bool b, bool c, bool d, bool e, bool f, bool g,
                   bool h);

};

extern PayloadConvert payload;

#endif // _PAYLOAD_H_
