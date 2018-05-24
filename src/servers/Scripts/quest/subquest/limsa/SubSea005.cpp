#include <Script/NativeScriptApi.h>
#include <Actor/Player.h>
#include "Event/EventHelper.h"
#include <ScriptObject.h>

// Quest Script: SubSea005_00115
// Quest Name: Duty Calls
// Quest ID: 65651
// Start NPC: 1000898
// End NPC: 1001537

class SubSea005 : public EventScript
{
   private:
      // Basic quest information 
      // Quest vars / flags used
      // GetQuestUI8AL
      // GetQuestUI8BH

      enum Sequence : uint8_t
      {
         Seq0 = 0,
         Seq1 = 1,
         SeqFinish = 255,
      };

      // Quest rewards 
      static constexpr auto RewardExpFactor = 50;
      uint16_t RewardItem[2] = { 4551, 4555 };
      uint16_t RewardItemCount[2] = { 5, 5 };

      // Entities found in the script data of the quest
      static constexpr auto Actor0 = 1000898;
      static constexpr auto Actor1 = 1001023;
      static constexpr auto Actor2 = 1001537;
      static constexpr auto Item0 = 2000449;

   public:
      SubSea005() : EventScript( 65651 ){}; 
      ~SubSea005(){}; 

   void onTalk( uint32_t eventId, Entity::Player& player, uint64_t actorId ) override
   {
      auto actor = Event::mapEventActorToRealActor( actorId );

      if( actor == Actor0 )
      {
         Scene00000( player );
      }
      else if( actor == Actor1 )
      {
         Scene00002( player );
      }
      else if( actor == Actor2 )
      {
         Scene00004( player );
      }
   }

   private:

   void Scene00000( Entity::Player& player )
   {
      player.playScene( getId(), 0, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         if( result.param2 == 1 )
         {
            player.updateQuest( getId(), 1 );
            player.setQuestUI8BH( getId(), 2 );
         }
      } );
   }

   void Scene00001( Entity::Player& player )
   {
      player.playScene( getId(), 1, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         player.playScene( getId(), 1, 0, 0, 0 );
      } );
   }

   void Scene00002( Entity::Player& player )
   {
      player.playScene( getId(), 2, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         if( result.param2 == 1 )
         {
            Scene00003( player );
         }
      } );
   }

   void Scene00003( Entity::Player& player )
   {
      player.playScene( getId(), 3, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         player.setQuestUI8BH( getId(), 1 );
         player.sendQuestMessage( getId(), 0, 1, 0, 0 );
         player.updateQuest( getId(), 255 );
      } );
   }

   void Scene00004( Entity::Player& player )
   {
      player.playScene( getId(), 4, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         if( result.param2 == 1 )
         {
            Scene00005( player );
         }
      } );
   }

   void Scene00005( Entity::Player& player )
   {
      player.playScene( getId(), 5, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         if(result.param2 == 1 )
         {
            if( player.giveQuestRewards( getId(), 0 ) )
            {
               player.setQuestUI8BH( getId(),0 );
               player.finishQuest( getId() );
            }
         }
      } );
   }
};

