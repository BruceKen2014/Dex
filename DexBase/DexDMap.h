#ifndef _DEX_DMAP_H
#define _DEX_DMAP_H

#include <map>

#define DMap  std::map
#include "DexString.h"
typedef DMap<DString, DString> DMapStrStr;
#endif