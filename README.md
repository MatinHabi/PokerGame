# Poker-Game

This is a terminal poker game written in c++

The game must have:
- 52 cards in the deck (suffled at the start)
- Chips for currency (black = $100 , green = $25 , blue = $10 , red = $5 , white = $1)
- moves (check = dont bet any money, raise = raise bet, fold = quit for the round, calling = match bet)
- number of players (starts with P1 -> P2 -> P3 -> etc.) 
    * for single player i could have 4 other player with their own cards making predictable decisions
- 5 card combo (best combo wins the bets)

# The Game itself
- each player start with 2 cards
- first round of betting starts immediatley after reciving the 2 cards AND before any cards are on the board *FIRST BETTING ROUND*
- This betting round only allows players to FOLD, CALL, RAISE, not check *FIRST BETTING ROUND*
    ## First Round
    - the top card is discared
    - 3 cards FACE UP are placed on the table
    - Betting commences (players can now check) *SECOND BETTING ROUND*
    ## Second Round
    - top card is discarded 
    - 1 card is now added to the board FACE UP 
    - Betting commences (players can now check) *THIRD BETTING ROUND*
    ## Third round [FINAL]
    - top card is discarded 
    - 1 card is now added to the board FACE UP 
    - Betting commences (players can now check) *FOURTH BETTING ROUND*
## SHOWDOWN
- players reveal their cards
- best hand wins all of the bets


# Summary
- given 2 cards -> BET 
- top card discarded, 3 cards FACE UP -> BET
- top card discarded, 1 cards FACE UP -> BET
- top card discarded, 1 cards FACE UP -> BET

Discarded cards : 3
Total cards placed (FACE UP) : 5

# moves
Fold- loses cards for that round so they cant play (cars must not be seen by other players)
Call - places the bet equal to what is required
Raise - Raises the bet amount (all other players must equal this amount) the new amount *Must NOT* be greater than how much money the player has
Check - $0 bet (if all players check that round is FREE otherwise if a bet is placed all other players *MUST* CALL, FOLD, or RAISE) [CHECKING not possible if there is a bet already made by a player before you]

# Players Hands
Players can use:
- 2 of *THEIR* whole cards and 3 community cards
- 1 of *THEIR* whole cards and 4 community cards
- 0 of *THEIR* whole cards and 5 community cards (CHOPPED POT)

# CHOPPED POT
2 or more players have the same hand and therefore the pot is split between them 

# C++ DEV

## CLASSES

**Cards**
- 52 cards
- func: shuffle
- func: get top card
**Enum Chips**

**Player**
- has balance
- recieves 2 cards

**Dealer/Player**
- controls the pot
- distrubuting winnings to winner
- deals cards

 
# ERROR INDEX
101 -> Invalid chip colour
102 -> Failed to find chip value
Texas holdem poker
requirements:
- player has cards + chips
- able to make bets 
- able to make moves (fold, call, raise)

- cards will shuffle at the start of each round
- burn the top card
- rank for each card will be between 2-14 because the lowest int card val 2 (1 = A)


# POKER HAND EVALUATION
obviously the most important aspect of poker is its compasiron engine and how it decides a winner. I have compiled my underastanding of comparisons required to determine a winner:

| Hand Type       | keys Format            | Why                       |
| --------------- | ---------------------- | ------------------------- |
| High Card       | `{r1,r2,r3,r4,r5}`     | Compare cards highest to  |
|                 |                        | lowest                    |
|                 |                        |                           |
| One Pair        | `{pair,k1,k2,k3}`      | Comare Pair first, then   |
|                 |                        | kickers                   |
| Two Pair        | `{highPair,lowPair,k}` | Compare higher pair first |
|                 |                        | then kickers              |
| Three of a Kind | `{trip,k1,k2}`         | Trips first               |
|                 |                        |                           |
| Straight        | `{top}`                | Only highest card matters |
|                 |                        |                           |
| Flush           | `{r1,r2,r3,r4,r5}`     | Same as high card         |
|                 |                        |                           |
| Full House      | `{trip,pair}`          | Trips decide first        |
|                 |                        |                           |
| Four of a Kind  | `{quad,k}`             | Quad rank first           |
|                 |                        |                           |
| Straight Flush  | `{top}`                | Same logic as straight    |
|                 |                        |                           |
| Royal Flush     | `{14}`                 | Highest possible straight |
|                 |                        |                           |

