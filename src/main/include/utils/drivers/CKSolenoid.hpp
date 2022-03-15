#pragma once

#include "SolenoidControl.pb.h"
#include "frc/Solenoid.h"
#include "frc/DoubleSolenoid.h"
#include "frc/Compressor.h"

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
    std::atomic<ck::SolenoidControl::Solenoid::SolenoidValue> mValue {ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_OFF};
    ck::SolenoidControl::Solenoid::ModuleType mModuleType {ck::SolenoidControl::Solenoid::ModuleType::SolenoidControl_Solenoid_ModuleType_REVPH};
    uint32_t mSolenoidId = 0;
    ck::SolenoidControl::Solenoid::SolenoidType mSolenoidType {ck::SolenoidControl::Solenoid::SolenoidType::SolenoidControl_Solenoid_SolenoidType_SINGLE};
    frc::Solenoid* mSingleSolenoid;
    frc::DoubleSolenoid* mDoubleSolenoid;
};