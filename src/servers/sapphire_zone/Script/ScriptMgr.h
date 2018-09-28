#ifndef _ScriptMgr_H_
#define _ScriptMgr_H_

#include <boost/shared_ptr.hpp>
#include <mutex>
#include <set>

#include <Common.h>
#include "Forwards.h"

namespace Core {
namespace Scripting {

class ScriptMgr
{
private:
  /*!
   * @brief A shared ptr to NativeScriptMgr, used for accessing and managing the native script system.
   */
  boost::shared_ptr< NativeScriptMgr > m_nativeScriptMgr;

  std::function< std::string( Entity::Player & ) > m_onFirstEnterWorld;

  /*!
   * @brief Used to ignore the first change notification that Watchdog emits.
   * Because reasons, it likes to emit an initial notification with all the files that match the filter, we don't want that so we ignore it.
   */
  bool m_firstScriptChangeNotificiation;

public:
  ScriptMgr();

  ~ScriptMgr();

  /*!
   * @brief Loads all the script modules and readies the ScriptMgr
   *
   * This gets all the modules inside the specified scripts folder and then attempts to load each one.
   * After that, it starts the directory watcher so the server can reload modules at runtime when changes occur.
   *
   * @return true if init success
   */
  bool init();

  /*!
   * @brief Called on each tick or at a regular interval. Allows for the NativeScriptMgr to process module loading and reloading.
   */
  void update();

  /*!
   * @brief Registers a directory watcher which allows for script modules to be reloaded when changes to the modules occur
   */
  void watchDirectories();

  void onPlayerFirstEnterWorld( Entity::Player& player );

  bool onTalk( Entity::Player& player, uint64_t actorId, uint32_t eventId );

  bool onEnterTerritory( Entity::Player& player, uint32_t eventId, uint16_t param1, uint16_t param2 );

  bool onWithinRange( Entity::Player& player, uint32_t eventId, uint32_t param1, float x, float y, float z );

  bool onOutsideRange( Entity::Player& player, uint32_t eventId, uint32_t param1, float x, float y, float z );

  bool onEmote( Entity::Player& player, uint64_t actorId, uint32_t eventId, uint8_t emoteId );

  bool onEventItem( Entity::Player& player, uint32_t eventItemId, uint32_t eventId, uint32_t castTime,
                    uint64_t targetId );

  bool onMobKill( Entity::Player& player, uint16_t nameId );

  bool onCastFinish( Entity::Player& pPlayer, Entity::CharaPtr pTarget, uint32_t actionId );

  bool onStatusReceive( Entity::CharaPtr pActor, uint32_t effectId );

  bool onStatusTick( Entity::CharaPtr pActor, Core::StatusEffect::StatusEffect& effect );

  bool onStatusTimeOut( Entity::CharaPtr pActor, uint32_t effectId );

  bool onZoneInit( ZonePtr pZone );

  bool onEventHandlerReturn( Entity::Player& player, uint32_t eventId, uint16_t subEvent, uint16_t param1,
                             uint16_t param2, uint16_t param3 );

  bool onEventHandlerTradeReturn( Entity::Player& player, uint32_t eventId, uint16_t subEvent, uint16_t param,
                                  uint32_t catalogId );

  bool onInstanceInit( InstanceContentPtr instance );

  bool onInstanceUpdate( InstanceContentPtr instance, uint32_t currTime );

  bool
  onInstanceEnterTerritory( InstanceContentPtr instance, Entity::Player& player, uint32_t eventId, uint16_t param1,
                            uint16_t param2 );

  bool loadDir( const std::string& dirname, std::set< std::string >& files, const std::string& ext );

  NativeScriptMgr& getNativeScriptHandler();
};
}
}
#endif
