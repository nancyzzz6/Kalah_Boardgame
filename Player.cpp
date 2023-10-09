
#include "Player.h"

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
//    no beans on the player's side
    if(b.beansInPlay(s)==0)
        return -1;
    int move;
    while(true)
    {
        std::cout << Player::name() << ", please select your move: ";
        std::cin >> move;
        if (b.beans(s, move) > 0)
            return move;
        else
            std::cout << "Your move must be a non-empty valid hole. Please try again." << std::endl;
    }
}


int BadPlayer::chooseMove(const Board& b, Side s) const
{
    if(b.beansInPlay(s) == 0)
        return -1;
    int move = 1;
    for (; move <= b.holes(); move++)
    {
//        choose the smallest valid hole as move
        if (b.beans(s, move) > 0)
            break;
    }
    cout << "Press ENTER to continue"<<std::endl;
    cin.ignore(1000, '\n');
    std::cout << Player::name() << " choose move " << move << std::endl;
        return move;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    if(b.beansInPlay(s) == 0)
        return -1;
    int move = b.holes();
    for (; move >= 1; move--)
    {
//        choose the smallest valid hole as move
        if (b.beans(s, move) > 0)
            break;
    }
    cout << "Press ENTER to continue"<<std::endl;
    cin.ignore(1000, '\n');
    std::cout << Player::name() << " choose move " << move << std::endl;
        return move;
}
