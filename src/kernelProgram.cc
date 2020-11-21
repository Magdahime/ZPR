#include "kernelProgram.h"

KernelProgram::KernelProgram(ClDevice& device, const std::string kernelPath, const std::string kernelName)
{
    std::ifstream fStream(kernelPath);
    kernelSource_ = std::string(
        (std::istreambuf_iterator<char>(fStream)),
        std::istreambuf_iterator<char>());
    auto kernelKey = "__" + kernelName;
    program_ = device.getGlobalCache()->get_or_build(kernelKey, "", kernelSource_, device.getContext());
    kernel_ = program_.create_kernel(kernelName);
}