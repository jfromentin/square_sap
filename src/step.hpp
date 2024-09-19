#ifndef STEP_HPP
#define STEP_HPP

enum Step:uint8_t {Left = 0, Right = 1, Up = 2, Down = 3};

char step_to_char(Step s);
Step char_to_step(char c);

inline char step_to_char(Step s) {
  switch(s){
  case Left:
    return 'L';
  case Right:
    return 'R';
  case Up:
    return 'U';
  case Down:
    return 'D';
  default:
    return '?';
    break;
  };
}

inline Step char_to_step(char c) {
  switch(c){
  case 'L':
    return Left;
  case 'R':
    return Right;
  case 'U':
    return Up;
  case 'D':
    return Down;
  default:
    assert(false);
    break;
  };
  return Right;
}

#endif
