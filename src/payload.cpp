#include "globals.h"
#include "payload.h"

PayloadConvert::PayloadConvert(uint8_t size) {
  buffer = (uint8_t *)malloc(size);
  cursor = 0;
}

PayloadConvert::~PayloadConvert(void) { free(buffer); }

void PayloadConvert::reset(void) { cursor = 0; }

uint8_t PayloadConvert::getSize(void) { return cursor; }

uint8_t *PayloadConvert::getBuffer(void) { return buffer; }



/* ---------------- packed format with LoRa serialization Encoder ----------
 */
// derived from
// https://github.com/thesolarnomad/lora-serialization/blob/master/src/LoraEncoder.cpp



void PayloadConvert::addByte(uint8_t value) { writeUint8(value); }

// wifi & Ble
void PayloadConvert::addCount(uint16_t value, uint8_t snifftype) {
  writeUint16(value);
}
//startbutton
void PayloadConvert::addStartButttonCount(uint16_t value, uint16_t interval){
   writeUint16(value);
   writeUint16(interval);
}
// beacon
void PayloadConvert::addAlarm(int8_t rssi, uint8_t msg) {
  writeUint8(rssi);
  writeUint8(msg);
}

void PayloadConvert::addVoltage(uint16_t value) { writeUint16(value); }

void PayloadConvert::addConfig(configData_t value) {
  writeUint8(value.loradr);
  writeUint8(value.txpower);
  writeUint16(value.rssilimit);
  writeUint8(value.sendcycle);
  writeUint8(value.wifichancycle);
  writeUint8(value.blescantime);
  writeUint8(value.rgblum);
  writeBitmap(value.adrmode ? true : false, value.screensaver ? true : false,
              value.screenon ? true : false, value.countermode ? true : false,
              value.blescan ? true : false, value.wifiant ? true : false,
              value.vendorfilter ? true : false,
              value.monitormode ? true : false);
  writeBitmap(value.payloadmask && GPS_DATA ? true : false,
              value.payloadmask && ALARM_DATA ? true : false,
              value.payloadmask && MEMS_DATA ? true : false,
              value.payloadmask && COUNT_DATA ? true : false,
              value.payloadmask && SENSOR1_DATA ? true : false,
              value.payloadmask && SENSOR2_DATA ? true : false,
              value.payloadmask && SENSOR3_DATA ? true : false,
              value.payloadmask && BATT_DATA ? true : false);
  writeVersion(value.version);
}

void PayloadConvert::addStatus(uint16_t voltage, uint64_t uptime, float cputemp,
                               uint32_t mem, uint8_t reset1, uint8_t reset2) {
  writeUint16(voltage);
  writeUptime(uptime);
  writeUint8((byte)cputemp);
  writeUint32(mem);
  writeUint8(reset1);
  writeUint8(reset2);
}

void PayloadConvert::addGPS(gpsStatus_t value) {
#if (HAS_GPS)
  writeLatLng(value.latitude, value.longitude);
  writeUint8(value.satellites);
  writeUint16(value.hdop);
  writeUint16(value.altitude);
#endif
}

void PayloadConvert::addSensor(uint8_t buf[]) {
#if (HAS_SENSORS)
  uint8_t length = buf[0];
  memcpy(buffer, buf + 1, length);
  cursor += length; // length of buffer
#endif
}

void PayloadConvert::addBME(bmeStatus_t value) {
#if (HAS_BME)
  writeFloat(value.temperature);
  writePressure(value.pressure);
  writeUFloat(value.humidity);
  writeUFloat(value.iaq);
#endif
}

void PayloadConvert::addSDS(sdsStatus_t sds) {
#if (HAS_SDS011)
  writeUint16((uint16_t)(sds.pm10 * 10));
  writeUint16((uint16_t)(sds.pm25 * 10));
#endif // HAS_SDS011
}

void PayloadConvert::addButton(uint8_t value) {
#ifdef HAS_BUTTON
  writeUint8(value);
#endif
}

void PayloadConvert::addTime(time_t value) {
  uint32_t time = (uint32_t)value;
  writeUint32(time);
}

void PayloadConvert::uintToBytes(uint64_t value, uint8_t byteSize) {
  for (uint8_t x = 0; x < byteSize; x++) {
    byte next = 0;
    if (sizeof(value) > x) {
      next = static_cast<byte>((value >> (x * 8)) & 0xFF);
    }
    buffer[cursor] = next;
    ++cursor;
  }
}

void PayloadConvert::writeUptime(uint64_t uptime) { writeUint64(uptime); }

void PayloadConvert::writeVersion(char *version) {
  memcpy(buffer + cursor, version, 10);
  cursor += 10;
}

void PayloadConvert::writeLatLng(double latitude, double longitude) {
  // Tested to at least work with int32_t, which are processed correctly.
  writeUint32(latitude);
  writeUint32(longitude);
}

void PayloadConvert::writeUint64(uint64_t i) { uintToBytes(i, 8); }

void PayloadConvert::writeUint32(uint32_t i) { uintToBytes(i, 4); }

void PayloadConvert::writeUint16(uint16_t i) { uintToBytes(i, 2); }

void PayloadConvert::writeUint8(uint8_t i) { uintToBytes(i, 1); }

void PayloadConvert::writeUFloat(float value) { writeUint16(value * 100); }

void PayloadConvert::writePressure(float value) { writeUint16(value * 10); }

/**
 * Uses a 16bit two's complement with two decimals, so the range is
 * -327.68 to +327.67 degrees
 */
void PayloadConvert::writeFloat(float value) {
  int16_t t = (int16_t)(value * 100);
  if (value < 0) {
    t = ~-t;
    t = t + 1;
  }
  buffer[cursor++] = (byte)((t >> 8) & 0xFF);
  buffer[cursor++] = (byte)t & 0xFF;
}

void PayloadConvert::writeBitmap(bool a, bool b, bool c, bool d, bool e, bool f,
                                 bool g, bool h) {
  uint8_t bitmap = 0;
  // LSB first
  bitmap |= (a & 1) << 7;
  bitmap |= (b & 1) << 6;
  bitmap |= (c & 1) << 5;
  bitmap |= (d & 1) << 4;
  bitmap |= (e & 1) << 3;
  bitmap |= (f & 1) << 2;
  bitmap |= (g & 1) << 1;
  bitmap |= (h & 1) << 0;
  writeUint8(bitmap);
}




void PayloadConvert::addChars(char *string, int len) {
  for (int i = 0; i < len; i++)
    addByte(string[i]);
}
