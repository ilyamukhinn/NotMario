#ifndef ENUMS_H
#define ENUMS_H

#include <string>
#include <unordered_map>
#include <type_traits>

template<typename Enum>
class EnumMask {
public:
    using Underlying = typename std::underlying_type<Enum>::type;

    EnumMask() : value(0) {}
    EnumMask(Enum e) : value(static_cast<Underlying>(e)) {}

    EnumMask& operator|=(EnumMask other) {
        value |= other.value;
        return *this;
    }

    EnumMask& operator&=(EnumMask other) {
        value &= other.value;
        return *this;
    }

    EnumMask& operator^=(EnumMask other) {
        value ^= other.value;
        return *this;
    }

    EnumMask operator|(EnumMask other) const {
        return EnumMask(static_cast<Enum>(value | other.value));
    }

    EnumMask operator&(EnumMask other) const {
        return EnumMask(static_cast<Enum>(value & other.value));
    }

    EnumMask operator^(EnumMask other) const {
        return EnumMask(static_cast<Enum>(value ^ other.value));
    }

    EnumMask operator~() const {
        return EnumMask(static_cast<Enum>(~value));
    }

    bool has(EnumMask mask) const {
        return (value & mask.value) == mask.value;
    }

private:
    Underlying value;
};

enum class SceneTag : uint32_t {
    UNDEFINED   = 0,
    MENU        = 1 << 0,
    PLAY        = 1 << 1
};

static std::unordered_map<std::string, SceneTag> const strToSceneTag = {
    { "Undefined",  SceneTag::UNDEFINED },
    { "Menu",       SceneTag::MENU },
    { "Play",       SceneTag::PLAY }
};

static std::unordered_map<SceneTag, std::string> const sceneTagToStr = {
    { SceneTag::UNDEFINED,  "Undefined" },
    { SceneTag::MENU,       "Menu" },
    { SceneTag::PLAY,       "Play" }
};

enum class EntityTag : uint32_t {
    UNDEFINED   = 0,
    PLAYER      = 1 << 0,
    BULLET      = 1 << 1,
    TILE        = 1 << 2,
    DEC         = 1 << 3,
    COIN        = 1 << 4
};

static std::unordered_map<std::string, EntityTag> const strToEntityTag = {
    { "Undefined",  EntityTag::UNDEFINED },
    { "Player",     EntityTag::PLAYER },
    { "Bullet",     EntityTag::BULLET },
    { "Tile",       EntityTag::TILE },
    { "Dec",        EntityTag::DEC }
};

static std::unordered_map<EntityTag, std::string> const entityTagToStr = {
    { EntityTag::UNDEFINED, "Undefined" },
    { EntityTag::PLAYER,    "Player" },
    { EntityTag::BULLET,    "Bullet" },
    { EntityTag::TILE,      "Tile" },
    { EntityTag::DEC,       "Dec" }
};

enum class AssetTag : uint32_t {
    UNDEFINED   = 0,
    TEXTURE     = 1 << 0,
    ANIMATION   = 1 << 1,
    FONT        = 1 << 2,
    SOUND       = 1 << 3
};

static std::unordered_map<std::string, AssetTag> const strToAssetTag = {
    { "Undefined",  AssetTag::UNDEFINED },
    { "Texture",    AssetTag::TEXTURE },
    { "Animation",  AssetTag::ANIMATION },
    { "Font",       AssetTag::FONT },
    { "Sound",      AssetTag::SOUND }
};

static std::unordered_map<AssetTag, std::string> const assetTagToStr = {
    { AssetTag::UNDEFINED,  "Undefined" },
    { AssetTag::TEXTURE,    "Texture" },
    { AssetTag::ANIMATION,  "Animation" },
    { AssetTag::FONT,       "Font" },
    { AssetTag::SOUND,      "Sound" }
};

enum class PlayerState : uint32_t {
    UNDEFINED   = 0,
    STAND       = 1 << 0,
    AIR         = 1 << 1,
    RUN         = 1 << 2
};

static std::unordered_map<std::string, PlayerState> const strToPlayerState = {
    { "Undefined",  PlayerState::UNDEFINED },
    { "Stand",      PlayerState::STAND },
    { "Air",        PlayerState::AIR },
    { "Run",        PlayerState::RUN }
};

static std::unordered_map<PlayerState, std::string> const playerStateToStr = {
    { PlayerState::UNDEFINED,   "Undefined" },
    { PlayerState::STAND,       "Stand" },
    { PlayerState::AIR,         "Air" },
    { PlayerState::RUN,         "Run" }
};

enum class SceneMenuAction : uint32_t {
    UNDEFINED   = 0,
    UP          = 1 << 0,
    DOWN        = 1 << 1,
    PLAY        = 1 << 2,
    QUIT        = 1 << 3
};

static std::unordered_map<std::string, SceneMenuAction> const strToSceneMenuAction = {
    { "Undefined",  SceneMenuAction::UNDEFINED },
    { "Up",         SceneMenuAction::UP },
    { "Down",       SceneMenuAction::DOWN },
    { "Play",       SceneMenuAction::PLAY },
    { "Quit",       SceneMenuAction::QUIT }
};

static std::unordered_map<SceneMenuAction, std::string> const sceneMenuActionToStr = {
    { SceneMenuAction::UNDEFINED,   "Undefined" },
    { SceneMenuAction::UP,          "Up" },
    { SceneMenuAction::DOWN,        "Down" },
    { SceneMenuAction::PLAY,        "Play" },
    { SceneMenuAction::QUIT,        "Quit" }
};

enum class ScenePlayAction : uint32_t {
    UNDEFINED           = 0,
    UP                  = 1 << 0,
    DOWN                = 1 << 1,
    PAUSE               = 1 << 2,
    QUIT                = 1 << 3,
    TOGGLE_TEXTURE      = 1 << 4,
    TOGGLE_COLLISION    = 1 << 5,
    TOGGLE_GRID         = 1 << 6,
    LEFT                = 1 << 7,
    RIGHT               = 1 << 8,
    SHOOT               = 1 << 9
};

static std::unordered_map<std::string, ScenePlayAction> const strToScenePlayAction = {
    { "Undefined",          ScenePlayAction::UNDEFINED },
    { "Up",                 ScenePlayAction::UP },
    { "Down",               ScenePlayAction::DOWN },
    { "Pause",              ScenePlayAction::PAUSE },
    { "Quit",               ScenePlayAction::QUIT },
    { "Toggle_texture",     ScenePlayAction::TOGGLE_TEXTURE },
    { "Toggle_collision",   ScenePlayAction::TOGGLE_COLLISION },
    { "Toggle_grid",        ScenePlayAction::TOGGLE_GRID },
    { "Left",               ScenePlayAction::LEFT },
    { "Right",              ScenePlayAction::RIGHT },
    { "Shoot",              ScenePlayAction::SHOOT }
};

static std::unordered_map<ScenePlayAction, std::string> const scenePlayActionToStr = {
    { ScenePlayAction::UNDEFINED,           "Undefined" },
    { ScenePlayAction::UP,                  "Up" },
    { ScenePlayAction::DOWN,                "Down" },
    { ScenePlayAction::PAUSE,               "Pause" },
    { ScenePlayAction::QUIT,                "Quit" },
    { ScenePlayAction::TOGGLE_TEXTURE,      "Toggle_texture" },
    { ScenePlayAction::TOGGLE_COLLISION,    "Toggle_collision" },
    { ScenePlayAction::TOGGLE_GRID,         "Toggle_grid" },
    { ScenePlayAction::LEFT,                "Left" },
    { ScenePlayAction::RIGHT,               "Right" },
    { ScenePlayAction::SHOOT,               "Shoot" }
};

enum class ActionType : uint32_t {
    UNDEFINED   = 0,
    START       = 1 << 0,
    END         = 1 << 1
};

static std::unordered_map<std::string, ActionType> const strToActionType = {
    { "Undefined",  ActionType::UNDEFINED },
    { "Start",      ActionType::START},
    { "End",        ActionType::END }
};

static std::unordered_map<ActionType, std::string> const actionTypeToStr = {
    { ActionType::UNDEFINED,    "Undefined" },
    { ActionType::START,        "Start" },
    { ActionType::END,          "End" }
};

enum class AnimationTag : uint32_t {
    UNDEFINED   = 0,
    STAND       = 1 << 0,
    RUN         = 1 << 1,
    AIR         = 1 << 2,
    BUSTER      = 1 << 3,
    EXPLOSION   = 1 << 4,
    COIN        = 1 << 5,
    QUESTION    = 1 << 6,
    BRICK       = 1 << 7,
    QUESTION2   = 1 << 8,
    PIPE_TALL   = 1 << 9,
    PIPE_SMALL  = 1 << 10,
    CLOUD_BIG   = 1 << 11,
    CLOUD_SMALL = 1 << 12,
    BUSH        = 1 << 13,
    BUSH_BIG    = 1 << 14,
    GROUND      = 1 << 15,
    BLOCK       = 1 << 16,
    FLAG        = 1 << 17,
    POLE        = 1 << 18,
    POLE_TOP    = 1 << 19   
};

static std::unordered_map<std::string, AnimationTag> const strToAnimationTag = {
    { "Undefined",  AnimationTag::UNDEFINED },
    { "Stand",      AnimationTag::STAND },
    { "Run",        AnimationTag::RUN },
    { "Air",        AnimationTag::AIR },
    { "Buster",     AnimationTag::BUSTER },
    { "Explosion",  AnimationTag::EXPLOSION },
    { "Coin",       AnimationTag::COIN },
    { "Question",   AnimationTag::QUESTION },
    { "Brick",      AnimationTag::BRICK },
    { "Question2",  AnimationTag::QUESTION2 },
    { "PipeTall",   AnimationTag::PIPE_TALL },
    { "PipeSmall",  AnimationTag::PIPE_TALL },
    { "CloudBig",   AnimationTag::CLOUD_BIG },
    { "CloudSmall", AnimationTag::CLOUD_SMALL },
    { "Bush",       AnimationTag::BUSH },
    { "BushBig",    AnimationTag::BUSH_BIG },
    { "Ground",     AnimationTag::GROUND },
    { "Block",      AnimationTag::BLOCK },
    { "Flag",       AnimationTag::FLAG },
    { "Pole",       AnimationTag::POLE },
    { "PoleTop",    AnimationTag::POLE_TOP }
};

static std::unordered_map<AnimationTag, std::string> const animationTagToStr = {
    { AnimationTag::UNDEFINED,      "Undefined" },
    { AnimationTag::STAND,          "Stand" },
    { AnimationTag::RUN,            "Run" },
    { AnimationTag::AIR,            "Air" },
    { AnimationTag::BUSTER,         "Buster" },
    { AnimationTag::EXPLOSION,      "Explosion" },
    { AnimationTag::COIN,           "Coin" },
    { AnimationTag::QUESTION,       "Question" },
    { AnimationTag::BRICK,          "Brick" },
    { AnimationTag::QUESTION2,      "Question2" },
    { AnimationTag::PIPE_TALL,      "PipeTall" },
    { AnimationTag::PIPE_TALL,      "PipeSmall" },
    { AnimationTag::CLOUD_BIG,      "CloudBig" },
    { AnimationTag::CLOUD_SMALL,    "CloudSmall" },
    { AnimationTag::BUSH,           "Bush" },
    { AnimationTag::BUSH_BIG,       "BushBig" },
    { AnimationTag::GROUND,         "Ground" },
    { AnimationTag::BLOCK,          "Block" },
    { AnimationTag::FLAG,           "Flag" },
    { AnimationTag::POLE,           "Pole" },
    { AnimationTag::POLE_TOP,       "PoleTop" }
};

using EntityTagMask         = EnumMask<EntityTag>;
using AssetTagMask          = EnumMask<AssetTag>;
using PlayerStateMask       = EnumMask<PlayerState>;
using SceneMenuActionMask   = EnumMask<SceneMenuAction>;
using ScenePlayActionMask   = EnumMask<ScenePlayAction>;
using ActionTypeMask        = EnumMask<ActionType>;
using SceneTagMask          = EnumMask<SceneTag>;
using AnimationTagMask      = EnumMask<AnimationTag>;

#endif // !ENUMS_H