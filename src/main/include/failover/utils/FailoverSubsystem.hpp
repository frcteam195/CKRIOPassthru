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

    virtual ~FailoverSubsystem() = default;
protected:
    virtual void init() = 0;
    virtual void run() = 0;

    virtual void registerMotors() = 0;
};