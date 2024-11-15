#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <climits>

using namespace std;

constexpr int Int_max=200;
constexpr int two_player_w=10;
constexpr int three_player_w=6;

int w;
int h;
int player_count;
int my_id;
//state
struct pathState
{
    int mx;
    int my;
    string dir;
    int step_left;
};
struct playerState
{
    int mx;
    int my;
    int mwl;
    int mstep;
    vector<pathState> path;
};
struct boardState
{
    int player;
    int hor; //-1:沒牆壁 0:牆壁開頭 1:牆壁結尾 左到右 上到下
    int ver;
};

//print movement
void PutWall(int x, int y, string ori)
{
    string t;
    t+=to_string(x);
    t+=" ";
    t+=to_string(y);
    t+=" ";
    t+=ori;
    cerr << "Put in "<< x <<" "<< y <<" "<< ori << endl;
    cout << t << endl;
}
void Move(string dir)
{
    cerr << "Move "<< dir << endl;
    cout << dir << endl;
}


class Judge_Movement
{
public:
    /*
    bool CanMoveF()
    bool CanMove()
    void JustMove()
    pair<int, string> BestMoveStep()
    bool CantMove()
    bool CanPutGoodWall()
    bool CanPutBestWall()
    */
    //by player 能不能往前
    bool CanMoveF(vector<vector<boardState>>& board, const int& mx, const int& my, const int& id)
    {
        cerr << "Try to move forward" << endl;
        if(id == 0)
        {
            if(mx+1<w && board[my][mx+1].ver>-1)
            {
                cerr << "Can't move right" << endl;
                return false;
            }
        
            cerr << "Can move right" << endl;
            return true;
        }
        else if(id == 1)
        {
            if(mx>0 && board[my][mx].ver>-1)
            {
                cerr << "Can't move left" << endl;
                return false;
            }
        
            cerr << "Can move left" << endl;
            return true;
        }
        else
        {
            if(my+1<h && board[my+1][mx].hor>-1)
            {
                cerr << "Can't move down" << endl;
                return false;
            }
        
            cerr << "Can move down" << endl;
            return true;
        }
    }  
    //by board 出界都不能走
    bool CanMove(vector<vector<boardState>>& board, const int& x, const int& y, string dir)
    {
        //out of border
        if(x<0 || y<0 || x>=w || y>=h) return false;
        if(dir == "RIGHT")
        {
            if(board[y][x].ver == -1) return true;
            else return false;
        }
        else if(dir == "LEFT")
        {
            if(x+1<w && board[y][x+1].ver==-1) return true;
            else return false;
        }
        else if(dir == "UP")
        {
            if(y+1<h && board[y+1][x].hor==-1) return true;
            else return false;
        }
        else //down
        {
            if(board[y][x].hor == -1) return true;
            else return false;
        }
    }
    //by player+boar 浪費步數原地做效果
    void JustMove(vector<vector<boardState>>& board, const int& mx, const int& my, const int& id)
    {
        cerr << "JustMove" << endl;
        if(id == 0)
        {
            if(my < h/2)//在上半
            {
                if(CanMove(board, mx, my+1, "DOWN"))
                {
                    Move("DOWN");
                }
                else if(CanMove(board, mx, my-1, "UP"))
                {
                    Move("UP");
                }
                else if(CanMove(board, mx-1, my, "LEFT"))
                {
                    Move("LEFT");
                }
                else
                {
                    Move("RIGHT");
                }
            }
            else
            {
                if(CanMove(board, mx, my-1, "UP"))
                {
                    Move("UP");
                }
                else if(CanMove(board, mx, my+1, "DOWN"))
                {
                    Move("DOWN");
                }
                else if(CanMove(board, mx-1, my, "LEFT"))
                {
                    Move("LEFT");
                }
                else
                {
                    Move("RIGHT");
                }
            }
        }
        else if(id == 1) //往左走
        {
            if(my < h/2)//在上半
            {
                if(CanMove(board, mx, my+1, "DOWN"))
                {
                    Move("DOWN");
                }
                else if(CanMove(board, mx, my-1, "UP"))
                {
                    Move("UP");
                }
                else if(CanMove(board, mx+1, my, "RIGHT"))
                {
                    Move("RIGHT");
                }
                else
                {
                    Move("LEFT");
                }
            }
            else
            {
                if(CanMove(board, mx, my-1, "UP"))
                {
                    Move("UP");
                }
                else if(CanMove(board, mx, my+1, "DOWN"))
                {
                    Move("DOWN");
                }
                else if(CanMove(board, mx+1, my, "RIGHT"))
                {
                    Move("RIGHT");
                }
                else
                {
                    Move("LEFT");
                }
            }
        }
        else //往下走
        {
            if(mx > w/2)//在右半
            {
                if(CanMove(board, mx-1, my, "LEFT"))
                {
                    Move("LEFT");
                }
                else if(CanMove(board, mx+1, my, "RIGHT"))
                {
                    Move("RIGHT");
                }
                else if(CanMove(board, mx, my-1, "UP"))
                {
                    Move("UP");
                }
                else
                {
                    Move("DOWN");
                }
            }
            else
            {
                if(CanMove(board, mx+1, my, "RIGHT"))
                {
                    Move("RIGHT");
                }
                else if(CanMove(board, mx-1, my, "LEFT"))
                {
                    Move("LEFT");
                }
                else if(CanMove(board, mx, my-1, "UP"))
                {
                    Move("UP");
                }
                else
                {
                    Move("DOWN");
                }
            }
        }
    }
    //找最短距離跟方向
    pair<int, vector<pathState>> BestMoveStep(vector<vector<boardState>>& board, const int& x, const int& y, const int& id)
    {
        cerr << "BestMoveStep " <<id << endl;
        vector<pathState> path(Int_max, {-1, -1, "NULL", Int_max});
        if(x == -1) return{Int_max, path};
        vector<vector<int>> visited(h, vector<int>(w, Int_max));
        visited[y][x]=0;
        int step;
        //for direction
        if(id == 0)
        {
            if(x == w-1)
            {
                path[0]={x, y, "RIGHT"};
                return {1, path};
            }
            step=dfsMove0(board, visited, x, y, "", path);
            cerr << "first step: "<< path[0].mx <<" "<<path[0].my <<" "<<path[0].dir<<" "<<path[0].step_left<<endl;
            cerr << "second step: "<< path[1].mx <<" "<<path[1].my <<" "<<path[1].dir<<" "<<path[1].step_left<<endl;
            return {step, path};
        }
        else if(id == 1)
        {
            if(x == 0)
            {
                path[0]={x, y, "LEFT"};
                return {1, path};
            }
            step=dfsMove1(board, visited, x, y, "", path);
            cerr << "last step: "<< path[step-1].mx <<" "<<path[step-1].my <<" "<<path[step-1].dir<<endl;
            //cerr << "last 2 step: "<< path[step-2].mx <<" "<<path[step-2].my <<" "<<path[step-2].dir<<endl;
            return {step, path};
        }
        else //往下
        {
            if(y == h-1)
            {
                path[0]={x, y, "DOWN"};
                return {1, path};
            }
            step=dfsMove2(board, visited, x, y, "", path);

            return {step, path};
        }
        //He needs to go back
        return {Int_max, {}};
    }
    bool OnlyOnePath(vector<vector<boardState>>& board, const int& x, const int& y, string& Ori, playerState* ps)
    {
        return false;
    }
    //by board 看放在這裡會不會讓player沒有BestMoveStep(無處走)
    bool CantMove(vector<vector<boardState>>& board, const int& x, const int& y, string& Ori, playerState* ps)
    {
        //如果放在x y會不會導致 player們走不了
        if(Ori == "V")
        {
            board[y][x].ver=0;
            board[y+1][x].ver=1;
        }
        else
        {
            board[y][x].hor=0;
            board[y][x+1].hor=1;
        }

        vector<int> morestep(player_count, 0);
        bool check=false;
        for(int i=0; i<player_count; ++i)
        {
            //看看會不會因為加入此牆而不能走 死人不用看
            if(ps[i].mstep == Int_max) continue;
            auto t=BestMoveStep(board, ps[i].mx, ps[i].my, i);
            if(t.first == Int_max)
            {
                cerr << "The wall makes "<<i<< " player can't move."<<endl;
                check=true;
                break;
            }
            else
            {
                morestep[i]=t.first-ps[i].mstep;
            }
        }

        //back track board改回來
        if(Ori == "V")
        {
            board[y][x].ver=-1;
            board[y+1][x].ver=-1;
        }
        else
        {
            board[y][x].hor=-1;
            board[y][x+1].hor=-1;
        }
        if(check) return true;

        for(int i=0; i<player_count; ++i)
        {
            if(i == my_id) continue;
            if(morestep[my_id] > morestep[i])
            {
                cerr << "The wall makes "<<i<< " player uses less step than me."<<endl;
                check=true;
                break;
            }
        }

        return check;
    }
    //看這個位置能否被擋住 要考慮擋它旁邊也擋到此點 有movement
    bool CanPutGoodWall(vector<vector<boardState>>& board, const int& x, const int& y, string Ori, playerState* ps)
    {
        if(Ori == "V")
        {
            cerr << x <<" " <<y<<endl;
            if(CanPutWall(board, x, y, Ori, ps))
            {
                PutWall(x, y, Ori);
                return true;
            }
            else if(CanPutWall(board, x, y-1, Ori, ps))
            {
                PutWall(x, y-1, Ori);
                return true;
            }
            return false;
        }
        else
        {
            if(CanPutWall(board, x, y, Ori, ps))
            {
                PutWall(x, y, Ori);
                return true;
            }
            else if(CanPutWall(board, x-1, y, Ori, ps))
            {
                PutWall(x-1, y, Ori);
                return true;
            }
            return false;
        }
    }
    //試著最佳擋法(擋第一步) 有movement
    bool CanPutBestWall(vector<vector<boardState>>& board, const int& x, const int& y, const int& id, playerState* ps)
    {
        if(id == 0)
        {
            int step=ps[0].mstep;
            string fdir=ps[0].path[0].dir;
            /*//先考慮能否擋最後一步
            if(CanPutGoodWall(board, ps[0].path[step-1].mx+1, ps[0].path[step-1].my, "V", ps))
            {
                cerr << "Can block player0's RIGHT last step" << endl;
                return true;
            }*/
            
            if(fdir == "RIGHT")
            {
                if(CanPutGoodWall(board, x+1, y, "V", ps))
                {
                    cerr << "Can block player0's RIGHT" << endl;
                    return true;
                }
            }
            else if(fdir == "UP")
            {
                if(CanPutGoodWall(board, x, y, "H", ps))
                {
                    cerr << "Can block player0's UP" << endl;
                    return true;
                }
            }
            else if(fdir == "DOWN")
            {
                if(CanPutGoodWall(board, x, y+1, "H", ps))
                {
                    cerr << "Can block player0's DOWN" << endl;
                    return true;
                }
            }
            else
            {
                if(CanPutGoodWall(board, x, y, "V", ps))
                {
                    cerr << "Can block player0's LEFT" << endl;
                    return true;
                }
            }

            /*//試著從他的後兩步步數開始擋
            string ldir;
            for(int i=step-2; i>-1; --i)
            {
                ldir=ps[0].path[i].dir;
                if(ldir == "RIGHT")
                {
                    if(CanPutGoodWall(board, ps[0].path[i].mx+1, ps[0].path[i].my, "V", ps))
                    {
                        cerr << "Can block player0's RIGHT last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else if(ldir == "UP")
                {
                    if(CanPutGoodWall(board, ps[0].path[i].mx, ps[0].path[i].my, "H", ps))
                    {
                        cerr << "Can block player0's UP last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else if(ldir == "DOWN")
                {
                    if(CanPutGoodWall(board, ps[0].path[i].mx, ps[0].path[i].my+1, "H", ps))
                    {
                        cerr << "Can block player0's DOWN last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else
                {
                    if(CanPutGoodWall(board, ps[0].path[i].mx, ps[0].path[i].my, "V", ps))
                    {
                        cerr << "Can block player0's LEFT last: " << i << " step"<< endl;
                        return true;
                    }
                }
            }*/

            cerr << "Can't best block player0" << endl;
            return false;
        }
        else if(id == 1)
        {   
            int step=ps[1].mstep;
            string fdir=ps[1].path[0].dir;
            /*//先考慮能否擋最後一步
            if(CanPutGoodWall(board, ps[1].path[step-1].mx, ps[1].path[step-1].my, "V", ps))
            {
                cerr << "Can block player1's LEFT last step" << endl;
                return true;
            }*/
               
            if(fdir == "LEFT")
            {
                if(CanPutGoodWall(board, x, y, "V", ps))
                {
                    cerr << "Can block player1's LEFT" << endl;
                    return true;
                }
            }
            else if(fdir == "UP")
            {
                if(CanPutGoodWall(board, x, y, "H", ps))
                {
                    cerr << "Can block player1's UP" << endl;
                    return true;
                }
            }
            else if(fdir == "DOWN")
            {
                if(CanPutGoodWall(board, x, y+1, "H", ps))
                {
                    cerr << "Can block player1's DOWN" << endl;
                    return true;
                }
            }
            else
            {
                if(CanPutGoodWall(board, x+1, y, "V", ps))
                {
                    cerr << "Can block player1's RIGHT" << endl;
                    return true;
                }
            }
            
            /*//試著從他的後面步數開始擋
            string ldir;
            for(int i=step-2; i>-1; --i)
            {
                ldir=ps[1].path[i].dir;
                if(ldir == "LEFT")
                {
                    if(CanPutGoodWall(board, ps[1].path[i].mx, ps[1].path[i].my, "V", ps))
                    {
                        cerr << "Can block player1's LEFT last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else if(ldir == "UP")
                {
                    if(CanPutGoodWall(board, ps[1].path[i].mx, ps[1].path[i].my, "H", ps))
                    {
                        cerr << "Can block player1's UP last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else if(ldir == "DOWN")
                {
                    if(CanPutGoodWall(board, ps[1].path[i].mx, ps[1].path[i].my+1, "H", ps))
                    {
                        cerr << "Can block player1's DOWN last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else
                {
                    if(CanPutGoodWall(board, ps[1].path[i].mx+1, ps[1].path[i].my, "V", ps))
                    {
                        cerr << "Can block player1's RIGHT last: " << i << " step"<< endl;
                        return true;
                    }
                }
            }*/

            cerr << "Can't best block player1" << endl;
            return false;
        }
        else
        {
            int step=ps[2].mstep;
            string fdir=ps[2].path[0].dir;
            /*//先考慮能否擋最後一步
            if(CanPutGoodWall(board, ps[2].path[step-1].mx, ps[2].path[step-1].my+1, "H", ps))
            {
                cerr << "Can block player2's DOWN last step" << endl;
                return true;
            }*/

            if(fdir == "DOWN")
            {
                if(CanPutGoodWall(board, x, y+1, "H", ps))
                {
                    cerr << "Can block player2's DOWN" << endl;
                    return true;
                }
            }
            else if(fdir == "LEFT")
            {
                if(CanPutGoodWall(board, x, y, "V", ps))
                {
                    cerr << "Can block player2's LEFT" << endl;
                    return true;
                }
            }
            else if(fdir == "RIGHT")
            {
                if(CanPutGoodWall(board, x+1, y, "V", ps))
                {
                    cerr << "Can block player2's RIGHT" << endl;
                    return true;
                }
            }
            else
            {
                if(CanPutGoodWall(board, x, y, "H", ps))
                {
                    cerr << "Can block player2's UP" << endl;
                    return true;
                }
            }
            
            /*//試著從他的後面步數開始擋
            string ldir;
            for(int i=step-2; i>-1; --i)
            {
                ldir=ps[2].path[i].dir;
                if(ldir == "DOWN")
                {
                    if(CanPutGoodWall(board, ps[2].path[i].mx, ps[2].path[i].my+1, "H", ps))
                    {
                        cerr << "Can block player2's DOWN last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else if(ldir == "RIGHT")
                {
                    if(CanPutGoodWall(board, ps[2].path[i].mx+1, ps[2].path[i].my, "V", ps))
                    {
                        cerr << "Can block player2's RIGHT last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else if(ldir == "LEFT")
                {
                    if(CanPutGoodWall(board, ps[2].path[i].mx, ps[2].path[i].my, "V", ps))
                    {
                        cerr << "Can block player2's LEFT last: " << i << " step"<< endl;
                        return true;
                    }
                }
                else
                {
                    if(CanPutGoodWall(board, ps[2].path[i].mx, ps[2].path[i].my, "H", ps))
                    {
                        cerr << "Can block player2's UP last: " << i << " step"<< endl;
                        return true;
                    }
                }
            }*/

            cerr << "Can't best block player2" << endl;
            return false;
        }   
    }

    bool CanPutTrappedWall(vector<vector<boardState>>& board, const int& x, const int& y, const int& id, playerState* ps)
    {
        cerr << "Try to put trapped wall" << endl;
        if(id == 0)
        {
            string fdir=ps[0].path[0].dir;
            if(fdir == "UP")
            {
                auto t=BestMoveStep(board, x, y-1, 0);
                string ns=t.second[0].dir;
                if(x == w-2) //檔終點前
                {
                    if(CanPutGoodWall(board, x+1, y, "V", ps))
                    {
                        cerr << "Trapping p0's right vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //他要繼續往上不用擋
                        //if(ps[0].path[1].dir == "UP")
                        if(ns == "UP")
                        {
                            cerr << "No need to trap p0 going up now he still down" << endl;
                            return false;
                        }
                        //往上右 考慮要不要擋
                        //if(ps[0].path[1].dir == "RIGHT")
                        if(ns == "RIGHT")
                        {
                            if(CanPutWall(board, x+1, y-2, "V", ps))//上右直的還能擋
                            {
                                cerr << "No need to trap p0 going up now, stop him later" << endl;
                                return false;
                            }
                            //最有效率擋上方
                            if(CanPutWall(board, x-1, y, "H", ps))
                            {
                                cerr << "Best trapping p0 while he's going through trap" << endl;
                                PutWall(x-1, y, "H");
                                return true;
                            }
                            else if(CanPutWall(board, x, y, "H", ps))
                            {
                                cerr << "Trapping p0 while he's going through trap" << endl;
                                PutWall(x, y, "H");
                                return true;
                            }
                        }
                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else if(ns == "RIGHT") //上右
                {
                    //右邊沒牆壁擋住 此人未靠牆
                    if(x+1<w && (board[y][x+1].ver==-1 && board[y][x+1].hor==-1))
                    {
                        cerr << "No need to trap p0 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutWall(board, x+1, y-2, "V", ps))
                    {
                        cerr << "Best trapping p0's right vertically" << endl;
                        PutWall(x+1, y-2, "V");
                        return true;
                    }
                    else if(CanPutWall(board, x+1, y-1, "V", ps))
                    {
                        cerr << "Trapping p0's right vertically" << endl;
                        PutWall(x+1, y-1, "V");
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋上方
                        if(CanPutWall(board, x-1, y, "H", ps))
                        {
                            cerr << "Best trapping p1 while he's going through trap" << endl;
                            PutWall(x-1, y, "H");
                            return true;
                        }
                        if(CanPutWall(board, x, y, "H", ps))
                        {
                            cerr << "Trapping p1 while he's going through trap" << endl;
                            PutWall(x, y, "H");
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "LEFT") //他後面的步數是上左 不是終點前 擋左邊
                else if(ns == "LEFT")
                {
                    //左邊沒牆壁擋住 此人未靠牆
                    if(board[y][x].ver==-1 && (x>0 && board[y][x-1].hor))
                    {
                        cerr << "No need to trap p0 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutGoodWall(board, x, y-1, "V", ps))
                    {
                        cerr << "Trapping p0's left vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋上方
                        if(CanPutGoodWall(board, x, y, "H", ps))
                        {
                            cerr << "Best trapping p0 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else
                {
                    cerr << "No need to stop him now" << endl;
                    return false;
                }
            }
            else if(fdir == "DOWN")
            {
                auto t=BestMoveStep(board, x, y+1, 0);
                string ns=t.second[0].dir;
                if(x == w-2) //檔終點前
                {
                    if(CanPutGoodWall(board, x+1, y, "V", ps))
                    {
                        cerr << "Trapping p0's right vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //他要繼續往下不用擋
                        //if(ps[0].path[1].dir == "DOWN")
                        if(ns == "DOWN")
                        {
                            cerr << "No need to trap p0 going down now he still down" << endl;
                            return false;
                        }
                        //往下右 考慮要不要擋
                        //if(ps[0].path[1].dir == "RIGHT")
                        if(ns == "RIGHT")
                        {
                            if(CanPutWall(board, x+1, y+1, "V", ps))//下右直的還能擋
                            {
                                cerr << "No need to trap p0 going up now, stop him later" << endl;
                                return false;
                            }
                            //最有效率擋下方
                            if(CanPutWall(board, x-1, y+1, "H", ps))
                            {
                                cerr << "Best trapping p0 while he's going through trap" << endl;
                                PutWall(x-1, y+1, "H");
                                return true;
                            }
                            else if(CanPutWall(board, x, y+1, "H", ps))
                            {
                                cerr << "Trapping p0 while he's going through trap" << endl;
                                PutWall(x, y+1, "H");
                                return true;
                            }
                        }
                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else if(ns == "RIGHT") //下右
                {
                    //右邊沒牆壁擋住 此人未靠牆
                    if(x+1<w && (board[y][x+1].ver==-1 && (y+1<h && board[y+1][x+1].hor==-1)))
                    {
                        cerr << "No need to trap p0 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutGoodWall(board, x+1, y+1, "V", ps))
                    {
                        cerr << "Trapping p0's right vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋下方
                        if(CanPutWall(board, x-1, y+1, "H", ps))
                        {
                            cerr << "Trapping p0 while he's going through trap" << endl;
                            PutWall(x-1, y+1, "H");
                            return true;
                        }
                        if(CanPutWall(board, x, y+1, "H", ps))
                        {
                            cerr << "Trapping p1 while he's going through trap" << endl;
                            PutWall(x, y+1, "H");
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "LEFT") //不是終點前 擋左邊
                else if(ns == "LEFT")
                {
                    //左邊沒牆壁擋住 此人未靠牆
                    if(board[y][x].ver==-1 && (x>0 && board[y][x-1].hor))
                    {
                        cerr << "No need to trap p0 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutGoodWall(board, x, y+1, "V", ps))
                    {
                        cerr << "Trapping p0's left vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋下方
                        if(CanPutGoodWall(board, x, y+1, "H", ps))
                        {
                            cerr << "Best trapping p0 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else
                {
                    cerr << "No need to stop him now" << endl;
                    return false;
                }
            }
            else if(fdir == "RIGHT")
            {
                auto t=BestMoveStep(board, x+1, y, 0);
                string ns=t.second[0].dir;
                if(ns == "UP") //想繞 要擋
                {
                    if(board[y][x].hor > -1)
                    {
                        if(CanPutGoodWall(board, x+1, y, "H", ps))
                        {
                            cerr << "Can stop p0 going right up" << endl;
                            return true;
                        }
                        if(CanPutGoodWall(board, x+1, y, "V", ps))
                        {
                            cerr << "Can stop p0 going right up right" << endl;
                            return true;
                        }
                    }
                }
                else if(ns == "DOWN")
                {
                    if(board[y+1][x].hor > -1)
                    {
                        if(CanPutGoodWall(board, x+1, y+1, "H", ps))
                        {
                            cerr << "Can stop p0 going right down" << endl;
                            return true;
                        }
                        if(CanPutGoodWall(board, x+1, y, "V", ps))
                        {
                            cerr << "Can stop p0 going right down right" << endl;
                            return true;
                        }
                    }
                }
                //同x且我下一步不是直走
                if(my_id==1 && ps[1].my==y  && ps[1].path[0].dir!="LEFT")
                {
                    for(int i=w-1; i>x+1; --i)
                    {
                        if(CanPutGoodWall(board, i, y, "V", ps))
                        {
                            cerr << "Stop p0 going right cause he want to trap me" << endl;
                            return true;
                        }
                    }
                }
                for(int i=w-1; i>x+1; --i)
                {
                    if(CanPutWall(board, i, y, "V", ps) || CanPutWall(board, i, y-1, "V", ps))
                    {
                        cerr << "No need to stop p0 going right" << endl;
                        return false;
                    }
                }

                if(y==1 || (y+1<h && board[y+1][x].hor>-1)) //看能否先卡邊界
                {
                    if(CanPutWall(board, x+1, y-1, "V", ps))
                    {
                        cerr << "Can best stop p0 going right" << endl;
                        PutWall(x+1, y-1, "V");
                        return true;
                    }
                    else if(CanPutWall(board, x+1, y, "V", ps))
                    {
                        cerr << "Can best stop p0 going right" << endl;
                        PutWall(x+1, y, "V");
                        return true;
                    }
                }
                else
                {
                    if(CanPutGoodWall(board, x+1, y, "V", ps))
                    {
                        cerr << "Can stop p0 going right" << endl;
                        return true;
                    }
                }

                cerr << "Can do nothing to stop him, just let go" << endl;
                return false;                    
            }
            else //走左 往回trap
            {
                auto t=BestMoveStep(board, x-1, y, 0);
                string ns=t.second[0].dir;
                //if(ps[0].path[1].dir == "UP") //左上
                if(ns == "UP")
                {
                    if(CanPutGoodWall(board, x-1, y, "H", ps)) //擋住上路
                    {
                        cerr << "Trapping p0's up horizontally" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        if(CanPutGoodWall(board, x, y, "V", ps)) //擋左邊看看
                        {
                            cerr << "Best trapping p0 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "DOWN") //左下
                else if(ns == "DOWN")
                {
                    if(CanPutGoodWall(board, x-1, y+1, "H", ps)) //擋住下路
                    {
                        cerr << "Trapping p0's down horizontally" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        if(CanPutWall(board, x, y-1, "V", ps)) //bestblock左邊
                        {
                            cerr << "Best trapping p0 while he's going through trap" << endl;
                            PutWall(x, y-1, "V");
                            return true;
                        }
                        if(CanPutWall(board, x, y, "V", ps))
                        {
                            cerr << "Best trapping p0 while he's going through trap" << endl;
                            PutWall(x, y, "V");
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else //左左 不用擋
                {
                    cerr << "No need to trap p0 going left now he still going left" << endl;
                    return false;
                }
            }
        }
        else if(id == 1)
        {
            string fdir=ps[1].path[0].dir;
            if(fdir == "UP")
            {
                auto t=BestMoveStep(board, x, y-1, 1);
                string ns=t.second[0].dir;
                if(x == 1) //檔終點前
                {
                    if(CanPutGoodWall(board, x, y, "V", ps))
                    {
                        cerr << "Trapping p1's left vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //他要繼續往上不用擋
                        //if(ps[0].path[1].dir == "UP")
                        if(ns == "UP")
                        {
                            cerr << "No need to trap p1 going up now he still down" << endl;
                            return false;
                        }
                        //往上左 考慮要不要擋
                        //if(ps[0].path[1].dir == "RIGHT")
                        if(ns == "LEFT")
                        {
                            if(CanPutWall(board, x, y-2, "V", ps))//上左直的還能擋
                            {
                                cerr << "No need to trap p1 going up now, stop him later" << endl;
                                return false;
                            }
                            //最有效率擋上方
                            if(CanPutGoodWall(board, x, y, "H", ps))
                            {
                                cerr << "Best trapping p0 while he's going through trap" << endl;
                                return true;
                            }
                        }
                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else if(ns == "LEFT") //上左
                {
                    //左邊沒牆壁擋住 此人未靠牆
                    if(board[y][x].ver==-1 && (x>0 && board[y][x-1].hor==-1))
                    {
                        cerr << "No need to trap p1 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutWall(board, x, y-2, "V", ps))
                    {
                        cerr << "Best trapping p1's left vertically" << endl;
                        PutWall(x, y-2, "V");
                        return true;
                    }
                    else if(CanPutWall(board, x, y-1, "V", ps))
                    {
                        cerr << "Trapping p1's left vertically" << endl;
                        PutWall(x, y-1, "V");
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋上方
                        if(CanPutGoodWall(board, x, y, "H", ps))
                        {
                            cerr << "Trapping p1 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "LEFT") //他後面的步數是上右 不是終點前 擋右邊
                else if(ns == "RIGHT")
                {
                    //右邊沒牆壁擋住 此人未靠牆
                    if(x+1<w && (board[y][x+1].ver==-1 && board[y][x+1].hor==-1))
                    {
                        cerr << "No need to trap p1 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutWall(board, x+1, y-2, "V", ps))
                    {
                        cerr << "Best trapping p1's right vertically" << endl;
                        PutWall(x+1, y-2, "V");
                        return true;
                    }
                    else if(CanPutWall(board, x+1, y-1, "V", ps))
                    {
                        cerr << "Trapping p1's right vertically" << endl;
                        PutWall(x+1, y-1, "V");
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋上方
                        if(CanPutWall(board, x-1, y, "H", ps))
                        {
                            cerr << "Best trapping p1 while he's going through trap" << endl;
                            PutWall(x-1, y, "H");
                            return true;
                        }
                        if(CanPutWall(board, x, y, "H", ps))
                        {
                            cerr << "Trapping p1 while he's going through trap" << endl;
                            PutWall(x, y, "H");
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else
                {
                    cerr << "No need to stop him now" << endl;
                    return false;
                }
            }
            else if(fdir == "DOWN")
            {
                auto t=BestMoveStep(board, x, y+1, 1);
                string ns=t.second[0].dir;
                if(x == 1) //檔終點前
                {
                    if(CanPutGoodWall(board, x, y, "V", ps))
                    {
                        cerr << "Trapping p1's left vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //他要繼續往下不用擋
                        //if(ps[0].path[1].dir == "DOWN")
                        if(ns == "DOWN")
                        {
                            cerr << "No need to trap p1 going down now he still down" << endl;
                            return false;
                        }
                        //往下左 考慮要不要擋
                        //if(ps[0].path[1].dir == "RIGHT")
                        if(ns == "LEFT")
                        {
                            if(CanPutWall(board, x, y+1, "V", ps))//下左直的還能擋
                            {
                                cerr << "No need to trap p1 going down now, stop him later" << endl;
                                return false;
                            }
                            //最有效率擋下方
                            if(CanPutGoodWall(board, x, y+1, "H", ps))
                            {
                                cerr << "Best trapping p1 while he's going through trap" << endl;
                                return true;
                            }
                        }
                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else if(ns == "LEFT") //下左
                {
                    //左邊沒牆壁擋住 此人未靠牆
                    if(board[y][x].ver==-1 && (x>0 && y+1<h && board[y+1][x-1].hor==-1))
                    {
                        cerr << "No need to trap p1 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutGoodWall(board, x, y+1, "V", ps))
                    {
                        cerr << "Trapping p1's left vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋下方
                        if(CanPutGoodWall(board, x, y+1, "H", ps))
                        {
                            cerr << "Trapping p1 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "LEFT") //不是終點前 擋右邊
                else if(ns == "RIGHT")
                {
                    //右邊沒牆壁擋住 此人未靠牆
                    if(x+1<w && (board[y][x+1].ver==-1 && board[y][x+1].hor==-1))
                    {
                        cerr << "No need to trap p1 cause he not next to wall" << endl;
                        return false;
                    }
                    else if(CanPutGoodWall(board, x+1, y+1, "V", ps))
                    {
                        cerr << "Trapping p1's right vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋下方
                        if(CanPutWall(board, x-1, y+1, "H", ps))
                        {
                            cerr << "Best trapping p1 while he's going through trap" << endl;
                            PutWall(x-1, y+1, "H");
                            return true;
                        }
                        if(CanPutWall(board, x, y+1, "H", ps))
                        {
                            cerr << "Trapping p1 while he's going through trap" << endl;
                            PutWall(x, y+1, "H");
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else
                {
                    cerr << "No need to stop him now" << endl;
                    return false;
                }
            }
            else if(fdir == "LEFT")
            {
                auto t=BestMoveStep(board, x-1, y, 1);
                string ns=t.second[0].dir;
                if(ns == "UP") //想繞 要擋
                {
                    if(board[y][x].hor > -1)
                    {
                        if(CanPutGoodWall(board, x-1, y, "H", ps))
                        {
                            cerr << "Can stop p1 going left up" << endl;
                            return true;
                        }
                        if(CanPutGoodWall(board, x, y, "V", ps))
                        {
                            cerr << "Can stop p1 going left up left" << endl;
                            return true;
                        }
                    }
                }
                else if(ns == "DOWN")
                {
                    if(board[y+1][x].hor > -1)
                    {
                        if(CanPutGoodWall(board, x-1, y+1, "H", ps))
                        {
                            cerr << "Can stop p1 going left down" << endl;
                            return true;
                        }
                        if(CanPutGoodWall(board, x, y, "V", ps))
                        {
                            cerr << "Can stop p1 going left down left" << endl;
                            return true;
                        }
                    }
                }
                //同x且我下一步不是直走
                if(my_id==0 && ps[0].my==y  && ps[0].path[0].dir!="RIGHT")
                {
                    for(int i=1; i<x; ++i)
                    {
                        if(CanPutGoodWall(board, i, y, "V", ps))
                        {
                            cerr << "Stop p1 going left cause he want to trap me" << endl;
                            return true;
                        }
                    }
                }
                for(int i=1; i<x; ++i)
                {
                    if(CanPutWall(board, i, y, "V", ps) || CanPutWall(board, i, y-1, "V", ps))
                    {
                        cerr << "No need to stop p1 going left" << endl;
                        return false;
                    }
                }
                if(y==1 || (y+1<h && board[y+1][x].hor>-1)) //看能否先卡邊界
                {
                    if(CanPutWall(board, x, y-1, "V", ps))
                    {
                        cerr << "Can best stop p1 going left" << endl;
                        PutWall(x, y-1, "V");
                        return true;
                    }
                    else if(CanPutWall(board, x, y, "V", ps))
                    {
                        cerr << "Can best stop p1 going left" << endl;
                        PutWall(x, y, "V");
                        return true;
                    }
                }
                else
                {
                    if(CanPutGoodWall(board, x, y, "V", ps))
                    {
                        cerr << "Can stop p1 going left" << endl;
                        return true;
                    }
                }

                cerr << "Can do nothing to stop him, just let go" << endl;
                return false;                    
            }
            else //走右 往回trap
            {
                auto t=BestMoveStep(board, x+1, y, 1);
                string ns=t.second[0].dir;
                //if(ps[0].path[1].dir == "UP") //右上
                if(ns == "UP")
                {
                    if(CanPutGoodWall(board, x+1, y, "H", ps)) //擋住上路
                    {
                        cerr << "Trapping p1's up horizontally" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        if(CanPutGoodWall(board, x+1, y, "V", ps)) //擋右邊看看
                        {
                            cerr << "Best trapping p1 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "DOWN") //右下
                else if(ns == "DOWN")
                {
                    if(CanPutGoodWall(board, x+1, y+1, "H", ps)) //擋住下路
                    {
                        cerr << "Trapping p1's down horizontally" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        if(CanPutWall(board, x+1, y-1, "V", ps)) //bestblock右邊
                        {
                            cerr << "Best trapping p1 while he's going through trap" << endl;
                            PutWall(x+1, y-1, "V");
                            return true;
                        }
                        if(CanPutWall(board, x+1, y, "V", ps))
                        {
                            cerr << "Best trapping p1 while he's going through trap" << endl;
                            PutWall(x+1, y, "V");
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else //左左 不用擋
                {
                    cerr << "No need to trap p1 going left now he still going left" << endl;
                    return false;
                }
            }
            
        }
        else //p2
        {
            string fdir=ps[2].path[0].dir;
            if(fdir == "LEFT")
            {
                auto t=BestMoveStep(board, x-1, y, 2);
                string ns=t.second[0].dir;
                if(y == h-2) //檔終點前
                {
                    if(CanPutGoodWall(board, x, y+1, "H", ps))
                    {
                        cerr << "Trapping p2's down horizontally" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //他要繼續往左不用擋
                        //if(ps[0].path[1].dir == "UP")
                        if(ns == "LEFT")
                        {
                            cerr << "No need to trap p2 going left now he still left" << endl;
                            return false;
                        }
                        //往左下 考慮要不要擋
                        //if(ps[0].path[1].dir == "RIGHT")
                        if(ns == "DOWN")
                        {
                            if(CanPutWall(board, x-2, y+1, "H", ps))//左下直的還能擋
                            {
                                cerr << "No need to trap p2 going left now, stop him later" << endl;
                                return false;
                            }
                            //最有效率擋左邊
                            if(CanPutWall(board, x, y-1, "V", ps))
                            {
                                cerr << "Best trapping p2 while he's going through trap" << endl;
                                PutWall(x, y-1, "V");
                                return true;
                            }
                            else if(CanPutWall(board, x, y, "V", ps))
                            {
                                cerr << "Trapping p2 while he's going through trap" << endl;
                                PutWall(x, y, "V");
                                return true;
                            }
                        }
                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                } 
                //else if(ps[0].path[1].dir == "LEFT") //他後面的步數是左上 不是終點前 擋上面
                else if(ns == "UP")
                {
                    if(CanPutGoodWall(board, x-1, y, "H", ps))
                    {
                        cerr << "Trapping p2's up horizontally" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋左邊
                        if(CanPutGoodWall(board, x, y, "V", ps))
                        {
                            cerr << "Best trapping p2 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else
                {
                    cerr << "No need to stop him now" << endl;
                    return false;
                }
            }
            else if(fdir == "RIGHT")
            {
                auto t=BestMoveStep(board, x+1, y, 2);
                string ns=t.second[0].dir;
                if(y == h-2) //檔終點前
                {
                    if(CanPutGoodWall(board, x, y+1, "H", ps))
                    {
                        cerr << "Trapping p2's down horizontally" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //他要繼續往右不用擋
                        //if(ps[0].path[1].dir == "DOWN")
                        if(ns == "RIGHT")
                        {
                            cerr << "No need to trap p2 going right now he still right" << endl;
                            return false;
                        }
                        //往右下 考慮要不要擋
                        //if(ps[0].path[1].dir == "RIGHT")
                        if(ns == "DOWN")
                        {
                            if(CanPutWall(board, x+1, y+1, "H", ps))//右下直的還能擋
                            {
                                cerr << "No need to trap p2 going right now, stop him later" << endl;
                                return false;
                            }
                            //最有效率擋右邊
                            if(CanPutWall(board, x+1, y-1, "V", ps))
                            {
                                cerr << "Best trapping p2 while he's going through trap" << endl;
                                PutWall(x+1, y-1, "V");
                                return true;
                            }
                            else if(CanPutWall(board, x+1, y, "V", ps))
                            {
                                cerr << "Trapping p2 while he's going through trap" << endl;
                                PutWall(x+1, y, "V");
                                return true;
                            }
                        }
                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "LEFT") //不是終點前 擋上面
                else if(ns == "UP")
                {
                    if(CanPutGoodWall(board, x+1, y, "H", ps))
                    {
                        cerr << "Trapping p2's up vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        //擋右邊
                        if(CanPutGoodWall(board, x+1, y, "V", ps))
                        {
                            cerr << "Best trapping p2 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else
                {
                    cerr << "No need to stop him now" << endl;
                    return false;
                }
            }
            else if(fdir == "DOWN")
            {
                for(int i=h-1; i>y+1; --i)
                {
                    if(CanPutWall(board, x, i, "H", ps) || CanPutWall(board, x-1, i, "H", ps))
                    {
                        cerr << "No need to stop p2 going down" << endl;
                        return false;
                    }
                }
                if(x==1 || (x+1<w && board[y][x+1].ver>-1)) //看能否先卡邊界
                {
                    if(CanPutWall(board, x-1, y+1, "H", ps))
                    {
                        cerr << "Can best stop p2 going down" << endl;
                        PutWall(x-1, y+1, "H");
                        return true;
                    }
                    else if(CanPutWall(board, x, y+1, "H", ps))
                    {
                        cerr << "Can best stop p2 going down" << endl;
                        PutWall(x, y+1, "H");
                        return true;
                    }
                }
                else
                {
                    if(CanPutGoodWall(board, x, y+1, "H", ps))
                    {
                        cerr << "Can stop p2 going down" << endl;
                        return true;
                    }
                }

                cerr << "Can do nothing to stop him, just let go" << endl;
                return false;                    
            }
            else //走上 往回trap
            {
                auto t=BestMoveStep(board, x, y-1, 2);
                string ns=t.second[0].dir;
                //if(ps[0].path[1].dir == "UP") //上左
                if(ns == "LEFT")
                {
                    if(CanPutGoodWall(board, x, y-1, "V", ps)) //擋住左邊
                    {
                        cerr << "Trapping p2's left vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        if(CanPutGoodWall(board, x, y, "H", ps)) //擋上面看看
                        {
                            cerr << "Best trapping p2 while he's going through trap" << endl;
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                //else if(ps[0].path[1].dir == "DOWN") //上右
                else if(ns == "RIGHT")
                {
                    if(CanPutGoodWall(board, x+1, y-1, "V", ps)) //擋住右邊
                    {
                        cerr << "Trapping p2's right vertically" << endl;
                        return true;
                    }
                    else//擋不了 代表要可能要越過陷阱了
                    {
                        if(CanPutWall(board, x-1, y, "H", ps)) //bestblock上面
                        {
                            cerr << "Best trapping p2 while he's going through trap" << endl;
                            PutWall(x-1, y, "H");
                            return true;
                        }
                        if(CanPutWall(board, x, y, "H", ps))
                        {
                            cerr << "Best trapping p2 while he's going through trap" << endl;
                            PutWall(x, y, "H");
                            return true;
                        }

                        cerr << "Can do nothing to stop him, just let go" << endl;
                        return false;
                    }
                }
                else //左左 不用擋
                {
                    cerr << "No need to trap p2 going left now he still going left" << endl;
                    return false;
                }
            }
        }
    }
    bool CanPutLastWall(vector<vector<boardState>>& board, const int& x, const int& y, const int& id, playerState* ps)
    {
        int my_x=ps[my_id].mx;
        if(id == 0)
        {
            int my_x=ps[my_id].mx;
            if(y > h/2)//在下半部
            {
                for(int i=w-1; i>my_x; i--)
                {
                    for(int j=h-2; j>-1; j--)
                    {
                        if(CanPutWall(board, i, j, "V", ps))
                        {
                            cerr << "Can block player0's RIGHT virtical step" << endl;
                            PutWall(w-i, j, "V");
                            return true;
                        }
                    }
                }
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
            else//在上半部
            {
                for(int i=w-1; i>my_x; i--)
                {
                    for(int j=0; j<h; j++)
                    {
                        if(CanPutWall(board, i, j, "V", ps))
                        {
                            cerr << "Can block player0's RIGHT virtical step" << endl;
                            PutWall(w-i, j, "V");
                            return true;
                        }
                    }
                }
            }
            /*
            int step=ps[0].mstep;
            //先考慮能否擋最後一步
            if(CanPutGoodWall(board, ps[0].path[step-1].mx+1, ps[0].path[step-1].my, "V", ps))
            {
                cerr << "Can block player0's RIGHT last step" << endl;
                return true;
            }*/
        }
        else if(id == 1)
        {
            for(int i=1; i<=my_x; i++)
            {
                if(i%2 == 1)
                {
                    for(int j=h-2; j>-1; j--)
                    {
                        if(CanPutWall(board, i, j, "V", ps))
                        {
                            cerr << "Can block player1's LEFT virtical step" << endl;
                            PutWall(i, j, "V");
                            return true;
                        }
                    }
                }
                else
                {
                    for(int j=0; j<h; j++)
                    {
                        if(CanPutWall(board, i, j, "V", ps))
                        {
                            cerr << "Can block player1's LEFT virtical step" << endl;
                            PutWall(i, j, "V");
                            return true;
                        }
                    }

                }
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     

            /*
            int step=ps[1].mstep;
            //先考慮能否擋最後一步
            if(CanPutGoodWall(board, ps[1].path[step-1].mx, ps[1].path[step-1].my, "V", ps))
            {
                cerr << "Can block player1's LEFT last step" << endl;
                return true;
            }*/
        }
        else
        {
            int step=ps[2].mstep;
            //先考慮能否擋最後一步
            if(CanPutGoodWall(board, ps[2].path[step-1].mx, ps[2].path[step-1].my+1, "H", ps))
            {
                cerr << "Can block player2's DOWN last step" << endl;
                return true;
            }
        }
        return false;
    }
private:
    /*
    int dfsMove0()
    int dfsMove1()
    int dfsMove2()
    void initvisited()
    bool CanPutWall()
    */
    //往右優先
    int dfsMove0(vector<vector<boardState>>& board, vector<vector<int>>& visited, const int x, const int y, string ld, vector<pathState>& path)
    {
        if(x==w-1 && board[y][x].ver==-1)
        {
            visited[y][x]=visited[y][x-1]+1;
            path[visited[y][x]].step_left=1;
            return visited[y][x];
        }
        
        //visited[y][x]=INT_MAX 代表沒走過
        if(ld == "RIGHT")
        {
            visited[y][x]=visited[y][x-1]+1;
        }
        else if(ld == "UP")
        {
            visited[y][x]=visited[y+1][x]+1;
        }
        else if(ld == "DOWN")
        {
            visited[y][x]=visited[y-1][x]+1;
        }
        else if(ld == "LEFT")
        {
            visited[y][x]=visited[y][x+1]+1;
        }
    
        int visit=visited[y][x];
        int right=Int_max, up=Int_max, down=Int_max, left=Int_max;
        if(ld!="DOWN" && y-1>-1 && visit+1<visited[y-1][x] && board[y][x].hor==-1) up=dfsMove0(board, visited, x, y-1, "UP", path);
        if(ld!="UP" && y+1<h && visit+1<visited[y+1][x] && board[y+1][x].hor==-1) down=dfsMove0(board, visited, x, y+1, "DOWN", path);
        if(ld!="LEFT" && visit+1<visited[y][x+1] && board[y][x+1].ver==-1) right=dfsMove0(board, visited, x+1, y, "RIGHT", path);
        if(ld!="RIGHT" && x-1>-1 && visit+1<visited[y][x-1] && board[y][x].ver==-1) left=dfsMove0(board, visited, x-1, y, "LEFT", path);
        
        int mindis=Int_max;
        if(up < mindis)
        {
            mindis=up;
            path[visit]={x, y, "UP", visit+1};
            //剩下: 原本的 比 現在的大 代表走現在這條比較快
            //if(path[visit].step_left>path[visited[y-1][x]].step_left+1)
            //    path[visit]={x, y, "UP", path[visited[y-1][x]].step_left+1};
        }
        if(down < mindis)
        {
            mindis=down;
            path[visit]={x, y, "DOWN", visit+1};
            //if(path[visit].step_left>path[visited[y+1][x]].step_left+1)
            //    path[visit]={x, y, "DOWN", path[visited[y+1][x]].step_left+1};
        }
        if(right < mindis)
        {
            mindis=right;
            path[visit]={x, y, "RIGHT", visit+1};
            //if(path[visit].step_left > path[visited[y][x+1]].step_left+1)
            //    path[visit]={x, y, "RIGHT", path[visited[y][x+1]].step_left+1};
        }
        if(left < mindis)
        {
            mindis=left;
            path[visit]={x, y, "LEFT", visit+1};
            //if(path[visit].step_left>path[visited[y][x-1]].step_left+1)
            //    path[visit]={x, y, "LEFT", path[visited[y][x-1]].step_left+1};
        }
        
        return mindis;
    }
    //往左優先
    int dfsMove1(vector<vector<boardState>>& board, vector<vector<int>>& visited, const int x, const int y, string ld, vector<pathState>& path)
    {
        if(x==0 && board[y][x+1].ver==-1)
        {
            visited[y][x]=visited[y][x+1]+1;
            path[visited[y][x]].step_left=0;
            return visited[y][x];
        }

        if(ld == "LEFT")
        {
            //if(board[y][x+1].ver || visited[y][x+1]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y][x+1]+1;
        }
        else if(ld == "UP")
        {
            //if(board[y+1][x].hor || visited[y+1][x]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y+1][x]+1;
        }
        else if(ld == "DOWN")
        {
            //if(board[y][x].hor || visited[y-1][x]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y-1][x]+1;
        }
        else if(ld == "RIGHT")
        {
            //if(board[y][x].ver || visited[y][x-1]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y][x-1]+1;
        }

        int visit=visited[y][x];
        int left=Int_max, up=Int_max, down=Int_max, right=Int_max;
        if(ld != "DOWN" && y-1>-1 && visit+1<visited[y-1][x] && board[y][x].hor==-1) up=dfsMove1(board, visited, x, y-1, "UP", path);
        if(ld != "UP" && y+1<h && visit+1<visited[y+1][x] && board[y+1][x].hor==-1) down=dfsMove1(board, visited, x, y+1, "DOWN", path);
        if(ld != "RIGHT" && visit+1<visited[y][x-1] && board[y][x].ver==-1) left=dfsMove1(board, visited, x-1, y, "LEFT", path);
        if(ld != "LEFT" && x+1<w && visit+1<visited[y][x+1] && board[y][x+1].ver==-1) right=dfsMove1(board, visited, x+1, y, "RIGHT", path);

        int mindis=Int_max;
        if(up < mindis)
        {
            mindis=up;
            path[visit]={x, y, "UP", visit+1};
            //剩下: 原本的 比 現在的大 代表走現在這條比較快
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "UP", path[visit+1].step_left+1};
        }
        if(down < mindis)
        {
            mindis=down;
            path[visit]={x, y, "DOWN", visit+1};
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "DOWN", path[visit+1].step_left+1};
        }
        if(left < mindis)
        {
            mindis=left;
            path[visit]={x, y, "LEFT", visit+1};
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "LEFT", path[visit+1].step_left+1};
        }
        if(right < mindis)
        {
            mindis=right;
            path[visit]={x, y, "RIGHT", visit+1};
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "RIGHT", path[visit+1].step_left+1};
        }
        
        return mindis;
    }
    //往下優先
    int dfsMove2(vector<vector<boardState>>& board, vector<vector<int>>& visited, const int x, const int y,  string ld, vector<pathState>& path)
    {
        if(y==h-1 && board[y][x].hor==-1)
        {
            visited[y][x]=visited[y-1][x]+1;
            path[visited[y][x]].step_left=0;
            return visited[y][x];
        }

        if(ld == "DOWN")
        {
            //if(board[y][x].hor || visited[y-1][x]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y-1][x]+1;
        }
        else if(ld == "LEFT")
        {
            //if(board[y][x+1].ver || visited[y][x+1]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y][x+1]+1;
        }
        else if(ld == "RIGHT")
        {
            //if(board[y][x].ver || visited[y][x-1]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y][x-1]+1;
        }
        else if(ld == "UP")
        {
            //if(board[y+1][x].hor || visited[y+1][x]>visited[y][x]) return Int_max;
            visited[y][x]=visited[y+1][x]+1;
        }

        int visit=visited[y][x];
        int down=Int_max, left=Int_max, right=Int_max, up=Int_max;
        if(ld != "RIGHT" && x-1>-1 && visit+1<visited[y][x-1] && board[y][x].ver==-1) left=dfsMove2(board, visited, x-1, y, "LEFT", path);
        if(ld != "LEFT" && x+1<w && visit+1<visited[y][x+1] && board[y][x+1].ver==-1) right=dfsMove2(board, visited, x+1, y, "RIGHT", path);
        if(ld != "UP" && visit+1<visited[y+1][x] && board[y+1][x].hor==-1) down=dfsMove2(board, visited, x, y+1, "DOWN", path);
        if(ld != "DOWN" && y-1>-1 && visit+1<visited[y-1][x] && board[y][x].hor==-1) up=dfsMove2(board, visited, x, y-1, "UP", path);
        
        int mindis=Int_max;
        if(left < mindis)
        {
            mindis=left;
            path[visit]={x, y, "LEFT", visit+1};
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "LEFT", path[visit+1].step_left+1};
        }
        if(right < mindis)
        {
            mindis=right;
            path[visit]={x, y, "RIGHT", visit+1};
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "RIGHT", path[visit+1].step_left+1};
        }
        if(down < mindis)
        {
            mindis=down;
            path[visit]={x, y, "DOWN", visit+1};
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "DOWN", path[visit+1].step_left+1};
        }
        if(up < mindis)
        {
            mindis=up;
            path[visit]={x, y, "UP", visit+1};
            //if(path[visit].step_left > path[visit+1].step_left+1)
            //    path[visit]={x, y, "UP", path[visit+1].step_left+1};
        }

        return mindis;
    }
    //給BestMove系列用的
    void initvisited(vector<vector<int>>& visited)
    {
        for(int i=0; i<h; ++i)
        {
            for(int j=0; j<w; ++j)
            {
                visited[i][j]=Int_max;
            }
        }
    }
    
    //by board 看此處能否放牆壁
    bool CanPutWall(vector<vector<boardState>>& board, const int& x, const int& y, string Ori, playerState* ps)
    {
        if(Ori == "V")
        {
            //此處左邊不能放了 或被垂直擋住 或往下會超出border
            if(x>=w || x<=0 || y<0 || board[y][x].ver>-1 || y+1>=h || board[y+1][x].ver>-1) 
            {
                cerr << "Can't put wall left cause blocked or outborder" << endl;
                return false;
            }
            //被水平的牆壁卡住了
            if(y+1<h && board[y+1][x].hor==1 && board[y+1][x-1].hor==0) 
            {
                cerr << "Can't put wall cause blocked by hor wall" << endl;
                return false;
            }
            //這個人要沒路可走了 或是 放這裡可能害人無處走
            if(CantMove(board, x, y, Ori, ps))
                return false;
            return true;
        }
        else
        {
            //此處上面不能放了 或被平行擋住 或往下會超出border
            if(y>=h || y<=0 || x<0 || board[y][x].hor>-1 || x+1>=w || board[y][x+1].hor>-1)
            {
                cerr << "Can't put wall top cause blocked or outborder" << endl;
                return false;
            }
            //被垂直的牆壁卡住了
            if(x+1<w && board[y][x+1].ver==1 && board[y-1][x+1].ver==0) 
            {
                cerr << "Can't put wall top cause blocked by ver wall" << endl;
                return false;
            }
            //這個人要沒路可走了
            if(CantMove(board, x, y, Ori, ps))
                return false;

            return true;
        }
    }
};


class Game_Plan
{
public:
    //Game plan
    //my_id=0 往右走, 只有一個對手 我是先出發的人 平手or領先情況優先考慮前進
    void Plan0(vector<vector<boardState>>& board, playerState* ps)
    {
        int mx=ps[0].mx;
        int my=ps[0].my;
        int mdis=ps[0].mstep;
        string mdir=ps[0].path[0].dir;

        //Player1
        int onedis=ps[1].mstep;
        string onedir=ps[1].path[0].dir;

        if(mx==0 && jm.CanMoveF(board, mx, my, 0)) //看看能否不要卡在左最邊
        {
            auto t=jm.BestMoveStep(board, mx+1, my, 0);
            if(t.first == mdis-1) //往右步數一樣 提前走
            {
                cerr << "Move right first" << endl;
                Move("RIGHT");
                return;
            }
        }
        //避免被逼邊關起來 他開場可能還沒用牆壁時先往中靠
        if(my<=2 && ps[1].mwl>two_player_w-2 && board[my+1][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my+1, 0);
            if(t.first <= mdis) //往下步數只差1 提前避開陷阱
            {
                cerr << "Move down first" << endl;
                Move("DOWN");
                return;
            }
        }
        if(my>=h-3 && ps[1].mwl>two_player_w-2 && board[my][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my-1, 0);
            if(t.first <= mdis) //往上步數只差1 提前避開陷阱
            {
                cerr << "Move up first" << endl;
                Move("UP");
                return;
            }
        }
        //領先可以先衝
        if(mdis<=onedis && ps[1].mwl==0)
        {
            cerr << "He has no wall. RUN!!!" << endl;
            Move(mdir);
        }
        else if(ps[0].mwl > 0)//有牆壁可以妨礙
        {
            cerr << "Try to put wall" << endl;
            cerr << "Block p1" << endl;
            int onex=ps[1].mx;
            int oney=ps[1].my;
            //對方快到 最佳擋法開搞
            if(jm.CanPutTrappedWall(board, onex, oney, 1, ps))
            {
                cerr << "Trapped Block!!!!" << endl;
            }
            else
            {
                cerr<< "Best Move"<<endl;
                Move(mdir);
            }
        }
        else //無法擋人 自己動
        {
            cerr << "No walls left, best Move" << endl;
            Move(mdir);
        }
    }
    //my_id=0 3人 往右走, 我是先出發的人 平手or領先情況優先考慮前進
    void Plan1(vector<vector<boardState>>& board, playerState* ps)
    {
        int mx=ps[0].mx;
        int my=ps[0].my;
        int mdis=ps[0].mstep;
        string mdir=ps[0].path[0].dir;

        //Player1
        int onedis=ps[1].mstep;
        string onedir=ps[1].path[0].dir;
        //Player2
        int twodis=ps[2].mstep;
        string twodir=ps[2].path[0].dir;

        if(mx==0 && jm.CanMoveF(board, mx, my, 0)) //看看能否不要卡在左最邊
        {
            auto t=jm.BestMoveStep(board, mx+1, my, 0);
            if(t.first == mdis-1) //往右步數一樣 提前走
            {
                cerr << "Move right first" << endl;
                Move("RIGHT");
                return;
            }
        }
        //避免被逼邊關起來 他開場可能還沒用牆壁時先往中靠
        if(my<=2 && ps[1].mwl>three_player_w-2 && ps[0].mwl>three_player_w-2 && board[my+1][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my+1, 0);
            if(t.first <= mdis) //往下步數只差1 提前避開陷阱
            {
                cerr << "Move down first" << endl;
                Move("DOWN");
                return;
            }
        }
        if(my>=h-3 && ps[1].mwl>three_player_w-2 && ps[0].mwl>three_player_w-2 && board[my][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my-1, 0);
            if(t.first <= mdis) //往上步數只差1 提前避開陷阱
            {
                cerr << "Move up first" << endl;
                Move("UP");
                return;
            }
        }
        //領先可以先衝 有人死 dis=Int_max, mwl=-1
        if(mdis<=onedis && ps[1].mwl<=0 && mdis<=twodis && ps[2].mwl<=0)
        {
            cerr << "They have no wall. RUN!!!" << endl;
            Move(mdir);
        }
        else if(ps[0].mwl > 0)//有牆壁可以妨礙
        {
            cerr << "Try to put wall" << endl;
            if(onedis == Int_max) //p1死
            {
                if(jm.CanPutTrappedWall(board, ps[2].mx, ps[2].my, 2, ps))
                {
                    cerr << "Trapped Block p2!!!" << endl;
                }
                else
                {
                    cerr << "Best Move" << endl;
                    Move(mdir);
                }
            }
            else if(twodis == Int_max) //p2死
            {
                if(jm.CanPutTrappedWall(board, ps[1].mx, ps[1].my, 1, ps))
                {
                    cerr << "Trapped Block p1!!!" << endl;
                }
                else
                {
                    cerr << "Best Move" << endl;
                    Move(mdir);
                }
            }
            else if(twodis < onedis) //p1跑比較慢 先搞他
            {
                cerr << "Block p1" << endl;
                int onex=ps[1].mx;
                int oney=ps[1].my;
                if(jm.CanPutTrappedWall(board, onex, oney, 1, ps))
                {
                    cerr << "Trapped Block p1!!!" << endl;
                }
                else
                {
                    if(jm.CanPutTrappedWall(board, ps[2].mx, ps[2].my, 2, ps))
                    {
                        cerr << "Can't trap p1, trapped block p2" << endl;
                    }
                    else
                    {
                        cerr << "Best Move" << endl;
                        Move(mdir);
                    }
                }
            }
            else
            {
                cerr << "Block p2" << endl;
                int twox=ps[2].mx;
                int twoy=ps[2].my;
                if(jm.CanPutTrappedWall(board, twox, twoy, 2, ps))
                {
                    cerr << "Trapped Block p2!!!" << endl;
                }
                else //擋不了 擋另一人
                {
                    if(jm.CanPutTrappedWall(board, ps[1].mx, ps[1].my, 1, ps))
                    {
                        cerr << "Can't trap p2, trapped block p1" << endl;
                    }
                    else
                    {
                        cerr << "Best Move" << endl;
                        Move(mdir);
                    }
                }
            }
        }
        else //無法擋人 自己動
        {
            cerr << "No walls left, best Move" << endl;
            Move(mdir);
        }
    }
    //my_id=1 2人 往左走, 我是後出發的人 只有一個對手 領先情況優先考慮前進
    void Plan2(vector<vector<boardState>>& board, playerState* ps)
    {
        int mx=ps[1].mx;
        int my=ps[1].my;
        int mdis=ps[1].mstep;
        string mdir=ps[1].path[0].dir;
        //Player0
        int zerodis=ps[0].mstep;
        string zerodir=ps[0].path[0].dir;

        if(mx==w-1 && jm.CanMoveF(board, mx, my, 1)) //看看能否不要卡在右最邊
        {
            auto t=jm.BestMoveStep(board, mx-1, my, 1);
            if(t.first == mdis-1) //往左步數一樣 提前走
            {
                cerr << "Move left first" << endl;
                Move("LEFT");
                return;
            }
        }
        //避免被逼邊關起來 他開場可能還沒用牆壁時先往中靠
        if(my<=2 && ps[0].mwl>two_player_w-2 && board[my+1][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my+1, 1);
            if(t.first <= mdis) //往下步數只差1 提前避開陷阱
            {
                cerr << "Move down first" << endl;
                Move("DOWN");
                return;
            }
        }
        if(my>=h-3 && ps[0].mwl>two_player_w-2 && board[my][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my-1, 1);
            if(t.first <= mdis) //往上步數只差1 提前避開陷阱
            {
                cerr << "Move up first" << endl;
                Move("UP");
                return;
            }
        }
        //領先可以先衝
        if(mdis<zerodis && ps[0].mwl==0)
        {
            cerr << "He has no wall. RUN!!!" << endl;
            Move(mdir);
        }
        else if(ps[1].mwl > 0)//有牆壁可以妨礙
        {
            cerr << "Try to put wall" << endl;
            cerr << "Block p0" << endl;
            int zerox=ps[0].mx;
            int zeroy=ps[0].my;
            //對方快到 最佳擋法開搞
            if(jm.CanPutTrappedWall(board, zerox, zeroy, 0, ps))
            {
                cerr << "Trapped Block!!!!" << endl;
            }
            else
            {
                cerr<< "Best Move"<<endl;
                Move(mdir);
            }
        }
        else //無法擋人 自己動
        {
            cerr << "No walls left, best Move" << endl;
            Move(mdir);
        }
    }
    //my_id=1 往左走, 我是第二個出發的人 有兩個對手 領先情況優先考慮前進 試著不要最後一名
    void Plan3(vector<vector<boardState>>& board, playerState* ps)
    {
        int mx=ps[1].mx;
        int my=ps[1].my;
        int mdis=ps[1].mstep;
        string mdir=ps[1].path[0].dir;

        //Player0
        int zerodis=ps[0].mstep;
        string zerodir=ps[0].path[0].dir;
        //Player2
        int twodis=ps[2].mstep;
        string twodir=ps[2].path[0].dir;

        if(mx==w-1 && jm.CanMoveF(board, mx, my, 1)) //看看能否不要卡在右最邊
        {
            auto t=jm.BestMoveStep(board, mx-1, my, 1);
            if(t.first == mdis-1) //往左步數一樣 提前走
            {
                cerr << "Move left first" << endl;
                Move("LEFT");
                return;
            }
        }
        //避免被逼邊關起來 他開場可能還沒用牆壁時先往中靠
        if(my<=2 && ps[0].mwl>three_player_w-2 && ps[2].mwl>three_player_w-2 && board[my+1][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my+1, 1);
            if(t.first <= mdis) //往下步數只差1 提前避開陷阱
            {
                cerr << "Move down first" << endl;
                Move("DOWN");
                return;
            }
        }
        if(my>=h-3 && ps[0].mwl>three_player_w-2 && ps[2].mwl>three_player_w-2 && board[my][mx].hor==-1)
        {
            auto t=jm.BestMoveStep(board, mx, my-1, 1);
            if(t.first <= mdis) //往上步數只差1 提前避開陷阱
            {
                cerr << "Move up first" << endl;
                Move("UP");
                return;
            }
        }
        //領先可以先衝 有人死 dis=Int_max, mwl=-1
        if(mdis<zerodis && ps[0].mwl<=0 && mdis<=twodis && ps[2].mwl<=0)
        {
            cerr << "They have no wall. RUN!!!" << endl;
            Move(mdir);
        }
        else if(ps[1].mwl > 0)//有牆壁可以妨礙
        {
            cerr << "Try to put wall" << endl;
            if(zerodis == Int_max) //p0死
            {
                if(jm.CanPutTrappedWall(board, ps[2].mx, ps[2].my, 2, ps))
                {
                    cerr << "Trapped Block p2!!!" << endl;
                }
                else
                {
                    cerr << "Best Move" << endl;
                    Move(mdir);
                }
            }
            else if(twodis == Int_max) //p2死
            {
                if(jm.CanPutTrappedWall(board, ps[0].mx, ps[0].my, 0, ps))
                {
                    cerr << "Trapped Block p0!!!" << endl;
                }
                else
                {
                    cerr << "Best Move" << endl;
                    Move(mdir);
                }
            }
            else if(twodis < zerodis) //p0跑比較慢 先搞他
            {
                cerr << "Block p0" << endl;
                int zerox=ps[0].mx;
                int zeroy=ps[0].my;
                if(jm.CanPutTrappedWall(board, zerox, zeroy, 0, ps))
                {
                    cerr << "Trapped Block p0!!!" << endl;
                }
                else
                {
                    if(jm.CanPutTrappedWall(board, ps[2].mx, ps[2].my, 2, ps))
                    {
                        cerr << "Can't trap p0, trapped block p2" << endl;
                    }
                    else
                    {
                        cerr << "Best Move" << endl;
                        Move(mdir);
                    }
                }
            }
            else
            {
                cerr << "Block p2" << endl;
                int twox=ps[2].mx;
                int twoy=ps[2].my;
                if(jm.CanPutTrappedWall(board, twox, twoy, 2, ps))
                {
                    cerr << "Trapped Block p2!!!" << endl;
                }
                else //擋不了 擋另一人
                {
                    if(jm.CanPutTrappedWall(board, ps[0].mx, ps[0].my, 0, ps))
                    {
                        cerr << "Can't trap p2, trapped block p0" << endl;
                    }
                    else
                    {
                        cerr << "Best Move" << endl;
                        Move(mdir);
                    }
                }
            }
        }
        else //無法擋人 自己動
        {
            cerr << "No walls left, best Move" << endl;
            Move(mdir);
        }
    }
    //my_id=2 往下走, 我是最後出發的人 有兩個對手 領先情況優先考慮前進 以不要最後一名為策略
    void Plan4(vector<vector<boardState>>& board, playerState* ps)
    {
        int mx=ps[2].mx;
        int my=ps[2].my;
        int mdis=ps[2].mstep;
        string mdir=ps[2].path[0].dir;

        //Player0
        int zerodis=ps[0].mstep;
        string zerodir=ps[0].path[0].dir;
        //Player1
        int onedis=ps[1].mstep;
        string onedir=ps[1].path[0].dir;

        if(my==0 && jm.CanMoveF(board, mx, my, 2)) //看看能否不要卡在上最邊
        {
            auto t=jm.BestMoveStep(board, mx, my+1, 2);
            if(t.first == mdis-1) //往下步數一樣 提前走
            {
                cerr << "Move down first" << endl;
                Move("DOWN");
                return;
            }
        }
        //避免被逼邊關起來 他開場可能還沒用牆壁時先往中靠
        if(mx<=2 && ps[0].mwl>three_player_w-2 && ps[1].mwl>three_player_w-2 && board[my][mx+1].ver==-1)
        {
            auto t=jm.BestMoveStep(board, mx+1, my, 2);
            if(t.first <= mdis) //往右步數只差1 提前避開陷阱
            {
                cerr << "Move right first" << endl;
                Move("RIGHT");
                return;
            }
        }
        if(mx>=w-3 && ps[0].mwl>three_player_w-2 && ps[1].mwl>three_player_w-2 && board[my][mx].ver==-1)
        {
            auto t=jm.BestMoveStep(board, mx-1, my, 2);
            if(t.first <= mdis) //往左步數只差1 提前避開陷阱
            {
                cerr << "Move left first" << endl;
                Move("LEFT");
                return;
            }
        }
        //領先可以先衝 有人死 dis=Int_max, mwl=-1
        if(mdis<zerodis && ps[0].mwl<=0 && mdis<onedis && ps[1].mwl<=0)
        {
            cerr << "They have no wall. RUN!!!" << endl;
            Move(mdir);
        }
        else if(ps[2].mwl > 0)//有牆壁可以妨礙
        {
            cerr << "Try to put wall" << endl;
            if(zerodis == Int_max) //p0死
            {
                if(jm.CanPutTrappedWall(board, ps[1].mx, ps[1].my, 1, ps))
                {
                    cerr << "Trapped Block p1!!!" << endl;
                }
                else
                {
                    cerr << "Best Move" << endl;
                    Move(mdir);
                }
            }
            else if(onedis == Int_max) //p1死
            {
                if(jm.CanPutTrappedWall(board, ps[0].mx, ps[0].my, 0, ps))
                {
                    cerr << "Trapped Block p0!!!" << endl;
                }
                else
                {
                    cerr << "Best Move" << endl;
                    Move(mdir);
                }
            }
            else if(onedis < zerodis) //p0跑比較慢 先搞他
            {
                cerr << "Block p0" << endl;
                int zerox=ps[0].mx;
                int zeroy=ps[0].my;
                if(jm.CanPutTrappedWall(board, zerox, zeroy, 0, ps))
                {
                    cerr << "Trapped Block p0!!!" << endl;
                }
                else
                {
                    if(jm.CanPutTrappedWall(board, ps[1].mx, ps[1].my, 1, ps))
                    {
                        cerr << "Can't trap p0, trapped block p1" << endl;
                    }
                    else
                    {
                        cerr << "Best Move" << endl;
                        Move(mdir);
                    }
                }
            }
            else
            {
                cerr << "Block p1" << endl;
                int onex=ps[1].mx;
                int oney=ps[1].my;
                if(jm.CanPutTrappedWall(board, onex, oney, 1, ps))
                {
                    cerr << "Trapped Block p1!!!" << endl;
                }
                else //擋不了 擋另一人
                {
                    if(jm.CanPutTrappedWall(board, ps[0].mx, ps[0].my, 0, ps))
                    {
                        cerr << "Can't trap p1, trapped block p0" << endl;
                    }
                    else
                    {
                        cerr << "Best Move" << endl;
                        Move(mdir);
                    }
                }
            }
        }
        else //無法擋人 自己動
        {
            cerr << "No walls left, best Move" << endl;
            Move(mdir);
        }
    }
private:
    Judge_Movement jm;
};

class Game_Controller
{
    /*
    void Game();
    void GameLoop();
    void InitPlayer();
    void InitBoard();
    */
public:
    //不同初始條件的Game
    void Game()
    {
        vector<vector<boardState>> board(w, vector<boardState>(h, {-1, -1, -1}));
        playerState ps[player_count]; //save every player state //save every player state 
        //game loop
        if(my_id == 0)
        {
            if(player_count == 2)
            {
                while(1)
                {
                    GameLoop(board, ps);
                    InitPlayer(board, ps);
                    gp.Plan0(board, ps);
                    InitBoard(board, ps);
                }
            }
            else
            {
                while(1)
                {
                    GameLoop(board, ps);
                    InitPlayer(board, ps);
                    gp.Plan1(board, ps);
                    InitBoard(board, ps);
                }
            }
        }
        else if(my_id == 1)
        {
            if(player_count == 2)
            {
                while(1)
                {
                    GameLoop(board, ps);
                    InitPlayer(board, ps);
                    gp.Plan2(board, ps);
                    InitBoard(board, ps);
                }
            }
            else
            {
                while(1)
                {
                    GameLoop(board, ps);
                    InitPlayer(board, ps);
                    gp.Plan3(board, ps);
                    InitBoard(board, ps);
                }
            }
        }
        else
        {
            while(1)
            {
                GameLoop(board, ps);
                InitPlayer(board, ps);
                gp.Plan4(board, ps);
                InitBoard(board, ps);
            }
        }
    }

private:
    Game_Plan gp;
    Judge_Movement jm0;
    void GameLoop(vector<vector<boardState>>& board, playerState* ps)
    {
        //player
        for(int i = 0; i < player_count; i++) 
        {
            int x; // x-coordinate of the player
            int y; // y-coordinate of the player
            int walls_left; // number of walls available for the player
            cin >> x >> y >> walls_left; cin.ignore();
            ps[i]={x, y, walls_left};
            if(x != -1) board[y][x].player=i;
        }

        //wall
        int wall_count; // number of walls on the board
        cin >> wall_count; cin.ignore();
        for(int i = 0; i < wall_count; i++) 
        {
            int wall_x; // x-coordinate of the wall
            int wall_y; // y-coordinate of the wall
            string wall_orientation; // wall orientation ('H' or 'V')
            cin >> wall_x >> wall_y >> wall_orientation; cin.ignore();
            if(wall_orientation == "H")
            {
                board[wall_y][wall_x].hor=0;
                board[wall_y][wall_x+1].hor=1;
            }
            else
            {
                board[wall_y][wall_x].ver=0;
                board[wall_y+1][wall_x].ver=1;
            }
        }
        cerr << "GameLoop" << endl;
    }
    void InitPlayer(vector<vector<boardState>>& board, playerState* ps)
    {
        for(int i=0; i<player_count; ++i)
        {
            auto t=jm0.BestMoveStep(board, ps[i].mx, ps[i].my, i);
            ps[i].mstep=t.first;
            ps[i].path=t.second;
            cerr << i<<" step: "<< ps[i].mstep << " dir: "<< ps[i].path[0].dir << endl;
        }
    }
    void InitBoard(vector<vector<boardState>>& board, playerState* ps)
    {
        for(int i=0; i<player_count; ++i)
        {
            if(ps[i].mx == -1) continue;
            board[ps[i].my][ps[i].mx].player=-1;
        }
    }
};

//初始main
int main()
{
    //int w; // width of the board
    //int h; // height of the board
    //int player_count; // number of players (2 or 3)
    // id of my player (0 = 1st player, 1 = 2nd player, ...)
    cin >> w >> h >> player_count >> my_id; cin.ignore();
    Game_Controller gc;
    gc.Game();
}
