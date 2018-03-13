Caltech CS2 Assignment 9: Othello

A simple Othello implementation

Contributors:

Jagath Vytheeswaran
Hyung Ju Suh (Terry)

See [assignment9_part1.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part1.html) and [assignment9_part2.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part2.html)

Both Terry and I contributed about equally to this project. Terry worked on the initial hueristic and I created the initial minimax. Then, Terry debugged the minimax (which was for some reaason performing worse than just using a hueristic) while I changed the hueristic to be a bit more accurate (the original hueristic would multiply the score by 3 for corners and -3 for squares adjacent to corners - this would penalize you for choosing corners when you were already losing, which makes no sense).

Then, Terry worked on adding mobility and I worked on adding stable disk recognition. Sadly, these improvements seemed to make our bot perform worse, so while we have the code in our commit history for you to see, they're not included in our final bot for tournament.

To make our bot tournament-ready, we changed our hueristic to something that scaled rewards based on score and made sure that corners would be rewarding no matter what (and pieces adjacent to edges would be bad no matter what). I realized that even that is a bit of a poor hueristic (as pieces adjacent to corners are stable and therefore quite good if you have the corners already), so I added stable disks (you can see them in commit e8b02b3) and Terry added mobility (also in commit e8b02b3). While the implementations did indeed recognize the number of free spaces/stable disks, we couldn't tune their contributions to the overall score well enough to make them better than our original bot with the simple improved hueristic and minimax, so we're not using them for tournament.

(Note - "I" refers to Jagath)
