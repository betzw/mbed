#include "mbed.h"

#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

class UnitPayload
{
    public:
    uint8_t length;
    uint8_t id;
    uint8_t *data;
        
    void set_length(uint8_t l) {
        length=l;   
    }

    void set_id(uint8_t i) {
        id=i;
    }

    void set_data(uint8_t* data1) {
        for(int j=0;j<length;j++)
        {
            data[j]=data1[j];
        }   
    }

    uint8_t get_length()  {
        return length;      
    }

    uint8_t get_id()  {
        return id;      
    }

    uint8_t* get_data() {
        return data;        
    }

};

class Payload {
    UnitPayload *payload;
    int stringLength;
    int payloadUnitCount;
    
public:
     Payload(const uint8_t *tokenString, uint8_t string_ength);
     Payload::Payload();
     uint8_t Payload::getPayloadUnitCount();
      
    uint8_t Payload::getIDAtIndex(int index);  
    uint8_t Payload::getLengthAtIndex(int index);   
    uint8_t* Payload::getDataAtIndex(int index);    
};

#endif // __PAYLOAD_H__