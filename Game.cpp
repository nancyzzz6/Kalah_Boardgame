
#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north)
:m_board(b),m_south(south),m_north(north)
{
}

void Game::display() const
{
//    North side
    cout<<endl;
    cout << "\t\t\t\t\tNorth" << endl;
    cout << "\t\t\t\t";
    // North hole index
      for (int i = 1; i <= m_board.holes(); i++)
      {
        cout << i << "  ";
      }
      cout << endl;
      cout << "\t\t\t\t--------------" << endl;
      
      // North holes
      cout << "\t\t\t\t";
      for (int i = 1; i <= m_board.holes(); i++)
      {
          cout << m_board.beans(NORTH, i) << "  ";
      }
      cout << endl;
      
      // two pots and beans amount
      cout <<m_north->name()<< "'s pot  " << m_board.beans(NORTH, 0);
      for (int i = 0; i < m_board.holes() * 4; i++)
        cout << " ";
      cout << m_board.beans(SOUTH, 0) <<"  "<< m_south->name()<< "'s pot" << endl;
      
      // South holes
      cout << "\t\t\t\t";
      for (int i = 1; i <= m_board.holes(); i++)
      {
          cout << m_board.beans(SOUTH, i) << "  ";
      }
      cout << endl;
      cout << "\t\t\t\t--------------" << endl;
    // south hole index
      cout << "\t\t\t\t";
      for (int i = 1; i <= m_board.holes(); i++)
      {
        cout << i << "  ";
      }
      cout <<endl;
      // south side
      cout << "\t\t\t\t\tSouth" << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    //If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
    over = false;
    // If one side can't make further move
    if (m_board.beansInPlay(NORTH) == 0 || m_board.beansInPlay(SOUTH) == 0)
    {
        over = true;
        // Determine the winner
        if (m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0))
        {
            hasWinner = true;
            winner = NORTH;
        }
        else if (m_board.beans(NORTH, 0) < m_board.beans(SOUTH, 0))
        {
            hasWinner = true;
            winner = SOUTH;
        }
        else
            // It's a tie
            hasWinner = false;
    }
}

bool Game::move(Side s)
{
    //Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
    bool turnEnd = false;
    Side endSide;
    int endHole;
    bool validSow;
    while(!turnEnd)
    {
        if(s == NORTH)
        {
            int selectMove = m_north->chooseMove(m_board, s);
            //            if no avalible beans in north side
            if (selectMove == -1)
            {
                for(int i = 1; i <= m_board.holes(); i++)
                {
                    m_board.moveToPot(SOUTH, i, SOUTH);
                }
                return false;
            }
            //            if there's valid move
            validSow = m_board.sow(s, selectMove, endSide, endHole);
            if(validSow == false)
                return false;
            //        if last bean was in s's pot
            if (endHole == 0)
            {
//                continue;
            }
            else
            {
                turnEnd = true;
                if(endSide==NORTH && m_board.beans(s, endHole)==1 && m_board.beans(SOUTH, endHole)>0)
                {
                    //                    capture
                    m_board.moveToPot(SOUTH, endHole, s);
                    m_board.moveToPot(NORTH, endHole, s);
                }
            }
            display();
        }
        //        SOUTH's turn
        else
        {
            int selectMove = m_south->chooseMove(m_board, s);
            //            if no avalible beans in north side
            if (selectMove == -1)
            {
                for(int i = 1; i <= m_board.holes(); i++)
                {
                    m_board.moveToPot(NORTH, i, NORTH);
                }
                return false;
            }
            //            if there's valid move
            validSow = m_board.sow(s, selectMove, endSide, endHole);
            if(validSow == false)
                return false;
            //        if last bean was in s's pot
            if (endHole == m_board.holes() && endSide == NORTH)
            {
//                continue;
            }
            else
            {
                turnEnd = true;
                if(endSide==SOUTH && m_board.beans(s, endHole)==1 && m_board.beans(NORTH, endHole)>0)
                {
                    // capture
                    m_board.moveToPot(NORTH, endHole, s);
                    m_board.moveToPot(SOUTH, endHole, s);
                }
            }
            display();
        }
    }
    return true;
}

void Game::play()
{
    bool over = false;
    bool hasWinner = false;
    Side winner;
    status(over, hasWinner, winner);
    //if both are not interactive
    display();
    if (!m_north->isInteractive() && !m_south->isInteractive())
    {
        while (!over)
        {
            move(NORTH);
            status(over, hasWinner, winner);
            if(over)
                break;
            move(SOUTH);
            status(over, hasWinner, winner);
        }
    }
//    if at least one side is not interactive
    else
    {
        while (!over) {
            move(NORTH);
            status(over, hasWinner, winner);
            move(SOUTH);
            status(over, hasWinner, winner);
        }
    }
//when game is over, move left beans to their pot
    cout <<"Sweeping beans......";
    for(int i = 1; i <= m_board.holes(); i++)
    {
        m_board.moveToPot(NORTH, i, NORTH);
        m_board.moveToPot(SOUTH, i, SOUTH);
    }
//    show final result
    display();
    status(over, hasWinner, winner);
    if(hasWinner)
    {
        if(winner == NORTH)
        {
            cout<<"The winner is "<<m_north->name()<<". Congratulation!"<<endl;
        }
        else
        {
            cout<<"The winner is "<<m_south->name()<<". Congratulation!"<<endl;
        }
    }
    else
    {
        cout<<"The result is tie."<<endl;
    }

    
}


int Game::beans(Side s, int hole) const
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
{
    if (hole < 0 || hole > m_board.holes())
        return -1;
    return m_board.beans(s, hole);
}
