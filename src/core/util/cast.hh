#ifndef MIRVCast_hh
#define MIRVCast_hh

template<typename To, typename From>
inline To safe_ptr_cast(From val)
{
  typedef boost::add_ref<boost::remove_ptr<To>::type>::type ref_type;

  if (DebugManager::active && DebugManager::instance().safe_cast()) {
    ref_type thisr(dynamic_cast<ref_type>(*val));
    return(&thisr);
  }
  else {
    return(static_cast<To>(this));
  }
}

#endif
