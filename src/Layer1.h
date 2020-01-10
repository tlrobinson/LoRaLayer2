
#include <unistd.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define DEBUG 0
#define SHA1_LENGTH 40
#define ADDR_LENGTH 6 

#define LORA // to use Layer1_LoRa.cpp
//#define SIM // to use Layer1_Sim.cpp

#ifdef LORA
#include <Arduino.h>

// for solar-powered module use these settings:
//
//   LoRa.setPins(2, 5, 16);
//   LoRa.setSPIFrequency(100E3);
//
// for TTGO LoRa32 V1.0 use these settings:
//
//   LoRa.setPins(18, 14, 26);
//   LoRa.setSPIFrequency(100E3);
//
// for TTGO LoRa32 V2.1 use these settings:
//
//   LoRa.setPins(18, 23, 26);
//   LoRa.setSPIFrequency(100E3);
//

class Layer1Class {
public:
    Layer1Class();
    int debug_printf(const char* format, ...);
    uint8_t* charToHex(char* charString);
    int setLocalAddress(char* macString);
    uint8_t* localAddress();
    int getTime();
    int loraInitialized();
    int init();
    int send_packet(char* data, int len);

private:
    uint8_t hex_digit(char ch);
    int isHashNew(char incoming[SHA1_LENGTH]);
    static void onReceive(int packetSize);

private:
    uint8_t _localAddress[ADDR_LENGTH];
    uint8_t _hashTable[256][SHA1_LENGTH];
    int _hashEntry;
    int _loraInitialized;
};

extern Layer1Class Layer1;

#endif

#ifdef SIM
typedef struct _serial {
  int (*printf)(const char*, ...);
} serial;

extern serial Serial;

// you must declare these in your router
int setup(); // called once on startup
int loop(); // called once per event loop iteration

class Layer1Class {
public:
    Layer1Class();
    int nsleep(unsigned int secs, useconds_t usecs);
    int simulationTime(int realTime);
    int setTimeDistortion(float newDistortion);
    int getTime();
    int debug_printf(const char* format, ...);
    //int print_err(const char* format, ...);
    int setLocalAddress(char* macString);
    uint8_t* localAddress();
    int setNodeID(char* newID);
    //char* nodeID();
    int parse_metadata(char* data, uint8_t len);
    int begin_packet();
    int send_packet(char* data, uint8_t len);

private:
    uint8_t hex_digit(char ch);
    int isHashNew(uint8_t hash[SHA1_LENGTH]);
    float timeDistortion();

private:
    int _transmitting;
    uint8_t _localAddress[ADDR_LENGTH];
    char* _nodeID;
    uint8_t _hashTable[256][SHA1_LENGTH];
    uint8_t _hashEntry;
    float _timeDistortion;

};

extern Layer1Class Layer1;
#endif
