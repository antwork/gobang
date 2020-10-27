
#ifndef _GO_BANG_DEFINES
#define _GO_BANG_DEFINES

// 角色，要么执黑棋，要么执白棋
typedef enum PlayerRole {
	BLACK = -1,
	WHITE = 1
} PlayerRole;

typedef enum Direction {
    LEFT_RIGHT = 0,                 //  左->右
    TOP_BOTTOM = 1,                 //  上->下
    TOP_LEFT_TO_BOTTOM_RIGHT = 2,   // 左上->右下
    BOTTOM_LEFT_TO_TOP_RIGHT = 3    // 左下->右上
} Direction;

// 棋子摆放位置
typedef struct Location {
	int row;
	int col;
} Location;

// 五子棋数据存储
typedef struct GoBang {

    // 总行数
    int row_count;
    
    // 总列数
    int col_count;

    // 数据
    int datas[16][16];

    // 执黑一方先行
    PlayerRole current_player;

    // 当前位置
    Location current_loc;

    // 下棋步骤
    Location steps[256];
} GoBang;

#endif