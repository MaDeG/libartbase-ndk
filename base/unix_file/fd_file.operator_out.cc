#include <iostream>

#include "../libartbase/base/unix_file/fd_file.h"

// This was automatically generated by art/tools/generate_operator_out.py --- do not edit!
namespace unix_file {
std::ostream& operator<<(std::ostream& os, FdFile::GuardState rhs) {
  switch (rhs) {
    case FdFile::GuardState::kBase: os << "Base"; break;
    case FdFile::GuardState::kFlushed: os << "Flushed"; break;
    case FdFile::GuardState::kClosed: os << "Closed"; break;
    case FdFile::GuardState::kNoCheck: os << "NoCheck"; break;
  }
  return os;
}
}  // namespace unix_file
