#include <Actor/Player.h>
#include "Event/EventHelper.h"
#include <ScriptObject.h>

// Quest Script: ManFst004_00124
// Quest Name: Close to Home
// Quest ID: 65660
// Start NPC: 1001140
// End NPC: 1000100

class ManFst004 : public EventScript
{
private:
   // Basic quest information
   // Quest vars / flags used
   // GetQuestBitFlag8
   // GetQuestUI8AL
   // GetQuestUI8BH
   // GetQuestUI8BL
   // GetQuestUI8CH

   // Steps in this quest ( 0 is before accepting,
   // 1 is first, 255 means ready for turning it in
   enum Sequence : uint8_t
   {
      Seq0 = 0,
      Seq1 = 1,
      SeqFinish = 255,
   };

   // Quest rewards
   static constexpr auto RewardExpFactor = 100;
   static constexpr auto RewardGil = 107;

   // Entities found in the script data of the quest
   static constexpr auto Actor0 = 1001140;
   static constexpr auto Actor1 = 2;
   static constexpr auto Actor2 = 1000323;
   static constexpr auto Actor20 = 1000159;
   static constexpr auto Actor3 = 1000768;
   static constexpr auto Actor4 = 1000100;
   static constexpr auto BindActor0 = 6229224;
   static constexpr auto Item0 = 2000120;
   static constexpr auto LocActor0 = 1003159;
   static constexpr auto LocMarker01 = 2153091;
   static constexpr auto LocMarker02 = 2153104;
   static constexpr auto LocMarker03 = 2153111;
   static constexpr auto LocMarker04 = 2154539;
   static constexpr auto LocMarker05 = 2154540;
   static constexpr auto LocMarker06 = 2154541;
   static constexpr auto LocMarker07 = 2210446;
   static constexpr auto LocMarker08 = 2210454;
   static constexpr auto LocMarker09 = 2210461;
   static constexpr auto LocMotion0 = 799;
   static constexpr auto Poprange0 = 2280858;
   static constexpr auto RewardDesion = 1;
   static constexpr auto Seq0Actor0 = 0;
   static constexpr auto Seq0Actor0Lq = 50;
   static constexpr auto Seq1Actor0 = 4;
   static constexpr auto Seq1Actor1 = 1;
   static constexpr auto Seq1Actor1Wait = 51;
   static constexpr auto Seq1Actor2 = 2;
   static constexpr auto Seq1Actor3 = 3;
   static constexpr auto Seq1Actor3Npctradeno = 99;
   static constexpr auto Seq1Actor3Npctradeok = 100;
   static constexpr auto Seq2Actor4 = 5;
   static constexpr auto Territorytype0 = 132;
   static constexpr auto UnlockDesion = 14;

public:
   ManFst004() : EventScript( 65660 )
   { };

   ~ManFst004()
   { };

   //////////////////////////////////////////////////////////////////////
   // Event Handlers
   void onTalk( uint32_t eventId, Entity::Player& player, uint64_t actorId ) override
   {
      auto actor = Event::mapEventActorToRealActor( static_cast< uint32_t >( actorId ) );

      if( actor == ManFst004::Actor0 )
      {
         Scene00000( player );
      }
      else if( actor == ManFst004::Actor1 )
      {
         // starting the Aetheryte eventaction
         // player.eventStart( actorId, 0x050002, 7, 0, 0);
         // starting the eventaction 0x13 ( attuning )
         player.eventActionStart( 0x050002, 0x13,
            [&]( Entity::Player& player, uint32_t eventId, uint64_t additional )
            {
               player.sendQuestMessage( 0x050002, 0, 1, 0, 0 );
               player.registerAetheryte( 2 );
               player.learnAction( 1 );
               Scene00051( player );
            },
            [&]( Entity::Player& player, uint32_t eventId, uint64_t additional )
            { },
            eventId );
      }
      else if( actor == ManFst004::Actor2 )
      {
         Scene00002( player );
      }
      else if( actor == ManFst004::Actor3 )
      {
         Scene00003( player );
      }
      else if( actor == ManFst004::Actor4 )
      {
         Scene00005( player );
      }
   }

private:

   void checkQuestCompletion( Entity::Player& player, uint32_t varIdx )
   {
      if( varIdx == 3 )
      {
         player.sendQuestMessage( m_id, 1, 0, 0, 0 );
      } else if( varIdx == 2 )
      {
         player.sendQuestMessage( m_id, 2, 0, 0, 0 );
      } else
      {
         player.sendQuestMessage( m_id, 0, 0, 0, 0 );
      }

      auto questId = m_id;

      auto QUEST_VAR_ATTUNE = player.getQuestUI8AL( questId );
      auto QUEST_VAR_CLASS = player.getQuestUI8BH( questId );
      auto QUEST_VAR_TRADE = player.getQuestUI8BL( questId );

      if( QUEST_VAR_ATTUNE == 1 && QUEST_VAR_CLASS == 1 && QUEST_VAR_TRADE == 1 )
      {
         player.updateQuest( questId, SeqFinish );
      }
   }

   //////////////////////////////////////////////////////////////////////
   // Available Scenes in this quest, not necessarly all are used
   void Scene00000( Entity::Player& player )
   {
      player.playScene( m_id, 0, 0x2000,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            if( result.param2 == 1 ) // accept quest
            {
               Scene00050( player );
            }
         } );
   }

   void Scene00001( Entity::Player& player )
   {
      player.playScene( m_id, 1, 0,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            player.setQuestUI8AL( m_id, 1 );
            checkQuestCompletion( player, 0 );
         } );
   }

   void Scene00002( Entity::Player& player )
   {
      player.playScene( m_id, 2, 0,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            player.setQuestUI8BH( m_id, 1 );
            checkQuestCompletion( player, 3 );
         } );
   }

   void Scene00003( Entity::Player& player )
   {
      player.playScene( m_id, 3, 0,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            if( result.param2 == 1 )
            {
               Scene00100( player );
            } else
            {
               Scene00099( player );
            }
         } );
   }

   void Scene00004( Entity::Player& player )
   {
      player.playScene( m_id, 4, 0, 0, 0 );
   }

   void Scene00005( Entity::Player& player )
   {
      player.playScene( m_id, 5, FADE_OUT | CONDITION_CUTSCENE | HIDE_UI, 0, 0,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            if( result.param2 == 1 ) // finish quest
            {
               if( player.giveQuestRewards( m_id, 0 ) )
               {
                  player.finishQuest( m_id );
               }
            }
         } );
   }

   void Scene00050( Entity::Player& player )
   {
      player.playScene( m_id, 50, FADE_OUT | CONDITION_CUTSCENE | HIDE_UI, 0/*unk*/, 0/*unk*/,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            // accepting quest "close to home"
            player.updateQuest( m_id, 1 );
            player.setQuestUI8CH( m_id, 1 ); // receive key item
            // event is done, need to teleport to real zone.
            player.setZone( 132 );
            //player.setZone(183); back to starting griania for debug purpose
         } );
   }

   void Scene00051( Entity::Player& player )
   {
      player.playScene( m_id, 51, NONE,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            Scene00001( player );
         } );
   }

   void Scene00099( Entity::Player& player )
   {
      player.playScene( m_id, 99, NONE,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            Scene00004( player );
         } );
   }

   void Scene00100( Entity::Player& player )
   {
      player.playScene( m_id, 100, NONE,
         [&]( Entity::Player& player, const Event::SceneResult& result )
         {
            player.setQuestUI8CH( m_id, 0 ); // remove key item, since we have just traded it
            player.setQuestUI8BL( m_id, 1 );
            checkQuestCompletion( player, 2 );
         } );
   }

};