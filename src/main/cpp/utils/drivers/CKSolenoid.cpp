#include "utils/drivers/CKSolenoid.hpp"
#include "utils/CKErrors.hpp"
#include "utils/GlobalConfig.hpp"
#include <sstream>

std::atomic_bool CKSolenoid::ctrePCMInitialized = false;

CKSolenoid::CKSolenoid(ck::SolenoidControl::Solenoid::ModuleType moduleType, uint32_t solenoidId, ck::SolenoidControl::Solenoid::SolenoidType solenoidType)
: mModuleType(moduleType), mSolenoidId(solenoidId), mSolenoidType(solenoidType)
{
    if (mModuleType == ck::SolenoidControl::Solenoid::ModuleType::SolenoidControl_Solenoid_ModuleType_CTREPCM)
    {
        if (getModuleId() >= 64 || getSolenoidId() >= 8)
        {
            ck::ReportError("Solenoid or PCM ID is invalid. Not creating solenoid");
            return;
        }

        if (!ctrePCMInitialized)
        {
            mCompressor = new frc::Compressor(getModuleId(), (frc::PneumaticsModuleType) mModuleType);
            mCompressor->EnableDigital();
            ctrePCMInitialized = true;
        }
    }
    else if (mModuleType == ck::SolenoidControl::Solenoid::ModuleType::SolenoidControl_Solenoid_ModuleType_REVPH)
    {
        if (getModuleId() >= 63 || getSolenoidId() >= 16)
        {
            std::stringstream ss;
            ss << "Solenoid or PH ID is invalid. Not creating module with ID" << getModuleId() << " and solenoid with ID " << mSolenoidId << std::endl;
            ck::ReportError(ss.str());
            return;
        }
    }

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
    if (mCompressor)
    {
        delete mCompressor;
        ctrePCMInitialized = false;
    }
}

void CKSolenoid::set(ck::SolenoidControl::Solenoid::SolenoidValue value)
{
    if (!mSingleSolenoid && !mDoubleSolenoid)
    {
        ck::ReportError("Solenoid or PH ID is invalid. Cannot set invalid solenoid with ID" + mSolenoidId);
        return;
    }

    mValue = value;
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
            std::string errMsg = "Invalid solenoid set for ID: " + mSolenoidId;
            ck::ReportError(errMsg);
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
            std::string errMsg = "Invalid solenoid set for ID: " + mSolenoidId;
            ck::ReportError(errMsg);
        }
    }
        break;
    default:
        break;
    }
}

uint16_t CKSolenoid::getModuleId()
{
    uint16_t retVal = (mSolenoidId >> 16) & 0xFFFF;
    if (mModuleType == ck::SolenoidControl_Solenoid_ModuleType::SolenoidControl_Solenoid_ModuleType_REVPH && retVal == 0)
    {
        //REV PH defaults to ID 1
        retVal = 1;
    }
    return retVal;
}

uint16_t CKSolenoid::getSolenoidId()
{
    return mSolenoidId & 0xFFFF;
}

ck::SolenoidControl::Solenoid::SolenoidValue CKSolenoid::getValue()
{
    return mValue;
}