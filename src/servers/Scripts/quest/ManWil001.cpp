#include <Actor/Player.h>
#include "Event/EventHelper.h"
#include <ScriptObject.h>

// Quest Script: ManWil001_00594
// Quest Name: Coming to Ul'dah
// Quest ID: 66130
// Start NPC: 1003987
// End NPC: 1003988

class ManWil001 : public EventScript
{
private:
   static constexpr auto SEQ_0 = 0;
   static constexpr auto SEQ_FINISH = 255;

   static constexpr auto ACTOR0 = 1003987;
   static constexpr auto ACTOR1 = 1003988;
   static constexpr auto ACTOR20 = 1001285;
   static constexpr auto CUT_EVENT = 188;
   static constexpr auto EOBJECT0 = 2001681;
   static constexpr auto EOBJECT1 = 2001682;
   static constexpr auto EOBJECT2 = 2001683;
   static constexpr auto EOBJECT3 = 2001706;
   static constexpr auto EVENT_ACTION_SEARCH = 1;
   static constexpr auto OPENING_EVENT_HANDLER = 1245187;

   void Scene00000( Entity::Player& player )
   {
      auto callback = [this]( Entity::Player& player, const Event::SceneResult& result )
      {
         if( result.param2 == 1 )
         {
            player.setOpeningSequence( 2 );
            Scene00001( player );
         }
      };

      player.playScene( getId(), 0, HIDE_HOTBAR, 0, 0, callback );
   }

   void Scene00001( Entity::Player& player )
   {
      auto callback = [this]( Entity::Player& player, const Event::SceneResult& result )
      {
         Scene00002( player );
      };

      player.playScene( getId(), 1, DISABLE_SKIP | HIDE_HOTBAR | SET_BASE, 0, 0, callback );
   }

   void Scene00002( Entity::Player& player )
   {
      auto callback = [this]( Entity::Player& player, const Event::SceneResult& result )
      {
         player.playScene( OPENING_EVENT_HANDLER, 0x1E, HIDE_HOTBAR | NO_DEFAULT_CAMERA, 0, 0 );
      };

      player.playScene( getId(), 2, NONE, 0, 0, callback );
   }

   void Scene00003( Entity::Player& player )
   {
      player.playScene( getId(), 3, NONE, 0, 0 );
   }

   void Scene00004( Entity::Player& player )
   {
      player.playSceneChain( getId(), 4, 0x2c02, bindScene( &ManWil001::Scene00005 ) );
   }

   void Scene00005( Entity::Player& player )
   {
      auto callback = [this]( Entity::Player& player, const Event::SceneResult& result )
      {
         if( result.param2 == 1 )
         {
            if( player.giveQuestRewards( getId(), 0 ))
               player.finishQuest( getId());
         }
      };

      player.playScene( getId(), 5, INVIS_OTHER_PC, 0, 0, callback );
   }

   void Scene00006( Entity::Player& player )
   {
      player.playScene( getId(), 6, 0, 0, 0 );
   }

   void Scene00007( Entity::Player& player )
   {
      player.playScene( getId(), 7, 0, 0, 0 );
   }

   void Scene00008( Entity::Player& player )
   {
      player.playScene( getId(), 8, 0, 0, 0 );
   }

   void Scene00009( Entity::Player& player )
   {
      player.playScene( getId(), 9, 0, 0, 0 );
   }

   void Scene00010( Entity::Player& player )
   {
      player.playScene( getId(), 10, 0, 0, 0 );
   }

   void Scene00011( Entity::Player& player )
   {
      player.playScene( getId(), 11, 0, 0, 0 );
   }

   void Scene00012( Entity::Player& player )
   {
      player.playScene( getId(), 12, 0, 0, 0 );
   }

   void Scene00013( Entity::Player& player )
   {
      player.playScene( getId(), 13, 0, 0, 0 );
   }


public:
   ManWil001() : EventScript( 66130 )
   {}

   void onTalk( uint32_t eventId, Entity::Player& player, uint64_t actorId ) override
   {
      auto actor = Event::mapEventActorToRealActor( static_cast< uint32_t >( actorId ) );

      if( actor == ACTOR0 )
         Scene00000( player );
      else if( actor == ACTOR1 )
         Scene00004( player );
   }
};