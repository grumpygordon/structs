//даны позиции единиц в массиве z (возрастающий порядок) и число k
//хотим найти все хорошие отрезки, что доля единиц в каждом хотя бы 1/k (т.е. sum(seg) / len(seg) >= 1 / k)
//функция возвращает непересекающиеся отрезки, что любой хороший будет подотрезком одного из полученных
//сумма длин линейна (z.size() * k), время работы линейно (z.size())
//функция возвращает отрицательные числа, не забудь max(0, l), min(n, r)

//note: если в массиве эти множества непересекающиеся, то каждый элемент будет покрыт примерно 2*log_{k/(k-1)}(n/2) отрезками (на практике больше)
vector<pair<int, int>> getlr(int k, vector<int> z) {
    auto getval = [&](int i) { return i * k - z[i]; };
    static vector<pair<int, int>> hs;
    hs.clear();
    for (int i = 0; i < z.size(); i++) {
        int lw = getval(i), up = lw + k - 1;
        while (!hs.empty() && hs.back().fr <= up) {
            setmin(lw, hs.back().fr);
            setmax(up, hs.back().fr);
            hs.pop_back();
        }
        hs.emplace_back(lw, up);
    }
    int head = 0;
    vector<pair<int, int>> lr;
    for (auto[lw, up]:hs) {
        int l = z[head] - (up - getval(head));
        while (head < z.size() && lw <= getval(head)) head++;
        int r = z[head - 1] + 1 + (getval(head - 1) + k - 1 - lw);
        lr.emplace_back(l, r);
    }
    return lr;
}