/* FFTA3 Project: http://beacon515l.co.uk/ffta3
 *
 * Movement Check Algorithm V.1.1
 * July 25 2015
 *
 * Language: C++
 *
 * Author: ZenonX @ http://beacon515l.co.uk/ffta3
 *
 * Description: This program receives map data as well as the
 * movement stats and position of a single character. Then, it
 * computes the available positions that the character can move
 * to as well as the shortest path.
 * Ported by Hoera63
 */

#include "../Header/PathFinder.h"
#include "../Header/BattleField.h"

PathFinder::PathFinder()
{
	m_BattleField = NULL;
	m_Unit = NULL;
}

PathFinder::~PathFinder()
{
	//dtor
}

void PathFinder::FindPaths(Unit* theUnit)
{
	m_Unit = theUnit;
	if (m_Unit == NULL || m_BattleField == NULL) return;
	m_PathData.clear();
	m_MoveData.clear();
	for (unsigned int i = 0; i < m_BattleField->Rows(); ++i)
	{
		m_PathData.push_back(vector<PathData>());
		m_MoveData.push_back(vector<int>());
	}
	for (unsigned int i = 0; i < m_BattleField->Rows(); ++i)
		for (unsigned int j = 0; j < m_BattleField->Columns(); ++j)
		{
			m_PathData[i].push_back(MoveImpossible);
			m_MoveData[i].push_back(-1);
		}
	m_MoveLeft = m_Unit->Move();
	int unitX = round_(m_Unit->BFPosition().X());
	int unitY = round_(m_Unit->BFPosition().Y());
	
	/** Generate path map */
	
    if (m_MoveLeft >= 1 && MoveCheck(unitX, unitY, unitX, unitY - 1)) // Move North
	{ 
        m_MoveData[unitY - 1][unitX] = m_MoveLeft;
        m_PathData[unitY - 1][unitX] = MoveNorth;
    } else
    {
        if (m_MoveLeft >= 2 && MoveCheck(unitX, unitY, unitX, unitY - 2)) // Leap North
        {
            m_MoveData[unitY - 2][unitX] = m_MoveLeft;
        	m_PathData[unitY - 2][unitX] = MoveNorthDouble;
        }
    }
    if (m_MoveLeft >= 1 && MoveCheck(unitX, unitY, unitX + 1, unitY)) // Move East
    {
        m_MoveData[unitY][unitX + 1] = m_MoveLeft;
		m_PathData[unitY][unitX + 1] = MoveEast;
    } else
    {
        if (m_MoveLeft >= 2 && MoveCheck(unitX, unitY, unitX + 2, unitY)) // Leap East
		{
            m_MoveData[unitY][unitX + 2] = m_MoveLeft;
			m_PathData[unitY][unitX + 2] = MoveEastDouble;
        }
    }
    if(m_MoveLeft >= 1 && MoveCheck(unitX, unitY, unitX, unitY + 1)) // Move South
	{
		m_MoveData[unitY + 1][unitX] = m_MoveLeft;
		m_PathData[unitY + 1][unitX] = MoveSouth;
    } else
    {
        if(m_MoveLeft >= 2 && MoveCheck(unitX, unitY, unitX, unitY + 2)) // Leap South
		{
            m_MoveData[unitY + 2][unitX] = m_MoveLeft;
			m_PathData[unitY + 2][unitX] = MoveSouthDouble;
        }
    }
    if (m_MoveLeft >= 1 && MoveCheck(unitX, unitY, unitX - 1, unitY)) // Move West
	{
        m_MoveData[unitY][unitX - 1] = m_MoveLeft;
		m_PathData[unitY][unitX - 1] = MoveWest;
    } else
    {
        if (m_MoveLeft >= 2 && MoveCheck(unitX, unitY, unitX - 2, unitY)) // Leap West
		{
            m_MoveData[unitY][unitX - 2] = m_MoveLeft;
			m_PathData[unitY][unitX - 2] = MoveWestDouble;
        }
    }
}

bool PathFinder::MoveCheck(int xOrigin, int yOrigin, int xTarget, int yTarget)
{
	/** Check if target location is valid */

    if (m_MoveLeft <= 0)
        // NOT VALID (no move remaining)
        return false;
    if (xTarget < 0 || yTarget < 0 || xTarget >= static_cast<int>(m_BattleField->Columns()) || yTarget >= static_cast<int>(m_BattleField->Rows()))
        // NOT VALID (out of bounds)
        return false;
    /*if (m_BattleField->GetHeightByPos(xTarget, yTarget) == 0)
        // NOT VALID (out of bounds)
        return false;*/
    /*if(m_BattleField->GetHeightByPos(xTarget, yTarget) < 0 && charData->enterWater == 0 && charData->ignoreWater == 0) {
        // NOT VALID (tile is water and unit cannot enter water)
        return false;
    }*/
    if (m_BattleField->GetUnitByPos(xTarget, yTarget) != NULL)
    	// NOT VALID (Already occupied)
    	return false;
	if ((xTarget - xOrigin) == 2)
	{
		if (m_BattleField->GetUnitByPos(xOrigin + 1, yTarget) != NULL)
			// NOT VALID (Already occupied)
			return false;
	} else if ((xTarget - xOrigin) == -2)
	{
		if (m_BattleField->GetUnitByPos(xOrigin - 1, yTarget) != NULL)
			// NOT VALID (Already occupied)
			return false;
	} else if ((yTarget - yOrigin) == 2)
	{
		if (m_BattleField->GetUnitByPos(xTarget, yOrigin + 1) != NULL)
			// NOT VALID (Already occupied)
			return false;
	} else if ((yTarget - yOrigin) == -2)
	{
		if (m_BattleField->GetUnitByPos(xTarget, yOrigin - 1) != NULL)
			// NOT VALID (Already occupied)
			return false;
	}
    int originHeight = m_BattleField->GetHeightByPos(xOrigin, yOrigin);
    originHeight = originHeight > 0 ? originHeight : -originHeight;

    int targetHeight = m_BattleField->GetHeightByPos(xTarget, yTarget);
    targetHeight = targetHeight > 0 ? targetHeight : -targetHeight;

    int heightDifference = originHeight - targetHeight;
    heightDifference = heightDifference > 0 ? heightDifference : -heightDifference;
    if(heightDifference > m_Unit->Jump())
        // NOT VALID (tile is too high or too low)
        return false;

    /** Check if a shorter path has already been mapped */

    if (m_MoveData[yTarget][xTarget] != -1) // Data has been mapped previously
        if (m_MoveData[yTarget][xTarget] >= m_MoveLeft) // If shorter or equal path already mapped
            return false;

    if (xTarget == xOrigin && yTarget == yOrigin)
        // NOT VALID (target is origin)
        return false;

    /** Check if it is possible to move again */

    m_MoveLeft--;
    if (MoveCheck(xTarget, yTarget, xTarget, yTarget - 1)) // Move North
	{
        m_MoveData[yTarget - 1][xTarget] = m_MoveLeft;
        m_PathData[yTarget - 1][xTarget] = MoveNorth;
    } else
    {
        m_MoveLeft--;
        if (MoveCheck(xTarget, yTarget, xTarget, yTarget - 2)) // Leap North
        {
            m_MoveData[yTarget - 2][xTarget] = m_MoveLeft;
        	m_PathData[yTarget - 2][xTarget] = MoveNorthDouble;
        }
        m_MoveLeft++;
    }
    if (MoveCheck(xTarget, yTarget, xTarget + 1, yTarget)) // Move East
    {
        m_MoveData[yTarget][xTarget + 1] = m_MoveLeft;
		m_PathData[yTarget][xTarget + 1] = MoveEast;
    } else
    {
        m_MoveLeft--;
        if (MoveCheck(xTarget, yTarget, xTarget + 2, yTarget)) // Leap East
        {
            m_MoveData[yTarget][xTarget + 2] = m_MoveLeft;
			m_PathData[yTarget][xTarget + 2] = MoveEastDouble;
        }
        m_MoveLeft++;
    }
    if (MoveCheck(xTarget, yTarget, xTarget, yTarget + 1)) // Move South
    {
        m_MoveData[yTarget + 1][xTarget] = m_MoveLeft;
		m_PathData[yTarget + 1][xTarget] = MoveSouth;
    } else 
    {
        m_MoveLeft--;
        if (MoveCheck(xTarget, yTarget, xTarget, yTarget + 2)) // Leap South
        {
            m_MoveData[yTarget + 2][xTarget] = m_MoveLeft;
			m_PathData[yTarget + 2][xTarget] = MoveSouthDouble;
        }
        m_MoveLeft++;
    }
    if (MoveCheck(xTarget, yTarget, xTarget - 1, yTarget)) // Move West
    {
        m_MoveData[yTarget][xTarget - 1] = m_MoveLeft;
		m_PathData[yTarget][xTarget - 1] = MoveWest;
    } else 
    {
        m_MoveLeft--;
        if (MoveCheck(xTarget, yTarget, xTarget - 2, yTarget)) // Leap West
        {
            m_MoveData[yTarget][xTarget - 2] = m_MoveLeft;
			m_PathData[yTarget][xTarget - 2] = MoveWestDouble;
        }
        m_MoveLeft++;
    }

    m_MoveLeft++;
    return true; // Valid move
}
