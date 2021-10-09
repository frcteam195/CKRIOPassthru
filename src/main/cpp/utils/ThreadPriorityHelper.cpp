#include "utils/ThreadPriorityHelper.hpp"

//#define SET_THREAD_PRIORITY

namespace ck
{
    bool configureSystemPriority()
    {
#ifdef SET_THREAD_PRIORITY
        //system("sh -c \"ps -o pid,args | grep FRC_Net | grep -v lvuser | grep -v grep | sed s:/.*$::g | sed s:' '::g | xargs -I {} chrt -a -p -f 99 {}\"");
        std::string netstrPID = s_exec("ps -o pid,args | grep FRC_Net | grep -v lvuser | grep -v grep | sed s:/.*$::g | sed s:' '::g");
        pid_t netPID = std::stoi(netstrPID);
        struct sched_param sp_set;
        sp_set.sched_priority = 99;
        int ret = sched_setscheduler(netPID, SCHED_FIFO, &sp_set);
        if (ret != 0) {

        }
        
        int policy = sched_getscheduler(netPID);
        struct sched_param sp;
        ret = sched_getparam(netPID, &sp);
        if (ret != 0) {
            
        }
        if (sp.sched_priority != 99 || policy != SCHED_FIFO)
        {
            std::cout << "Error setting FRCNetComm priority" << std::endl;
            std::cout << "rtprio: " << sp.sched_priority << std::endl;
            switch(policy) {
                case SCHED_OTHER: printf("SCHED_OTHER\n"); break;
                case SCHED_RR:   printf("SCHED_RR\n"); break;
                case SCHED_FIFO:  printf("SCHED_FIFO\n"); break;
                default:   printf("Unknown...\n");
            }
            return false;
        }
#endif
        return true;
    }

    bool configureThreadPriority(int priority)
    {
#ifdef SET_THREAD_PRIORITY
        ///////////////////////////
        //Set Main Thread Priority
        //Must also set configuration for rtprio 99, priority -20, and nice -20 in /etc/security/limits.conf
        //Main Thread should be set to 98 (1 below NetComm process)
        sched_param sch;
        sch.sched_priority = priority;
        if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch) != 0)
        {
            std::cout << "Error setting main thread priority!" << std::endl;
            return false;
        }
#endif
        return true;
        ///////////////////////////
    }

    std::string s_exec(const char* cmd)
    {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

}