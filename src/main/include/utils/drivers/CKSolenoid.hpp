#pragma once

#include "SolenoidControl.pb.h"
#include "frc/Solenoid.h"
#include "frc/DoubleSolenoid.h"

class CKSolenoid
{
public:
    CKSolenoid(ck::SolenoidControl::Solenoid::ModuleType moduleType, uint32_t solenoidId, ck::SolenoidControl::Solenoid::SolenoidType solenoidType);
    ~CKSolenoid();

    void set(ck::SolenoidControl::Solenoid::SolenoidValue value);
    ck::SolenoidControl::Solenoid::SolenoidValue getValue();
    uint16_t getSolenoidId();
    uint16_t getModuleId();
private:
    ck::SolenoidControl::Solenoid::ModuleType mModuleType;
    uint32_t mSolenoidId;
    ck::SolenoidControl::Solenoid::SolenoidType mSolenoidType;
    frc::Solenoid* mSingleSolenoid;
    frc::DoubleSolenoid* mDoubleSolenoid;
};