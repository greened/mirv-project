#ifndef mirv_Core_Filter_FilterManager_hpp
#define mirv_Core_Filter_FilterManager_hpp

#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Library/MultiMap.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/Set.hpp>
#include <mirv/Library/Singleton.hpp>
#include <mirv/Library/Vector.hpp>

namespace mirv {
  class Control;
  class Function;
  class Module;
  class Producer;

  // This class manages the dependencies among filters.  Filters
  // register with the dependence manager, specifying which
  // information they provide.  The dependence manager helps the
  // filter manager determine what filters must be run to satisfy
  // requirements.
  class FilterDependenceManager : public Singleton<FilterDependenceManager> {
  private:
    typedef std::function<ptr<Filter>(void)> FilterConstructor;

    typedef MultiMap<std::string, FilterConstructor> ProvidesMap;

    ProvidesMap provides;

    FilterDependenceManager(void) {};

    friend Singleton<FilterDependenceManager>;

  public:
    static void registerFilter(FilterConstructor factory,
                               const std::string &data);
    template<typename ForwardRange>
    static void registerFilter(FilterConstructor factory,
                               ForwardRange data) {
      for (auto i : data) {
        registerFilter(factory, *i);
      }
    }

    typedef Range<ProvidesMap::const_iterator> range;

    range providers(const std::string &dependence) {
      return provides.equal_range(dependence);
    }
  };

  // This class holds a set of filters to run.  When a filter is
  // added, the filter manager will add any necessary filters to
  // satisfy requirements.
  class FilterManager {
  private:
    typedef Vector<ptr<Filter>> FilterList;
    FilterList filters;

    typedef Set<std::string> LiveSet;
    LiveSet liveFilters;

    void addRequired(const std::string &dependence);

  public:
    template<typename FilterType>
    void add(ptr<FilterType> filter) {
      for (auto i : filter->requires()) {
        addRequired(i);
      }

      filters.push_back(filter);

      for (auto k : filter->kills()) {
        liveFilters.erase(k);
      }
      for (auto p : filter->provides()) {
        liveFilters.insert(p);
      }
    }

    void run(ptr<Module> M);
    void run(ptr<Function> F);
    void run(ptr<Control> F);
    void run(ptr<Producer> F);
  };
}

#endif
