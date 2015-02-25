#include <QCoreApplication>
#include <QFile>
#include <QHash>
#include <QDebug>
#
void draw(QHash <int, QHash<int, double> > cave, QString msg = "")
{
    qDebug() << msg;
    for(int i = 0; i < cave.size(); i++)
    {
        QString rr = "";
        for(int j = 0; j < cave[i].size(); j++)
        {
            rr.append(QString::number(cave[i][j],'f', 2)).append(" ");
        }
        qDebug() << rr;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file("input.txt");
    QHash <int, QHash<int, double> > cave;
    int inputCol = 0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        int i = 0;
        while (!file.atEnd())
        {
            QString inp = file.readLine().trimmed();
            for(int j=0;j<inp.length();j++)
            {
                cave[i][j] = inp.mid(j,1).toDouble()*2;
                if (i == 0 && cave[i][j] == 0) inputCol = j;
            }
            i++;
        }
    }
    double volume = 0;
    draw(cave, "begin");
    bool exit = false;
    int u = cave.size() - 1;
    while (!exit)
    {
        volume++;
        //отрицательные значения - те, по которым течет вода
        for (int i = 0; i <= u; i++)
        {
            for (int j = 0; j <= u; j++) {
                if (cave[i][j] >= 3)
                {
                    cave[i][j] -= 3;
                } else if (cave[i][j] < 0)
                {
                    cave[i][j] = fabs(cave[i][j]);
                }
            }
        }
        cave[0][inputCol] = -1;
        //сообщающиеся сосуды
        bool up = true;
        while(up)
        {
            //ищем уровень воды
            int waterline = -1;
            bool linefull = true;
            for (int i = 0; i <= u; i++)
            {
                for (int j = 0; j <= u; j++) {
                    if (cave[i][j] < 2 && cave[i][j] > 0)
                    {
                        waterline = i;
                        if (cave[i][j] < 1) linefull = false;
                    }
                }
                if (waterline > -1) break;
            }
            if (waterline > -1)
            {
                if (linefull)
                {
                //бежим наверх

                } else
                {
                //равномерно растекаемся по уровню
                //todo: придумать как...
                }

            }
        }
        draw(cave, "next liter");
        bool waterIsFlowing = true;
        while (waterIsFlowing)
        {
            //проверка, что вода выбежала
            //проверка на боковых выходах
            for (int i = 0; i <= u; i++) {
                if ((cave[i][0] != 2 && fabs(cave[i][0]) > 0) ||
                    (cave[i][u] != 2 && fabs(cave[i][u]) > 0))
                {
                    exit = true;
                    break;
                }
            }
            if (!exit)
            {
                //проверка снизу
                for (int j = 0; j <= u; j++) {
                    if ((cave[u][j] != 2 && fabs(cave[u][j]) > 0))
                    {
                        exit = true;
                        break;
                    }
                }
            }
            //----------------------------------
            bool canmove = false;
            if (!exit)
            {
                //бежит водичка...
                for (int i = 0; i <= u; i++) {
                    bool move = false;
                    for (int j = 0; j <= u; j++) {
                        if (cave[i][j] < 0)
                        {
                            double curVol = fabs(cave[i][j]);
                            //сначала бежим вниз (если можем)
                            if (i < u && cave[i+1][j] < 1 && cave[i+1][j] >= 0)
                            {
                                //вместимость целевой ячейки
                                double cap = 1 - fabs(cave[i+1][j]);
                                if (cap >= curVol)
                                {
                                    //всю воду можно слить вниз
                                    cave[i+1][j] += curVol;
                                    cave[i+1][j] *= -1;
                                    cave[i][j] = 3;
                                    move = true;
                                    break;
                                } else
                                {
                                    //можно перелить только часть
                                    cave[i+1][j] = -1;
                                    cave[i][j] += cap;
                                    cave[i][j] = fabs(cave[i][j])+3;
                                    move = true;
                                    break;
                                }
                            } else
                            {
                            //вниз не получилось, бежим влево и вправо
                                if (j > 0 && j < u)
                                {
                                    if (cave[i][j-1] < 1 && cave[i][j+1] < 1 &&
                                        cave[i][j-1] >= 0 && cave[i][j+1] >= 0)
                                    {
                                        //можем бежать в обе стороны
                                        double lcap = 1 - fabs(cave[i][j-1]);
                                        double rcap = 1 - fabs(cave[i][j+1]);
                                        if (lcap > rcap)
                                        {
                                            if (lcap >= curVol)
                                            {
                                                //всю воду можно перелить
                                                cave[i][j-1] += curVol;
                                                cave[i][j-1] *= -1;
                                                cave[i][j] = 3;
                                                move = true;
                                                break;
                                            } else
                                            {
                                                //можно перелить только часть
                                                cave[i][j-1] = -1;
                                                cave[i][j] += lcap;
                                                cave[i][j] = fabs(cave[i][j])+3;
                                                move = true;
                                                break;
                                            }
                                        } else if (lcap < rcap)
                                        {
                                            if (rcap >= curVol)
                                            {
                                                //всю воду можно перелить
                                                cave[i][j+1] += curVol;
                                                cave[i][j+1] *= -1;
                                                cave[i][j] = 3;
                                                move = true;
                                                break;
                                            } else
                                            {
                                                //можно перелить только часть
                                                cave[i][j+1] = -1;
                                                cave[i][j] += rcap;
                                                cave[i][j] = fabs(cave[i][j])+3;
                                                move = true;
                                                break;
                                            }
                                        } else
                                        {
                                            //по обе стороны равное кол-во воды
                                            //распределяем поровну
                                            int cap = lcap*2;
                                            if (cap >= curVol)
                                            {
                                                //всю воду можно перелить
                                                cave[i][j-1] += curVol/2;
                                                cave[i][j+1] += curVol/2;
                                                cave[i][j-1] *= -1;
                                                cave[i][j+1] *= -1;
                                                draw(cave);
                                                cave[i][j] = 3;
                                                move = true;
                                                break;
                                            } else
                                            {
                                                //можно перелить только часть
                                                cave[i][j-1] = -1;
                                                cave[i][j+1] = -1;
                                                cave[i][j] += cap;
                                                cave[i][j] = fabs(cave[i][j])+3;
                                                move = true;
                                                break;
                                            }
                                        }
                                    } else
                                    {
                                        if (cave[i][j+1] >= 2 && cave[i][j-1] < 1 &&
                                            cave[i][j-1] >= 0)
                                        {
                                            //можем бежать только влево
                                            double cap = 1 - fabs(cave[i][j-1]);
                                            if (cap >= curVol)
                                            {
                                                //всю воду можно перелить
                                                cave[i][j-1] += curVol;
                                                cave[i][j-1] *= -1;
                                                cave[i][j] = 3;
                                                move = true;
                                                break;
                                            } else
                                            {
                                                //можно перелить только часть
                                                cave[i][j-1] = -1;
                                                cave[i][j] += cap;
                                                cave[i][j] = fabs(cave[i][j])+3;
                                                move = true;
                                                break;
                                            }
                                        } else
                                        {
                                            if (cave[i][j-1] >= 2 && cave[i][j+1] < 1 &&
                                                fabs(cave[i][j+1]) >= 0)
                                            {
                                                //можем бежать только вправо
                                                double cap = 1 - fabs(cave[i][j+1]);
                                                if (cap >= curVol)
                                                {
                                                    //всю воду можно перелить
                                                    cave[i][j+1] += curVol;
                                                    cave[i][j+1] *= -1;
                                                    cave[i][j] = 3;
                                                    move = true;
                                                    break;
                                                } else
                                                {
                                                    //можно перелить только часть
                                                    cave[i][j+1] = -1;
                                                    cave[i][j] += cap;
                                                    cave[i][j] = fabs(cave[i][j])+3;
                                                    move = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (move)
                    {
                        canmove = true;
                        break;
                    }
                }
            } else
            {
                break;
            }
            //проверка, что залитая вода еще может бежать дальше
            {
                if (!canmove)
                {
                    //похоже, тупик...
                    //qDebug() << "deadlock!";
                    waterIsFlowing = false;
                    //exit = true;
                    break;
                }
                waterIsFlowing = false;
                for(int i = 0; i <= u; i ++)
                {
                    for(int j = 0; j <= u; j++)
                    {
                       if (cave[i][j] < 0)
                       {
                           waterIsFlowing = true;
                           break;
                       }
                    }
                    if (waterIsFlowing) break;
                }
            }
            draw(cave, QString::number(volume));
        }
    }
    qDebug() << "need" << volume << " cubes of water";
    return a.exec();
}
