#ifndef mirv_Core_Filter_Filter_hpp
#define mirv_Core_Filter_Filter_hpp

#include <mirv/Core/Memory/Heap.hpp>

namespace mirv {
  /// This is the base class for all IR filters.  A filter is
  /// essenetially a pass that examines and/or modifies the IR in some
  /// way.
   template<typename Visited, typename Result = void>
   class Filter {
   public:
      typedef Result result_type;

     /// Examine and/or operate on the given IR tree.
     virtual result_type operator()(typename ptr<Visited>::type node) = 0;
   };
}

#endif