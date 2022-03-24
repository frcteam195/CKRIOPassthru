#pragma once

class FailoverSubsystem {
public:
    void SubsystemInit() {
        init();
    }

    void SubsystemPeriodic() {
        periodic();
    }

    void SubsystemRun()
    {
        run();
    }

    void SubsystemReset()
    {
        uninit();
    }

    virtual ~FailoverSubsystem() = default;
protected:
    //TODO: Make these more clear/name better
    virtual void init() = 0;
    virtual void periodic() = 0;
    virtual void run() = 0;
    virtual void uninit() = 0;
};