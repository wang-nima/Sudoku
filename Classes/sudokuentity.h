#ifndef SUDUKUENTITY_H
#define SUDUKUENTITY_H

class SudokuEntity
{
public:
    int getHoleCount();
    void regenerate(int difficulty);

private:
    void generateHavingFun();

public:
    int startStatus[9][9];
    int endStatus[9][9];
    // 挖孔的个数
    int holeCount;
    // 难度系数
    int difficulty;
};

#endif // SUDUKUENTITY_H
