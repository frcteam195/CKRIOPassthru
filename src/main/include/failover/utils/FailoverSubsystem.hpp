#pragma once

class FailoverSubsystem {
public:
    void SubsystemInit() {
        registerMotors();
        init();
    }

    void SubsystemRun()
    {
        registerMotors();
        run();
    }

    void SubsystemReset()
    {
        uninit();
    }

    virtual ~FailoverSubsystem() = default;
protected:
    virtual void init() = 0;
    virtual void run() = 0;
    virtual void uninit() = 0;

    virtual void registerMotors() = 0;
};