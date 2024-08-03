#include "ScenePlay.hpp"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.hpp"
#include "Components.hpp"
#include "Action.hpp"

#include <iostream>
#include <fstream>

ScenePlay::ScenePlay(GameEngine* gameEngine,
	const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath) {
	this->init(m_levelPath);
}

void ScenePlay::init(const std::string& levelPath) {
	this->registerAction(sf::Keyboard::P,		static_cast<uint32_t>(ScenePlayAction::PAUSE));
	this->registerAction(sf::Keyboard::Escape,	static_cast<uint32_t>(ScenePlayAction::QUIT));
	this->registerAction(sf::Keyboard::T,		static_cast<uint32_t>(ScenePlayAction::TOGGLE_TEXTURE));
	this->registerAction(sf::Keyboard::C,		static_cast<uint32_t>(ScenePlayAction::TOGGLE_COLLISION));
	this->registerAction(sf::Keyboard::G,		static_cast<uint32_t>(ScenePlayAction::TOGGLE_GRID));
	this->registerAction(sf::Keyboard::W,		static_cast<uint32_t>(ScenePlayAction::UP));
	this->registerAction(sf::Keyboard::A,		static_cast<uint32_t>(ScenePlayAction::LEFT));
	this->registerAction(sf::Keyboard::S,		static_cast<uint32_t>(ScenePlayAction::DOWN));
	this->registerAction(sf::Keyboard::D,		static_cast<uint32_t>(ScenePlayAction::RIGHT));
	this->registerAction(sf::Keyboard::Space,	static_cast<uint32_t>(ScenePlayAction::SHOOT));

	this->m_gridText.setCharacterSize(12);
	this->m_gridText.setFont(this->m_game->assets().getFont("Tech"));

	this->loadLevel(levelPath);
}

Vec2f ScenePlay::gridToMidPixel(float gridX, float gridY, 
	std::shared_ptr<Entity> e) const {
	const Vec2f& size = e->get<CAnimation>().m_animation.getSize();
	return Vec2f((gridX * this->m_gridSize.x) + (size.x / 2.0f), 
		this->height() - (gridY * this->m_gridSize.y) - (size.y / 2.0f));
}

void ScenePlay::loadLevel(const std::string& filename) {
	this->m_entityManager = EntityManager();

	std::ifstream fin(filename);
	std::string str_type{ "" };
	while (fin.good()) {
		fin >> str_type;
		EntityTag type = strToEntityTag.at(str_type);

		if (type == EntityTag::PLAYER) {
			fin >> this->m_playerConfig.X >> this->m_playerConfig.Y >>
				this->m_playerConfig.CW >> this->m_playerConfig.CH >>
				this->m_playerConfig.SX >> this->m_playerConfig.SY >>
				this->m_playerConfig.SM >> this->m_playerConfig.GY >>
				this->m_playerConfig.WEAPON;

			this->spawnPlayer();
		} else {
			auto entity = m_entityManager.addEntity(type);
			std::string name{ "" };
			float gridX{ 0.0f }, gridY{ 0.0f };
			fin >> name >> gridX >> gridY;

			switch (type)
			{
			case EntityTag::TILE: {
				entity->add<CAnimation>(this->m_game->assets().getAnimation(strToAnimationTag.at(name)), true);
				entity->add<CTransform>(this->gridToMidPixel(gridX, gridY, entity));
				entity->add<CBoundingBox>(this->m_game->assets().getAnimation(strToAnimationTag.at(name)).getSize());
				break;
			}
			case EntityTag::DEC: {
				entity->add<CAnimation>(this->m_game->assets().getAnimation(strToAnimationTag.at(name)), false);
				entity->add<CTransform>(this->gridToMidPixel(gridX, gridY, entity));
				break;
			}
			default:
				break;
			}
		}
	}
}

void ScenePlay::spawnPlayer() {
	this->m_player = this->m_entityManager.addEntity(EntityTag::PLAYER);
	this->m_player->add<CAnimation>(this->m_game->assets().getAnimation(
		strToAnimationTag.at(playerStateToStr.at(PlayerState::AIR))), true);
	this->m_player->add<CTransform>(this->gridToMidPixel(
		this->m_playerConfig.X, this->m_playerConfig.Y, this->m_player));
	this->m_player->add<CGravity>(this->m_playerConfig.GY);
	this->m_player->add<CBoundingBox>(Vec2f(this->m_playerConfig.CW, this->m_playerConfig.CH));
}

void ScenePlay::spawnBullet(std::shared_ptr<Entity> e) {
	auto entity = this->m_entityManager.addEntity(EntityTag::BULLET);
	entity->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::BUSTER), true);

	auto eTransform = e->get<CTransform>();

	entity->add<CTransform>(Vec2f(eTransform.m_pos.x, eTransform.m_pos.y), Vec2f(eTransform.m_scale.x * 10.0f, 0.0f), 
		eTransform.m_scale, 0.0f);
	entity->add<CBoundingBox>(this->m_game->assets().getAnimation(AnimationTag::BUSTER).getSize());
	entity->add<CLifeSpan>(45.0f);
}

void ScenePlay::update() {
	this->m_entityManager.update();

	if (!this->m_paused) {
		this->sMovement();
		this->sLifespan();
		this->sCollision();
		this->sAnimation();
	}

	if (!this->m_player->isActive()) {
		this->spawnPlayer();
	}
}

void ScenePlay::sMovement() {
	if (this->m_player->isActive()) {
		auto& input = this->m_player->get<CInput>();
		auto& transform = this->m_player->get<CTransform>();

		if (input.m_up && input.m_canJump) {
			transform.m_velocity.y += this->m_playerConfig.SY;
			input.m_canJump = false;
		}

		float playerVelocityX = 0.0f;
		if (input.m_left) {
			playerVelocityX -= this->m_playerConfig.SX;
			transform.m_scale.x = -1.0f;
		}

		if (input.m_right) {
			playerVelocityX += this->m_playerConfig.SX;
			transform.m_scale.x = 1.0f;
		}
		transform.m_velocity.x = playerVelocityX;

		auto& playerState = this->m_player->get<CState>();

		if (playerVelocityX == 0.0f) {
			if (playerState.m_state != PlayerState::STAND) {
				playerState.m_prevState = playerState.m_state;
				playerState.m_state = PlayerState::STAND;
			}
		}
		else {
			if (playerState.m_state != PlayerState::RUN) {
				playerState.m_prevState = playerState.m_state;
				playerState.m_state = PlayerState::RUN;
			}
		}

		transform.m_velocity.y += this->m_player->get<CGravity>().m_gravity;

		if (transform.m_velocity.y >= this->m_playerConfig.SM) {
			transform.m_velocity.y = this->m_playerConfig.SM;
		}

		transform.m_prevPos = transform.m_pos;
		transform.m_pos += transform.m_velocity;
	}

	for (auto entity : this->m_entityManager.getEntities(EntityTag::BULLET)) {
		if (entity->isActive()) {
			auto& entityTransform = entity->get<CTransform>();
			entityTransform.m_prevPos = entityTransform.m_pos;
			entityTransform.m_pos += entityTransform.m_velocity;
		}
	}
}

void ScenePlay::sLifespan() {
	for (auto e : this->m_entityManager.getEntities()) {
		if (e->isActive()) {
			if (e->has<CLifeSpan>()) {
				auto& lifespan = e->get<CLifeSpan>();
				if (lifespan.m_remaining > 0) {
					lifespan.m_remaining--;
				}

				if (lifespan.m_remaining > 0) {
					sf::Uint8 newAlpha = 255 * lifespan.m_remaining / lifespan.m_total;

					auto& sprite = e->get<CAnimation>().m_animation.getSprite();
					auto spriteColor = sprite.getColor();
					spriteColor.a = newAlpha;
					sprite.setColor(spriteColor);
				}
				else {
					e->destroy();
				}
			}
		}
	}
}

void ScenePlay::sCollision() {
	auto& playerPos = this->m_player->get<CTransform>().m_pos;
	bool hasCollision = false;

	for (auto entity : this->m_entityManager.getEntities(EntityTag::TILE)) {
		if (entity->isActive()) {
			if (entity->get<CAnimation>().m_animation.getName() == AnimationTag::BRICK) {
				for (auto bullet : this->m_entityManager.getEntities(EntityTag::BULLET)) {
					if (bullet->isActive()) {
						Vec2f overlap = Physics::GetOverlap(bullet, entity);

						if (overlap.x > 0.0f &&
							overlap.y > 0.0f) {
							bullet->destroy();
							entity->destroy();

							auto explosion = m_entityManager.addEntity(EntityTag::DEC);
							explosion->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::EXPLOSION), false);
							explosion->add<CTransform>(entity->get<CTransform>().m_pos);
						}
					}
				}
			}

			if (this->m_player->isActive()) {
				Vec2f overlap = Physics::GetOverlap(this->m_player, entity);
				Vec2f prevOverlap = Physics::GetPreviousOverlap(this->m_player, entity);

				if (overlap.x > 0.0f &&
					overlap.y > 0.0f) {
					hasCollision = true;

					auto& entityPos = entity->get<CTransform>().m_pos;
					auto& playerPrevPos = this->m_player->get<CTransform>().m_prevPos;

					if (prevOverlap.y > 0.0f) {
						playerPos.x += playerPrevPos.x < entityPos.x ?
							-overlap.x : overlap.x;
					}

					if (prevOverlap.x > 0.0f) {
						if (playerPrevPos.y > entityPos.y) {
							playerPos.y += overlap.y;
							const AnimationTag name =
								entity->get<CAnimation>().m_animation.getName();
							if (name == AnimationTag::QUESTION) {
								entity->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::QUESTION2), true);

								auto coin = this->m_entityManager.addEntity(EntityTag::DEC);
								coin->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::COIN), true);

								auto eTransform = entity->get<CTransform>();

								coin->add<CTransform>(Vec2f(eTransform.m_pos.x, eTransform.m_pos.y - 64.0f));
								coin->add<CLifeSpan>(60.0f);

							}
							else if (name == AnimationTag::BRICK) {
								entity->destroy();
							}
						}
						else {
							playerPos.y -= overlap.y;
							this->m_player->get<CInput>().m_canJump = true;
						}

						this->m_player->get<CTransform>().m_velocity.y = 0.0f;
					}
				}
			}
		}
	}

	if (!hasCollision && this->m_player->isActive()) {
		auto& playerState = this->m_player->get<CState>();
		if (playerState.m_state != PlayerState::AIR) {
			playerState.m_prevState = playerState.m_state;
			playerState.m_state = PlayerState::AIR;
		}
	}

	if (playerPos.y > this->height()) {
		this->m_player->destroy();
	}
}

void ScenePlay::sAnimation() {
	if (this->m_player->isActive()) {
		auto& playerState = this->m_player->get<CState>();
		if (playerState.m_state != playerState.m_prevState) {
			playerState.m_prevState = playerState.m_state;
			this->m_player->add<CAnimation>(this->m_game->assets().getAnimation(
				strToAnimationTag.at(playerStateToStr.at(this->m_player->get<CState>().m_state))), true);
		}
	}

	for (auto entity : this->m_entityManager.getEntities()) {
		if (entity->has<CAnimation>() && entity->isActive()) {
			entity->get<CAnimation>().m_animation.update();

			if (!entity->get<CAnimation>().m_repeat && 
				entity->get<CAnimation>().m_animation.hasEnded()) {
				entity->destroy();
			}
		}
	}
}

void ScenePlay::sDoAction(const Action& action) {
	if (this->m_player->isActive()) {
		auto& inputComponent = this->m_player->get<CInput>();
		if (action.type() == ActionType::START) {
			switch (action.name())
			{
			case static_cast<uint32_t>(ScenePlayAction::TOGGLE_TEXTURE): {
				this->m_drawTextures = !this->m_drawTextures;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::TOGGLE_COLLISION): {
				this->m_drawCollision = !this->m_drawCollision;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::TOGGLE_GRID): {
				this->m_drawGrid = !this->m_drawGrid;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::PAUSE): {
				this->setPaused(!this->m_paused);
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::QUIT): {
				this->onEnd();
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::LEFT): {
				inputComponent.m_left = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::RIGHT): {
				inputComponent.m_right = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::UP): {
				inputComponent.m_up = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::DOWN): {
				inputComponent.m_down = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::SHOOT): {
				inputComponent.m_canShoot = true; this->spawnBullet(this->m_player);
				break;
			}
			default:
				break;
			}
		}
		else if (action.type() == ActionType::END) {
			switch (action.name())
			{
			case static_cast<uint32_t>(ScenePlayAction::LEFT): {
				inputComponent.m_left = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::RIGHT): {
				inputComponent.m_right = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::UP): {
				inputComponent.m_up = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::DOWN): {
				inputComponent.m_down = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::SHOOT): {
				inputComponent.m_canShoot = false;
				break;
			}
			default:
				break;
			}
		}
	}
}

void ScenePlay::onEnd() {
	this->m_game->changeScene(SceneTag::MENU, nullptr, true);
}

void ScenePlay::sRender() {
	this->m_game->window().clear(!this->m_paused ?
		sf::Color(148, 148, 255) : sf::Color(58, 58, 100));

	auto& pPos = this->m_player->get<CTransform>().m_pos;
	sf::View view = this->m_game->window().getView();
	float windowCenterX = std::max(view.getSize().x / 2.0f, pPos.x);
	view.setCenter(windowCenterX, 
		this->m_game->window().getSize().y - view.getSize().y / 2.0f);
	this->m_game->window().setView(view);

	if (this->m_drawTextures) {
		for (auto e : this->m_entityManager.getEntities()) {
			auto& transform = e->get<CTransform>();

			if (e->has<CAnimation>()) {
				auto& animation = e->get<CAnimation>().m_animation;
				animation.getSprite().setRotation(transform.m_angle);
				animation.getSprite().setPosition(transform.m_pos.x, transform.m_pos.y);
				animation.getSprite().setScale(transform.m_scale.x, transform.m_scale.y);
				this->m_game->window().draw(animation.getSprite());
			}
		}
	}

	if (this->m_drawCollision) {
		for (auto e : this->m_entityManager.getEntities()) {
			if (e->has<CBoundingBox>()) {
				auto& box = e->get<CBoundingBox>();
				auto& transform = e->get<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.m_size.x - 1, box.m_size.y - 1));
				rect.setOrigin(sf::Vector2f(box.m_halfSize.x, box.m_halfSize.y));
				rect.setPosition(transform.m_pos.x, transform.m_pos.y);
				rect.setFillColor(sf::Color::Transparent);
				rect.setOutlineColor(sf::Color::White);
				rect.setOutlineThickness(1.0f);
				this->m_game->window().draw(rect);
			}
		}
	}

	if (this->m_drawGrid) {
		float vWidth = view.getSize().x;
		float vHeight = view.getSize().y;
		float leftX = view.getCenter().x - vWidth / 2.0f;
		float rightX = leftX + vWidth + this->m_gridSize.x;
		float nextGridX = leftX - (static_cast<int>(leftX) % 
			static_cast<int>(this->m_gridSize.x));

		for (float x = nextGridX; x < rightX; x += this->m_gridSize.x) {
			drawLine(Vec2(x, height() - vHeight), Vec2f(x, height()));
		}

		for (float y = 0; y < vHeight; y += this->m_gridSize.y) {
			drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

			for (float x = nextGridX; x < rightX; x += this->m_gridSize.x) {
				std::string xCell = std::to_string(static_cast<int>(x) / 
					static_cast<int>(this->m_gridSize.x));
				std::string yCell = std::to_string(static_cast<int>(y) /
					static_cast<int>(this->m_gridSize.y));
				this->m_gridText.setString("(" + xCell + "," + yCell + ")");
				this->m_gridText.setPosition(x + 3.0f, height() - y - this->m_gridSize.y + 2.0f);
				this->m_game->window().draw(this->m_gridText);
			}
		}
	}
}