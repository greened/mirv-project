#ifndef mirv_Library_Range_hpp
#define mirv_Library_Range_hpp

#include <boost/range/iterator_range_core.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/map.hpp>

#include <vector>

namespace mirv {
  template<typename Iterator>
  using Range = boost::iterator_range<Iterator>;

  template<typename Iterator>
  inline Range<Iterator> MakeRange(Iterator start, Iterator end) {
    return boost::make_iterator_range(start, end);
  }

  template<typename Range, typename Function>
  inline auto TransformRange(Range &&R, Function &&F) {
    return boost::adaptors::transform(std::forward<Range>(R),
                                      std::forward<Function>(F));
  }

  namespace {
    const auto Transformed = boost::adaptors::transformed;
    const auto MapValues = boost::adaptors::map_values;
  }
}

#endif
