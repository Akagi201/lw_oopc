#ifndef ANIMAL_H_INCLUDED_
#define ANIMAL_H_INCLUDED_

#include "lw_oopc.h"

INTERFACE(IMoveable)
{
    void (*move)(IMoveable* t);     // Move行为
};

ABS_CLASS(Animal)
{
    char name[128];     // 动物的昵称(假设小于128个字符)
    int age;            // 动物的年龄

    void (*setName)(Animal* t, const char* name);   // 设置动物的昵称
    void (*setAge)(Animal* t, int age);             // 设置动物的年龄 
    void (*sayHello)(Animal* t);                    // 动物打招呼
    void (*eat)(Animal* t);                         // 动物都会吃（抽象方法，由子类实现）
    void (*breathe)(Animal* t);                     // 动物都会呼吸（抽象方法，由子类实现）
    void (*init)(Animal* t, const char* name, int age); // 初始化昵称和年龄
};

CLASS(Fish)
{
    EXTENDS(Animal);        // 继承Animal抽象类
    IMPLEMENTS(IMoveable);  // 实现IMoveable接口

    void (*init)(Fish* t, const char* name, int age);
};

CLASS(Dog)
{
    EXTENDS(Animal);        // 继承Animal抽象类
    IMPLEMENTS(IMoveable);  // 实现IMoveable接口


    void(*init)(Dog* t, const char* name, int age);
};

CLASS(Car)
{
    IMPLEMENTS(IMoveable);  // 实现IMoveable接口（车子不是动物，但可以Move）
};

#endif