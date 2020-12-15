#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include <array>

class Board {
    std::array<unsigned char, 9> m_board;
    std::vector<int>m_validLocations;
public:
    Board();
    auto& get();
    void shuffleValidLocations();
    bool isMoveValid(int move);
    auto& getValidLocation();
    void reset();
    void display() const;
    void removeFromValidLocation(int move);
    ~Board() = default;
};


class Player {
public:
    
    unsigned char m_symbol;
    const std::string m_type;
    int m_wins;
    int m_draws;
    virtual int nextMove(Board& board)  = 0;
    Player(const unsigned char symbol, std::string&& type);
    virtual ~Player() = default;
};

class Human : public Player {
public:
    Human(unsigned char symbol) :Player{ symbol, "Human" } {}
    virtual int nextMove(Board& board)  override;
    virtual ~Human() = default;
};
class Robot : public Player {
public:
    Robot(unsigned char symbol) :Player{ symbol, "Robot" } {}
    virtual int nextMove(Board& board)  override;
    virtual ~Robot() = default;
};
class NoughtsAndCrosses
{
    Player* m_one;
    Player* m_two;
    Player* m_turn;
    Player* m_winner;
    Board m_board;

    bool draw();
    void playerMove();
    void switchPlayers();
    bool win();
public:

    NoughtsAndCrosses(Player& one, Player& two);
    Player* playerWithMostWins();
    Player* play();
    ~NoughtsAndCrosses() = default;
};

auto& Board::get() { return m_board; }
Board::Board()
{
    m_board = { '-', '-', '-', '-', '-', '-', '-', '-', '-', };
    m_validLocations = { 1,2,3,4,5,6,7,8,9 };
}
void Board::shuffleValidLocations() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(m_validLocations.begin(), m_validLocations.end(), e);
}
bool Board::isMoveValid(int move) {
    return (std::any_of(m_validLocations.begin(), m_validLocations.end(), [&](int& num) {return num == move; }));
}
auto& Board::getValidLocation() { return m_validLocations; }
void Board::reset()
{
    std::fill(m_board.begin(), m_board.end(), '-');
    m_validLocations = { 1,2,3,4,5,6,7,8,9 };
}
void Board::display() const
{
    int num = 0;
    for (auto const& cell : m_board)
    {
        num++;
        if (num % 3 == 1) {
            std::cout << "\n\n";
        }
        if (cell != '-') {
            std::cout << cell << "        ";
        }
        else
        {
            std::cout << num << "        ";
        }
    }
    std::cout << "\n\n";
}
void Board::removeFromValidLocation(int move) {
    std::remove_if(m_board.begin(), m_board.end(), [&](auto& number) {
        return number == move;
        });
}

Player::Player(const unsigned char symbol, std::string&& type)
    :m_symbol{ symbol }, m_type{ type }, m_wins{ 0 }, m_draws{ 0 } {}
 int Human::nextMove(Board& board) {
    int move = 0;
    bool isMoveValid = false;
    do
    {
        std::cout << "Enter a number on the board (e.g. 1): ";
        std::cin >> move;
        if (!(isMoveValid = board.isMoveValid(move)))
        {
            std::cout << "Invalid move! Choose a valid location!\n";
        }
    } while (!isMoveValid);
    board.removeFromValidLocation(move);
    return move - 1;
}

 int Robot::nextMove(Board& board)   {
    int move = 0;
    board.shuffleValidLocations();
    move = board.getValidLocation().at(0);
    board.getValidLocation().erase(board.getValidLocation().begin());
    return move - 1;
}

NoughtsAndCrosses::NoughtsAndCrosses(Player& one, Player& two)
    :m_one(&one), m_two(&two), m_turn(&one), m_winner(nullptr) {}
bool NoughtsAndCrosses:: draw() {
    return std::all_of(m_board.get().begin(), m_board.get().end(), [&](auto& pair) {return pair != '-'; });
}
void NoughtsAndCrosses::playerMove()
{
    m_board.get().at(m_turn->nextMove(m_board)) = m_turn->m_symbol;
}
void NoughtsAndCrosses::switchPlayers()
{
    m_turn = m_turn == m_one ? m_two : m_one;
}
bool NoughtsAndCrosses::win() {
    if (m_board.get().at(0) == m_turn->m_symbol && m_board.get().at(1) == m_turn->m_symbol && m_board.get().at(2) == m_turn->m_symbol) {
        m_winner = m_turn;
        return true;
    }
    else if (m_board.get().at(3) == m_turn->m_symbol && m_board.get().at(4) == m_turn->m_symbol && m_board.get().at(5) == m_turn->m_symbol) {
        m_winner = m_turn;
        return true;
    }
    else if (m_board.get().at(6) == m_turn->m_symbol && m_board.get().at(7) == m_turn->m_symbol && m_board.get().at(8) == m_turn->m_symbol) {
        m_winner = m_turn;
        return true;
    }
    else if (m_board.get().at(0) == m_turn->m_symbol && m_board.get().at(3) == m_turn->m_symbol && m_board.get().at(6) == m_turn->m_symbol) {
        m_winner = m_turn;
        return true;
    }
    else if (m_board.get().at(1) == m_turn->m_symbol && m_board.get().at(4) == m_turn->m_symbol && m_board.get().at(7) == m_turn->m_symbol) {
        m_winner = m_turn;
        m_winner->m_wins++;
        return true;
    }
    else if (m_board.get().at(2) == m_turn->m_symbol && m_board.get().at(5) == m_turn->m_symbol && m_board.get().at(8) == m_turn->m_symbol) {
        m_winner = m_turn;
        return true;
    }
    else if (m_board.get().at(0) == m_turn->m_symbol && m_board.get().at(4) == m_turn->m_symbol && m_board.get().at(8) == m_turn->m_symbol) {
        m_winner = m_turn;
        return true;
    }
    else if (m_board.get().at(6) == m_turn->m_symbol && m_board.get().at(4) == m_turn->m_symbol && m_board.get().at(2) == m_turn->m_symbol) {
        m_winner = m_turn;
        return true;
    }
    else
    {
        return false;
    }
}
Player* NoughtsAndCrosses::playerWithMostWins() {
    return m_one->m_wins > m_two->m_wins ? m_one : m_two;
}
Player* NoughtsAndCrosses::play() {
    while (true)
    {
        m_board.display();
        playerMove();
        if (win())
        {
            std::cout << m_winner->m_symbol << " is the winner!\n";
            break;
        }
        else if (draw())
        {
            m_winner = nullptr;
            break;
        }
        switchPlayers();
    }
    m_board.display();
    m_board.reset();
    m_turn = m_one;
    return m_winner;
}


int main()
{
    Robot robot1('X');
    Robot robot2('O');

    //pass these in to NoughtsAndCrosses if you want to play human vs human, or one if you want to play human vs Robot
    Human human1('X');
    Human human2('O');
  
    NoughtsAndCrosses game(robot1, robot2);
    int round = 3;
    int roundCount = 0;
    Player* winner = nullptr;
    do
    {
        int gameCount = 1;
        int totalGamesinRound = 3;
        std::cout << "START GAME!\n";
        system("pause");
        system("cls");
        std::cout << "\nROUND " << ++roundCount << ". . .\n";
        do
        {
            std::cout << "Game " << gameCount << " of round " << roundCount << "\n";
            winner = game.play();

            if (winner != nullptr)
            {
                std::cout << "Winner of game " << gameCount << " is type: " << winner->m_type << ": " << winner->m_symbol << "\n";
                winner->m_wins++;
            }
            else
            {              
                std::cout << "Game " << gameCount << " is a draw!\n";  
                robot1.m_draws++;
                robot2.m_draws++;
            }
            gameCount++;
            totalGamesinRound--;
        } while (totalGamesinRound != 0);

        /* std::cout << "Game 2: Human vs Robot\n";
         game.play(robot1, robot1);*/
        std::cout << "Wins for " << robot1.m_type << ": Player : " << robot1.m_symbol << " - " << robot1.m_wins << "\n";
        std::cout << "Wins for " << robot2.m_type << ": Player : " << robot2.m_symbol << " - " << robot2.m_wins << "\n";
        std::cout << "Drawed: " << robot1.m_draws << "\n";

        std::cout << "Winner of round " << roundCount << " is " << game.playerWithMostWins()->m_symbol << "\n";
        round--;
    } 
    while (round != 0);
}