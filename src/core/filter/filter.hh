#ifndef mirv_core_filter_filter_hh
#define mirv_core_filter_filter_hh

#include <mirv/core/mem/heap.hh>

namespace mirv {
   template<typename Visited, typename Result = void>
   class Filter {
   public:
      typedef Result result_type;

     virtual result_type operator()(typename ptr<Visited>::type node) = 0;
   };
}

#endif
