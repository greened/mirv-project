// Test building of struct types.
//
// STDOUT: mdef testmodule {
// STDOUT:    tdecl teststruct {
// STDOUT:       int32
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/Print.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Node;
using mirv::Base;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::make;

namespace Builder = mirv::Builder;

using Builder::int_;
using Builder::float_;
using Builder::struct_;
using Builder::module;

int main(void)
{
  ptr<Node<Base> >::type code =
    Builder::translate(
      module["testmodule"] [
        struct_["teststruct"] [
          int_(32)
        ]
      ]
    );

  PrintFilter print(std::cout);
  
  print(code);

  return(0);
}
