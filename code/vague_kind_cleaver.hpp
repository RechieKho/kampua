#ifndef VAGUE_KIND_CLEAVER
#define VAGUE_KIND_CLEAVER

#include "kind_cleaver.hpp"

template <typename T>
class VagueKindCleaver : public KindCleaver<T, cleaver_option_underlying_type> {
 public:
 private:
 public:
  attribute_type make_result(cleaver_option_underlying_type p_option,
                             chunk_type p_chunk) override {
    return p_option;
  }
};

#endif  // VAGUE_KIND_CLEAVER