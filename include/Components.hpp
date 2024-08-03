#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Animation.hpp"
#include "Assets.hpp"
#include "Enums.hpp"

class Component {
public:
    bool has{ false };
    Component() = default;
};

class CTransform : public Component {
public:
    Vec2f m_pos       { 0.0, 0.0 };
    Vec2f m_prevPos   { 0.0, 0.0 };
    Vec2f m_scale     { 1.0, 1.0 };
    Vec2f m_velocity  { 0.0, 0.0 };
    float m_angle     { 0.0f };

    CTransform() = default;
    CTransform(const Vec2f& p)
        : m_prevPos(p), m_pos(p) {}
    CTransform(const Vec2f& p, const Vec2f& sc)
        : m_prevPos(p), m_pos(p), m_scale(sc) {}
    CTransform(const Vec2f& p, const Vec2f& v, const Vec2f& sc, float a)
        : m_pos(p), m_prevPos(p), m_velocity(v), m_scale(sc), m_angle(a) {}

};

class CLifeSpan : public Component {
public:
    int m_remaining { 0 };
    int m_total     { 0 };

    CLifeSpan() = default;
    CLifeSpan(int total)
        : m_remaining(total), m_total(total) {}
};

class CInput : public Component {
public:
    bool m_up           { false };
    bool m_down         { false };
    bool m_left         { false };
    bool m_right        { false };
    bool m_shoot        { false };
    bool m_canShoot     { true };
    bool m_canJump      { true };

    CInput() = default;
};

class CBoundingBox : public Component {
public:
    Vec2f   m_size          { 0.0f, 0.0f };
    Vec2f   m_halfSize      { 0.0f, 0.0f };
    
    CBoundingBox() = default;
    CBoundingBox(const Vec2f& s)
        : m_size(s), m_halfSize(s.x / 2, s.y / 2) {}
};

class CAnimation : public Component {
public:
    Animation m_animation;
    bool m_repeat{ false };

    CAnimation() = default;
    CAnimation(const Animation& animation, bool r)
        : m_animation(animation), m_repeat(r) {}
};

class CGravity : public Component {
public:
    float m_gravity{ 0.0f };

    CGravity() = default;
    CGravity(float g) : m_gravity(g) {}
};

class CState : public Component {
public:
    PlayerState m_prevState { PlayerState::AIR };
    PlayerState m_state     { PlayerState::AIR };
    
    CState() = default;
    CState(const PlayerState& s)
        : m_prevState(s), m_state(s) {}
};

#endif // !COMPONENTS_H
