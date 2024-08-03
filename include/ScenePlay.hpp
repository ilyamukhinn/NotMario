#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H

#include <map>
#include <memory>

#include "Scene.hpp"
#include "EntityManager.hpp"

class ScenePlay : public Scene {
	struct PlayerConfig {
		float X, Y, CW, CH, SX, SY, SM, GY;
		std::string WEAPON;
	};

protected:

	std::shared_ptr<Entity> m_player;
	std::string				m_levelPath;
	PlayerConfig			m_playerConfig;
	bool					m_drawTextures	{ true };
	bool					m_drawCollision	{ false };
	bool					m_drawGrid		{ false };
	const Vec2f				m_gridSize		{ 64.0f, 64.0f };
	sf::Text				m_gridText;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);
	void spawnPlayer();
	void spawnBullet(std::shared_ptr<Entity> e);
	Vec2f gridToMidPixel(float gridX, float gridY, 
		std::shared_ptr<Entity> e) const;
	std::shared_ptr<Entity> player();
	void update() override;
	void onEnd() override;

	void sMovement();
	void sLifespan();
	void sCollision();
	void sAnimation();
	void sRender();
	void sDoAction(const Action& action) override;

public:
	ScenePlay(GameEngine* gameEngine, 
		const std::string& levelPath);
};

#endif // !SCENE_PLAY_H
