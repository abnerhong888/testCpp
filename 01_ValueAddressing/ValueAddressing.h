#ifndef __VALUEADDRESSING_H__
#define __VALUEADDRESSING_H__

#include <vector>
#include "stdint.h"

#ifndef __ARRAY_SIZE
#   define __ARRAY_SIZE(__ValueTypeArray)  (size_t)(sizeof(__ValueTypeArray) / sizeof(__ValueTypeArray[0]))
#endif

template<typename StructType>
class StructAddressing{
public:
    // set address
    void setAddress(uint64_t _address){
        address = _address;
        structObject.setValuesAddress(address);
    }
    // get address
    uint64_t getAddress(){
        return address;
    }

    // set address
    void operator= (uint64_t _address){
        setAddress(_address);
        //printf("StructAddressing:: operator set address\n");
    }
    // get address
    operator uint64_t(){
        //printf("StructAddressing:: operator return address\n");
        return address;
    }

    StructType* getStructObject(){
        return &structObject;
    }

    StructType* operator->(){
        //printf("StructAddressing:: call struct pointer\n");
        return &structObject;
    }

private:
    StructType structObject;
    uint64_t address = 0;
};


class ValueAddressingBase{
public:
    uint64_t getAddress(){
        return address;
    }

    void setAddress(uint64_t _address){
        address = _address;
    }

    virtual size_t size() = 0;

    operator ValueAddressingBase* (){
        return this;
    }
    
protected:
    uint64_t address = 0;
};

template<typename DataType>
class ValueAddressing: public ValueAddressingBase{
public:
    // set value
    void operator= (DataType _value){
        value = _value;
        //printf("ValueAddressing:: operator set value\n");
    }
    // get value
    operator DataType(){
        //printf("ValueAddressing:: operator return value\n");
        return value;
    }
    // get address
    uint64_t operator& (){
        return address;
    }

    size_t size() override{
        return sizeof(DataType);
    }
private: 
    DataType value = {};
};

class StructAddressingBase{
public:
    virtual void setValuesAddress(uint64_t _address) = 0;

    void valueSet(ValueAddressingBase* value){
        value->setAddress(address + total_size);
        total_size += value->size();
    }

    template<typename T>
    void structSet(T* structValue){
        *structValue = address + total_size;
        total_size += (*structValue)->size();
    }

    template<typename T>
    void arraySet(T array, size_t n){
        ValueAddressingBase* base = nullptr;
        for(size_t i = 0; i < n; ++i){
            base = array[i];
            base->setAddress(address + total_size);
            total_size += base->size();
        }
    }

    void setAddress(uint64_t _address){
        address = _address;
    }

    uint64_t getAddress(){
        return address;
    }

    size_t size(){
        return total_size;
    }
protected:
    uint64_t address = 0;
    size_t total_size = 0;
};



#define DEF_STRUCT(_struct_name) \
class _struct_name : public StructAddressingBase


#define DEF_SET_ADDRESS_START \
void setValuesAddress(uint64_t _address){ \
        setAddress(_address); \

#define DEF_SET_ADDRESS_END }

#define DEF_VALUE_VARIABLE(_type, _name) \
ValueAddressing<_type> _name;

#define DEF_VALUE_ARRAY(_type, _name, _size) \
ValueAddressing<_type> _name[_size];

#define DEF_VALUE_STRUCT(_type, _name) \
StructAddressing<_type> _name;

#define DEF_SET_VARIABLE(_type, _name) \
valueSet(_name);

#define DEF_SET_ARRAY(_type, _name, _size) \
arraySet(_name, _size);

#define DEF_SET_STRUCT(_type, _name) \
structSet(&_name);


/* EXAMPLE


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


*/


#endif //__VALUEADDRESSING_H__