#include "../Header/BattleField.h"
#include "../Header/Project.h"

BattleField::BattleField()
{
	m_ControlState = NoControl;
	m_ActiveUnit = NULL;
	m_PathFinder.SetBattleField(this);
	m_HitDataBlock.Visible(false);
	m_Project = NULL;
	m_LeftDataBlock.Sprite().Node()->setVisible(false);
	m_RightDataBlock.Sprite().Node()->setVisible(false);
	m_ActionMenuBlock.Visible(false);
}

BattleField::~BattleField()
{
	for (vector<AnimatingObject*>::iterator i = m_Overlayers.begin(); i != m_Overlayers.end(); ++i)
	{
		delete (*i);
	}
	m_Overlayers.clear();
}

void BattleField::Init(vector3di tileSize, vector2di bottomTilePosition, Animation animation, vector< vector<int> > heightMap, vector2di spriteSize)
{
	m_TileSize = tileSize;
	m_BottomTilePosition = bottomTilePosition;
	Animations().push_back(animation);
	m_HeightMap = heightMap;
	PlayAnimation(animation.Name());
	m_TextureSize.X = animation.FrameSize().X;
	m_TextureSize.Y = animation.FrameSize().Y;
	Sprite().NodeSize(spriteSize.X, spriteSize.Y);
	Sprite().Node()->setPosition(vector3df(0, 0, 400));
	m_Selector.Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.X * 2 * GetScaleY());
}

void BattleField::Activate()
{
	PlayAnimation(0);
	m_TextureSize.X = Sprite().CurrentAnimation()->FrameSize().X;
	m_TextureSize.Y = Sprite().CurrentAnimation()->FrameSize().Y;
	Sprite().NodeSize(m_TextureSize.X * 3, m_TextureSize.Y * 3);
	Sprite().Node()->setPosition(vector3df(0, 0, 400));
	for (vector<Unit>::iterator i = m_Units.begin(); i != m_Units.end(); ++i)
	{
		i->PlayAnimation(0);
	}
	int z = 0;
	for (vector<AnimatingObject*>::iterator i = m_Overlayers.begin(); i != m_Overlayers.end(); ++i)
	{
		(*i)->PlayAnimation(0);
		(*i)->Sprite().Node()->getParent()->setParent(Sprite().Node());
		(*i)->Sprite().NodeSize((*i)->Sprite().CurrentAnimation()->FrameSize().X * GetScaleX(), (*i)->Sprite().CurrentAnimation()->FrameSize().Y * GetScaleY());
		(*i)->Sprite().Node()->setPosition(vector3df(((*i)->Sprite().CurrentAnimation()->FramePosition().X - m_TextureSize.X / 2.0f + (*i)->Sprite().CurrentAnimation()->FrameSize().X / 2.0f) * GetScaleX(),
									(m_TextureSize.Y / 2.0f - (*i)->Sprite().CurrentAnimation()->FramePosition().Y - (*i)->Sprite().CurrentAnimation()->FrameSize().Y) * GetScaleY(),
									 m_OverlayZs[z] - Sprite().Node()->getPosition().Z));
		z++;
	}
	m_Selector = m_Project->GetSelector();
	m_Selector.PlayAnimation(0);
	m_Selector.Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.Y * 4 * GetScaleY());
}

void BattleField::Update(float deltaTime)
{
	Sprite().Node()->setPosition(vector3df(-((m_Selector.BFPosition().X() + 1.0f - m_Selector.BFPosition().Y()) * m_TileSize.X / 2.0f * GetScaleX()),
										   -((m_Selector.BFPosition().X() + 1.0f + m_Selector.BFPosition().Y()) * m_TileSize.Y / 2.0f * GetScaleY() + m_Selector.BFPosition().Z() * m_TileSize.Z * GetScaleY()),
											400));
	for (vector<Unit>::iterator i = m_Units.begin(); i != m_Units.end(); ++i) 
	{
		i->Update(deltaTime);
		i->Sprite().Node()->setPosition(BFPositionToPosition(i->BFPosition()));
	}
	for (vector<AnimatingObject*>::iterator i = m_Overlayers.begin(); i != m_Overlayers.end(); ++i)
	{
		(*i)->Update(deltaTime);
	}
	m_Selector.Update(deltaTime);
	m_Selector.BFPosition(BattleFieldPosition(m_Selector.BFPosition().X(), m_Selector.BFPosition().Y(), GetHeightByPos(m_Selector.BFPosition().X(), m_Selector.BFPosition().Y())));
	m_Selector.Sprite().Node()->setPosition(BFPositionToPosition(m_Selector.BFPosition(), false));
	for (vector<SelectableTile>::iterator i = m_SelectableTiles.begin(); i != m_SelectableTiles.end(); ++i)
	{
		i->Update(deltaTime);
		i->Sprite().Node()->setPosition(BFPositionToPosition(i->BFPosition(), false));
	}
	switch(m_ControlState)
	{
		case TargetControl:
		{
			CheckSelectorMovement();
			Unit* selectedUnit = GetUnitByPos(round_(m_Selector.BFPosition().X()), round_(m_Selector.BFPosition().Y()));
			if (selectedUnit != NULL)
			{
				m_LeftDataBlock.DataUnit(selectedUnit, true, true);
				m_RightDataBlock.DataUnit(NULL, false, false);
				m_LeftDataBlock.Sprite().Node()->setVisible(true);
			} else
			{
				m_LeftDataBlock.Sprite().Node()->setVisible(false);
			}
			if (selectedUnit == m_ActiveUnit && GetIrrlicht::Events()->keyPressed(KEY_RETURN))
			{
				m_ControlState = UnitControl;
				m_PlayerTurnState = Start;
			}
			break;
		}
		case NoControl:
		{
			m_LeftDataBlock.Sprite().Node()->setVisible(false);
			m_RightDataBlock.Sprite().Node()->setVisible(false);
			if (m_Units.size() == 0) break;
			bool unitReady = false;
			while(!unitReady)
			{
				for (vector<Unit>::iterator i = m_Units.begin(); i != m_Units.end(); ++i)
				{
					if (i->ClockTick()) unitReady = true;
				}
			}
			std::sort(m_Units.begin(), m_Units.end(), UnitSort);
			int reserve = m_Units[0].TurnCounter() - 1000;
			for (vector<Unit>::iterator i = m_Units.begin(); i != m_Units.end(); ++i)
			{
				i->ReserveFill(reserve);
			}
			m_ActiveUnit = &(m_Units[0]);
			m_Selector.BFPosition(m_ActiveUnit->BFPosition());
			if (m_ActiveUnit->AIControlled())
			{
				m_ControlState = AIControl;
				m_LeftDataBlock.DataUnit(m_ActiveUnit, false, true);
			} else
			{
				m_ControlState = UnitControl;
				m_LeftDataBlock.DataUnit(m_ActiveUnit, true, true);
				m_UnitActed = false;
				m_UnitMoved = false;
				m_PlayerTurnState = Start;
				m_UnitOldPos = m_ActiveUnit->BFPosition();
				m_UnitOldFac = m_ActiveUnit->Facing();
			}
			m_LeftDataBlock.Sprite().Node()->setVisible(true);
			break;
		}
		case UnitControl:
		{
			switch(m_PlayerTurnState)
			{
				case Start:
				{
					if (m_UnitMoved && m_UnitActed)
					{
						m_ActiveUnit->TurnPassed(m_UnitMoved, m_UnitActed);
						m_ControlState = NoControl;
						break;
					}
					vector<stringw> standardActions;
					if (!m_UnitMoved) standardActions.push_back(L"*Move");
					if (!m_UnitActed) standardActions.push_back(L"*Action");
					standardActions.push_back(L"*Wait");
					m_ActionMenuBlock.SetActions(standardActions);
					m_ActionMenuBlock.Visible(true);
					m_PlayerTurnState = MenuControl;
					break;
				}
				case MenuControl:
				{
					m_ActionMenuBlock.Update(deltaTime);
					if (GetIrrlicht::Events()->keyPressed(KEY_ESCAPE) && m_UnitMoved && !m_UnitActed)
					{
						m_ActiveUnit->BFPosition(m_UnitOldPos);
						m_ActiveUnit->Facing(m_UnitOldFac);
						m_UnitMoved = false;
						m_Selector.BFPosition(m_UnitOldPos);
						m_PlayerTurnState = Start;
					} else if (GetIrrlicht::Events()->keyPressed(KEY_RETURN))
					{
						if (m_ActionMenuBlock.SelectedAction() == 0 && !m_UnitMoved) // Move
						{
							m_ActionMenuBlock.Visible(false);
							m_PlayerTurnState = MoveControl;
							m_PathFinder.FindPaths(m_ActiveUnit);
							for (unsigned int y = 0; y < m_PathFinder.GetPathData().size(); y++)
								for (unsigned int x = 0; x < m_PathFinder.GetPathData()[0].size(); x++)
								{
									if (m_PathFinder.GetPathData()[y][x] != MoveImpossible)
									{
										m_SelectableTiles.push_back(m_Project->GetSelectableTile());
										m_SelectableTiles[m_SelectableTiles.size() - 1].PlayAnimation(0);
										m_SelectableTiles[m_SelectableTiles.size() - 1].Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.Y * GetScaleY());
										m_SelectableTiles[m_SelectableTiles.size() - 1].BFPosition(BattleFieldPosition(x,y,GetHeightByPos(x,y)));
									}
								}
						} else if ((m_ActionMenuBlock.SelectedAction() == 1 && !m_UnitActed && !m_UnitMoved) || (m_ActionMenuBlock.SelectedAction() == 0 && m_UnitMoved)) // Action
						{
							m_ActionMenuBlock.SetActions(m_ActiveUnit->GetActionListAsString());
							m_PlayerTurnState = ActionChoose;
						} else // Wait
						{
							m_ActiveUnit->TurnPassed(m_UnitMoved, m_UnitActed);
							m_ControlState = NoControl;
						}
					}
					break;
				}
				case MoveControl:
				{
					CheckSelectorMovement();
					if (GetIrrlicht::Events()->keyPressed(KEY_ESCAPE))
					{
						m_PlayerTurnState = Start;
						for (vector<SelectableTile>::iterator i = m_SelectableTiles.begin(); i != m_SelectableTiles.end(); ++i)
						{
							i->Sprite().Node()->remove();
						}
						m_SelectableTiles.clear();
						break;
					} else if (GetIrrlicht::Events()->keyPressed(KEY_RETURN) &&
								m_PathFinder.GetPathData()[round_(m_Selector.BFPosition().Y())][round_(m_Selector.BFPosition().X())] != MoveImpossible)
					{
						vector<vector3di> thePath;
						int x = round_(m_Selector.BFPosition().X());
						int y = round_(m_Selector.BFPosition().Y());
						int z = round_(m_Selector.BFPosition().Z());
						thePath.push_back(vector3di(x, y, z));
						PathData currentData;
						
						while (!(x == round_(m_ActiveUnit->BFPosition().X()) && y == round_(m_ActiveUnit->BFPosition().Y())))
						{
							currentData = m_PathFinder.GetPathData()[y][x];
							switch(currentData)
							{
								case MoveNorth:
									y++;
									break;
								case MoveNorthDouble:
									y += 2;
									break;
								case MoveSouth:
									y--;
									break;
								case MoveSouthDouble:
									y -= 2;
									break;
								case MoveWest:
									x++;
									break;
								case MoveWestDouble:
									x += 2;
									break;
								case MoveEast:
									x--;
									break;
								case MoveEastDouble:
									x -= 2;
									break;
								default:
									break;								
							}
							z = GetHeightByPos(x,y);
							thePath.push_back(vector3di(x, y, z));
						}
						thePath.push_back(vector3di(x, y, z));
						std::reverse(thePath.begin(), thePath.end());
						m_ActiveUnit->OrderMove(thePath);
						m_ActiveUnit->NextMovingStep();
						for (vector<SelectableTile>::iterator i = m_SelectableTiles.begin(); i != m_SelectableTiles.end(); ++i)
						{
							i->Sprite().Node()->remove();
						}
						m_SelectableTiles.clear();
						m_PlayerTurnState = MoveWaiting;
					}
					break;
				}
				case MoveWaiting:
				{
					if (!m_ActiveUnit->Walking())
					{
						m_UnitMoved = true;
						m_PlayerTurnState = Start;
					}
					break;
				}
				case ActionChoose:
				{
					m_ActionMenuBlock.Update(deltaTime);
					if (GetIrrlicht::Events()->keyPressed(KEY_ESCAPE))
					{
						m_PlayerTurnState = Start;
					} else if (GetIrrlicht::Events()->keyPressed(KEY_RETURN))
					{
						// Super Simplified for now.
						if (m_ActionMenuBlock.SelectedAction() == 0) // Attack
						{
							m_SelectableTiles.push_back(m_Project->GetSelectableTile());
							m_SelectableTiles[m_SelectableTiles.size() - 1].PlayAnimation(L"Red Space");
							m_SelectableTiles[m_SelectableTiles.size() - 1].Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.Y * GetScaleY());
							m_SelectableTiles[m_SelectableTiles.size() - 1].BFPosition(BattleFieldPosition(m_ActiveUnit->BFPosition().X() + 1,m_ActiveUnit->BFPosition().Y(),GetHeightByPos(m_ActiveUnit->BFPosition().X() + 1,m_ActiveUnit->BFPosition().Y())));
							m_SelectableTiles.push_back(m_Project->GetSelectableTile());
							m_SelectableTiles[m_SelectableTiles.size() - 1].PlayAnimation(L"Red Space");
							m_SelectableTiles[m_SelectableTiles.size() - 1].Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.Y * GetScaleY());
							m_SelectableTiles[m_SelectableTiles.size() - 1].BFPosition(BattleFieldPosition(m_ActiveUnit->BFPosition().X() - 1,m_ActiveUnit->BFPosition().Y(),GetHeightByPos(m_ActiveUnit->BFPosition().X() - 1,m_ActiveUnit->BFPosition().Y())));
							m_SelectableTiles[m_SelectableTiles.size() - 1].PlayAnimation(L"Red Space");
							m_SelectableTiles.push_back(m_Project->GetSelectableTile());
							m_SelectableTiles[m_SelectableTiles.size() - 1].PlayAnimation(L"Red Space");
							m_SelectableTiles[m_SelectableTiles.size() - 1].Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.Y * GetScaleY());
							m_SelectableTiles[m_SelectableTiles.size() - 1].BFPosition(BattleFieldPosition(m_ActiveUnit->BFPosition().X(),m_ActiveUnit->BFPosition().Y() + 1,GetHeightByPos(m_ActiveUnit->BFPosition().X(),m_ActiveUnit->BFPosition().Y() + 1)));
							m_SelectableTiles[m_SelectableTiles.size() - 1].PlayAnimation(L"Red Space");
							m_SelectableTiles.push_back(m_Project->GetSelectableTile());
							m_SelectableTiles[m_SelectableTiles.size() - 1].PlayAnimation(L"Red Space");
							m_SelectableTiles[m_SelectableTiles.size() - 1].Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.Y * GetScaleY());
							m_SelectableTiles[m_SelectableTiles.size() - 1].BFPosition(BattleFieldPosition(m_ActiveUnit->BFPosition().X(),m_ActiveUnit->BFPosition().Y() - 1,GetHeightByPos(m_ActiveUnit->BFPosition().X(),m_ActiveUnit->BFPosition().Y() - 1)));
							m_PlayerTurnState = ActionAim;
							m_ActionMenuBlock.Visible(false);
						}
					}
					break;
				}
				case ActionAim:
				{
					CheckSelectorMovement();
					Unit* targetedUnit = GetUnitByPos(m_Selector.BFPosition().X(), m_Selector.BFPosition().Y());
					if (targetedUnit != NULL && m_ActiveUnit != targetedUnit)
					{
						m_RightDataBlock.DataUnit(targetedUnit, false, false);
						m_RightDataBlock.Sprite().Node()->setVisible(true);
					} else m_RightDataBlock.Sprite().Node()->setVisible(false);
					if (GetIrrlicht::Events()->keyPressed(KEY_ESCAPE))
					{
						m_PlayerTurnState = ActionChoose;
						for (vector<SelectableTile>::iterator i = m_SelectableTiles.begin(); i != m_SelectableTiles.end(); ++i)
						{
							i->Sprite().Node()->remove();
						}
						m_SelectableTiles.clear();
						m_ActionMenuBlock.Visible(true);
						m_Selector.BFPosition(m_ActiveUnit->BFPosition());
						m_RightDataBlock.Sprite().Node()->setVisible(false);
					} else if (GetIrrlicht::Events()->keyPressed(KEY_RETURN))
					{
						if (targetedUnit != NULL && m_ActiveUnit != targetedUnit && targetedUnit->CurrentHP() > 0)
						{
							bool validTarget = false;
							for (vector<SelectableTile>::iterator i = m_SelectableTiles.begin(); i != m_SelectableTiles.end(); ++i)
							{
								if (i->BFPosition().X() == targetedUnit->BFPosition().X() && i->BFPosition().Y() == targetedUnit->BFPosition().Y())
								{
									validTarget = true;
									break;
								}
							}
							if (validTarget)
							{
								m_PlayerTurnState = ActionConfirm;
								m_Hitdata = m_ActiveUnit->GetDamageData(m_ActionMenuBlock.SelectedAction(), targetedUnit);
								m_HitDataBlock.SetData(m_Hitdata);
								m_HitDataBlock.Visible(true);
							}
						}
					}
					break;
				}
				case ActionConfirm:
				{
					if (GetIrrlicht::Events()->keyPressed(KEY_ESCAPE))
					{
						m_PlayerTurnState = ActionAim;
						m_HitDataBlock.Visible(false);
					}
					if (GetIrrlicht::Events()->keyPressed(KEY_RETURN))
					{
						m_ActiveUnit->Attack();
						m_PlayerTurnState = ActionWaiting;
						m_HitDataBlock.Visible(false);
						m_RightDataBlock.Sprite().Node()->setVisible(false);
						m_LeftDataBlock.Sprite().Node()->setVisible(false);
						for (vector<SelectableTile>::iterator i = m_SelectableTiles.begin(); i != m_SelectableTiles.end(); ++i)
						{
							i->Sprite().Node()->remove();
						}
						m_UnitActed = true;
					}
					break;
				}	
				case ActionWaiting:
				{
					if (!m_ActiveUnit->Attacking())
					{
						m_RightDataBlock.DataUnit()->Attacked(m_Hitdata);
						m_PlayerTurnState = ReactionWaiting;
					}
					break;
				}
				case ReactionWaiting:
				{
					if (!m_RightDataBlock.DataUnit()->Hurting())
					{
						m_PlayerTurnState = Start;
						m_Selector.BFPosition(m_ActiveUnit->BFPosition());
					}
					break;
				}				
				default:
					break;
			}
		}		
		default:
			break;		
	}
	m_LeftDataBlock.Update(deltaTime);
	m_RightDataBlock.Update(deltaTime);
}

void BattleField::CheckSelectorMovement()
{
	if ((GetIrrlicht::Events()->keyPressed(KEY_KEY_A) || GetIrrlicht::Events()->keyPressed(KEY_LEFT)) && (m_Selector.BFPosition().Y() + 1.0f) < Rows())
	{
		m_Selector.BFPosition(BattleFieldPosition(m_Selector.BFPosition().X(),
																 m_Selector.BFPosition().Y() + 1.0f,
																 GetHeightByPos(round_(m_Selector.BFPosition().X()), round_(m_Selector.BFPosition().Y() + 1))));
	}
	if ((GetIrrlicht::Events()->keyPressed(KEY_KEY_D) || GetIrrlicht::Events()->keyPressed(KEY_RIGHT)) && (m_Selector.BFPosition().Y() - 1.0f) >= 0.0f)
	{
		m_Selector.BFPosition(BattleFieldPosition(m_Selector.BFPosition().X(),
																 m_Selector.BFPosition().Y() - 1.0f,
																 GetHeightByPos(round_(m_Selector.BFPosition().X()), round_(m_Selector.BFPosition().Y() - 1))));
	}
	if ((GetIrrlicht::Events()->keyPressed(KEY_KEY_W) || GetIrrlicht::Events()->keyPressed(KEY_UP)) && (m_Selector.BFPosition().X() + 1.0f) < Columns())
	{
		m_Selector.BFPosition(BattleFieldPosition(m_Selector.BFPosition().X() + 1.0f,
																 m_Selector.BFPosition().Y(),
																 GetHeightByPos(round_(m_Selector.BFPosition().X() + 1.0f), round_(m_Selector.BFPosition().Y()))));
	}
	if ((GetIrrlicht::Events()->keyPressed(KEY_KEY_S) || GetIrrlicht::Events()->keyPressed(KEY_DOWN)) && (m_Selector.BFPosition().X() - 1.0f) >= 0.0f)
	{
		m_Selector.BFPosition(BattleFieldPosition(m_Selector.BFPosition().X() - 1.0f,
																 m_Selector.BFPosition().Y(),
																 GetHeightByPos(round_(m_Selector.BFPosition().X() - 1.0f), round_(m_Selector.BFPosition().Y()))));
	}
}

void BattleField::AddUnit(Unit unit)
{
	m_Units.push_back(unit);
}

void BattleField::RemoveUnit(Unit unit)
{
	for(unsigned int i = 0; i < m_Units.size(); ++i) 
	{
		if (m_Units[i] == unit)
		{
			m_Units.erase(m_Units.begin() + i);
			return;
		} 
	}
}

void BattleField::RemoveUnit(stringw unitName)
{
	for(unsigned int i = 0; i < m_Units.size(); ++i) 
	{
		if (m_Units[i] == unitName)
		{
			m_Units.erase(m_Units.begin() + i);
			return;
		} 
	}
}

vector3df BattleField::BFPositionToPosition(BattleFieldPosition battleFieldPosition, bool unit)
{
	vector3df position = Sprite().Node()->getPosition();
	position.X = position.X - Sprite().Node()->getSize().Width / 2.0f + m_BottomTilePosition.X * GetScaleX()
	+ (battleFieldPosition.X() - battleFieldPosition.Y()) * m_TileSize.X / 2.0f * GetScaleX();
	position.Y = position.Y + Sprite().Node()->getSize().Height - m_BottomTilePosition.Y * GetScaleY()
	+ (battleFieldPosition.X() + battleFieldPosition.Y()) * m_TileSize.Y / 2.0f * GetScaleY()
	+ battleFieldPosition.Z() * m_TileSize.Z * GetScaleY()
	- (unit ? 0.0f : (m_TileSize.Y / 4.0f * GetScaleY()));
	position.Z = battleFieldPosition.X() + battleFieldPosition.Y() + (unit ? 0.0f : 0.1f);
	return position;
}

int BattleField::GetHeightByPos(int x, int y)
{
	/*std::cout << "x= " << x << ", y= " << y << "\n";
	for (unsigned int i = 0; i < m_HeightMap[0].size(); ++i) 
	{
		for (unsigned int j = 0; j < m_HeightMap.size(); ++j)
		{
			std::cout << m_HeightMap[j][i] << ",";
		}
		std::cout << "\n";
	}
	std::cout << "\n";*/
	if (x >= static_cast<int>(m_HeightMap.size()) || x < 0 || y >= static_cast<int>(m_HeightMap[x].size()) || y < 0) return -1;
	return m_HeightMap[x][static_cast<int>(m_HeightMap[x].size()) - y - 1];
}

Unit* BattleField::GetUnitByPos(int x, int y)
{
	for (vector<Unit>::iterator i = m_Units.begin(); i != m_Units.end(); ++i)
	{
		if (round_(i->BFPosition().X()) == x && round_(i->BFPosition().Y()) == y) return &(*i);
	}
	return NULL;
}

void BattleField::AddOverlay(Animation overlay, float z)
{
	AnimatingObject& newOverlay = *(new AnimatingObject());
	newOverlay.Animations().push_back(overlay);
	newOverlay.PlayAnimation(0);
	newOverlay.Sprite().Node()->getParent()->setParent(Sprite().Node());
	newOverlay.Sprite().NodeSize(newOverlay.Sprite().CurrentAnimation()->FrameSize().X * GetScaleX(), newOverlay.Sprite().CurrentAnimation()->FrameSize().Y * GetScaleY());
	newOverlay.Sprite().Node()->setPosition(vector3df((newOverlay.Sprite().CurrentAnimation()->FramePosition().X - m_TextureSize.X / 2.0f + newOverlay.Sprite().CurrentAnimation()->FrameSize().X / 2.0f) * GetScaleX(),
													(m_TextureSize.Y / 2.0f - newOverlay.Sprite().CurrentAnimation()->FramePosition().Y - newOverlay.Sprite().CurrentAnimation()->FrameSize().Y) * GetScaleY(),
													 z - Sprite().Node()->getPosition().Z));
	m_Overlayers.push_back(&newOverlay);
}

float BattleField::GetScaleX()
{
	return Sprite().Node()->getSize().Width / Sprite().CurrentAnimation()->FrameSize().X;
}

float BattleField::GetScaleY()
{
	return Sprite().Node()->getSize().Height / Sprite().CurrentAnimation()->FrameSize().Y;
}

bool BattleField::UnitSort(const Unit& i, const Unit& j)
{
	if (i.TurnCounter() == j.TurnCounter()) return i > j;
	return i.TurnCounter() > j.TurnCounter();
}

void BattleField::SetProject(Project* theProject)
{
	m_Project = theProject;
}

Project* BattleField::GetProject()
{
	return m_Project;
}

// XML
void BattleField::ReadXML(IXMLReader* xml)
{
	m_Units.clear();
	for (vector<AnimatingObject*>::iterator i = m_Overlayers.begin(); i != m_Overlayers.end(); ++i)
	{
		delete (*i);
	}
	m_Overlayers.clear();
	m_HeightMap.clear();
	bool inOverlay = false;
	bool heightMapComing = false;
	int heightMapWidth = 0;
	int heightMapHeight = 0;
	m_OverlayZs.clear();
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (NameTag.equals_ignore_case(xml->getNodeName()))
				{
					NamedObject::ReadXML(xml);
				} else if (AnimatingObjectTag.equals_ignore_case(xml->getNodeName()))
				{
					if (inOverlay)
					{
						m_Overlayers.push_back(new AnimatingObject());
						(*(m_Overlayers.end() - 1))->ReadXML(xml);
					} else
					{
						AnimatingObject::ReadXML(xml);
					}
				} else if (OverlayZTag.equals_ignore_case(xml->getNodeName()))
				{
					m_OverlayZs.push_back(xml->getAttributeValueAsFloat(L"ZValue"));
				} else if (UnitTag.equals_ignore_case(xml->getNodeName()))
				{
					m_Units.push_back(Unit());
					(m_Units.end() - 1)->ReadXML(xml);
				} else if (BattleFieldTag.equals_ignore_case(xml->getNodeName()))
				{
					m_TileSize.X = xml->getAttributeValueAsInt(L"TileSizeX");
					m_TileSize.Y = xml->getAttributeValueAsInt(L"TileSizeY");
					m_TileSize.Z = xml->getAttributeValueAsInt(L"TileSizeZ");
					m_BottomTilePosition.X = xml->getAttributeValueAsInt(L"BottomTilePositionX");
					m_BottomTilePosition.Y = xml->getAttributeValueAsInt(L"BottomTilePositionY");
				} else if (OverlayTag.equals_ignore_case(xml->getNodeName()))
				{
					inOverlay = true;
				} else if (HeightMapTag.equals_ignore_case(xml->getNodeName()))
				{
					heightMapWidth = xml->getAttributeValueAsInt(L"xSize");
					heightMapHeight = xml->getAttributeValueAsInt(L"ySize");
					heightMapComing = true;
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
			{
				if (BattleFieldTag.equals_ignore_case(xml->getNodeName()))
				{
					/*PlayAnimation(0);
					m_TextureSize.X = Sprite().CurrentAnimation()->FrameSize().X;
					m_TextureSize.Y = Sprite().CurrentAnimation()->FrameSize().Y;
					Sprite().NodeSize(m_TextureSize.X * 3, m_TextureSize.Y * 3);
					Sprite().Node()->setPosition(vector3df(0, 0, 400));
					for (vector<Unit>::iterator i = m_Units.begin(); i != m_Units.end(); ++i)
					{
						i->PlayAnimation(0);
					}
					int z = 0;
					for (vector<AnimatingObject*>::iterator i = m_Overlayers.begin(); i != m_Overlayers.end(); ++i)
					{
						
						(*i)->PlayAnimation(0);
						(*i)->Sprite().Node()->getParent()->setParent(Sprite().Node());
						(*i)->Sprite().NodeSize((*i)->Sprite().CurrentAnimation()->FrameSize().X * GetScaleX(), (*i)->Sprite().CurrentAnimation()->FrameSize().Y * GetScaleY());
						(*i)->Sprite().Node()->setPosition(vector3df(((*i)->Sprite().CurrentAnimation()->FramePosition().X - m_TextureSize.X / 2.0f + (*i)->Sprite().CurrentAnimation()->FrameSize().X /2.0f) * GetScaleX(),
													(m_TextureSize.Y / 2.0f - (*i)->Sprite().CurrentAnimation()->FramePosition().Y - (*i)->Sprite().CurrentAnimation()->FrameSize().Y) * GetScaleY(),
													 overlayZs[z] - Sprite().Node()->getPosition().Z));
						z++;
					}
					m_Selector.PlayAnimation(0);
					m_Selector.Sprite().NodeSize(m_TileSize.X * GetScaleX(), m_TileSize.Y * 4 * GetScaleY());*/
					return;
				} 
				else if (OverlayTag.equals_ignore_case(xml->getNodeName())) inOverlay = false;
				else if (HeightMapTag.equals_ignore_case(xml->getNodeName())) heightMapComing = false;
				break;
			}
			case irr::io::EXN_TEXT:
			{
				if (heightMapComing)
				{
					irr::core::stringc heightMapString(xml->getNodeData());
					vector<irr::core::stringc> heights;
					heightMapString.split(heights, ",");
					for (int i = 0; i < heightMapWidth; ++i)
					{
						m_HeightMap.push_back(vector<int>());
					}
					for (int j = 0; j < heightMapHeight; ++j)
					{
						for (int k = 0; k < heightMapWidth; ++k)
						{
							m_HeightMap[k].push_back(atoi(heights[k + j * heightMapWidth].c_str()));
						}
					}
				}
				break;
			}
			default:
				break;
		}
	} while (xml->read());
}

void BattleField::WriteXML(IXMLWriter* xml)
{
	irr::core::array<stringw> nameArray;
	nameArray.push_back(L"TileSizeX");
	nameArray.push_back(L"TileSizeY");
	nameArray.push_back(L"TileSizeZ");
	nameArray.push_back(L"BottomTilePositionX");
	nameArray.push_back(L"BottomTilePositionY");
	irr::core::array<stringw> valueArray;
	valueArray.push_back(stringw(m_TileSize.X));
	valueArray.push_back(stringw(m_TileSize.Y));
	valueArray.push_back(stringw(m_TileSize.Z));
	valueArray.push_back(stringw(m_BottomTilePosition.X));
	valueArray.push_back(stringw(m_BottomTilePosition.Y));
	xml->writeElement(BattleFieldTag.c_str(), false, nameArray, valueArray);
	xml->writeLineBreak();
	NamedObject::WriteXML(xml);
	AnimatingObject::WriteXML(xml);
	xml->writeElement(HeightMapTag.c_str(), false, L"xSize", stringw(m_HeightMap.size()).c_str(), L"ySize", stringw(m_HeightMap[0].size()).c_str());
	xml->writeLineBreak();
	std::ostringstream heightMap;
	for (unsigned int i = 0; i < m_HeightMap[0].size(); ++i) 
	{
		for (unsigned int j = 0; j < m_HeightMap.size(); ++j)
		{
			heightMap << m_HeightMap[j][i] << ",";
		}
		xml->writeText(stringw(heightMap.str().c_str()).c_str());
		xml->writeLineBreak();
		heightMap.clear();
		heightMap.str("");
	}
	xml->writeClosingTag(HeightMapTag.c_str());
	xml->writeLineBreak();
	for (vector<Unit>::iterator i = m_Units.begin(); i != m_Units.end(); ++i)
	{
		i->WriteXML(xml);
	}
	xml->writeElement(OverlayTag.c_str(), false);
	xml->writeLineBreak();
	for (vector<AnimatingObject*>::iterator i = m_Overlayers.begin(); i != m_Overlayers.end(); ++i)
	{
		(*i)->WriteXML(xml);
		xml->writeElement(OverlayZTag.c_str(), true, L"ZValue", stringw((*i)->Sprite().Node()->getPosition().Z + Sprite().Node()->getPosition().Z).c_str());
		xml->writeLineBreak();
	}
	xml->writeClosingTag(OverlayTag.c_str());
	xml->writeLineBreak();
	xml->writeClosingTag(BattleFieldTag.c_str());
	xml->writeLineBreak();
}
