#include <QCoreApplication>
#include <QFile>
#include <QHash>
#include <QDebug>

QHash <int, QHash<int, int> > cave;
int volume = 0;
int u = 0;
int inputCol = 0;
bool full = false;

void draw(QString msg = "")
{
    qDebug() << msg;
    for(int i = 0; i < cave.size(); i++)
    {
        QString rr = "";
        for(int j = 0; j < cave[i].size(); j++)
        {
            rr.append(QString::number(abs(cave[i][j]))).append(" ");
        }
        qDebug() << rr;
    }
}

void run()
{
    while(true)
    {
        bool move = false;
        for (int i = 0; i <= u; i++) {
            for (int j = 0; j <= u; j++) {
                if (cave[i][j] < 0)
                {
                    //сначала бежим вниз (если можем)
                    if (i < u && cave[i+1][j] == 0)
                    {
                        cave[i+1][j] = -1;
                        cave[i][j] = 3;
                        move = true;
                        break;
                    } else
                    {
                        //вниз не получилось, бежим влево и вправо
                        if (j > 0 && j < u)
                        {
                            if (cave[i][j-1] == 0 && cave[i][j+1] == 0)
                            {
                                //можем бежать в обе стороны
                                cave[i][j-1] = -1;
                                cave[i][j+1] = -1;
                                cave[i][j] = 3;
                                move = true;
                                break;
                            } else
                            {
                                if ((cave[i][j+1] >= 2 || cave[i][j+1] == -1) && cave[i][j-1] == 0)
                                {
                                    //можем бежать только влево
                                    cave[i][j-1] = -1;
                                    cave[i][j] = 3;
                                    move = true;
                                    break;
                                } else
                                {
                                    if ((cave[i][j-1] >= 2 || cave[i][j-1] == -1) && cave[i][j+1] == 0)
                                    {
                                        //можем бежать только вправо
                                        cave[i][j+1] = -1;
                                        cave[i][j] = 3;
                                        move = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (move) break;
        }
        if (!move) break;
    }
}

bool checkExit()
{
    //убираем курсоры
    for(int i =0; i <= u; i++)
    {
        for(int j = 0; j <= u; j++)
        {
            if (cave[i][j] < 0) cave[i][j] = 3;
        }
    }
    bool exit = false;
    //проверка, что вода выбежала
    //проверка на боковых выходах
    for (int i = 0; i <= u; i++) {
        if ((cave[i][0] != 2 && cave[i][0] > 0) ||
            (cave[i][u] != 2 && cave[i][u] > 0))
        {
            exit = true;
            break;
        }
    }
    if (!exit)
    {
        //проверка снизу и сверху
        for (int j = 0; j <= u; j++) {
            if ((cave[u][j] != 2 && cave[u][j] > 0) ||
                (cave[0][j] != 2 && cave[0][j] > 0 && (j != inputCol || full)))
            {
                exit = true;
                break;
            }
        }
    }
    return exit;
}

void fillBottom()
{
    //заполняем пройденный маршрут "водой"
    //ищем "дно"
    int bottom = -1;
    for(int i = u; i >= 0; i--)
    {
        for(int j = 0; j <= u; j++)
        {
            if (cave[i][j] == 3 || cave[i][j] == -1)
            {
                bottom = i;
                break;
            }
        }
        if (bottom > -1) break;
    }
    if (bottom > -1)
    {
        //считаем требуемый объем, чтобы заполнить наше текущее дно
        for(int i = 0; i <= u; i++)
        {
            if (cave[bottom][i] == 3) cave[bottom][i] = 1;
        }
        volume = 0;
        for(int i =0; i <= u; i++)
        {
            for(int j = 0; j <= u; j++)
            {
                if (cave[i][j] == 1) volume++;
            }
        }
        //поднимаем уровень воды
        if (bottom > 0)
        {
            bool found = false;
            for(int i = 0; i <= u; i++)
            {
                if ((cave[bottom-1][i] == 0 || cave[bottom-1][i] == 3) && cave[bottom][i] == 1)
                {
                    cave[bottom-1][i] = -1;
                    found = true;
                }
            }
            if (found) volume++;
        } else if (bottom == 0)
        {
            volume++;
            full = true;
        }
    }
    //сбрасываем маршрут
    for(int i =0; i <= u; i++)
    {
        for(int j = 0; j <= u; j++)
        {
            if (cave[i][j] == 3) cave[i][j] = 0;
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file("input.txt");
    inputCol = 0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        int i = 0;
        while (!file.atEnd())
        {
            QString inp = file.readLine().trimmed();
            for(int j=0;j<inp.length();j++)
            {
                cave[i][j] = inp.mid(j,1).toInt()*2;
                if (i == 0 && cave[i][j] == 0) inputCol = j;
            }
            i++;
        }
        volume = 1;
        draw("begin");
        u = cave.size() - 1;
        cave[0][inputCol] = -1;
        while (true)
        {
            run();
            if (!checkExit())
            {
                fillBottom();
            } else
            {
                break;
            }
        }
        draw("end");
        qDebug() << volume << " liters required";
    }
    return a.exec();
}
