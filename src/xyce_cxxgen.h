#ifndef __xyce_cxxgen_h
#define __xyce_cxxgen_h

#include <sstream>
template <typename T>
void BuildFormatString(std::ostringstream& builder,
                       const T& fmt_spec) {
    builder << fmt_spec;
}

template <typename T, typename... Types>
void BuildFormatString(std::ostringstream& builder,
                       const string_t& fmt_spec,
                       const T& first,
                       const Types&... args) {
    auto pos = fmt_spec.find("{}");
    if(pos == string_t::npos) {
        builder << fmt_spec;
        return;
    }
    builder.write(fmt_spec.data(), pos);
    builder << first;
    BuildFormatString(builder, fmt_spec.substr(pos + 2), args...);
}
//python .format(...) style string format
template <typename... Types>
const string_t str_format(const string_t& fmt_spec,
                               const Types&... args) {
    std::ostringstream builder;
    BuildFormatString(builder, fmt_spec, args...);
    return builder.str();
}

typedef struct _instanceInfo {
  int numExtVars;       //external nodes number
  int numIntVars;       //internal nodes number
  int numBRAs;          //BRA items, like li_BRA_**
  intPair fNodePtrIndex; //start and end index of fNodePtrs in m_variables
  intPair qNodePtrIndex;
  intPair mNodeOffsetIndex;
  strVec m_variables;   //member variable needed to be initialized for Class Instance
  strVec m_probeConsts;   //probe node IDs
  strPairVec stampNodeMtrix; //node stamp matrix vector with node pair
} instanceInfo;

typedef struct _flagDict {
  string_t flag;
  string_t val;
}flagDict;
#endif

