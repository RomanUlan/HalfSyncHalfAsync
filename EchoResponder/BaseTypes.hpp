#ifndef BASE_TYPES_HPP
#define BASE_TYPES_HPP

#include "../EventEngines/HalfSyncHalfAsync.hpp"
#include "../EventEngines/SyncEventHandler.hpp"
#include "../EventEngines/AsyncEventHandler.hpp"

#include "EpollED.hpp"
#include "SocketES.hpp"

#include <string>

typedef HalfSyncHalfAsync<EpollED, SocketES, std::string> HSHA;
typedef SyncEventHandler<SocketES, std::string> SEH;
typedef AsyncEventHandler<SocketES, std::string> AEH;

#endif //BASE_TYPES_HPP

