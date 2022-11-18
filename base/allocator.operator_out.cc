#include <iostream>

#include "../libartbase/base/allocator.h"

// This was automatically generated by art/tools/generate_operator_out.py --- do not edit!
namespace art {
std::ostream& operator<<(std::ostream& os, AllocatorTag rhs) {
  switch (rhs) {
    case kAllocatorTagHeap: os << "AllocatorTagHeap"; break;
    case kAllocatorTagMonitorList: os << "AllocatorTagMonitorList"; break;
    case kAllocatorTagClassTable: os << "AllocatorTagClassTable"; break;
    case kAllocatorTagInternTable: os << "AllocatorTagInternTable"; break;
    case kAllocatorTagMaps: os << "AllocatorTagMaps"; break;
    case kAllocatorTagLOS: os << "AllocatorTagLOS"; break;
    case kAllocatorTagSafeMap: os << "AllocatorTagSafeMap"; break;
    case kAllocatorTagLOSMaps: os << "AllocatorTagLOSMaps"; break;
    case kAllocatorTagReferenceTable: os << "AllocatorTagReferenceTable"; break;
    case kAllocatorTagHeapBitmap: os << "AllocatorTagHeapBitmap"; break;
    case kAllocatorTagHeapBitmapLOS: os << "AllocatorTagHeapBitmapLOS"; break;
    case kAllocatorTagMonitorPool: os << "AllocatorTagMonitorPool"; break;
    case kAllocatorTagLOSFreeList: os << "AllocatorTagLOSFreeList"; break;
    case kAllocatorTagVerifier: os << "AllocatorTagVerifier"; break;
    case kAllocatorTagRememberedSet: os << "AllocatorTagRememberedSet"; break;
    case kAllocatorTagModUnionCardSet: os << "AllocatorTagModUnionCardSet"; break;
    case kAllocatorTagModUnionReferenceArray: os << "AllocatorTagModUnionReferenceArray"; break;
    case kAllocatorTagJNILibraries: os << "AllocatorTagJNILibraries"; break;
    case kAllocatorTagCompileTimeClassPath: os << "AllocatorTagCompileTimeClassPath"; break;
    case kAllocatorTagOatFile: os << "AllocatorTagOatFile"; break;
    case kAllocatorTagDexFileVerifier: os << "AllocatorTagDexFileVerifier"; break;
    case kAllocatorTagRosAlloc: os << "AllocatorTagRosAlloc"; break;
    case kAllocatorTagCount: os << "AllocatorTagCount"; break;
    default: os << "AllocatorTag[" << static_cast<int>(rhs) << "]"; break;
  }
  return os;
}
}  // namespace art

