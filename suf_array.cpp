
vector<int> sufarray(string &s){
    auto md = [](int x, int y) {
	return x >= y ? x - y : x < 0 ? x + y : x;
    };
    s.push_back('$');
    int n = s.length(), sum;
    vector<int> mas(n), buck(max(n, 256)), col(n), buf(n);
    for (int i = 0; i < n; i++)
        col[i] = s[i];
    fill(buck.begin(), buck.end(), 0);
    sum = 0;
    for (int i = 0; i < n; i++)
        buck[col[i]]++;
    for (int i = 0; i < buck.size(); i++)
        sum += buck[i], buck[i] = sum - buck[i];
    for (int i = 0; i < n; i++)
        mas[buck[col[i]]++] = i;
    col[mas[0]] = 0;
    for (int i = 1; i < n; i++)
        if (s[mas[i]] == s[mas[i - 1]])
            col[mas[i]] = col[mas[i - 1]];
        else
            col[mas[i]] = col[mas[i - 1]] + 1;
    for (int len = 1; len < n; len <<= 1){
        fill(buck.begin(), buck.end(), 0);
        sum = 0;
        for (int i = 0; i < n; i++)
            buck[col[i]]++;
        for (int i = 0; i < n; i++)
            sum += buck[i], buck[i] = sum - buck[i];
        for (int i = 0; i < n; i++)
            buf[buck[col[md(mas[i] - len, n)]]++] = md(mas[i] - len, n);
        mas = buf;
        buf[mas[0]] = 0;
        for (int i = 1; i < n; i++)
            if (col[mas[i]] == col[mas[i - 1]] && col[md(mas[i] + len, n)] == col[md(mas[i - 1] + len, n)])
                buf[mas[i]] = buf[mas[i - 1]];
            else
                buf[mas[i]] = buf[mas[i - 1]] + 1;
        col = buf;
    }
    s.pop_back();
    for (int i = 0; i < n; i++)
        if (mas[i] == n - 1){
            mas.erase(mas.begin() + i);
            break;
        }
    return mas;
}

vector<int> lcpsufarray(vector<int> &sufmas, string &s){
    int n = s.length();
    vector<int> obr(n), mas(n);
    for (int i = 0; i < n; i++)
        obr[sufmas[i]] = i;
    int k = 0;
    for (int i = 0; i < n; i++)
        if (obr[i] == n - 1)
            mas[n - 1] = -1;
        else{
            while (max(i + k, sufmas[obr[i] + 1] + k) < n && s[i + k] == s[sufmas[obr[i] + 1] + k])
                k++;
            mas[obr[i]] = k;
            if (k > 0)
                k--;
        }
    return mas;
}
