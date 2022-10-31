#include "pch.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CTileMap.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::KeyBoardInput(const sf::Keyboard::Key& key)
{
}

void CScene::Update(const float ElapsedTime)
{
	m_pPlayer->Update(ElapsedTime);
}

void CScene::Render(sf::RenderWindow& RW)
{
	m_pPlayer->Render(RW);
}

void CScene::Collide_Wall()
{
	if (m_pTileMap) {
		// �������� �ƴ� �� �Ʒ��� �ٴ��� �ִ��� üũ
		if (!m_pPlayer->GetJump()) {
			m_pPlayer->SetFall(true);
			for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
				if (m_pPlayer->GetFallBB().intersects(wall.GetAABB())) {
					m_pPlayer->SetFall(false);
					break;
				}
			}
			if (m_pPlayer->GetFall()) {
				m_pPlayer->SetJump(true);
				m_pPlayer->SetJumpVelocity(JUMP_SPEED);
				m_pPlayer->SetJumpCnt(m_pPlayer->GetJumpChange());
			}
		}
		for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
			//�߾Ʒ� ����� ���� ��
			if (m_pPlayer->GetAABB().intersects(wall.GetAABB())) {
				m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());

				//jump �浹ó��
				if (m_pPlayer->GetJump()) {
					if (m_pPlayer->GetJumpDir()) {
						m_pPlayer->SetJumpCnt(m_pPlayer->GetJumpChange());
					}
					else {
						m_pPlayer->SetJump(false);
						m_pPlayer->SetPosition(sf::Vector2f(m_pPlayer->GetSprite().getPosition().x, wall.GetSprite().getPosition().y - m_pPlayer->GetSprite().getGlobalBounds().height));
					}
				}
				break;
			}
		}
	}
}

void CScene::Collide_OBJ()
{
	Collide_Wall();
	Collide_Potion();
	Collide_Gate();
	Collide_Turret();
	Collide_Jump();
}

void CScene::Collide_Potion()
{
	// Red Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::YELLOW);
			else if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::PURPLE);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::RED);
			break;
		}
	}

	// Green Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::RED == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::YELLOW);
			else if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::GB);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::GREEN);
			break;
		}
	}

	// Blue Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::RED == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::PURPLE);
			else if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::GB);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::BLUE);
			break;
		}
	}

	// Black Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::BLACK_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			m_pPlayer->SetColor(PLAYER_COLOR::NORMAL);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			break;
		}
	}
}

void CScene::Collide_Gate()
{
	// Red Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_G)->second) {
		if (PLAYER_COLOR::RED != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Green Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_G)->second) {
		if (PLAYER_COLOR::GREEN != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Blue Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_G)->second) {
		if (PLAYER_COLOR::BLUE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Yellow Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_G)->second) {
		if (PLAYER_COLOR::YELLOW != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Purple Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_G)->second) {
		if (PLAYER_COLOR::PURPLE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// GB Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_G)->second) {
		if (PLAYER_COLOR::GB != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}
}

void CScene::Collide_Turret()
{


	// Red Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_G)->second) {
		if (PLAYER_COLOR::RED != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Green Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_G)->second) {
		if (PLAYER_COLOR::GREEN != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Blue Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_G)->second) {
		if (PLAYER_COLOR::BLUE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Yellow Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_G)->second) {
		if (PLAYER_COLOR::YELLOW != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Purple Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_G)->second) {
		if (PLAYER_COLOR::PURPLE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// GB Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_G)->second) {
		if (PLAYER_COLOR::GB != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}
}

void CScene::Collide_Jump()
{
	//Red Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_J)->second) {
		if (PLAYER_COLOR::RED == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}
	
	//Green Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_J)->second) {
		if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Blue Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_J)->second) {
		if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Yellow Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_J)->second) {
		if (PLAYER_COLOR::YELLOW == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Purple Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_J)->second) {
		if (PLAYER_COLOR::PURPLE == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//GB Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_J)->second) {
		if (PLAYER_COLOR::GB == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	m_pPlayer->SetSuperJump(false);
}

void CScene::Next_Stage()
{

}

void CScene::Reset() 
{
}
