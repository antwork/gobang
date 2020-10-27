#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "defines.h"

// 输出当前数据
void printGame(int row, int col, int gameDatas[row][col]) {
    for (int i = -1; i < col; i++) {
        if (i == -1) {
            printf("  ");
        } else {
            printf("%x ", i);
        }
    }
    printf("\n");
    for (int i = 0; i < row; i++) {
        printf("%x ", i);
		for (int j = 0; j < col; j++) {
            if (gameDatas[i][j] == WHITE) {
                printf("● ");
            } else if (gameDatas[i][j] == BLACK) {
                printf("○ ");
            } else {
                printf("x ");
            }
			
		}
		printf("\n");
	}
}

// 获取位置
// :param location 当前位置
// :param direction 前进方向
// :param offset 偏移值
//
// :return 下一个位置
Location offsetLocation(Location location, Direction direction, int offset) {
    Location target;
    target.row = location.row;
    target.col = location.col;
    switch (direction)
    {
    case TOP_BOTTOM:
        target.row += offset;
        break;
    case LEFT_RIGHT:
        target.col += offset;
        break;
    case TOP_LEFT_TO_BOTTOM_RIGHT:
        target.row += offset;
        target.col += offset;
        break;
    case BOTTOM_LEFT_TO_TOP_RIGHT:
        target.row -= offset;
        target.col += offset;
    default:
        break;
    }

    return target;
}

int checkResultOfLocation(int row, int col, int gameDatas[row][col], Location start_location, Direction direction) {
    int result, i;
    Location minus_loc, curr_loc;

    // 0 0 0 0 x 0 0 0 0, 目标位置的左右各四个,总共九个
    for(i = 0, result = 0; i < 9; i++) {
        curr_loc = offsetLocation(start_location, direction, i); 
        result += gameDatas[curr_loc.row][curr_loc.col];
        
        // 为了避免重复计算,超过5个时,将头部的值移除,保证只计算5个值
        if (i >= 4) {
            if (i >= 5) {
                minus_loc = offsetLocation(start_location, LEFT_RIGHT, i - 5);
                result -= gameDatas[minus_loc.row][minus_loc.col];
            }
            
            if (result == 5) {
                return WHITE;
            } else if (result == -5) {
                return BLACK;
            }
        }
    }
    
    return 0;
}

int checkWin(GoBang *gobang) {
    if (gobang == NULL) {
        return 0;
    }
    Location location = gobang -> current_loc;
    int row = gobang -> row_count;
    int col = gobang -> col_count;

    int min_row = fmax(location.row - 4, 0);
    int min_col = fmax(location.col - 4, 0);

    int max_row = fmin(location.row + 5, row);

    int result, i;
    Location cur_location;
    // 检查left->right
    cur_location.row = location.row;
    cur_location.col = min_col;

    result = checkResultOfLocation(row, col, gobang->datas, cur_location, LEFT_RIGHT);
    if (result == WHITE || result == BLACK) {
        return result;
    }  

    // 检查top -> bottom
    cur_location.row = min_row;
    cur_location.col = location.col;
    result = checkResultOfLocation(row, col, gobang->datas, cur_location, TOP_BOTTOM);
    if (result == WHITE || result == BLACK) {
        return result;
    } 

    // 检查 topleft -> bottom_right
    cur_location.row = min_row;
    cur_location.col = min_col;
    result = checkResultOfLocation(row, col, gobang->datas, cur_location, TOP_LEFT_TO_BOTTOM_RIGHT);
    if (result == WHITE || result == BLACK) {
        return result;
    } 
    // 检查 bottomLeft -> top_right
    cur_location.row = max_row;
    cur_location.col = min_col;
    result = checkResultOfLocation(row, col, gobang->datas, cur_location, BOTTOM_LEFT_TO_TOP_RIGHT);
    if (result == WHITE || result == BLACK) {
        return result;
    } 
    return 0;
}

// 判断位置是否有效
// :param gobang 棋局
// :param 位置
// :return 有效返回true, 否则返回false
bool isAvaiable(GoBang *gobang, Location location) {
    // 越界
    if (location.row < 0 || 
        location.row >= gobang -> row_count ||
        location.col < 0 || 
        location.col >= gobang->col_count) { 
        return false;
    }
    
    bool result = gobang->datas[location.row][location.col] == 0;
    return result;
}


// 下棋
// :param gobang 棋局
// :param location 位置
// :param win 是否胜利
// :return 有效返回true, 无效返回false
bool move(GoBang *gobang, Location location, int *win) {
    if (isAvaiable(gobang, location) == false) {
        return false;
    }

    int value;
    if (gobang -> current_player == BLACK) {
        gobang -> current_player = WHITE;
        value = BLACK;
    } else {
        gobang -> current_player = BLACK;
        value = WHITE;
    }

    gobang->datas[location.row][location.col] = value;
    gobang->current_loc = location;
    *win = checkWin(gobang);

    return true;
}

void clear(GoBang *gobang) {
    free(gobang);
}

// 创建新游戏
GoBang createNewGame(int size) {
    GoBang goBang = {
        size,
        size,
        {0},
        BLACK
    };
    return goBang;
}


int main () {
	
    GoBang game = createNewGame(16);
    Location l1 = {0, 0};
    Location l2 = {0, 1};
    Location l3 = {0, 2};
    Location l4 = {0, 3};
    int result;

    printGame(16, 16, game.datas);
    printf("游戏即将开始, 双方轮流下棋, 黑方先下!\n输入格式:行 空格 列, 如(5 a)表示2行10列\n允许输入字符:0123456789abcdef\n");

    int row, col;
    while (result == 0)
    {
        if (game.current_player == BLACK) {
            printf("黑方下棋: ");
        } else {
            printf("白方下棋: ");
        }
        scanf("%x %x", &row, &col);
        if (row < 0 || row > 16 || col < 0 || col > 16) {
            printf("请输入合法字符:0123456789abcdef\n");
            continue;
        }
        
        Location location = {row, col};
        bool success = move(&game, location, &result);
        if (!success) {
            printf("当前位置不允许放棋\n");
        } else {
            printGame(16, 16, game.datas);
        }
    }
    printf("恭喜%s获得胜利!", result == WHITE ? "白方" :"黑方");
    
	return 0;
}