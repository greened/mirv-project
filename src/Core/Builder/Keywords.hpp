#ifndef mirv_Core_Builder_Keywords_hpp
#define mirv_Core_Builder_Keywords_hpp

namespace mirv {
  namespace Builder {
    namespace keyword {
      /// This is a proto tag to build a terminal symbol to kick off
      /// module construction.
      struct module {
        friend std::ostream& operator<<(std::ostream& sout, module) {
          return sout << "module";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// function construction.
      struct func {
        friend std::ostream& operator<<(std::ostream& sout, func) {
          return sout << "func";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// variable construction.
      struct var {
        friend std::ostream& operator<<(std::ostream& sout, var) {
          return sout << "var";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// type lookup.
      struct type {
        friend std::ostream& operator<<(std::ostream& sout, type) {
          return sout << "type";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// struct type construction.
      struct struct_ {
        friend std::ostream& operator<<(std::ostream& sout, struct_) {
          return sout << "struct_";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// integral type construction.
      struct int_ {
        friend std::ostream& operator<<(std::ostream& sout, int_) {
          return sout << "int_";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// floating point type construction.
      struct float_ {
        friend std::ostream& operator<<(std::ostream& sout, float_) {
          return sout << "float_";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// type construction when we need a way to specify no type.
      /// For example, conmstructing a function type with no return
      /// value.
      struct void_ {
        friend std::ostream& operator<<(std::ostream& sout, void_) {
          return sout << "void_";
        }
      };
      /// This is a proto tag to build a terminal symbol to represent
      /// a vararg function argument.
      struct vararg {
        friend std::ostream& operator<<(std::ostream& sout,
                                        vararg) {
          return sout << "vararg";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// if-then and if-else statement construction.
      struct if_ {
        friend std::ostream& operator<<(std::ostream& sout, if_) {
          return sout << "if_";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// building of the else portion of an if-else statement.
      struct else_ { 
        friend std::ostream& operator<<(std::ostream& sout, else_) {
          return sout << "else_";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// do-while statement construction.
      struct do_ {
        friend std::ostream& operator<<(std::ostream& sout, do_) {
          return sout << "do_";
        }
      };
      /// This is a proto tag to build a terminal symbol to kick off
      /// do-while statement construction specified as a C-like while
      /// statement.  It also serves to kick off construction of the
      /// while portion of a do-while statement.
      struct while_ {
        friend std::ostream& operator<<(std::ostream& sout, while_) {
          return sout << "while_";
        }
      };
    }
  }
}

#endif
