#define TRUE 1
#define FALSE 0

typedef enum {dirUP, dirDOWN, dirLEFT, dirRIGHT} t_dir;
typedef enum {fxEMPTY, fxWALL, fxROOM, fxCORRIDOR, fxDOOR} t_fixe;
typedef enum {wV, wH, wUR, wUL, wDR, wDL} t_wall;
typedef enum {dNONE, dOPEN, dCLOSE} t_door;
typedef enum {int line, column, lvl} t_pos;
typedef struct {
	t_fixe f;
	t_wall w;
	t_door d;
} t_cell;
