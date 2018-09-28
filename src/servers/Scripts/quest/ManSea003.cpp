#include <Actor/Player.h>
#include "Event/EventHelper.h"
#include <ScriptObject.h>

// Quest Script: ManSea003_00109
// Quest Name: Close to Home
// Quest ID: 65645
// Start NPC: 1002697
// End NPC: 1000972

class ManSea003 :
  public EventScript
{
private:
  // Basic quest information
  // Quest vars / flags used
  // GetQuestBitFlag8
  // GetQuestUI8AL
  // GetQuestUI8BH
  // GetQuestUI8BL
  // GetQuestUI8CH

  enum Sequence :
    uint8_t
  {
    Seq0 = 0,
    Seq1 = 1,
    SeqFinish = 255,
  };

  // Quest rewards
  static constexpr auto RewardExpFactor = 100;
  static constexpr auto RewardGil = 107;

  // Entities found in the script data of the quest
  static constexpr auto Actor0 = 1002697;
  static constexpr auto Actor1 = 1001217;
  static constexpr auto Actor2 = 1000895;
  static constexpr auto Actor3 = 1000972;
  static constexpr auto Aetheryte0 = 8;
  static constexpr auto BindActor0 = 6229226;
  static constexpr auto Item0 = 2000105;
  static constexpr auto LocActor1 = 1001023;
  static constexpr auto LocFace0 = 604;
  static constexpr auto LocFace1 = 605;
  static constexpr auto LocPosCam1 = 4106696;
  static constexpr auto LocPosCam2 = 4106698;
  static constexpr auto Reward0 = 1;
  static constexpr auto Screenimage0 = 14;
  static constexpr auto UnlockDesion = 14;

public:
  ManSea003() :
    EventScript( 65645 )
  {
  };

  ~ManSea003()
  {
  };

  void onTalk( uint32_t eventId, Entity::Player& player, uint64_t actorId ) override
  {
    auto actor = Event::mapEventActorToRealActor( static_cast< uint32_t >( actorId ) );

    if( actor == Actor0 )
    {
      Scene00000( player );
    }
    else if( actor == Aetheryte0 )
    {
      player.eventActionStart( 0x050002, 0x13,
                               [ & ]( Entity::Player& player, uint32_t eventId, uint64_t additional )
                               {
                                 player.sendQuestMessage( 0x050002, 0, 1, 0, 0 );
                                 player.registerAetheryte( 2 );
                                 player.learnAction( 1 );
                                 Scene00002( player );
                               },
                               nullptr, eventId );
    }
    else if( actor == Actor1 )
    {
      Scene00004( player );
    }
    else if( actor == Actor2 )
    {
      Scene00006( player );
    }
    else if( actor == Actor3 )
    {
      Scene00007( player );
    }
  }

private:

  void checkQuestCompletion( Entity::Player& player, uint32_t varIdx )
  {
    if( varIdx == 1 )
    {
      player.sendQuestMessage( getId(), 1, 0, 0, 0 );
    }
    else if( varIdx == 2 )
    {
      player.sendQuestMessage( getId(), 2, 0, 0, 0 );
    }
    else
    {
      player.sendQuestMessage( getId(), 0, 0, 0, 0 );
    }

    auto questId = getId();

    auto QUEST_VAR_ATTUNE = player.getQuestUI8AL( questId );
    auto QUEST_VAR_CLASS = player.getQuestUI8BH( questId );
    auto QUEST_VAR_TRADE = player.getQuestUI8BL( questId );

    if( QUEST_VAR_ATTUNE == 1 && QUEST_VAR_CLASS == 1 && QUEST_VAR_TRADE == 1 )
    {
      player.updateQuest( questId, 255 );
    }
  }

  void Scene00000( Entity::Player& player )
  {
    player.playScene( getId(), 0, HIDE_HOTBAR,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        if( result.param2 == 1 )
                        {
                          Scene00001( player );
                        }
                      } );
  }

  void Scene00001( Entity::Player& player )
  {
    player.playScene( getId(), 1, FADE_OUT | CONDITION_CUTSCENE | HIDE_UI,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        player.updateQuest( getId(), 1 );
                        player.setQuestUI8CH( getId(), 1 );
                      } );
  }

  void Scene00002( Entity::Player& player )
  {
    player.playScene( getId(), 2, HIDE_HOTBAR,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        Scene00003( player );
                      } );
  }

  void Scene00003( Entity::Player& player )
  {
    player.playScene( getId(), 3, SET_EOBJ_BASE | HIDE_HOTBAR | INVIS_EOBJ,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        player.setQuestUI8BL( getId(), 1 );
                        checkQuestCompletion( player, 0 );
                      } );
  }

  void Scene00004( Entity::Player& player )
  {
    player.playScene( getId(), 4, HIDE_HOTBAR,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        if( result.param2 == 1 )
                        {
                          Scene00005( player );
                        }
                        else
                          return;
                      } );
  }

  void Scene00005( Entity::Player& player )
  {
    player.playScene( getId(), 5, SET_EOBJ_BASE | HIDE_HOTBAR | INVIS_EOBJ, 0, 0,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        player.setQuestUI8CH( getId(), 0 );
                        player.setQuestUI8BH( getId(), 1 );
                        checkQuestCompletion( player, 1 );
                      } );
  }

  void Scene00006( Entity::Player& player )
  {
    player.playScene( getId(), 6, HIDE_HOTBAR,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        player.setQuestUI8AL( getId(), 1 );
                        checkQuestCompletion( player, 2 );
                      } );
  }

  void Scene00007( Entity::Player& player )
  {
    player.playScene( getId(), 7, SET_EOBJ_BASE | HIDE_HOTBAR | INVIS_EOBJ, 0, 0,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                        if( result.param2 == 1 )
                        {
                          if( player.giveQuestRewards( getId(), 0 ) )
                          {
                            player.finishQuest( getId() );
                          }
                        }
                      } );
  }
};