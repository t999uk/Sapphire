#include <boost/format.hpp>

#include <Common.h>
#include <Network/CommonNetwork.h>
#include <Exd/ExdDataGenerated.h>
#include <Network/GamePacketNew.h>
#include <Network/PacketContainer.h>
#include <Network/CommonActorControl.h>
#include <Network/PacketDef/Zone/ClientZoneDef.h>
#include <Logging/Logger.h>

#include "Network/GameConnection.h"
#include "Network/PacketWrappers/ServerNoticePacket.h"
#include "Network/PacketWrappers/ActorControlPacket142.h"
#include "Network/PacketWrappers/ActorControlPacket143.h"
#include "Network/PacketWrappers/ActorControlPacket144.h"
#include "Network/PacketWrappers/MoveActorPacket.h"

#include "Network/PacketWrappers/PlayerStateFlagsPacket.h"

#include "DebugCommand/DebugCommandHandler.h"

#include "Action/Action.h"
#include "Action/ActionCast.h"
#include "Action/ActionMount.h"

#include "Script/ScriptMgr.h"

#include "Session.h"
#include "Framework.h"

extern Core::Framework g_fw;

using namespace Core::Common;
using namespace Core::Network::Packets;
using namespace Core::Network::Packets::Server;
using namespace Core::Network::ActorControl;

void Core::Network::GameConnection::actionHandler( const Packets::FFXIVARR_PACKET_RAW& inPacket,
                                                   Entity::Player& player )
{
  const auto packet = ZoneChannelPacket< Client::FFXIVIpcSkillHandler >( inPacket );

  const auto& type = packet.data().type;
  const auto& action = packet.data().actionId;
  const auto& useCount = packet.data().useCount;
  const auto& targetId = packet.data().targetId;

  player.sendDebug( "Skill type:" + std::to_string( type ) );

  auto pExdData = g_fw.get< Data::ExdDataGenerated >();
  auto pScriptMgr = g_fw.get< Scripting::ScriptMgr >();

  switch( type )
  {
    case Common::SkillType::Normal:

      if( action < 1000000 ) // normal action
      {
        std::string actionIdStr = boost::str( boost::format( "%|04X|" ) % action );
        player.sendDebug( "---------------------------------------" );
        player.sendDebug( "ActionHandler ( " + actionIdStr + " | " +
                          pExdData->get< Core::Data::Action >( action )->name +
                          " | " + std::to_string( targetId ) + " )" );

        player.queuePacket( makeActorControl142( player.getId(), ActorControlType::ActionStart, 0x01, action ) );

        if( action == 5 )
        {
          auto currentAction = player.getCurrentAction();

          // we should always have an action here, if not there is a bug
          assert( currentAction );
          currentAction->onStart();
        }
        else
        {
          Core::Entity::ActorPtr targetActor = player.getAsPlayer();

          if( targetId != player.getId() )
          {
            targetActor = player.lookupTargetById( targetId );
          }

          // Check if we actually have an actor
          if( !targetActor )
          {
            // todo: interrupt a cast.
            player.sendDebug( "Invalid target." );
            return;
          }

          if( !player.actionHasCastTime( action ) )
          {
            pScriptMgr->onCastFinish( player, targetActor->getAsChara(), action );
          }
          else
          {
            auto pActionCast = Action::make_ActionCast( player.getAsPlayer(), targetActor->getAsChara(), action );
            player.setCurrentAction( pActionCast );
            player.sendDebug( "setCurrentAction()" );
            player.getCurrentAction()->onStart();
          }
        }
      }
      else if( action < 2000000 ) // craft action
      {

      }
      else if( action < 3000000 ) // item action
      {
        auto info = pExdData->get< Core::Data::EventItem >( action );
        if( info )
        {
          pScriptMgr->onEventItem( player, action, info->quest, info->castTime, targetId );
        }
      }
      else if( action > 3000000 ) // unknown
      {

      }

      break;

    case Common::SkillType::MountSkill:

      player.sendDebug( "Request mount " + std::to_string( action ) );

      auto pActionMount = Action::make_ActionMount( player.getAsPlayer(), action );
      player.setCurrentAction( pActionMount );
      player.sendDebug( "setCurrentAction()" );
      player.getCurrentAction()->onStart();

      break;

  }

}
