#ifndef BASE_TYPES_HPP
#define BASE_TYPES_HPP

#include "../EventEngines/HalfSyncHalfAsync.hpp"
#include "../EventEngines/SyncEventHandler.hpp"
#include "../EventEngines/AsyncEventHandler.hpp"

#include "EpollED.hpp"
#include "SocketES.hpp"

#include <string>

typedef HalfSyncHalfAsync<std::string> HSHA;
typedef SyncEventHandler<std::string> SEH;
typedef AsyncEventHandler<std::string> AEH;

#endif //BASE_TYPES_HPP

