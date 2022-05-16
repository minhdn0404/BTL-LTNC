
#pragma once
void init_game (Snake &snake,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
    Dot d0(x1,y1);   // head
    Dot d1(x2,y2);
    Dot d2(x3,y3);
    Dot d3(x4,y4);
    snake.body.push_back(d0);
    snake.body.push_back(d1);
    snake.body.push_back(d2);
    snake.body.push_back(d3);
}
