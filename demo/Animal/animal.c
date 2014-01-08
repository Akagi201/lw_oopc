#include <stdio.h>
#include <string.h>
#include "animal.h"

/* 设置动物的昵称 */
void Animal_setName(Animal* t, const char* name)
{
    // 这里假定name小于128个字符，为简化示例代码，不做保护（产品代码中不要这样写）
    strcpy(t->name, name);
}
/* 设置动物的年龄 */
void Animal_setAge(Animal* t, int age)
{
    t->age = age;
}
/* 动物和我们打招呼 */
void Animal_sayHello(Animal* t)
{
    printf("Hello! 我是%s，今年%d岁了! \n", t->name, t->age);
}
/* 初始化动物的昵称和年龄 */
void Animal_init(Animal* t, const char* name, int age)
{
    t->setName(t, name);
    t->setAge(t, age);
}

ABS_CTOR(Animal)
FUNCTION_SETTING(setName, Animal_setName);
FUNCTION_SETTING(setAge, Animal_setAge);
FUNCTION_SETTING(sayHello, Animal_sayHello);
FUNCTION_SETTING(init, Animal_init);
END_ABS_CTOR

/* 鱼的吃行为 */
void Fish_eat(Animal* t)
{
    printf("鱼吃水草! \n");
}
/* 鱼的呼吸行为 */
void Fish_breathe(Animal* t)
{
    printf("鱼用鳃呼吸! \n");
}
/* 鱼的移动行为 */
void Fish_move(IMoveable* t)
{
    printf("鱼在水里游! \n");
}
/* 初始化鱼的昵称和年龄 */
void Fish_init(Fish* t, const char* name, int age)
{
    Animal* animal = SUPER_PTR(t, Animal);
    animal->setName(animal, name);
    animal->setAge(animal, age);
}

CTOR(Fish)
SUPER_CTOR(Animal);
FUNCTION_SETTING(Animal.eat, Fish_eat);
FUNCTION_SETTING(Animal.breathe, Fish_breathe);
FUNCTION_SETTING(IMoveable.move, Fish_move);
FUNCTION_SETTING(init, Fish_init);
END_CTOR

/* 狗的吃行为 */
void Dog_eat(Animal* t)
{
    printf("狗吃骨头! \n");
}
/* 狗的呼吸行为 */
void Dog_breathe(Animal* t)
{
    printf("狗用肺呼吸! \n");
}
/* 狗的移动行为 */
void Dog_move(IMoveable* t)
{
    printf("狗在地上跑! \n");
}
/* 初始化狗的昵称和年龄 */
void Dog_init(Dog* t, const char* name, int age)
{
    Animal* animal = SUPER_PTR(t, Animal);
    animal->setName(animal, name);
    animal->setAge(animal, age);
}

CTOR(Dog)
SUPER_CTOR(Animal);
FUNCTION_SETTING(Animal.eat, Dog_eat);
FUNCTION_SETTING(Animal.breathe, Dog_breathe);
FUNCTION_SETTING(IMoveable.move, Dog_move);
FUNCTION_SETTING(init, Dog_init);
END_CTOR

void Car_move(IMoveable* t)
{
    printf("汽车在开动! \n");
}

CTOR(Car)
FUNCTION_SETTING(IMoveable.move, Car_move);
END_CTOR