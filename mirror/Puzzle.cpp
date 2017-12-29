#ifndef PUZZLE
#define PUZZLE

class Puzzle {
  private:
    bool _isSolved = false;

  public:
    Puzzle() {
    }

    bool isSolved(){
      return _isSolved;
    }

    void solve(){
      _isSolved = true;
    }
};
#endif
