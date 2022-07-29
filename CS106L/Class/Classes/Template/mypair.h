//
// Created by cby on 22-7-27.
//

#ifndef CS106L_MYPAIR_H
#define CS106L_MYPAIR_H


template<typename First,typename Second> class MyPair {
public:
    First getFirst();
    Second getSecond();

    void setFirst(First f);
    void setSecond(Second f);

private:
    First first;
    Second second;
};



#endif //CS106L_MYPAIR_H
