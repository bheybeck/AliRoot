//**************************************************************************\
//* This file is property of and copyright by the ALICE Project            *\
//* ALICE Experiment at CERN, All rights reserved.                         *\
//*                                                                        *\
//* Primary Authors: Matthias Richter <Matthias.Richter@ift.uib.no>        *\
//*                  for The ALICE HLT Project.                            *\
//*                                                                        *\
//* Permission to use, copy, modify and distribute this software and its   *\
//* documentation strictly for non-commercial purposes is hereby granted   *\
//* without fee, provided that the above copyright notice appears in all   *\
//* copies and that both the copyright notice and this permission notice   *\
//* appear in the supporting documentation. The authors make no claims     *\
//* about the suitability of this software for any purpose. It is          *\
//* provided "as is" without express or implied warranty.                  *\
//**************************************************************************

/// \file GPUReconstructionOCL.h
/// \author David Rohr

#ifndef GPURECONSTRUCTIONOCL_H
#define GPURECONSTRUCTIONOCL_H

#include "GPUReconstructionDeviceBase.h"

#ifdef _WIN32
extern "C" __declspec(dllexport) GPUCA_NAMESPACE::gpu::GPUReconstruction* GPUReconstruction_Create_OCLconst GPUCA_NAMESPACE::gpu::GPUSettingsDeviceBackend& cfg);
#else
extern "C" GPUCA_NAMESPACE::gpu::GPUReconstruction* GPUReconstruction_Create_OCL(const GPUCA_NAMESPACE::gpu::GPUSettingsDeviceBackend& cfg);
#endif

namespace GPUCA_NAMESPACE::gpu
{
struct GPUReconstructionOCLInternals;

class GPUReconstructionOCL : public GPUReconstructionDeviceBase
{
 public:
  ~GPUReconstructionOCL() override;
  GPUReconstructionOCL(const GPUSettingsDeviceBackend& cfg);

 protected:
  int InitDevice_Runtime() override;
  int ExitDevice_Runtime() override;
  void UpdateSettings() override;

  void SynchronizeGPU() override;
  int DoStuckProtection(int stream, void* event) override;
  int GPUDebug(const char* state = "UNKNOWN", int stream = -1, bool force = false) override;
  void SynchronizeStream(int stream) override;
  void SynchronizeEvents(deviceEvent* evList, int nEvents = 1) override;
  void StreamWaitForEvents(int stream, deviceEvent* evList, int nEvents = 1) override;
  bool IsEventDone(deviceEvent* evList, int nEvents = 1) override;

  size_t WriteToConstantMemory(size_t offset, const void* src, size_t size, int stream = -1, deviceEvent* ev = nullptr) override;
  size_t TransferMemoryInternal(GPUMemoryResource* res, int stream, deviceEvent* ev, deviceEvent* evList, int nEvents, bool toGPU, const void* src, void* dst) override;
  size_t GPUMemCpy(void* dst, const void* src, size_t size, int stream, int toGPU, deviceEvent* ev = nullptr, deviceEvent* evList = nullptr, int nEvents = 1) override;
  void ReleaseEvent(deviceEvent* ev) override;
  void RecordMarker(deviceEvent* ev, int stream) override;

  virtual int GetOCLPrograms() = 0;
  virtual bool CheckPlatform(unsigned int i) = 0;
  virtual bool ContextForAllPlatforms() { return false; }

  template <class T, int I = 0>
  int AddKernel(bool multi = false);
  template <class T, int I = 0>
  unsigned int FindKernel(int num);
  template <typename K, typename... Args>
  int runKernelBackendCommon(krnlSetup& _xyz, K& k, const Args&... args);
  template <class T, int I = 0>
  const krnlProperties getKernelPropertiesBackend();

  GPUReconstructionOCLInternals* mInternals;
};

} // namespace GPUCA_NAMESPACE::gpu

#endif
