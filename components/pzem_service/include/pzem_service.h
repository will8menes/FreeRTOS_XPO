#ifndef PZEM_SERVICE_H_
#define PZEM_SERVICE_H_

#define PZEM_COMMS_ALARM 0x00CE
#define PZEM_POWER_ALARM 0xFFFF

float PZEM_voltage(uint16_t addr);
float PZEM_current(uint16_t addr);
float PZEM_power(uint16_t addr);
float PZEM_frequency(uint16_t addr);
float PZEM_pf(uint16_t addr);
float PZEM_energy(uint16_t addr);

bool PZEM_setAddress(uint8_t addr);

uint8_t PZEM_getAddress();

uint8_t PZEM_readAddress(bool update);

uint8_t _addr;   // Device address

bool _isConnected; // Flag set on successful communication


struct pzem_values {
    float voltage;
    float current;
    float power;
    float energy;
    float frequency;
    float pf;
    uint16_t alarms;
}  _currentValues; // Measured values

struct pzem_value_variation {
    float voltage;
    float current;
    float power;
    float energy;
    float frequency;
    float pf;
    uint16_t alarms;
}; // Measured values

void PZEM_init();

bool PZEM_updateValues(uint16_t slave_address);   // Get most up to date values from device registers and cache them

int PZEM_receive(uint8_t *resp, uint16_t size); // Receive len bytes into a buffer

bool PZEM_sendCmd8(uint8_t cmd, uint16_t rAddr, uint16_t val, bool check, uint16_t slave_addr); // Send 8 byte command

void PZEM_setCRC(uint8_t *buf, uint16_t len);           // Set the CRC for a buffer

bool PZEM_checkCRC(const uint8_t *buf, uint16_t len);   // Check CRC of buffer

uint16_t PZEM_CRC16(const uint8_t *data, uint16_t len); // Calculate CRC of buffer

bool PZEM_setPowerAlarm(uint16_t watts, uint16_t addr);

bool PZEM_getPowerAlarm(uint16_t addr);

bool PZEM_resetEnergy(uint16_t addr);

uint16_t PZEM_readPowerTh(uint16_t addr);

#endif
