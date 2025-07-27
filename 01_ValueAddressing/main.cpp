#include "stdio.h"
#include "stdlib.h"


#include "ValueAddressing.h"


class StructureB : public StructAddressingBase{
public:
        ValueAddressing<uint32_t> Value1;
        ValueAddressing<uint32_t> Value2;
public:
    void setValuesAddress(uint64_t _address){
        setAddress(_address);
        valueSet(Value1);
        valueSet(Value2);
    }
    
};

class StructureA : public StructAddressingBase{
public:
        ValueAddressing<uint32_t> Value1;
        ValueAddressing<uint32_t> Value2;
        ValueAddressing<uint32_t> Value3;
        ValueAddressing<uint32_t> Array1[2];
        ValueAddressing<uint64_t> Value4;
        StructAddressing<StructureB> structureB;
        ValueAddressing<uint64_t> Value5;
        ValueAddressing<uint64_t> Value6;
public:
    void setValuesAddress(uint64_t _address){
        setAddress(_address);

        valueSet(Value1);
        valueSet(Value2);
        valueSet(Value3);
        arraySet(Array1, __ARRAY_SIZE(Array1));
        valueSet(Value4);

        structSet(&structureB);

        valueSet(Value5);
        valueSet(Value6);

    }
    
};

typedef struct Handler{
    StructAddressing<StructureA> Instance;
}Handler;


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

int main(){
    Handler* handle = new Handler();
    handle->Instance = (uint64_t)0x1111111100000000;

    uint64_t address = handle->Instance;
    printf("---handle->Instance = 0x%016llx\n", address);

    handle->Instance->Value1 = 16;
    int v = handle->Instance->Value1;
    printf("---handle->Instance->Value1 = %d\n", v);
     
#define PRINT(_V) printf("---" #_V " addr = 0x%016llx, size = %lld\n", &_V, _V.size());
    PRINT(handle->Instance->Value1);
    PRINT(handle->Instance->Value2);
    PRINT(handle->Instance->Value3);
    PRINT(handle->Instance->Array1[0]);
    PRINT(handle->Instance->Array1[1]);
    PRINT(handle->Instance->Value4);
    PRINT(handle->Instance->structureB->Value1);
    PRINT(handle->Instance->structureB->Value2);
    PRINT(handle->Instance->Value5);
    PRINT(handle->Instance->Value6);
       
    printf("total size = %lld Bytes\n", handle->Instance->size());
    

    StructAddressing<StructureD> Instance2;
    Instance2 = (uint64_t)0x0000000011111110;
    printf("---Instance2 = 0x%016llx\n", Instance2.getAddress());

    PRINT(Instance2->Value1);
    PRINT(Instance2->Value2);
    PRINT(Instance2->structC->Value1);
    PRINT(Instance2->structC->Value2);
    PRINT(Instance2->structC->Array1[0]);
    PRINT(Instance2->structC->Array1[1]);
    PRINT(Instance2->structC->Array1[2]);

    printf("total size = %lld Bytes\n", Instance2->size());
    return 0;
}
