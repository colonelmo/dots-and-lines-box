# dots-and-lines-box
A dots and boxes game bot

The whole code lives inside a single file now because it had 
to be compiled in a "specific" way on a windows box.

## Compile
```
g++ decision.cpp
```

## Use
After running the program, input `numer of rows`, `number of columns`,
and `whose turn it is (currently the third integer is unused).

From now on, you get to choose who should play. If you want to play, you input three numbers, corresponding to the row and column of the box, and the which of its surrounding box you want to draw (0 for bottom, 1 for left, 2 for top, and three for right). If you want the computer to play, input three numbers with the first of them being -1.

## Example run
```
./a.out
```
A four by four game:
```
>>> 4 4 0
```
Now I tell the computer to play, with the first input being -1:
```
>>> -1 0 0
```
The computer outputs:
```
<<< 2 3 2
<<< . . . . .
<<<          
<<< . . . . .
<<<          
<<< . . . ._.
<<<          
<<< . . . . .
<<<          
<<< . . . . .
```
Meaning it had played the move 2 3 2 (row 2, col 3, top side)

Now I choose to play on the opposeit side:
```
>>> 0 0 1
```
Which results in:
```
<<< . . . . .
<<< |        
<<< . . . . .
<<<          
<<< . . . ._.
<<<          
<<< . . . . .
<<<          
<<< . . . . .
```

And then I go on to ask the computer to play its best move over and over again as I just want to watch:

```
>>> -1 0 0 
<<< 3 0 1
<<< . . . . .
<<< |        
<<< . . . . .
<<<          
<<< . . . ._.
<<<          
<<< . . . . .
<<< |        
<<< . . . . .
```
Once again:
```
>>> -1 0 0
<<< 3 2 1
<<< . . . . .
<<< |        
<<< . . . . .
<<<          
<<< . . . ._.
<<<          
<<< . . . . .
<<< |   |    
<<< . . . . .
```

As you can see there is no notion of turn inside the game. If you choose to play, you can play. Similarly the computer will play if and only if you instruct it to. This is also the case even after winning boxes: The computer won't play its bonuses unless you tell it to. This is because in the original competition the control and the "state" of the turns was being passed to the program by the runner.
