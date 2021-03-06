Profiling				{#cpuProfiling}
===============

Code profiling is an important process to determine performance bottlenecks. Profiling measures code execution times and memory allocations. bs::f provides a built-in profiler through the @ref bs::ProfilerCPU "ProfilerCPU" module. This module can be globally accessed through @ref bs::gProfilerCPU() "gProfilerCPU()".

The profiler allows you to profile blocks of code and output information about how long the block took to execute, as well as information about number and amount of memory allocations.

The profiler supports two separate measuring modes, the normal mode measures time in milliseconds, while the precise mode measures time in CPU cycles. The precise mode does have drawbacks as it is inacurrate for longer code as it will not account for OS context switches and similar. Usually you will be using the normal measuring mode, and reserve the precise mode when you need to know exactly how many cycles some relatively small operation takes.

# Recording
To start profiling a block call surround it with either:
 - @ref bs::ProfilerCPU::beginSample "ProfilerCPU::beginSample()" / @ref bs::ProfilerCPU::endSample "ProfilerCPU::endSample()" - Records timing information (in milliseconds) about the code in-between, as well as memory allocation information
 - @ref bs::ProfilerCPU::beginSamplePrecise "ProfilerCPU::beginSamplePrecise()" / @ref bs::ProfilerCPU::endSamplePrecise "ProfilerCPU::endSamplePrecise()" - Records timing information (in CPU cyles) about the code in-between, as well as memory allocation information
 
All of the methods above expect a name as a parameter, which is arbitrary and it is used so you can later identify the profiling information. 

~~~~~~~~~~~~~{.cpp}
void doSomethingIntensive()
{
	// ...
}

gProfilerCPU().beginSample("myProfilingBlock");
doSomethingIntensive();
gProfilerCPU().endSample("myProfilingBlock");
~~~~~~~~~~~~~

Each sample needs to have a *begin()* and an *end()* pair. Samples can be nested between other samples.

# Reporting
Once you have placed sample points around your code, you can retrieve the profiling report by calling @ref bs::ProfilerCPU::generateReport() "ProfilerCPU::generateReport()". This will return a @ref bs::CPUProfilerReport "CPUProfilerReport" object, which contains a list of normal and precise samples.

Each sampling entry is represented either by @ref bs::CPUProfilerBasicSamplingEntry "CPUProfilerBasicSamplingEntry" or @ref bs::CPUProfilerPreciseSamplingEntry "CPUProfilerPreciseSamplingEntry". Sampling entries contain information about the time it took to execute the code in the sampled block of code, as well as number of memory allocations & deallocations, and total number of allocated and deallocated bytes. Each sample also contains a list of child samples (if any).

~~~~~~~~~~~~~{.cpp}
CPUProfilerReport report = gProfilerCPU().generateReport();
for(CPUProfilerBasicSamplingEntry& entry : report.getBasicSamplingData())
{ /* ... */ }

for(CPUProfilerPreciseSamplingEntry& entry : report.getPreciseSamplingData())
{ /* ... */ }
~~~~~~~~~~~~~

After retrieving the data, you can log it, display it on screen, or similar.

## Profiler overlay
You can easily display the profiler reports on screen by adding the @ref bs::CProfilerOverlay "ProfilerOverlay" component to the scene and assigning it a camera. It will then automatically read profiler reports every frame and display them on the provided camera.

~~~~~~~~~~~~~{.cpp}
HCamera camera = ...; // Add a scene camera

HSceneObject profilerOverlaySO = SceneObject::create("Profiler overlay");
HProfilerOverlay profilerOverlay = profilerOverlaySO->addComponent<CProfilerOverlay>(camera);
~~~~~~~~~~~~~

## Threads
The profiler is thread-safe, but if you are profiling code on threads not managed by the engine, you must manually call @ref bs::ProfilerCPU::beginThread "ProfilerCPU::beginThread()" before any sample calls, and @ref bs::ProfilerCPU::endThread "ProfilerCPU::endThread()" after all sample calls.

## Overhead
Profiler code itself will introduce a certain amount of overhead which will slightly skew profiling results. The profiler attempts to estimate this error, which is reported in the returned reports. You can choose to take this into consideration if you need really precise results.