// ダイクストラ法を用いた最短経路探索プログラム
//標準入力からグラフを受け取り、始点から終点への最短所要時間と経路を出力する

#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// 到達不能を表す大きな値
const int INF = 99999;

// 駅名（文字列）と内部ID（整数）の対応を管理するグローバル変数
// 内部ID置かないと処理が重くなる。
map<string, int> ids;   // 駅名 -> ID
vector<string> names;   // ID  -> 駅名（出力時に駅名へ戻すために使う）

// 駅名を整数IDに変換する。初めて登場した駅には新しいIDを割り当てる。
// 例えば、"Minami-Osawa" が最初に呼ばれたとき ID=0 が返る
int node_id(const string &name) {
  if (ids.count(name) == 0) {
    ids[name] = names.size(); // 現在の駅数が次のIDになる
    names.push_back(name);
  }
  return ids[name];
}

// 標準入力からグラフを読み取り、隣接リスト形式で返す。
// 記述方式: 駅名 隣接駅1 所要時間1 隣接駅2 所要時間2 ...
// 例えば、 "Hashimoto Aihara 3 Sagamihara 3" は橋本から相原へ3分、相模原へ3分を意味する
// 無向グラフなので、A→Bを登録したらB→Aも自動的に登録される
vector<vector<pair<int, int>>> read_graph() {
  vector<vector<pair<int, int>>> graph; // 動的にサイズを拡張するため初期化時はサイズ指定なし
  string line;
  while (getline(cin, line)) {
    istringstream iss(line);
    string station;
    if (!(iss >> station)) continue; // 空行はスキップ
    int u = node_id(station);
    string neighbor;
    int w;
    while (iss >> neighbor >> w) {
      if (w < 0) {
        // ダイクストラ法は辺の重みが負だと正しく動かないのでエラーメッセージを表示する。
        cout << "error: invalid travel time: " << w << endl;
        exit(1);
      }
      int v = node_id(neighbor);
      
      // u と v のうち、より大きい方のIDを取得
      int max_id = max(u, v);
      
      // グラフのサイズが足りない場合、max_id が収まるサイズ(max_id + 1)まで拡張する
      if (max_id >= graph.size()) {
        graph.resize(max_id + 1);
      }

      graph[u].push_back({v, w}); // u -> v
      graph[v].push_back({u, w}); // v -> u（無向グラフなので逆向きも追加）
    }
  }
  
  // main関数で先にID登録された始点・終点が、グラフの接続データに一切登場しなかった場合のセグフォ対策
  if (names.size() > graph.size()) {
    graph.resize(names.size());
  }
  
  return graph;
}

// ダイクストラ法で始点から全駅への最短所要時間を求める。
// まだ確定していない駅の中で、始点から近い駅を順に確定していく。
// 優先度付きキュー（min-heap）を使うことで、常に最短の候補を素早く取り出せる。
// prev[v] には、vへの最短経路で、vの一つ手前にいる駅を記録しておく。
// 最後にprevをたどると始点まで逆にさかのぼれるため、経路の復元に使う。
vector<int> dijkstra(int start, const vector<vector<pair<int, int>>> &graph,
                     vector<int> &prev) {
  int N = names.size();
  vector<int> dist(N, INF); // 始点からの最短時間。最初は全駅到達不能扱い
  prev.assign(N, -1);       // 直前の駅。-1はまだ決まっていない
  dist[start] = 0;          // 始点だけは時間0で確定

  // キューには (所要時間, 駅ID) の組を入れる。smaller-first（min-heap）にするため greater を指定する
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
  pq.push({0, start});

  while (!pq.empty()) {
    int d = pq.top().first;  // 今見ている駅への暫定所要時間
    int u = pq.top().second; // 今見ている駅のID
    pq.pop();

    // 同じ駅がキューに複数入っていることがある。
    // すでにより短い経路で確定済みなら、この情報は古いので読み飛ばす
    if (d > dist[u]) continue;

    // 駅 u の最短時間が確定したことを表示する
    cout << "fixed: " << names[u] << " (time=" << d << ")" << endl;

    // u に隣接する駅を調べ、u を経由したほうが早く着けるなら更新する
    for (int i = 0; i < (int)graph[u].size(); i++) {
      int v = graph[u][i].first;  // 隣の駅
      int w = graph[u][i].second; // u から v への所要時間
      if (dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w; // より短い経路が見つかったので更新
        prev[v] = u;           // v への最短経路では u を経由する
        pq.push({dist[v], v}); // 更新した駅を再びキューに入れる
      }
    }
  }
  return dist;
}

// 経路を始点→終点の順に「A -> B -> C」の形で出力する。
// prev をたどると終点から始点へ逆順にたどれるので、最後に reverse で向きを直す
void print_path(int goal, const vector<int> &prev) {
  vector<int> path;
  for (int v = goal; v != -1; v = prev[v]) {
    path.push_back(v);
  }
  reverse(path.begin(), path.end()); // 逆順になっているので正順に並べ直す
  for (int i = 0; i < (int)path.size(); i++) {
    if (i > 0) cout << " -> ";
    cout << names[path[i]];
  }
  cout << endl;
}

int main() {
  // 始点と終点の駅名を読み取る
  string start_name, goal_name;
  cin >> start_name >> goal_name;
  string dummy;
  getline(cin, dummy); // 改行を読み飛ばして、次のgetlineに備える

  // 始点・終点を先にIDとして登録しておく(グラフの入力に登場しない駅名でもエラーにならないようにするため）
  int start = node_id(start_name);
  int goal  = node_id(goal_name);

  // グラフを読み取る（駅のグラフ情報）
  auto graph = read_graph();

  // ダイクストラ法で最短時間を計算する
  vector<int> prev;
  vector<int> dist = dijkstra(start, graph, prev);

  // 終点に到達できなかった場合はエラーで終了
  if (dist[goal] >= INF) {
    cout << "error: unreachable" << endl;
    return 1;
  }

  //最短時間と経路を出力する。
  cout << "shortest time: " << dist[goal] << " min" << endl;
  cout << "path: ";
  print_path(goal, prev);

  return 0;
}
