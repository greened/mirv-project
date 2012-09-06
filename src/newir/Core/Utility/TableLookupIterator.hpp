#ifndef mirv_Core_Utility_TableLookupIterator_hpp
#define mirv_Core_Utility_TableLookupIterator_hpp

namespace mirv {
  namespace detail {
    template<typename T>
    T lookup(T *table, long long int i) {
      return table[i];
    }

    template<typename T, typename I>
    class LookerUpper {
    public:
      LookerUpper(const T *t) : table(t) {}

      bool operator()(typename I::value_type i) {
        return lookup(table, i);
      }

    private:
      const T *table;
    }
  }

  template<typename T, typename I>
  using TableLookupIterator =
    boost::transform_iterator<detail::LookerUpper<T, I>, I>;

  template<typename T, typename I>
  TableLookupIterator<T, I> makeTableLookupIterator(const T *table, I iter) {
    return TableLookupIterator<T, I>(table, iter);
  }
}

#endif
