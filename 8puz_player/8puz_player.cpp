#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <cstdlib>
#include <random>
#include <string>


using namespace std;

vector<vector<string>> hoandoiz(const vector<vector<string>>& puz, int x1, int y1, int x2, int y2);


//------------------------------
class Node
{
public:
    vector<vector<string>> matran;
    int lv;
    int giatrif;
    Node(vector<vector<string>> matran, int lv, int giatrif)
    {
        this->matran = matran;
        this->giatrif = giatrif;
        this->lv = lv;
    }
    // ============================
    pair<int, int> tim0()                                   //tìm tọa độ của '0'
    {
        for (int i = 0; i < matran.size(); i++)
        {
            for (int j = 0; j < matran[i].size(); j++)
            {
                if (matran[i][j] == "0") {
                    return make_pair(i, j);                 //tìm thấy, trả về tọa độ [i][j]
                }
            }
        }
        return make_pair(-1, -1);                           //ko tìm thấy
    }
    // ============================
    vector<vector<string>> hoandoi(int x1, int y1, int x2, int y2)
    {
        if (x2 >= 0 && x2 < matran.size() && y2 >= 0 && y2 < matran.size())     //điều kiện nằm trong puz(3x3)
        {
            vector<vector<string>> matrancon(matran);
            swap(matrancon[x1][y1], matrancon[x2][y2]);
            return matrancon;
        }
        else
        {
            return vector<vector<string>>();                                    //trả về vector rỗng
        }
    }
    // ============================
    vector<Node> tao_nodecon(Node a)
    {
        int lv_Temp = a.lv;
        vector<Node> nodecons;
        pair<int, int> x = tim0();
        vector<pair<int, int>> dichuyen = { {x.first, x.second - 1}/*trái*/, {x.first, x.second + 1}/*phải*/,
                                            {x.first - 1, x.second}/*lên*/, {x.first + 1, x.second}/*xuống*/ };

        for (auto i : dichuyen)
        {
            int x1 = x.first;     //tọa độ x của '0'
            int y1 = x.second;    //tọa độ y của '0'
            int x2 = i.first;       //tọa độ x của i(hướng đi)
            int y2 = i.second;      //tọa độ y của i(hướng đi)

            vector<vector<string>> matrancon = hoandoi(x1, y1, x2, y2);
            if (!matrancon.empty())
            {
                Node nodeCon(matrancon, lv + 1, 0);
                nodecons.push_back(nodeCon);
            }
        }
        cout << "\n";
        for (auto i : nodecons)
        {
            for (auto j : i.matran)
            {
                for (auto k : j) {
                    cout << k << "  ";
                }
                cout << endl;
            }
            cout << endl;

        }
        return nodecons;
    }
};
//------------------------------
class Puzzle
{
public:
    int sz;
    int step;
    vector<Node> Open;
    vector<Node> Close;
    map<vector<vector<string>>, vector<vector<string>>> Duongdi;
    Puzzle(int sz, int step)
    {
        this->sz = sz;
        this->step = step;
    }
    // ============================
    int tinh_h(const vector<vector<string>>& puz_dau, const vector<vector<string>>& puz_cuoi)
    {
        int h = 0;
        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                if (puz_dau[i][j] != puz_cuoi[i][j] && puz_dau[i][j] != "0")
                {
                    h += 1;     //so với puz cuối, mỗi ô của puz đầu != puz cuối h+1
                }
            }
        }
        return h;
    }
    // ============================
    int tinh_f(const Node& puz_dau, const vector<vector<string>>& puz_cuoi)
    {
        int f = tinh_h(puz_dau.matran, puz_cuoi) + puz_dau.lv;
        return tinh_h(puz_dau.matran, puz_cuoi) + puz_dau.lv;
    }
    // ============================
    vector<Node> giai_puz(const vector<vector<string>> &puz, const vector<vector<string>>& puzc) {

        vector<vector<string>> puz_dau = puz;

        vector<vector<string>> puz_cuoi = puzc;



        cout << "   ====     " << endl;

        cout << "Trang thai ban dau:" << endl;
        for (auto i : puz_dau) {
            for (auto j : i) {
                cout << j << "  ";
            }
            cout << endl;
        }
        cout << "\n";
        cout << "Trang thai cuoi cung:" << endl;
        for (auto i : puz_cuoi) {
            for (auto j : i) {
                cout << j << "  ";
            }
            cout << endl;
        }
        cout << "\n   ====";

        Node node_dau(puz_dau, 0, 0);
        node_dau.giatrif = tinh_f(node_dau, puz_cuoi);      //tính giá trị f cho trạng thái ban đầu
        Open.push_back(node_dau);                           //bỏ vô tập Open

        while (true)
        {
            Node node_cur = Open[0];

            cout << "\nStep " << step << ":\n";
            for (auto j : node_cur.matran)
            {
                for (auto k : j) {
                    cout << k << "  ";
                }
                cout << endl;
            }
            cout << endl;


            if (tinh_h(node_cur.matran, puz_cuoi) == 0)
            {
                cout << "   |  \n\n";
                cout << "Ket thuc\n";       //h=0 -> các ô trùng khớp với puz cuối
                vector<Node> path;
                while (Duongdi.find(node_cur.matran) != Duongdi.end())
                {
                    path.push_back(node_cur);
                    node_cur.matran = Duongdi[node_cur.matran];
                }
                path.push_back(node_dau);
                reverse(path.begin(), path.end());
                return path;
            }
            else
            {
                cout << "   |  \n";         //ngược lại
            }


            for (auto i : node_cur.tao_nodecon(node_cur))
            {
                i.giatrif = tinh_f(i, puz_cuoi);
                if (!Check(Open, i) && !Check(Close, i))
                {
                    Duongdi[i.matran] = node_cur.matran;
                    Open.push_back(i);
                    Close.push_back(node_cur); //thêm curnode vào ds closed 

                }
            }

            Open.erase(Open.begin());   //xóa curnode bên ds open
            step += 1;
            sort(Open.begin(), Open.end(), [](const Node& a, const Node& b)
                {
                    return a.giatrif < b.giatrif;               //sắp xếp node có giá trị f thấp nhất lên đầu
                });
        }
    }
    bool Check(const vector<Node>& list1, const Node& list2) {
        for (const auto& item : list1) {
            if (item.matran.size() == list2.matran.size() && std::equal(item.matran.begin(), item.matran.end(), list2.matran.begin())) {
                return true;
            }
        }
        return false;
    }
};











//------------------------------
void menu()
{
    cout << "Menu" << endl;
    cout << "1. Di chuyen len" << endl;
    cout << "2. Di chuyen xuong" << endl;
    cout << "3. Di chuyen qua trai" << endl;
    cout << "4. Di chuyen qua phai" << endl;
    cout << "5. Tu dong giai" << endl;
    cout << "6. Thoat" << endl;
}




void displayPuz(const vector<vector<string>> p)
{
    for (auto i : p) {
        for (auto j : i) {
            cout << j << "  ";
        }
        cout << endl;
    }
}



void dichuyen(vector<vector<string>>& puz, const int move)
{
    pair<int, int> x;
    for (int i = 0; i < puz.size(); i++)
    {
        for (int j = 0; j < puz[i].size(); j++)
        {
            if (puz[i][j] == "0") {
                x = { i, j };                 //tìm thấy, trả về tọa độ [i][j]
            }
        }
    }
    vector<pair<int, int>> dichuyen = { {x.first, x.second - 1}/*trái*/, {x.first, x.second + 1}/*phải*/,
                                        {x.first - 1, x.second}/*lên*/, {x.first + 1, x.second}/*xuống*/ };
    switch (move)

    {
    case 1:
    {
        pair<int, int> dichuyenlen = { x.first - 1, x.second };
        int x1 = x.first;     //tọa độ x của '0'
        int y1 = x.second;    //tọa độ y của '0'
        int x2 = dichuyenlen.first;       //tọa độ x của i(hướng đi)
        int y2 = dichuyenlen.second;
        if (x2 >= 0 && x2 < puz.size() && y2 >= 0 && y2 < puz[0].size())     //điều kiện nằm trong puz (3x3)
            puz = hoandoiz(puz, x1, y1, x2, y2);
    }
    break;
    case 2:
    {
        pair<int, int> dichuyenxuong = { x.first + 1, x.second };
        int x1 = x.first;     //tọa độ x của '0'
        int y1 = x.second;    //tọa độ y của '0'
        int x2 = dichuyenxuong.first;       //tọa độ x của i(hướng đi)
        int y2 = dichuyenxuong.second;
        if (x2 >= 0 && x2 < puz.size() && y2 >= 0 && y2 < puz[0].size())     //điều kiện nằm trong puz (3x3)
            puz = hoandoiz(puz, x1, y1, x2, y2);
    }
    break;
    case 3:
    {
        pair<int, int> dichuyentrai = { x.first, x.second - 1 };
        int x1 = x.first;     //tọa độ x của '0'
        int y1 = x.second;    //tọa độ y của '0'
        int x2 = dichuyentrai.first;       //tọa độ x của i(hướng đi)
        int y2 = dichuyentrai.second;
        if (x2 >= 0 && x2 < puz.size() && y2 >= 0 && y2 < puz[0].size())     //điều kiện nằm trong puz (3x3)
            puz = hoandoiz(puz, x1, y1, x2, y2);
    }
    break;
    case 4:
    {
        pair<int, int> dichuyenphai = { x.first, x.second + 1 };
        int x1 = x.first;     //tọa độ x của '0'
        int y1 = x.second;    //tọa độ y của '0'
        int x2 = dichuyenphai.first;       //tọa độ x của i(hướng đi)
        int y2 = dichuyenphai.second;
        if (x2 >= 0 && x2 < puz.size() && y2 >= 0 && y2 < puz[0].size())     //điều kiện nằm trong puz (3x3)
            puz = hoandoiz(puz, x1, y1, x2, y2);
    }
    break;
    }
}

vector<vector<string>> hoandoiz(const vector<vector<string>>& puz, int x1, int y1, int x2, int y2)
{
    if (x2 >= 0 && x2 < puz.size() && y2 >= 0 && y2 < puz[0].size())     //điều kiện nằm trong puz (3x3)
    {
        vector<vector<string>> matrancon(puz);
        swap(matrancon[x1][y1], matrancon[x2][y2]);
        return matrancon;
    }
    else
    {
        return vector<vector<string>>();                                    //trả về vector rỗng
    }
}



int check_puz(const vector<vector<string>>& puz_dau, const vector<vector<string>>& puz_cuoi)
{
    int h = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (puz_dau[i][j] != puz_cuoi[i][j] && puz_dau[i][j] != "0")
            {
                h += 1;     //so với puz cuối, mỗi ô của puz đầu != puz cuối h+1
            }
        }
    }
    return h;
}

vector<vector<string>> shufflePuzzle(const vector<vector<string>>& puz_cuoi, int n) {
    vector<vector<string>> puzzle = puz_cuoi;

    random_device rd;
    mt19937 gen(rd());

    pair<int, int> zeroPosition;

    // Tìm vị trí của số 0 trong puzzle
    for (int row = 0; row < puzzle.size(); ++row) {
        for (int col = 0; col < puzzle[row].size(); ++col) {
            if (puzzle[row][col] == "0") {
                zeroPosition = make_pair(row, col);
                break;
            }
        }
    }

    vector<pair<int, int>> directions = {
        {0, -1},  // Trái
        {0, 1},   // Phải
        {-1, 0},  // Lên
        {1, 0}    // Xuống
    };

    for (int i = 0; i < n; ++i) {
        shuffle(directions.begin(), directions.end(), gen);

        pair<int, int> newPosition;
        bool validMove = false;

        // Tìm hướng di chuyển hợp lệ
        for (const auto& direction : directions) {
            int newRow = zeroPosition.first + direction.first;
            int newCol = zeroPosition.second + direction.second;

            if (newRow >= 0 && newRow < puzzle.size() && newCol >= 0 && newCol < puzzle[0].size()) {
                validMove = true;
                newPosition = make_pair(newRow, newCol);
                break;
            }
        }

        // Nếu có hướng di chuyển hợp lệ, hoán đổi số 0 với số đang đứng ở vị trí mới
        if (validMove) {
            swap(puzzle[zeroPosition.first][zeroPosition.second], puzzle[newPosition.first][newPosition.second]);
            zeroPosition = newPosition;
        }
        else {
            // Nếu không có hướng di chuyển hợp lệ, không thay đổi puzzle
            break;
        }
    }

    return puzzle;
}
//------------------------------

int main()
{
    vector<vector<string>> puz;

    vector<vector<string>> puz_cuoi = {
            {"1", "8", "4"},
            {"2", "6", "5"},
            {"3", "7", "0"}
    };
    int n;
    cout << "nhap n:";
    cin >> n;

    puz = shufflePuzzle(puz_cuoi,n);
   
    Puzzle puzz(3,0);
    int luachon = -1;
    vector<Node> duongdi;
    while (luachon != 0 && luachon != 5)
    {

        system("cls");
        displayPuz(puz);

        menu();
        cout << "Nhap lua chon (1-6): ";
        cin >> luachon;
        switch (luachon)
        {
        case 1:
            dichuyen(puz, 1);
            displayPuz(puz);
            if (check_puz(puz, puz_cuoi) == 0)
            {
                cout << "Giai xong" << endl;
                return 0;
            }
            break;
        case 2:
            dichuyen(puz, 2);
            displayPuz(puz);
            if (check_puz(puz, puz_cuoi) == 0)
            {
                cout << "Giai xong" << endl;
                return 0;
            }
            break;
        case 3:
            dichuyen(puz, 3);
            displayPuz(puz);
            if (check_puz(puz, puz_cuoi) == 0)
            {
                cout << "Giai xong" << endl;
                return 0;
            }
            break;
        case 4:
            dichuyen(puz, 4);
            displayPuz(puz);
            if (check_puz(puz, puz_cuoi) == 0)
            {
                cout << "Giai xong" << endl;
                return 0;
            }
            break;
        case 5:
            duongdi = puzz.giai_puz(puz, puz_cuoi);
            break;
        case 6:
            cout << "Thoat" << endl;
            return 0;
        default:
            cout << "Nhap lai!" << endl;
            break;
        }
        cout << "\n   ====\n";
        cout << "Duong di:" << endl;
        for (auto i : duongdi)
        {
            for (auto j : i.matran)
            {
                for (auto k : j) {
                    cout << k << "  ";
                }
                cout << endl;
            }
            cout << endl;


        }
        
    }
    
}