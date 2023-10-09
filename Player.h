

#ifndef Player_h
#define Player_h
#include <string>
#include <iostream>
#include "Side.h"
#include "Board.h"

class Player
{
public:
    
    Player(std::string name){ m_name = name; }
    //Create a Player with the indicated name.
    std::string name() const{return m_name;}
    //Return the name of the player.
    virtual bool isInteractive() const{return false;}
    //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const = 0;
    //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player(){}
    //Since this class is designed as a base class, it should have a virtual destructor.
    //Each concrete class derived from Player will implement the chooseMove function in its own way. Of the classes listed here, only HumanPlayer::isInteractive should return true. (When testing, we may supply other kinds of interactive players.) Each of the three classes listed here must have a constructor taking a string representing the name of the player.
private:
    std::string m_name;
};
//
class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name) : Player(name) {}
    virtual bool isInteractive() const { return true; }
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~HumanPlayer(){}
};

//
class BadPlayer : public Player
{
public:
    BadPlayer(std::string name) : Player(name) {}
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~BadPlayer(){}
};

//
class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name) : Player(name) {}
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~SmartPlayer(){}
};
////Here's your chance to shine. A SmartPlayer chooses a valid move and returns it. For any game played on a board of up to six holes per side, with up to four initial beans per hole, SmartPlayer::chooseMove must return its choice in no more than five seconds on the SEASnet Linux server lnxsrv07.seas.ucla.edu or lnxsrv09.seas.ucla.edu: (We'll give you a way of determining the time soon; until then, you can meet this requirement by limiting the depth of your game tree search or the number of game tree positions you explore to a limit you determine experimentally.) SmartPlayer::chooseMove will be worth about 15-20% of the points for this project.
#endif
