#ifndef mirv_core_filter_dataflow_hh
#define mirv_core_filter_dataflow_hh

namespace mirv {
  struct NullDataflow {
    struct Confluence {
      void operator()(NullDataflow &,
		      const NullDataflow &,
		      const NullDataflow &) {}
    };
    bool change(void) const {
      return false;
    }
  };
}

#endif
