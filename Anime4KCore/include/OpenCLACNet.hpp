#pragma once

#include<fstream>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#ifdef __APPLE__
#include<OpenCL/opencl.h>
#else
#include<CL/cl.h>
#endif // SPECIAL OS

#include"AC.hpp"
#include"CNN.hpp"


namespace Anime4KCPP
{
    namespace OpenCL
    {
        constexpr cl_int L2 = 0, L3 = 1, L4 = 2, L5 = 3, L6 = 4, L7 = 5, L8 = 6, L9 = 7;

        enum ACNetType
        {
            HDNL0 = 0, HDNL1, HDNL2, HDNL3, TotalTypeCount
        };

        class DLL ACNet;
    }
}

class Anime4KCPP::OpenCL::ACNet :public AC
{
public:
    ACNet(const Parameters& parameters = Parameters());
    virtual ~ACNet() = default;
    virtual void setArguments(const Parameters& parameters) override;

    virtual std::string getInfo() override;
    virtual std::string getFiltersInfo() override;

    static void initGPU(unsigned int platformID = 0, 
        unsigned int deviceID = 0, 
        const CNNType type = CNNType::Default, 
        const int OpenCLQueueNum = 4,
        const bool OpenCLParallelIO = false);
    static void releaseGPU() noexcept;
    static bool isInitializedGPU();
private:
    virtual void processYUVImageB() override;
    virtual void processRGBImageB() override;
    virtual void processGrayscaleB() override;
    virtual void processRGBVideoB() override;

    virtual void processYUVImageW() override;
    virtual void processRGBImageW() override;
    virtual void processGrayscaleW() override;

    virtual void processYUVImageF() override;
    virtual void processRGBImageF() override;
    virtual void processGrayscaleF() override;

    virtual Processor::Type getProcessorType() noexcept override;
    virtual std::string getProcessorInfo() override;

    void runKernelB(const cv::Mat& orgImg, cv::Mat& dstImg);
    void runKernelW(const cv::Mat& orgImg, cv::Mat& dstImg);
    void runKernelF(const cv::Mat& orgImg, cv::Mat& dstImg);

    void runKernelPB(const cv::Mat& orgImg, cv::Mat& dstImg);
    void runKernelPW(const cv::Mat& orgImg, cv::Mat& dstImg);
    void runKernelPF(const cv::Mat& orgImg, cv::Mat& dstImg);

    static void initOpenCL(const CNNType type);
    static void releaseOpenCL() noexcept;
    static std::string readKernel(const std::string& fileName);
private:
    int currACNetypeIndex;

    static bool isInitialized;

    static cl_context context;

    static int commandQueueNum;
    static int commandQueueCount;
    static std::vector<cl_command_queue> commandQueueList;
    static bool parallelIO;
    static cl_command_queue commandQueueIO;

    static cl_program program[TotalTypeCount];
    static cl_device_id device;

    static size_t workGroupSizeLog;

    static unsigned int pID;
    static unsigned int dID;

#ifdef BUILT_IN_KERNEL
    static const std::string ACNetKernelSourceString[TotalTypeCount];
#endif // BUILT_IN_KERNEL
};
