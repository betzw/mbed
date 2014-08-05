/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
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
    int8_t getInt8AtIndex(int index);    
};

#endif // __PAYLOAD_H__