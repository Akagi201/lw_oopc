#include "animal.h"

int main()
{    
    Fish* fish = Fish_new();    // 创建鱼对象
    Dog* dog = Dog_new();       // 创建狗对象
    Car* car = Car_new();       // 创建车子对象

    Animal* animals[2] = { 0 };     // 初始化动物容器（这里是Animal指针数组)
    IMoveable* moveObjs[3] = { 0 }; // 初始化可移动物体容器（这里是IMoveable指针数组)

    int i = 0;                  // i和j是循环变量
    int j = 0;

    // 初始化鱼对象的昵称为：小鲤鱼，年龄为：1岁
    fish->init(fish, "小鲤鱼", 1);

    // 将fish指针转型为Animal类型指针，并赋值给animals数组的第一个成员
    animals[0] = SUPER_PTR(fish, Animal);

    // 初始化狗对象的昵称为：牧羊犬，年龄为：2岁
    dog->init(dog, "牧羊犬", 2);

    // 将dog指针转型为Animal类型指针，并赋值给animals数组的第二个成员
    animals[1] = SUPER_PTR(dog, Animal);

    // 将fish指针转型为IMoveable接口类型指针，并赋值给moveObjs数组的第一个成员
    moveObjs[0] = SUPER_PTR(fish, IMoveable);

    // 将dog指针转型为IMoveable接口类型指针，并赋值给moveObjs数组的第二个成员
    moveObjs[1] = SUPER_PTR(dog, IMoveable);

    // 将car指针转型为IMoveable接口类型指针，并赋值给moveObjs数组的第三个成员
    moveObjs[2] = SUPER_PTR(car, IMoveable);

    // 循环打印动物容器内的动物信息
    for (i = 0; i < 2; ++i)
    {
        Animal* animal = animals[i];
        animal->eat(animal);
        animal->breathe(animal);
        animal->sayHello(animal);
    }

    // 循环打印可移动物体容器内的可移动物体移动方式的信息
    for (j = 0; j < 3; ++j)
    {
        IMoveable* moveObj = moveObjs[j];
        moveObj->move(moveObj);
    }

    lw_oopc_delete(fish);
    lw_oopc_delete(dog);
    lw_oopc_delete(car);

    return 0;
}
