#include <QDebug>

int notFib(int x)
{
    int result = x;
    if (x < 1000) {
        result = (notFib(x + notFib(x + 1)));
    }
    return result;
}



int main(int argc, char *argv[])
{
    for (int i = 0; i < 1000; ++ i) {
        QList<int>   nums{1, 4, 7, 8, 12, 17, 31};

        for (auto n : nums) {
            qDebug() << n << " --> " << notFib(n);
        }
    }
}
