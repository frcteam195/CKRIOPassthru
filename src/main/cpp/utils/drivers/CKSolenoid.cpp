#include "utils/drivers/CKSolenoid.hpp"
#include "frc/Errors.h"

CKSolenoid::CKSolenoid(ck::SolenoidControl::Solenoid::ModuleType moduleType, uint32_t solenoidId, ck::SolenoidControl::Solenoid::SolenoidType solenoidType)
: mModuleType(moduleType), mSolenoidId(solenoidId), mSolenoidType(solenoidType)
{
    switch (mSolenoidType)
    {
    case ck::SolenoidControl::Solenoid::SolenoidType::SolenoidControl_Solenoid_SolenoidType_SINGLE:
    {
        mSingleSolenoid = new frc::Solenoid(getModuleId(), (frc::PneumaticsModuleType) moduleType, getSolenoidId());
    }
        break;
    case ck::SolenoidControl::Solenoid::SolenoidType::SolenoidControl_Solenoid_SolenoidType_DOUBLE:
    {
        mDoubleSolenoid = new frc::DoubleSolenoid(getModuleId(), (frc::PneumaticsModuleType) moduleType, getSolenoidId(), getSolenoidId() + 1);
    }
        break;
    default:
        break;
    }
}

CKSolenoid::~CKSolenoid()
{
    if (mSingleSolenoid)
    {
        delete mSingleSolenoid;
    }
    if (mDoubleSolenoid)
    {
        delete mDoubleSolenoid;
    }
}

void CKSolenoid::set(ck::SolenoidControl::Solenoid::SolenoidValue value)
{
    switch (mSolenoidType)
    {
    case ck::SolenoidControl::Solenoid::SolenoidType::SolenoidControl_Solenoid_SolenoidType_SINGLE:
    {
        if (value == ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_ON
            || value == ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_OFF)
        {
            mSingleSolenoid->Set(value == ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_ON ? true : false);
        }
        else
        {
            FRC_ReportError(-100, "Invalid solenoid set for ID: %d", mSolenoidId);
        }
    }
        break;
    case ck::SolenoidControl::Solenoid::SolenoidType::SolenoidControl_Solenoid_SolenoidType_DOUBLE:
    {
        if (value == ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_OFF
            || value == ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_FORWARD
            || value == ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_REVERSE)
        {
            mDoubleSolenoid->Set((frc::DoubleSolenoid::Value)value);
        }
        else
        {
            FRC_ReportError(-100, "Invalid solenoid set for ID: %d", mSolenoidId);
        }
    }
        break;
    default:
        break;
    }
}

uint16_t CKSolenoid::getModuleId()
{
    return (mSolenoidId >> 16) & 0xFFFF;
}

uint16_t CKSolenoid::getSolenoidId()
{
    return mSolenoidId & 0xFFFF;
}

ck::SolenoidControl::Solenoid::SolenoidValue CKSolenoid::getValue()
{
    switch (mSolenoidType)
    {
    case ck::SolenoidControl::Solenoid::SolenoidType::SolenoidControl_Solenoid_SolenoidType_SINGLE:
        return mSingleSolenoid->Get()
            ? ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_ON
            : ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_OFF;
    case ck::SolenoidControl::Solenoid::SolenoidType::SolenoidControl_Solenoid_SolenoidType_DOUBLE:
        return (ck::SolenoidControl::Solenoid::SolenoidValue)mDoubleSolenoid->Get();
    default:
        return ck::SolenoidControl::Solenoid::SolenoidValue::SolenoidControl_Solenoid_SolenoidValue_OFF;
    }
}