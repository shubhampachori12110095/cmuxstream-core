/*
 * See "Practical Hash Functions for Similarity Estimation
 * and Dimensionality Reduction" for a study on various hash
 * functions: MurmurHash3 works as well as a theoretically
 * "regular" hash function proposed in (Dahlgaard, FOCS 2015).
 */
#include <iostream>
#include <limits>
#include <math.h>
#include "MurmurHash3.h"
#include "param.h"
#include <random>
#include <vector>

namespace std {

  void
  streamhash_init_seeds(vector<vector<uint64_t>>& h, mt19937_64& prng) {
    uint c = h.size();
    uint k = h[0].size();
    for (uint c_i = 0; c_i < c; c_i++) {
      for (uint k_i = 0; k_i < k; k_i++) {
        h[c_i][k_i] = prng();
      }
    }
  }

  float
  streamhash_compute_constant(float density, uint k) {
    return sqrt(static_cast<double>(1.0/(density * k)));
  }

  float
  streamhash_hash(string& s, uint64_t seed, float density, float constant) {
    uint64_t hash_value[2]; // 128 bits
    int len = s.length();

    MurmurHash3_x64_128(&s, len, seed, &hash_value);

    float hash_value_f = static_cast<float>(hash_value[0]);
    float max_value_f = static_cast<float>((uint64_t)-1);
    hash_value_f = hash_value_f/max_value_f;

    if (hash_value_f <= density/2.0) {
      return -1.0 * constant;
    } else if (hash_value_f <= density) {
      return constant;
    } else {
      return 0.0;
    }
  }

  float
  streamhash_empirical_density(string& s, mt19937_64& prng,
                               float density, float constant) {
    int ntrials = 100000;
    float nonzeros = 0.0;
    for (int i = 0; i < ntrials; i++) {
      if (abs(streamhash_hash(s, prng(), density, constant)) > EPSILON) {
        nonzeros++;
      }
    }
    return nonzeros/ntrials; 
  }

}
