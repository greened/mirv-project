#ifndef mirv_core_filter_dataflow_hh
#define mirv_core_filter_dataflow_hh

namespace mirv {
  struct NullDataflow {
    struct Confluence {
      void operator()(NullDataflow &,
		      const NullDataflow &,
		      const NullDataflow &) {}
      typedef void result_type;
    };
    bool change(void) const {
      return false;
    }
  };
}

#endif
