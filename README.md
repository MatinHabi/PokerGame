# PokerGame
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

