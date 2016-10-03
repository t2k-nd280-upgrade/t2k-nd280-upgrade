#include <MemoryUsage.hxx>
#include "TH1F.h"
#include <algorithm>
#include <stdio.h>
#include <iostream>

MemoryUsage::MemoryUsage() :
  fEnabled(false) {
}

MemoryUsage::~MemoryUsage() {
}

void MemoryUsage::Enable(bool enable) {
  fEnabled = enable;
}

void MemoryUsage::LogMemory() {
  if (!fEnabled) {
    return;
  }
  ProcInfo_t info;
  gSystem->GetProcInfo(&info);
  fEventMemory.push_back(info);
}

void MemoryUsage::Write() {
  if (!fEnabled) {
    return;
  }

  // Create the histograms we need.
  int numBins = fEventMemory.size();
  TH1F hMemRes("memory_resident", "Resident memory usage", numBins, 0, numBins);
  hMemRes.GetXaxis()->SetTitle("Event");
  hMemRes.GetYaxis()->SetTitle("Resident memory (GB)");

  TH1F hMemVirt("memory_virtual", "Virtual memory usage", numBins, 0, numBins);
  hMemVirt.GetXaxis()->SetTitle("Event");
  hMemVirt.GetYaxis()->SetTitle("Virtual memory (GB)");

  // Convert the vector of memory usage to entries in the
  // histograms. Memory usage is converted from KB to GB
  // for easier human-based parsing of the numbers.
  int bin = 1;
  for (std::vector<ProcInfo_t>::iterator it = fEventMemory.begin(); it != fEventMemory.end(); it++, bin++) {
    ProcInfo_t info = (*it);
    hMemRes.SetBinContent(bin, info.fMemResident / (1024. * 1024.));
    hMemVirt.SetBinContent(bin, info.fMemVirtual / (1024. * 1024.));
  }

  hMemRes.Write();
  hMemVirt.Write();

  // Log the peak memory usage.
  double maxMemResident = (std::max_element(fEventMemory.begin(), fEventMemory.end(), CmpMemResident()))->fMemResident;
  double maxMemVirtual = (std::max_element(fEventMemory.begin(), fEventMemory.end(), CmpMemVirtual()))->fMemVirtual;

  std::cout << "Maximum resident memory usage: " << (maxMemResident / (1024. * 1024.)) << " GB" << std::endl;
  std::cout << "Maximum virtual memory usage:  " << (maxMemVirtual / (1024. * 1024.)) << " GB" << std::endl;
}
