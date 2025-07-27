#include "stdio.h"
#include "stdlib.h"


#include "ValueAddressing.h"


DEF_STRUCT(StructureC){
public :
    DEF_VALUE_VARIABLE(uint8_t, Value1)
    DEF_VALUE_VARIABLE(uint8_t, Value2)
    DEF_VALUE_ARRAY(uint8_t, Array1, 3)
public:
    DEF_SET_ADDRESS_START
        DEF_SET_VARIABLE(uint8_t, Value1)
        DEF_SET_VARIABLE(uint8_t, Value2)
        DEF_SET_ARRAY(uint8_t, Array1, 3)
    DEF_SET_ADDRESS_END
};

DEF_STRUCT(StructureD){
public :
    DEF_VALUE_VARIABLE(uint8_t, Value1)
    DEF_VALUE_VARIABLE(uint8_t, Value2)
    DEF_VALUE_STRUCT(StructureC, structC)
public:
    DEF_SET_ADDRESS_START
        DEF_SET_VARIABLE(uint8_t, Value1)
        DEF_SET_VARIABLE(uint8_t, Value2)
        DEF_SET_STRUCT(StructureC, structC)
    DEF_SET_ADDRESS_END
};

uint32_t toWrite(uint64_t address, uint8_t* buf2Write, size_t size){
    printf("toWrite addr = 0x%016llx, size = %lld\n", address, size);
    return 0;
}

uint32_t toRead(uint64_t address, uint8_t* buf2Read, size_t size){
    printf("toRead addr = 0x%016llx, size = %lld\n", address, size);
    return 0;
}

int main(){
    
#define PRINT(_V) printf("---" #_V " addr = 0x%016llx, size = %lld\n", &_V, _V.size());
    

    StructAddressing<StructureD> Instance;
    Instance->setCallback(toWrite, toRead);
    Instance = (uint64_t)0x0000000011111110;

    StructureD structValue;
    structValue.setValuesAddress((uint64_t)0x1111111100000000);
    Instance = structValue;

    printf("---Instance = 0x%016llx\n", Instance.getAddress());
    
    Instance->Value1 = 100;
    uint32_t v = Instance->Value1;
    printf("v = %d\n", v);

    PRINT(Instance->Value1);
    PRINT(Instance->Value2);
    PRINT(Instance->structC->Value1);
    PRINT(Instance->structC->Value2);
    PRINT(Instance->structC->Array1[0]);
    PRINT(Instance->structC->Array1[1]);
    PRINT(Instance->structC->Array1[2]);
    Instance->structC->Value1 = 255;
    printf("total size = %lld Bytes\n", Instance->size());
    return 0;
}
