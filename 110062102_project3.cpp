#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <limits>

#define pos_inf 9223372036000000000.00000000;
#define neg_inf -9223372036000000000.00000000;
#define dep 5;
using namespace std;

enum SPOT_STATE
{
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
typedef struct point
{
    int x;
    int y;
} point;

typedef struct node
{
public:
    point putting_loca;
    double alpha;
    double beta;
    int is_player;
    double val;
    vector<node *> child;
} Node;

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

node *root;

double pow(int x, double y)
{
    double ret = 1.0000000;
    for (int i = 0; i < x; i++)
    {
        ret *= y;
    }
    return ret;
}

bool avail(point p)
{
    if (p.x >= 0 && p.y >= 0 && p.x <= 14 && p.y <= 14)
    {
        return 1;
    }
    return 0;
}

double count_struc(int x, int y, int is_player)
{
    double ret = 0.0000000;
    point inp;
    inp.x = x;
    inp.y = y;

    int dir_op[4][2] = {{1, 0}, {0, 1}, {1, -1}, {1, 1}};

    for (int i = 0; i < 4; i++)
    {
        point e1 = inp;
        point e2 = inp;
        int consec_num = 0;
        while (avail(e2))
        {
            if (board[e2.x][e2.y] == is_player)
            {
                consec_num++;
                e2.x += dir_op[i][0];
                e2.y += dir_op[i][1];
            }
            else
            {
                break;
            }
        }
        e2.x -= dir_op[i][0];
        e2.y -= dir_op[i][1];

        int sch_num = 5 - consec_num;

        int cou = sch_num;
        point b1 = e1;
        point b2 = e2;
        double path_fill_1 = 0.0000000;
        double path_fill_2 = 0.0000000;

        b1.x -= dir_op[i][0];
        b1.y -= dir_op[i][1];
        while (avail(b1) && cou)
        {
            if (board[b1.x][b1.y] == 0)
            {
                b1.x -= dir_op[i][0];
                b1.y -= dir_op[i][1];
            }
            else if (board[b1.x][b1.y] == is_player)
            {
                b1.x -= dir_op[i][0];
                b1.y -= dir_op[i][1];
                path_fill_1++;
            }
            else
            {
                break;
            }
            cou--;
        }
        b1.x += dir_op[i][0];
        b1.y += dir_op[i][1];

        b2.x += dir_op[i][0];
        b2.y += dir_op[i][1];
        cou = sch_num;
        while (avail(b2) && cou)
        {
            if (board[b2.x][b2.y] == 0)
            {
                b2.x += dir_op[i][0];
                b2.y += dir_op[i][1];
            }
            else if (board[b2.x][b2.y] == is_player)
            {
                b2.x += dir_op[i][0];
                b2.y += dir_op[i][1];
                path_fill_2++;
            }
            else
            {
                break;
            }
            cou--;
        }
        b2.x -= dir_op[i][0];
        b2.y -= dir_op[i][1];

        if (b2.x - b1.x + 1 < 5)
        {
            return 0.0000000;
        }

        double multiplier = 1 + (((double)(e1.x - b1.x) + (path_fill_1)) / consec_num) * (((double)(b2.x - e2.x) - (path_fill_2)) / consec_num);
        ret += pow(consec_num, 1000) * multiplier;
    }

    return ret;
}

double count_lengh(int x, int y, int is_player)
{
    double ret = 0.0000000;
    int dir_op[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

    for (int i = 0; i < 8; i++)
    {
        point p;
        p.x = x;
        p.y = y;

        while (board[p.x][p.y] != 3 - is_player)
        {
            p.x += dir_op[i][0];
            p.y += dir_op[i][1];
            ret += 1;
            if (!avail(p))
            {
                break;
            }
        }
        ret -= 1;
    }

    return ret;
}

double gen_val(int is_player)
{
    double ret = 0.00000000;
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            if (is_player == 1)
            {
                ret += count_lengh(i, j, is_player);
                ret += count_struc(i, j, is_player);
            }
            else
            {
                ret -= 5 * count_lengh(i, j, 1 + is_player);
                ret -= 5 * count_struc(i, j, 1 + is_player);
            }
        }
    }
    return ret;
}

node *gen_tree(int depth, int x, int y, double a, double b, int is_player)
{
    if (is_player == 1 && a >= b)
    {
        return nullptr;
    }
    if (is_player == 2 && a <= b)
    {
        return nullptr;
    }

    node _new;
    _new.putting_loca.x = x;
    _new.putting_loca.y = y;
    _new.alpha = a;
    _new.beta = b;
    if (depth == 0)
    {
        if (is_player == 1)
        {
            _new.alpha = gen_val(is_player);
        }
        if (is_player == 2)
        {
            _new.beta = gen_val(is_player);
        }
        node *ret = &_new;
        return ret;
    }

    if (is_player == 1)
    {
        _new.alpha = neg_inf;
        _new.beta = b;
    }
    else if (is_player == 2)
    {
        _new.alpha = a;
        _new.beta = pos_inf;
    }
    else if (is_player == 0)
    {
        _new.alpha = neg_inf;
        _new.beta = pos_inf;
    }

    vector<point> choi;
    int po[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    int px, py;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            int flag = 0;
            for (int k = 0; k < 8; k++)
            {
                if (flag)
                {
                    continue;
                }
                px = i + po[k][0];
                py = j + po[k][1];
                if (px >= 0 && px <= 14 && py >= 0 && py <= 14)
                {
                    if (board[px][py] != 0)
                    {
                        choi.push_back({i, j});
                        flag = 1;
                    }
                }
            }
        }
    }
    for (auto it : choi)
    {
        if (is_player != 2)
        {
            node *child_ptr = gen_tree(depth - 1, it.x, it.y, _new.alpha, _new.beta, 2);
            if (_new.alpha < child_ptr->beta)
            {
                _new.alpha = child_ptr->beta;
                if (_new.alpha >= _new.beta)
                {
                    break;
                }
            }
            _new.child.push_back(child_ptr);
        }
        else
        {
            node *child_ptr = gen_tree(depth - 1, it.x, it.y, _new.alpha, _new.beta, 1);
            if (_new.beta < child_ptr->alpha)
            {
                _new.beta = child_ptr->alpha;
                if (_new.beta >= _new.alpha)
                {
                    break;
                }
            }
            _new.child.push_back(child_ptr);
        }
    }

    node *ret = &_new;
    return ret;
}

void read_board(std::ifstream &fin)
{
    fin >> player;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream &fout)
{
    root = gen_tree(5, -1, -1, -9000000000, 900000000, 0);
    /*
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
    */
}

int main(int, char **argv)
{
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
