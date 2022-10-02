#pragma once
#include "CObject.h"

constexpr float PLAYER_SPEED = 150.0f;
constexpr float JUMP_SPEED = 300.0f;
constexpr float SUPERJUMP_SPEED = 600.0f;
constexpr float GRAVITY = 15.0f;

#define TIME_PER_ACTION 0.25f
#define ACTION_PER_TIME 1.0f / TIME_PER_ACTION
#define FRAMES_PER_ACTION 3

class CPlayer : public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	void Update(const float ElapsedTime);
	void Render(sf::RenderWindow& RW);
	virtual void UpdateAABB();

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

	const PLAYER_COLOR& GetColor() const { return m_eColor; }
	const sf::Vector2f& GetPrevPos() const { return m_vec2fPrevPos; }
	const bool GetJump() const { return m_bJump; }
	const bool GetSuperJump() const { return m_bSuperJump; }

	virtual void SetPosition(const sf::Vector2f& vec) { m_vec2fPos = vec; }
	void SetColor(const PLAYER_COLOR color) { m_eColor = color; m_sfSprite.setTexture(m_umTextures.find(color)->second); }
	void SetJump(const bool jump) { m_bJump = jump;}
	void SetSuperJump(const bool jump) { m_bSuperJump = jump; }

private:
	void Animation(const float ElapsedTime);

private:
	// Pos, Move
	sf::Vector2f m_vec2fPrevPos = {};
	int m_iDir = 0;
	bool m_bJump = false;
	bool m_bSuperJump = false;
	int m_iJumpCnt = 0;
	int m_iJumpChange = 0;
	float m_fJumpVelocity = 0.0f;

	// State, Color
	float m_fSpriteLeft = 0;
	int m_iSpriteTop = 32;
	PLAYER_STATE m_eState = PLAYER_STATE::IDLE;
	PLAYER_COLOR m_eColor = PLAYER_COLOR::NORMAL;
	unordered_map<PLAYER_COLOR, sf::Texture> m_umTextures = {};
};

