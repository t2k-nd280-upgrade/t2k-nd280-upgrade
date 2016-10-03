#ifndef MemoryUsage_hxx_seen
#define MemoryUsage_hxx_seen
#include <typeinfo>
#include <TSystem.h>
#include <vector>

/// This class can be used to generate histograms of the memory used by
/// a highland analysis. Logging can be enabled by specifying the "-m"
/// option on the command line. AnalysisBase and SimpleLoopBase will then
/// instantiate, log and write out the memory usage to the output file.
class MemoryUsage {
  public:
    struct CmpMemResident {
        /// Comparison operator for selecting which ProcInfo_t has the
        /// largest resident memory value.
        bool operator()(ProcInfo_t lhs, ProcInfo_t rhs) {
          return lhs.fMemResident < rhs.fMemResident;
        }
    };

    struct CmpMemVirtual {
        /// Comparison operator for selecting which ProcInfo_t has the
        /// largest virtual memory value.
        bool operator()(ProcInfo_t lhs, ProcInfo_t rhs) {
          return lhs.fMemVirtual < rhs.fMemVirtual;
        }
    };

    /// Construct the memory usage class with logging disabled.
    MemoryUsage();
    ~MemoryUsage();

    /// Called to enable the memory logging.  Enable the logging of memory
    /// usage. If this is not called, LogMemory() and Write() have no effects.
    /// This can be called with a bool argument to set the enabled flag.  If
    /// the argument is false, then memory logging is disabled.
    void Enable(bool enable = true);

    /// Log the current memory usage.  Record the current memory being used by
    /// the process.  If Enable() has not been called, this function does
    /// nothing.  In general, the memory usage should be logged once per
    /// event, but it can be called any number of times.  Logging memory
    /// requires saving two float and two longs into a vector.
    void LogMemory();

    /// Write histograms of the memory usage to the output file.
    void Write();

  private:
    /// Whether to log memory usage. Set using Enable()
    bool fEnabled;

    /// Record of the memory usage each time LogMemory() is called.
    ///
    /// From TSystem.h:
    ///   struct ProcInfo_t {
    ///     Float_t fCpuUser; // user time used by this process in seconds
    ///     Float_t fCpuSys; // system time used by this process in seconds
    ///     Long_t fMemResident; // resident memory used by this process in KB
    ///     Long_t fMemVirtual; // virtual memory used by this process in KB
    ///   }
    std::vector<ProcInfo_t> fEventMemory;
};
#endif

