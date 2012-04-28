#ifndef mirv_Core_Builder_Fusion_hpp
#define mirv_Core_Builder_Fusion_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>

#include <boost/proto/proto.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

#include <tr1/functional>

namespace mirv {
  namespace Builder {
    namespace detail {
      struct PopAndTranslateFusionExpressionSequence {
        template<typename Expr, typename OutputIterator>
        void operator()(ptr<SymbolTable>::type symtab, const Expr &expr, OutputIterator out) {
          TranslateToExpression<Expression<Base>> translator(symtab);
          typedef std::vector<ptr<Expression<Base> >::type> result_type;
          result_type results;
          void (result_type::*push_back)(const result_type::value_type &) = &result_type::push_back;

          boost::fusion::for_each(boost::fusion::transform(
                                    boost::fusion::pop_front(expr),
                                    translator),
                                  boost::bind(push_back,
                                              &results,
                                              _1));
          // TODO: This is inefficient.  Find a way to assign to the
          // output iterator directly.
          std::copy(results.begin(), results.end(), out);
        }
      };

      struct FlattenAndTranslateFusionExpressionSequence {
        template<typename Expr, typename OutputIterator>
        void operator()(ptr<SymbolTable>::type symtab, const Expr &expr, OutputIterator out) {
          TranslateToExpression<Expression<Base>> translator(symtab);
          typedef std::vector<ptr<Expression<Base> >::type> result_type;
          result_type results;
          void (result_type::*push_back)(const result_type::value_type &) = &result_type::push_back;

          boost::fusion::for_each(boost::fusion::transform(
                                    boost::proto::flatten(expr),
                                    translator),
                                  boost::bind(push_back,
                                              &results,
                                              _1));
          // TODO: This is inefficient.  Find a way to assign to the
          // output iterator directly.
          std::copy(results.begin(), results.end(), out);
        }
      };

      struct FlattenAndTranslateFusionTypeSequence {
        template<typename Expr, typename OutputIterator>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        const Expr &expr, OutputIterator out) {
          typedef std::vector<ptr<Symbol<Type<TypeBase> > >::const_type> result_type;
          TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
          result_type results;
          void (result_type::*push_back)(const result_type::value_type &) = &result_type::push_back;

          // std::cout << "Flattening and translating type:\n";
          // boost::proto::display_expr(expr);

          boost::fusion::for_each(boost::fusion::transform(
                                    boost::proto::flatten(expr), translator),
                                  boost::bind(push_back,
                                              &results,
                                              _1));
          // TODO: This is inefficient.  Find a way to assign to the
          // output iterator directly.
          std::copy(results.begin(), results.end(), out);
        }
      };

      struct PopAndTranslateFusionTypeSequence {
        template<typename Expr, typename OutputIterator>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        const Expr &expr, OutputIterator out) {
          typedef std::vector<ptr<Symbol<Type<TypeBase> > >::const_type> result_type;
          TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
          result_type results;
          void (result_type::*push_back)(const result_type::value_type &) = &result_type::push_back;

          // std::cout << "Popping and translating type:\n";
          // boost::proto::display_expr(expr);

          boost::fusion::for_each(boost::fusion::transform(
                                    boost::fusion::pop_front(expr), translator),
                                  boost::bind(push_back,
                                              &results,
                                              _1));
          // TODO: This is inefficient.  Find a way to assign to the
          // output iterator directly.
          std::copy(results.begin(), results.end(), out);
        }
      };

      struct PopFrontBackAndTranslateFusionTypeSequence {
        template<typename Expr, typename OutputIterator>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        const Expr &expr, OutputIterator out) {
          typedef std::vector<ptr<Symbol<Type<TypeBase> > >::const_type> result_type;
          TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
          result_type results;
          void (result_type::*push_back)(const result_type::value_type &) = &result_type::push_back;

          // std::cout << "Popping front back and translating type:\n";
          // boost::proto::display_expr(expr);

          boost::fusion::for_each(boost::fusion::transform(
                                    boost::fusion::pop_front(
                                      boost::fusion::pop_back(expr)),
                                    translator),
                                  boost::bind(push_back,
                                              &results,
                                              _1));
          // TODO: This is inefficient.  Find a way to assign to the
          // output iterator directly.
          std::copy(results.begin(), results.end(), out);
        }
      };
    }
  }
}

#endif
