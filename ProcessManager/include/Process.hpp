#ifndef PROCESS_HPP
#define PROCESS_HPP

#include<string>
#include<vector>

/**
 * @brief Wrapper class for processes
 * 
 */
class Process
{
public:

    typedef enum
    {
        NOT_STARTED,
        RUNNING,
        STOPPED
    } enuProcessStatus;

    typedef enum
    {
        NORMAL,
        MEMCHECK,
        CALLGRIND,
        GDB
    } enuInitOptions;

    enuProcessStatus getStatus() const { return m_status; }

    /// Default constructor is deleted since Process without executable makes no sense
    Process() = delete;

    /**
     * @brief Construct a new Process object. Not started until `initiate()` is called.
     *
     * @param pathname Pathname to a process executable
     */
    Process(const std::string& pathname, enuInitOptions options = enuInitOptions::NORMAL);

    /**
     * @brief Construct a new Process object. Not started until `initiate()` is called.
     * @param pathname Pathname to a process executable
     * @param arguments Vector of strings which are passed to the process as arguments. (Process name implicitly passed as the first argument)
    */
    Process(const std::string& pathname, const std::vector<char*>& arguments, enuInitOptions options = enuInitOptions::NORMAL);

    /**
     * @brief Destroy the Process object
     *
     */
    ~Process() {}; // for shared memory

    /**
     * @brief Executes the process.
     *
     * Process executable pathname is initialized in constructor, \n
     * and stored in `pathname` memeber variable. \n
     *
     * @return int returns PID of process
     */
    int initiate();

    /// Kills the process referenced by current Process object -- hard kill
    void forceKillProcess();

    /// Kills the process referenced by current Process object -- soft kill
    void killProcess();

    std::string getName() const;

    unsigned int getPID() const { return m_pid; }

    /// Restarts the process
    void restart();

    /// `wait(2)`s for current process
    int waitForProcess();

    /// `wait(2)`s for current process with WNOHANG option and returns true if process has already exited, false otherwise(including error TODO?)
    bool hasExited();


    private:

    using ProcessInitiator = int (Process::*)();

    ProcessInitiator m_initiator;

    /// Pathname to a process executable
    std::string m_pathname = "";
    std::vector<char*> m_arguments;

    // TODO : Create getter.
    /// Process PID, set to -1 by default. Set by `initiate()`.
    int m_pid = -1;

    enuProcessStatus m_status;

    void setProcessInitiator(enuInitOptions options);

    int initiate_wMemcheck();
    int initiate_normal();
    int initiate_wCallgrind();
    int initiate_wGDB();


};

#endif

