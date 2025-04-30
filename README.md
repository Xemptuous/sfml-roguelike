# sfml-roguelike

A simple ASCII Roguelike from scratch in C++ using SFML

## Inspiration

I wanted an opportunity to write something in C++ again, and I wanted
to see if I could write a roguelike from scratch using SDL or something
similar. I went with SFML because it was simpler and more "modern".

I also wanted to try out the Entity-Component-System (ECS) style of
game design and see how it felt. Overall, seems like an interesting
way of handling sandbox-y games where you are another equal part of
the world.

## Controls

Standard ASCII roguelike keybinds for movement

Can also use arrow keys in place of hjkl
 
- <kbd>h</kbd> move West
- <kbd>j</kbd> move South
- <kbd>k</kbd> move North
- <kbd>l</kbd> move East
- <kbd>y</kbd> move North-West
- <kbd>u</kbd> move North-East
- <kbd>b</kbd> move South-West
- <kbd>n</kbd> move South-East

- <kbd>Esc</kbd> - close active menu item
- <kbd>a</kbd> - toggles between ASCII and Spritesheet graphics (might take a few tries atm)
- <kbd>i</kbd> - show inventory
  
- <kbd>q</kbd> - close the game
