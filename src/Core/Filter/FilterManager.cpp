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
      FilterDependenceManager::range providers =
        FilterDependenceManager::instance().providers(dependence);
      checkInvariant(!providers.empty(), "No providers for " + dependence);

      // Just pick the first one.
      add(providers.begin()->second());
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
