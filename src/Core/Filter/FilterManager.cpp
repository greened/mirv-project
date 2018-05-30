#include <mirv/Core/Filter/FilterManager.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>

namespace mirv {
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

  void FilterManager::run(ptr<Module> M) {
    for (auto f : filters) {
      f->run(M);
    }
  }
  void FilterManager::run(ptr<Function> Func) {
    for (auto f : filters) {
      f->run(Func);
    }
  }
  void FilterManager::run(ptr<Control> C) {
    for (auto f : filters) {
      f->run(C);
    }
  }
  void FilterManager::run(ptr<Producer> P) {
    for (auto f : filters) {
      f->run(P);
    }
  }
}
