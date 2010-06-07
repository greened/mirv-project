#ifndef mirv_Core_Builder_SequenceRules_hpp
#define mirv_Core_Builder_SequenceRules_hpp

#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/Builder/DomainFwd.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Keywords.hpp>
#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match a sequence of statements.  The
     /// comma serves the same purpose as the semicolon in C-like
     /// languages.
     typedef boost::proto::comma<ConstructStatementGrammar, ConstructStatementGrammar> CommaRule;
   }
}

#endif
