#ifndef KERNELPROGRAM
#define KERNELPROGRAM

//OpenCL stuff
#include <boost/compute/core.hpp>
#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/math.hpp>

namespace compute = boost::compute;

class ClDevice
{
    compute::device device_;
    compute::context context_;
    compute::command_queue queue_;
    compute::wait_list waitList_;

public:
    ClDevice()
    {
        device_ = compute::system::default_device();
        context_ = compute::context(device_);
        queue_ = compute::command_queue(context_, device_);
    };
    compute::device getDevice() { return device_; };
    compute::context getContext() { return context_; };
    compute::command_queue getQueue() { return queue_; };
    compute::wait_list getWaitlist() { return waitList_; };
    boost::shared_ptr<compute::program_cache> getGlobalCache()
    {
        return compute::program_cache::get_global_cache(context_);
    };
};

class KernelProgram
{
    std::string kernelKey_;
    std::string kernelOptions_;
    std::string kernelSource_;
    boost::compute::program program_;
    boost::compute::kernel kernel_;

public:
    KernelProgram(ClDevice &device, const std::string kernelPath, const std::string kernelName);

    boost::compute::kernel getKernel() { return kernel_; };
};

#endif
