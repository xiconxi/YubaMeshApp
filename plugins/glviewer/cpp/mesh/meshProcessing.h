#ifndef CORE_MESPROCESSING_H
#define CORE_MESPROCESSING_H
#include "PickableMesh.h"
#include <functional>
#include <set>

namespace YbCore {

template<class Key>
using PairKey = std::pair<Key,Key>;

template<class Key>
using PairKeyCompare = std::function<bool(const PairKey<Key>&,const PairKey<Key>&)>;

template<class Key,class Value>
using PairKeyMap = std::map<PairKey<Key>, Value, PairKeyCompare<Key> >;

template<class Key>
using PairKeySet = std::set<PairKey<Key>, PairKeyCompare<Key> >;

LIBSHARED_EXPORT void centerlized(PickableMesh* ref) ;

LIBSHARED_EXPORT void calculateNorm(PickableMesh* ref);

LIBSHARED_EXPORT bool isFaceInersected(PickableMesh* mesh,glm::ivec3 f, glm::vec3 n, float d);

LIBSHARED_EXPORT std::array<glm::vec3,2> getFaceIntersection(PickableMesh* mesh,glm::ivec3 f, glm::vec3 n, float d);

LIBSHARED_EXPORT std::array<std::vector<glm::ivec3>::iterator,2> getSliceInterval(PickableMesh*mesh, glm::vec3 n, float d, float gap);

LIBSHARED_EXPORT void sortByVector(PickableMesh* mesh, glm::vec3 x);

LIBSHARED_EXPORT glm::vec3 pca_analysic(PickableMesh* mesh, uint r_id);

}

#endif // CORE_MESPROCESSING_H
