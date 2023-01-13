#ifndef _UNITY_H
#define _UNITY_H

#include <android/log.h>
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"

float NormalizeAngle(float angle);
Vector3 NormalizeAngles (Vector3 angles);
Vector3 ToEulerRad(Quaternion q1);
Quaternion GetRotationToLocation(Vector3 targetLocation, float y_bias, Vector3 myLoc);

template <typename T>
struct monoArray
{
    void* klass;
    void* monitor;
    void* bounds;
    int   max_length;
    void* vector [1];

    int getLength()
    {
        return max_length;
    }
    T getPointer()
    {
        return (T)vector;
    }
};

template <typename T>
struct monoList {
    void *unk0;
    void *unk1;
    monoArray<T> *items;
    int size;
    int version;

    T getItems(){
        return items->getPointer();
    }

    int getSize(){
        return size;
    }

    int getVersion(){
        return version;
    }
};

template <typename K, typename V>
struct monoDictionary {
    void *unk0;
    void *unk1;
    monoArray<int **> *table;
    monoArray<void **> *linkSlots;
    monoArray<K> *keys;
    monoArray<V> *values;
    int touchedSlots;
    int emptySlot;
    int size;

    K getKeys(){
        return keys->getPointer();
    }

    V getValues(){
        return values->getPointer();
    }

    int getNumKeys(){
        return keys->getLength();
    }

    int getNumValues(){
        return values->getLength();
    }

    int getSize(){
        return size;
    }
};

union intfloat {
	int i;
	float f;
};

typedef struct _monoString
{
    void* klass;
    void* monitor;
    int length;    
    char chars[1];

    int getLength()
    {
      return length;
    }
    char* getChars()
    {
        return chars;
    }
}monoString;

/**
* Get the real value of an ObscuredInt.
* @param location: the location of the ObscuredInt
* @return: Actual value ObscuredInt
*/
int GetObscuredIntValue(uint64_t location);
/**
* Get the real value of an ObscuredBool.
* @param location: the location of the ObscuredBool
* @return: Actual value ObscuredBool
*/
int GetObscuredBoolValue(uint64_t location);

/**
* Get the real value of an ObscuredFloat.
* @param location: the location of the ObscuredFloat
* @return: Actual value ObscuredFloat
*/
float GetObscuredFloatValue(uint64_t location);

/**
* Set the real value of an ObscuredInt.
* @param location: the location of the ObscuredInt
* @param value: the value we're setting the ObscuredInt to
*/
void SetObscuredIntValue(uint64_t location, int value);

/**
* Set the real value of an ObscuredFloat.
* @param location: the location of the ObscuredFloat
* @param value: the value we're setting the ObscuredFloat to
*/
void SetObscuredFloatValue(uint64_t location, float value);

#endif _UNITY_H
