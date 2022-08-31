#ifndef BASEOPERATIONS_H
#define BASEOPERATIONS_H

#include <swsmodule.h>
#include <swsmodulefactory.h>
#include <swsvalue.h>

class swsModuleValue : public swsModule
{
public:
    swsModuleValue(): swsModule()
    {
        newPlug("value", swsPlug::direction::output, 0);
    };

    virtual ~swsModuleValue() {}

    static std::string getType() { return "value"; }

    void step() override
    {};
};

class swsModuleAdd : public swsModule
{
public:
    swsModuleAdd() /*: swsModule()
    {
        mResult = newPlug("result", swsPlug::direction::output, 0);
        mOp1 = newPlug("op1", swsPlug::direction::input, 0);
        mOp2 = newPlug("op2", swsPlug::direction::input, 0);
    }*/;

    virtual ~swsModuleAdd() {}

    static std::string getType() { /*printf("Type=%s\n", mRegistration.type.c_str());*/ return "add"; }

    void step() override/*
    {
        mResult->setValue(mOp1->getValue() + mOp2->getValue());
    }*/;

private:
    swsPlug *mResult, *mOp1, *mOp2;

    static swsModuleRegistration<swsModuleAdd> mRegistration;
};

//void *__swsModuleAddRegistration = (void *) &swsModuleAdd::mRegistration;

class swsModuleMultiply : public swsModule
{
public:
    swsModuleMultiply(): swsModule()
    {
        mResult = newPlug("result", swsPlug::direction::output, 0);
        mOp1 = newPlug("op1", swsPlug::direction::input, 0);
        mOp2 = newPlug("op2", swsPlug::direction::input, 0);
    };

    virtual ~swsModuleMultiply() {}

    static std::string getType() { return "multiply"; }

    void step() override
    {
        mResult->setValue(mOp1->getValue() * mOp2->getValue());
    };

private:
    swsPlug *mResult, *mOp1, *mOp2;
};

//static swsModuleRegistration<swsModuleAdd>      swsModuleAddRegistration;
static swsModuleRegistration<swsModuleValue>    swsModuleValueAddRegistration;
static swsModuleRegistration<swsModuleMultiply> swsModuleMultiplyRegistration;

#endif // BASEOPERATIONS_H
