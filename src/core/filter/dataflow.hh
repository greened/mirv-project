#ifndef mirv_core_filter_dataflow_hh
#define mirv_core_filter_dataflow_hh

namespace mirv {
  /// This is a dataflow class that carries no information.
  struct NullDataflow {
    struct Confluence {
      void operator()(NullDataflow &,
		      const NullDataflow &,
		      const NullDataflow &) {}
      typedef void result_type;
    };
    /// No data never changes.
   bool change(void) const {
      return false;
    }
  };
}

#endif
