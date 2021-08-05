#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include <string>

enum class CardSuit
{
    club,
    diamond,
    heart,
    spade,

    max_suits
};


enum class CardRank
{
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8,
    rank_9,
    rank_10,
    rank_Jack,
    rank_Queen,
    rank_King,
    rank_Ace,

    max_ranks
};

struct Card
{
    CardRank rank{};
    CardSuit suit{};
};

struct Player
{
    int score{};
};

void printCard(const Card& card)
{
    switch (card.rank)
    {
    case CardRank::rank_2:          std::cout << "2";   break;
    case CardRank::rank_3:          std::cout << "3";   break;
    case CardRank::rank_4:          std::cout << "4";   break;
    case CardRank::rank_5:          std::cout << "5";   break;
    case CardRank::rank_6:          std::cout << "6";   break;
    case CardRank::rank_7:          std::cout << "7";   break;
    case CardRank::rank_8:          std::cout << "8";   break;
    case CardRank::rank_9:          std::cout << "9";   break;
    case CardRank::rank_10:         std::cout << "10";  break;
    case CardRank::rank_Jack:       std::cout << "J";   break;
    case CardRank::rank_Queen:      std::cout << "Q";   break;
    case CardRank::rank_King:       std::cout << "K";   break;
    case CardRank::rank_Ace:        std::cout << "A";   break;
    default:
        std::cout << "?";
        break;
    }

    switch (card.suit)
    {
    case CardSuit::club:            std::cout << "C";   break;
    case CardSuit::diamond:         std::cout << "D";   break;
    case CardSuit::heart:           std::cout << "H";   break;
    case CardSuit::spade:           std::cout << "S";   break;
    default:
        std::cout << "?";
        break;
    }

}

constexpr int minValue{ 17 };
constexpr int maxValue{ 21 };

using myArray = std::array<Card, static_cast<int>(CardRank::max_ranks) * static_cast<int>(CardSuit::max_suits)>;

void createDeck(myArray& deck)
{
    int card_nr{ 0 };
    for (int suit{ 0 }; suit < static_cast<int>(CardSuit::max_suits); ++suit)
    {
        for (int rank{ 0 }; rank < static_cast<int>(CardRank::max_ranks); ++rank)
        {
            deck[card_nr].rank = static_cast<CardRank>(rank);
            deck[card_nr].suit = static_cast<CardSuit>(suit);

            ++card_nr;
        }
    }
}

void printDeck(const myArray& deck)
{
    for (const auto& card : deck)
    {
        printCard(card);
        std::cout << " ";
    }
    std::cout << '\n';
}

void shuffleDeck(myArray& deck)
{
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    std::shuffle(deck.begin(), deck.end(), mt);
}

int getCardValue(const Card& card, int& card_nr, const Player& player)
{
    card_nr++;
    switch (card.rank)
    {
    case CardRank::rank_2:          return 2;
    case CardRank::rank_3:          return 3;
    case CardRank::rank_4:          return 4;
    case CardRank::rank_5:          return 5;
    case CardRank::rank_6:          return 6;
    case CardRank::rank_7:          return 7;
    case CardRank::rank_8:          return 8;
    case CardRank::rank_9:          return 9;
    case CardRank::rank_10:         return 10;
    case CardRank::rank_Jack:       return 10;
    case CardRank::rank_Queen:      return 10;
    case CardRank::rank_King:       return 10;
    case CardRank::rank_Ace:        
        if (player.score < 11) return 11;
        else return 1;
    default:
        return 0;
    }
}

bool blackjack(myArray deck, Player& player, Player& dealer)
{
    bool gameRunning = true;
    int card_dealt{ 0 };
    dealer.score = getCardValue(deck[0], card_dealt, dealer);
    player.score = getCardValue(deck[1], card_dealt, player) + getCardValue(deck[2], card_dealt, player);
    std::string player_choice{ "hit" };
    std::string dealer_choice{ "hit" };

    do {
        if (player_choice == "hit")
        {
            std::cout << "Your score is: " << player.score << "\nDo you want to hit or stand? ";
            std::cin >> player_choice;
            if (player.score < minValue && player_choice == "stand")
            {
                std::cout << "You can't stand under " << minValue << " points!\n";
                player_choice = "hit";
            }
            if (player_choice == "hit") player.score += getCardValue(deck[card_dealt], card_dealt, player);

            if (player.score == maxValue) return true;
            if (player.score > maxValue) return false;
        }
        else
        {
            if (dealer_choice == "hit")
            {
                dealer.score += getCardValue(deck[card_dealt], card_dealt, dealer);
                if (dealer.score >= minValue && dealer.score < maxValue) dealer_choice = "stand";
            }

            if (dealer_choice == "stand")
            {
                if (player.score > dealer.score) return true;
                else return false;
            }

            if (dealer.score > maxValue) return true;
        }

    } while (gameRunning);
}

int main()
{
    myArray mainDeck;
    Player user;
    Player dealer;

    createDeck(mainDeck);
    shuffleDeck(mainDeck);
    printDeck(mainDeck);

    char tryAgain{};
    do {
        if (blackjack(mainDeck, user, dealer))
        {
            std::cout << "Player won with " << user.score << " against dealer with " << dealer.score;
        }
        else
        {
            if (user.score == dealer.score)
            {
                std::cout << "Player and dealer tied with a score of " << user.score << " vs " << dealer.score;
            }
            else
                std::cout << "Dealer won with " << dealer.score << " against player with " << user.score;
        }

        std::cout << "\nDo you want to try again? y/n"; std::cin >> tryAgain;
        shuffleDeck(mainDeck);
    } while (tryAgain == 'y');
}
