#ifndef __VALUEADDRESSING_H__
#define __VALUEADDRESSING_H__

#include <vector>
#include "stdint.h"

#ifndef __ARRAY_SIZE
#   define __ARRAY_SIZE(__ValueTypeArray)  (size_t)(sizeof(__ValueTypeArray) / sizeof(__ValueTypeArray[0]))
#endif

template<typename T>
class StructureType{
public:
    // set address
    void operator= (uint64_t _address){
        address = _address;
        structObject.set_values_address(address);
        //printf("StructureType:: operator set address\n");
    }
    // get address
    operator uint64_t(){
        //printf("StructureType:: operator return address\n");
        return address;
    }

    T* operator->(){
        //printf("StructureType:: call struct pointer\n");
        return &structObject;
    }

private:
    T structObject;
    uint64_t address = 0;
};


class ValueTypeBase{
public:
    void set_address(uint64_t _address){
        address = _address;
    }

    virtual size_t size() = 0;

    operator ValueTypeBase* (){
        return this;
    }
    
protected:
    uint64_t address = 0;
};

template<typename T>
class ValueType: public ValueTypeBase{
public:
    // set value
    void operator= (T _value){
        value = _value;
        //printf("ValueType:: operator set value\n");
    }
    // get value
    operator T(){
        //printf("ValueType:: operator return value\n");
        return value;
    }

    uint64_t operator& (){
        return address;
    }

    size_t size() override{
        return sizeof(T);
    }
private: 
    T value = {};
};

class StructureBase{
public:
    virtual void set_values_address(uint64_t _address) = 0;

    void valueSet(ValueTypeBase* value){
        value->set_address(address);
        address += value->size();
        total_size += value->size();
    }

    template<typename T>
    void structSet(T* structValue){
        *structValue = address;
        address = (*structValue)->getAddress();
        total_size += (*structValue)->size();
    }

    template<typename T>
    void arraySet(T array, size_t n){
        ValueTypeBase* base = nullptr;
        for(size_t i = 0; i < n; ++i){
            base = array[i];
            base->set_address(address);
            address += base->size();
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
class _struct_name : public StructureBase


#define DEF_SET_ADDRESS_START \
void set_values_address(uint64_t _address){ \
        setAddress(_address); \

#define DEF_SET_ADDRESS_END }

#define DEF_VALUE_VARIABLE(_type, _name) \
ValueType<_type> _name;

#define DEF_VALUE_ARRAY(_type, _name, _size) \
ValueType<_type> _name[_size];

#define DEF_VALUE_STRUCT(_type, _name) \
StructureType<_type> _name;

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