#ifndef MIRV_Core_Filter_Filter_hh
#define MIRV_Core_Filter_Filter_hh

namespace MIRV {
   template<typename Visited, typename Result = void>
   class Filter {
   public:
      typedef Result result_type;

      virtual result_type operator()(Visited &node) = 0;
   };
}

#endif
