#include <Script/NativeScriptApi.h>
#include <Actor/Player.h>
#include "Event/EventHelper.h"
#include <ScriptObject.h>

// Quest Script: SubSea003_00113
// Quest Name: Proper Procedure
// Quest ID: 65649
// Start NPC: 1001209
// End NPC: 1001209

class SubSea003 : public EventScript
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
      static constexpr auto RewardGil = 103;

      // Entities found in the script data of the quest
      static constexpr auto ActionTimelineGobEventItem = 759;
      static constexpr auto ActionTimelineGobEventJoyBig = 945;
      static constexpr auto ActionTimelineGobEventTrouble = 944;
      static constexpr auto Actor0 = 1001209;
      static constexpr auto Actor1 = 1000898;
      static constexpr auto Item0 = 2000448;

   public:
      SubSea003() : EventScript( 65649 ){}; 
      ~SubSea003(){}; 

   void onTalk( uint32_t eventId, Entity::Player& player, uint64_t actorId ) override
   {
      auto actor = Event::mapEventActorToRealActor( actorId );

      if( actor == Actor0 && !player.hasQuest( getId() ) )
      {
         Scene00000( player );
      }
      else if( actor == Actor1 )
      {
         Scene00001( player );
      }
      else if ( actor == Actor0 && player.getQuestSeq( getId() == 255 ) )
      {
         Scene00003(player);
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
            player.setQuestUI8BH( getId(), 1 );
         }
      });
   }

   void Scene00001( Entity::Player& player )
   {
      player.playScene( getId(), 1, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         if( result.param2 == 1 )
         {
            Scene00002( player );
         }
      });
   }

   void Scene00002( Entity::Player& player )
   {
      player.playScene( getId(), 2, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         player.sendQuestMessage( getId(), 0, 1, 0, 0 );
         player.updateQuest( getId(), 255 );
      } );
   }

   void Scene00003( Entity::Player& player )
   {
      player.playScene( getId(), 3, HIDE_HOTBAR,
         [&]( Entity::Player& player, const Event::SceneResult& result )
      {
         if( result.param2 == 1 )
         {
            if(player.giveQuestRewards( getId(), 0 ) )
            {
               player.setQuestUI8BH( getId(), 0 );
               player.finishQuest( getId() );
            }
         }
      } );
   }
};

