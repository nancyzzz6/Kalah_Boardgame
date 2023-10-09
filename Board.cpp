
#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    //Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
    if (nHoles <= 0)
        m_holes = 1;
    else
        m_holes = nHoles;
    if(nInitialBeansPerHole<0)
        m_beansPerHole = 0;
    else
        m_beansPerHole = nInitialBeansPerHole;
    m_north_side.push_back(0);
    m_south_side.push_back(0);
    for(int i = 0; i < m_holes; i++)
    {
        m_north_side.push_back(m_beansPerHole);
        m_south_side.push_back(m_beansPerHole);
    }
}

int Board::holes() const
{
    return m_holes;
}

int Board::beans(Side s, int hole) const
{
    //    Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
    if( hole > m_holes || hole < 0)
        return -1;
    if(s == SOUTH)
        return m_south_side[hole];
    else
        return m_north_side[hole];
}

int Board::beansInPlay(Side s) const
{
    //  Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
    int sum = 0;
//    summing up south side beans
    if (s == SOUTH)
    {
        for(int i = 1; i <= m_holes; i++)
            sum += m_south_side[i];
    }
    //    summing up north side beans
    else
    {
        for(int i = 1; i <= m_holes; i++)
            sum += m_north_side[i];
    }
    return sum;
}

int Board::totalBeans() const
{
    //Return the total number of beans in the game, including any in the pots.
    int totBeans = 0;
    for(int i = 0; i <= m_holes; i++)
    {
        totBeans += m_north_side[i];
        totBeans += m_south_side[i];
    }
    return totBeans;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    
    //If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
//    if hole is empty or invalid
    if(beans(s, hole) == -1 || beans(s, hole) == 0 || hole <= 0)
        return false;
    bool onNorth = false;
    int beansNeedSow;
    endHole = hole;
    int tempHole = endHole;

    if(s == NORTH)
    {
        onNorth = true;
        beansNeedSow = m_north_side[hole];
        m_north_side[hole] = 0;
    }
    else
    {
        beansNeedSow = m_south_side[hole];
        m_south_side[hole] = 0;
    }
//    sow untill all beans in the pot were sowed
    while(beansNeedSow > 0)
    {
        --beansNeedSow;
//        start from north player
        if(s == NORTH)
        {
            if(onNorth && endHole > 0)
            {
                endHole -= 1;
                m_north_side[endHole] += 1;
            }
//            move from north to south
            else if (onNorth && endHole == 0)
            {
                onNorth = false;
                endHole += 1;
                m_south_side[endHole] += 1;
            }
//            sow on south side
            else if(onNorth == false && endHole < m_holes)
            {
                endHole += 1;
                m_south_side[endHole] += 1;
            }
//            move from south back to north
            else if(onNorth == false && endHole == m_holes)
            {
                onNorth = true;
                endHole = m_holes;
                m_north_side[endHole] +=1;
            }
        }
//        start from south player
        else
        {
            if(onNorth == false && endHole < m_holes && endHole!= 0)
            {
                endHole += 1;
                m_south_side[endHole] += 1;
            }
//            move from south to north
            else if (onNorth == false && endHole == m_holes)
            {
                tempHole = endHole;
                endHole = 0;
//                onNorth = true;
                m_south_side[0] += 1;
            }
//            sow on the last hole in north side
            else if(onNorth == false && endHole == 0)
            {
                endHole = tempHole;
                m_north_side[m_holes] += 1;
                onNorth = true;
            }
//            sow in general holes in north side
            else if(onNorth == true && endHole <= m_holes&&endHole>1 && tempHole == m_holes)
            {
                tempHole = endHole;
                endHole -=1;
                m_north_side[endHole] += 1;
            }
//            move from north back to south
            else if(onNorth == true && endHole == 0)
            {
                onNorth = false;
                endHole = 1;
                m_south_side[endHole] +=1;
            }
        }
    }
    
    if(onNorth == true)
        endSide = NORTH;
    else
        endSide = SOUTH;
    return true;
}


bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    // If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
    if( hole > m_holes || hole <= 0)
        return false;
//    if it's north player
    if( potOwner == NORTH)
    {
        switch (s)
        {
            case NORTH:
                m_north_side[0] += m_north_side[hole];
                m_north_side[hole] = 0;
                break;
            case SOUTH:
                m_north_side[0] += m_south_side[hole];
                m_south_side[hole] = 0;
                break;
        }
    }
//    if it's south player
    else
    {
        switch (s)
        {
            case NORTH:
                m_south_side[0] += m_north_side[hole];
                m_north_side[hole] = 0;
                break;
            case SOUTH:
                m_south_side[0] += m_south_side[hole];
                m_south_side[hole] = 0;
                break;
        }
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans)
{
    // If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
    if(hole > m_holes || hole < 0 )
        return false;
    if(s == NORTH)
        m_north_side[hole] = beans;
    else
        m_south_side[hole] = beans;
    return true;
}


