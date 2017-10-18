#include "Props/Guts.hpp"
#include "System/GameWorld.hpp"
#include "System/Utils.hpp"

#include <iostream>

size_t Guts::_textureID;

Guts::Guts() :
    StandardEntity()
{

}

Guts::Guts(const Vec2& pos, const Vec2& v) :
    StandardEntity(pos.x, pos.y, 0.0),
    _initialVelocity(v)
{
    _type   = PROPS;
    _isDone = false;
    _duration = 5;
}

bool Guts::isDone() const
{
    return _isDone;
}

void Guts::initPhysics(GameWorld* world)
{
    U_2DBody* body = m_initBody(world);
    float radius = 1.f;
    _bodies.push_back(m_bodyID);
    body->setRadius(radius);
    body->setMass(0.005);

    size_t count=getRandInt(5, 10);
    for (size_t i(0); i<count; ++i)
    {
        BodyID newBodyID = world->addBody();
        U_2DBody* newBody = world->getBodyByID(newBodyID);
        newBody->setEntity(this);
        newBody->setRadius(radius);
        newBody->setPosition(m_coord+Vec2(getRandInt(-2, 2), getRandInt(-2, 2)));
        newBody->accelerate2D(_initialVelocity*1.0f);

        _constraints.push_back(world->addConstraint(newBodyID, _bodies.back(), 2.0f));
        _bodies.push_back(newBodyID);

    }
}

void Guts::update(GameWorld& world)
{
    _duration -= DT;
    _isDone = _duration<0;

    if (_isDone)
    {
        world.removeBody(m_bodyID);
        for (BodyID b : _bodies)
            world.removeBody(b);

        for (U_2DConstraint* c : _constraints)
            world.removeConstraint(c);
    }
}

void Guts::render()
{
    for (BodyID& id : _bodies)
    {
        Vec2 coord = GameWorld::getBodyByID(id)->getPosition();
        sf::Vector2f pos(coord.x, coord.y);

        sf::VertexArray va(sf::Quads, 4);
        GraphicUtils::initQuad(va, sf::Vector2f(4, 4), sf::Vector2f(2, 2), 1.0f);
        GraphicUtils::move(va, pos);
        va[0].color = sf::Color::White;
        va[1].color = sf::Color::Red;
        va[2].color = sf::Color::White;
        va[3].color = sf::Color::Red;

        va[0].texCoords = sf::Vector2f(0 , 0);
        va[1].texCoords = sf::Vector2f(70, 0);
        va[2].texCoords = sf::Vector2f(70, 76);
        va[3].texCoords = sf::Vector2f(0 , 76);

        if (!_isDone)
        {
            GameRender::addQuad(_textureID, va, RenderLayer::RENDER);
        }
        else
        {
            GameRender::addQuad(_textureID, va, RenderLayer::GROUND);
        }
    }
}

void Guts::init()
{
    _textureID = GameRender::registerTexture("data/textures/guts.png");
}
