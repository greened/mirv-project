#include <mirv/Core/Filter/FilterManager.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Node.hpp>

namespace mirv {
  namespace lib {
    template<> std::shared_ptr<FilterDependenceManager>
    Singleton<FilterDependenceManager>::inst = 0;
  }

  void FilterDependenceManager::registerFilter(FilterConstructor factory,
                                               const std::string &data) {
    instance().provides.insert(std::make_pair(data, factory));
  }

  void FilterManager::addRequired(const std::string &dependence) {
    LiveSet addedFilters;
    if (liveFilters.find(dependence) == liveFilters.end()) {
      FilterDependenceManager::const_iterator begin, end;
      std::tie(begin, end) =
        FilterDependenceManager::instance().providers(dependence);
      checkInvariant(begin != end, "No providers for " + dependence);

      // Just pick the frist one.
      add(begin->second());
      liveFilters.insert(dependence);
    }
  }

  void FilterManager::run(ptr<Node<Base> > node) {
    for (auto f : filters) {
      (*f)(node);
    }
  }
  void FilterManager::run(ptr<const Node<Base> > node) {
    for (auto f : filters) {
      (*f)(node);
    }
  }
}
