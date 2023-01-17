#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;


int main() {
    int n, m;
//    ifstream file("C:\\Users\\Asus\\Downloads\\Telegram Desktop\\3.txt");
    cin >> n >> m;
    int** arr = new int*[n + 1];
    for (int i = 0; i <= n; ++i) {
        arr[i] = new int[m + 1];
        for (int j = 0; j <= m; ++j)
            arr[i][j] = 0;
    }

    string num;
//    arr[0][0] = 0;
    int size = 0, left = 0, right = 0;
    for (int i = 1; i <= n; ++i) {
        cin >> num;
        for (int j = 1; j <= m; ++j) {
            arr[i][j] = (int)(num[j - 1] == '5');
        }
    }


    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (arr[i][j] != 0) {
                arr[i][j] = min(arr[i - 1][j - 1], min(arr[i - 1][j], arr[i][j - 1])) + 1;
                if (arr[i][j] >= size) {
                    size = arr[i][j];
                    left = i - size + 1;
                    right = j - size + 1;
                }
            }
        }
    }

    cout << size << " " << left << " " << right << endl;
    return 0;
}