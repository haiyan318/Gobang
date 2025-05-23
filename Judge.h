#ifndef JUDGE_H
#define JUDGE_H

#include "GameDefines.h"

class Judge
{
public:

	static bool judgeWin(int row_chess, int col_chess, ChessColor color, int** chessMap);
private:

};

#endif // !JUDGE_H

