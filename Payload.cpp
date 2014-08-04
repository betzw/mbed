#include <Payload.h>   
   
   Payload::Payload() {
        stringLength = 0;
        payloadUnitCount = 0;
        payload = NULL;
    }
    
    Payload::Payload(const uint8_t *tokenString, uint8_t string_ength) {
        // initialize private data members
        stringLength = string_ength;
        payloadUnitCount = 0;
        payload = NULL;
        
        int index = 0;
        while( index!=stringLength) {
            int len=tokenString[index];
            index=index+1+len;
            payloadUnitCount++;               
        }
    
        UnitPayload *obj = new UnitPayload[payloadUnitCount];
        int i=0;
        int c=0;
        int j,k;

        while(i<payloadUnitCount)
        {   
            obj[i].length=tokenString[c];
            obj[i].id=tokenString[c+1];

            obj[i].data = new uint8_t[obj[i].length];
            for(j=c+2,k=0;(j<(c+obj[i].length+1))&&(k<obj[i].length-1);j++,k++)
            {
                obj[i].data[k]=tokenString[j];
        
            }           
            
            c=c+obj[i].length+1;
            i++;

        }
        payload = obj;
    }

uint8_t Payload::getPayloadUnitCount() {
    return payloadUnitCount;
}

uint8_t Payload::getIDAtIndex(int index) {
    return payload[index].get_id();
}

uint8_t Payload::getLengthAtIndex(int index) {
    return payload[index].get_length();
}

uint8_t* Payload::getDataAtIndex(int index) {
    return payload[index].get_data();
}