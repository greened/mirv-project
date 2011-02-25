#include <mirv/Core/Builder/MultiplyOperators.hpp>
#include <mirv/Core/Builder/AddOperators.hpp>
#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ConstExpressionFlow.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/ConstSymbolFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/IR/TupleType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

#include <sstream>

namespace mirv {
  namespace detail {
    namespace {
      /// This is essentially a specialized print filter for
      /// expressions.
      class ExpressionPrintFilter : public ConstFilter<Node<Base> > {
      private:
        typedef std::ostream Stream;
        /// A handle to the output stream.
        Stream &out;

        /// Define the inherited attibute.
        class InheritedAttribute {
        private:
          /// The stream to dump to.
          Stream *ot;
          
        public:
          InheritedAttribute(void) : ot(0) {}
          InheritedAttribute(Stream &o) : ot(&o) {}

          Stream &out(void) const {
            checkInvariant(ot != 0, "No output stream");
            return *ot;
          }
        };

        struct SynthesizedAttribute {};

        typedef FlowAttributeManager<
          InheritedAttribute,
          SynthesizedAttribute
          > FlowAttributeManagerType;

        /// Entering each expression
        class EnterExpressionVisitor : public ConstExpressionVisitor {
        private:
          FlowAttributeManagerType &attributeManager;

          template<typename ValueType>
          void visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr);

        public:
          EnterExpressionVisitor(FlowAttributeManagerType &am)
              : attributeManager(am) {}

          void visit(ptr<Expression<Add> >::const_type expr);
          void visit(ptr<Expression<Subtract> >::const_type expr);
          void visit(ptr<Expression<Multiply> >::const_type expr);
          void visit(ptr<Expression<Divide> >::const_type expr);
          void visit(ptr<Expression<Modulus> >::const_type expr);
          void visit(ptr<Expression<Negate> >::const_type expr);
          void visit(ptr<Expression<LogicalAnd> >::const_type expr);
          void visit(ptr<Expression<LogicalOr> >::const_type expr);
          void visit(ptr<Expression<LogicalNot> >::const_type expr);
          void visit(ptr<Expression<BitwiseAnd> >::const_type expr);
          void visit(ptr<Expression<BitwiseOr> >::const_type expr);
          void visit(ptr<Expression<BitwiseComplement> >::const_type expr);
          void visit(ptr<Expression<LessThan> >::const_type expr);
          void visit(ptr<Expression<LessThanOrEqual> >::const_type expr);
          void visit(ptr<Expression<Equal> >::const_type expr);
          void visit(ptr<Expression<NotEqual> >::const_type expr);
          void visit(ptr<Expression<GreaterThan> >::const_type expr);
          void visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr);
          void visit(ptr<Expression<Reference<Variable> > >::const_type expr);
          void visit(ptr<Expression<Reference<Function> > >::const_type expr);
          void visit(ptr<Expression<Reference<Array> > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr);
          void visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr);
        };

        class EnterExpressionAction : public VisitAction<EnterExpressionVisitor> {
        public:
          EnterExpressionAction(FlowAttributeManagerType &attributeManager) 
              : VisitAction<EnterExpressionVisitor>(attributeManager) {}
        };

        /// This is the flow to print expressions.
        class PrintExpressionFlow : public AttributeFlow<
          InheritedAttribute,
          SynthesizedAttribute,
          ConstForwardExpressionFlowGenerator,
          EnterExpressionAction,
          NullAction,
          NullAction,
          NullAction,
          NullAction
          > {
        private:
          typedef AttributeFlow<
          InheritedAttribute,
          SynthesizedAttribute,
          ConstForwardExpressionFlowGenerator,
          EnterExpressionAction,
          NullAction,
          NullAction,
          NullAction,
          NullAction
          > BaseType;
          
        public:
          PrintExpressionFlow(Stream &out)
              : BaseType(InheritedAttribute(out)) {}

          PrintExpressionFlow(FlowAttributeManagerType &)
              : BaseType(InheritedAttribute()) {}        

        };

      public:
        ExpressionPrintFilter(Stream &o) : out(o) {}

        void operator()(ptr<Node<Base> >::const_type node) {
          ptr<Expression<Base> >::const_type e =
            safe_cast<const Expression<Base> >(node);
          ptr<ConstExpressionVisitor>::type flow(new PrintExpressionFlow(out));
          e->accept(*flow);
        }
      };

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Add> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "+ ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Subtract> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "- ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Multiply> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "* ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Divide> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "/ ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Modulus> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "% ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Negate> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "neg ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "&& ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalOr> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "|| ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalNot> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "! ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "& ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "| ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "~ ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThan> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "< ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "<= ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Equal> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "== ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<NotEqual> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "!= ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThan> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "> ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << ">= ";
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << expr->getSymbol()->name() << ' ';
      }

      template<typename ValueType>
      void ExpressionPrintFilter::EnterExpressionVisitor::visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << expr->getSymbol()->type()->name()
          << ' ' << expr->getSymbol()->valueString() << ' ';
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr)
      {
        visitConstant(expr);
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << expr->getSymbol()->name() << ' ';
      }

      void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Array> > >::const_type expr)
      {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "aref ";
      }
    }

    std::string stringize(ptr<Expression<Base> >::type expr)
    {
      std::stringstream stream;
      ExpressionPrintFilter print(stream);
      return stream.str();
    }
  }

  void
  Tuple::Interface::construct_optimized(ptr<Symbol<Type<TypeBase> > >::const_type t,
                                        std::uint64_t count) 
  {
    push_back(t);
    multiplier =
      Builder::makeExpression(count, this->parent<Symbol<Module> >());
  }

  Tuple::Interface::BitSizeType Tuple::Interface::bitsize(void) const
  {
    if (empty()) {
      return Builder::makeExpression(0, this->parent<Symbol<Module> >());
    }

    if (multiplier) {
      BitSizeType bitsize = (*begin())->bitsize();
      return bitsize * multiplier;
    }
    BitSizeType expr =
      Builder::makeExpression(0, this->parent<Symbol<Module> >());
    for (auto m = begin(); m != end(); ++m) {
      expr = expr + (*m)->bitsize();
    }
    return expr;
  }
}
