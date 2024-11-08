//
// Created by qc on 24-10-17.
//

#ifndef MUDUO_TIMESTAMP_H
#define MUDUO_TIMESTAMP_H
#include<iostream>

//时间类
class Timestamp
{
public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    static Timestamp now();
    std::string toString() const;
private:
    int64_t microSecondsSinceEpoch_;
};

#endif //MUDUO_TIMESTAMP_H
