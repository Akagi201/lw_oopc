
轻量级的面向对象 C 语言编程框架 LW_OOPC 介绍
========================================

### 摘要

本文介绍一种轻量级的面向对象的 C 语言编程框架: LW_OOPC. LW_OOPC 是 Light-Weight Object-Oriented Programming in(with) C 的缩写，总共一个.h 文件，20 个宏，约 130 行代码，非常的轻量级，但却很好的支持了很多面向对象的特性，比如继承，多态。可以优美的实现面向接口编程. 这个框架是由台湾的高焕堂先生以及他的 MISOO 团队首创，之后由我继续改进优化，最后，经高焕堂同意以 LGPL 协议开源 (开源网址参见后文).

用 C 语言实现 OO？我没听错吗？这听起来真是太疯狂了！... 大家都知道，C++ 支持了面向对象和面向泛型编程，比 C 要更强大些。那么，为什么要在 C 语言中实践面向对象呢？为什么不直接使用 C++ 呢？

### 为什么要用面向对象？

面向过程方式开发的系统，代码复杂，耦合性强，难以维护。随着我们所要解决的问题越来越复杂，代码也变得越来越复杂，越来越难以掌控。而面向对象改变了程序员的思维方式，以更加符合客观世界的方式来认识世界，通过合理的运用抽象，封装，继承和多态，更好的组织程序，从而很好地应对这种复杂性。

### 为什么不直接使用 C++?

C和 C++ 之争由来已久，可能要持续到它们中的一种去世^_^. C 语言以其简洁明快，功能强大的特点，深得开发人员的喜爱，尤其是在嵌入式开发领域，C 语言更是占据了绝对老大的地位。在我看来，语言只是工具，作为程序员，我们要做的是：选择合适的语言，解决恰当的问题。我们要尊重事实，考虑开发环境 (软硬件环境)，考虑团队成员的水平，从商用工程的角度讲，选择团队成员擅长的语言进行开发，风险要小很多。

一些从 Java/C#转到 C 的程序员们，无法从面向对象切换到面向过程，但又必须与 C 语言同事们在遗留的 C 系统上开发软件，他们有时会非常困惑: C 语言是面向过程的编程语言，如何实践面向对象，甚至面向接口编程呢？此时，就非常需要在 C 语言中实现面向对象的手段，而 LW_OOPC 正是应对这一难题的解决之道。

### LW_OOPC 是什么？

简而言之: LW_OOPC 是一套 C 语言的宏，总共 1 个.h 文件 (如果需要内存泄漏检测支持以及调试打印支持，那么还需要 1 个.c 文件 (w_oopc.c，约 145 行)), 20 个宏，约 130 行代码. LW_OOPC 是一种 C 语言编程框架，用于支持在 C 语言中进行面向对象编程。

### LW_OOPC 宏介绍

下面，先通过一个简单的示例来展示 LW_OOPC 这套宏的使用方法。我们要创建这样一些对象: 动物 (Animal)，鱼 (Fish)，狗 (Dog)，车子 (Car). 显然，鱼和狗都属于动物，都会动。车子也会动，但是车子不是动物。会动是这些对象的共同特征，但是，显然它们不属于一个家族。因此，我们首先考虑抽象出一个接口 (IMoveable)，以描述会动这一行为特征：

```C
INTERFACE(IMoveable)
{
    void (*move)(IMoveable* t);     // Move 行为
};
```

INTERFACE 宏用于定义接口，其成员 (方法) 均是函数指针类型。
然后，我们分析 Animal，它应该是抽象类还是接口呢？动物都会吃，都需要呼吸，如果仅仅考虑这两个特征，显然可以把 Animal 定为接口。不过，这里，为了展示抽象类在 LW_OOPC 中如何应用。我们让 Animal 拥有昵称和年龄属性，并且，让动物和我们打招呼 (sayHello 方法)，但，我们不允许用户直接创建 Animal 对象，所以，这里把 Animal 定为抽象类：

```C
ABS_CLASS(Animal)
{
    char name[128];     // 动物的昵称 (假设小于 128 个字符)
    int age;            // 动物的年龄

    void (*setName)(Animal* t, const char* name);   // 设置动物的昵称
    void (*setAge)(Animal* t, int age);             // 设置动物的年龄
    void (*sayHello)(Animal* t);                    // 动物打招呼
    void (*eat)(Animal* t);                         // 动物都会吃（抽象方法，由子类实现）
    void (*breathe)(Animal* t);                   // 动物都会呼吸（抽象方法，由子类实现）
    void (*init)(Animal* t, const char* name, int age);		// 初始化昵称和年龄
};
```

ABS_CLASS 宏用于定义抽象类，允许有成员属性。代码的含义参见代码注释. 紧接着，我们来定义 Fish 和 Dog 类，它们都继承动物，然后还实现了 IMoveable 接口：

```C
CLASS(Fish)
{
    EXTENDS(Animal);		     // 继承 Animal 抽象类
    IMPLEMENTS(IMoveable);	// 实现 IMoveable 接口

    void (*init)(Fish* t, const char* name, int age);	// 初始化昵称和年龄
};

CLASS(Dog)
{
    EXTENDS(Animal);			// 继承 Animal 抽象类
    IMPLEMENTS(IMoveable);	// 实现 IMoveable 接口

    void(*init)(Dog* t, const char* name, int age);		// 初始化昵称和年龄
};
```

为了让 Fish 对象或 Dog 对象在创建之后，能够很方便地初始化昵称和年龄，Fish 和 Dog 类均提供了 init 方法。下面，我们来定义 Car，车子不是动物，但可以 Move，因此，让 Car 实现 IMoveable 接口即可：

```C
CLASS(Car)
{
    IMPLEMENTS(IMoveable);  // 实现 IMoveable 接口 (车子不是动物，但可以 Move)
};
```

接口，抽象类，具体类的定义都已经完成了。下面，我们开始实现它们。接口是不需要实现的，所以 IMoveable 没有对应的实现代码. Animal 是抽象动物接口，是半成品，所以需要提供半成品的实现：

```C
/* 设置动物的昵称*/
void Animal_setName(Animal* t, const char* name)
{
    // 这里假定 name 不会超过 128 个字符，为简化示例代码，不做保护 (产品代码中不要这样写)
    strcpy(t->name, name);
}
/* 设置动物的年龄*/
void Animal_setAge(Animal* t, int age)
{
    t->age = age;
}
/* 动物和我们打招呼*/
void Animal_sayHello(Animal* t)
{
    printf("Hello！我是%s，今年%d岁了！\n", t->name, t->age);
}
/* 初始化动物的昵称和年龄*/
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
```

这里出现了几个新的宏，我们逐个进行讲解. ABS_CTOR 表示抽象类的定义开始，ABS_CTOR(Animal) 的含义是 Animal 抽象类的"构造函数"开始。在 C 语言里边其实是没有 C++ 中的构造函数的概念的. LW_OOPC 中的 CTOR 系列宏 (CTOR/END_CTOR, ABS_CTOR/END_ABS_CTOR) 除了给对象 (在 C 语言中是 struct 实例) 分配内存，然后，紧接着要为结构体中的函数指针成员赋值，这一过程，也可以称为函数绑定 (有点类似 C++ 中的动态联编). 函数绑定的过程由 FUNCTION_SETTING 宏来完成。
对于 Fish 和 Dog 类的实现，与 Animal 基本上是类似的，除了将 ABS_CTOR 换成了 CTOR，直接参见代码：

```C
/* 鱼的吃行为 */
void Fish_eat(Animal* t)
{
    printf("鱼吃水草！\n");
}
/* 鱼的呼吸行为 */
void Fish_breathe(Animal* t)
{
    printf("鱼用鳃呼吸！\n");
}
/* 鱼的移动行为 */
void Fish_move(IMoveable* t)
{
    printf("鱼在水里游！\n");
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
```

上面是 Fish 的实现，下面看 Dog 的实现：

/* 狗的吃行为 */
void Dog_eat(Animal* t)
{
    printf("狗吃骨头！\n");
}
/* 狗的呼吸行为 */
void Dog_breathe(Animal* t)
{
    printf("狗用肺呼吸！\n");
}
/* 狗的移动行为 */
void Dog_move(IMoveable* t)
{
    printf("狗在地上跑！\n");
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

细心的朋友可能已经注意到了，这里又有一个陌生的宏: SUPER_CTOR 未介绍。这个宏是提供给子类用的，用于调用其直接父类的构造函数 (类似 Java 语言中的 super() 调用，在这里，其实质是要先调用父类的函数绑定过程，再调用自身的函数绑定过程)，类似 Java 那样，SUPER_CTOR 如果要出现，需要是 ABS_CTOR 或者 CTOR 下面紧跟的第一条语句。
最后，我们把 Car 类也实现了：

```C
void Car_move(IMoveable* t)
{
    printf("汽车在开动！\n");
}

CTOR(Car)
FUNCTION_SETTING(IMoveable.move, Car_move);
END_CTOR
```

下面，我们实现 main 方法，以展示 LW_OOPC 的威力：

```C
#include "animal.h"

int main()
{
    Fish* fish = Fish_new();    // 创建鱼对象
    Dog* dog = Dog_new();       // 创建狗对象
    Car* car = Car_new();       // 创建车子对象

    Animal* animals[2] = { 0 };     // 初始化动物容器 (这里是 Animal 指针数组)
    IMoveable* moveObjs[3] = { 0 }; // 初始化可移动物体容器 (这里是 IMoveable 指针数组)

    int i = 0;                  // i 和 j 是循环变量
    int j = 0;

    // 初始化鱼对象的昵称为：小鲤鱼，年龄为：1 岁
    fish->init(fish, "小鲤鱼", 1);          

    // 将 fish 指针转型为 Animal 类型指针，并赋值给 animals 数组的第一个成员
    animals[0] = SUPER_PTR(fish, Animal);   

    // 初始化狗对象的昵称为：牧羊犬，年龄为：2 岁
    dog->init(dog, "牧羊犬", 2);            

    // 将 dog 指针转型为 Animal 类型指针，并赋值给 animals 数组的第二个成员
    animals[1] = SUPER_PTR(dog, Animal);    

    // 将 fish 指针转型为 IMoveable 接口类型指针，并赋值给 moveOjbs 数组的第一个成员
    moveObjs[0] = SUPER_PTR(fish, IMoveable);

    // 将 dog 指针转型为 IMoveable 接口类型指针，并赋值给 moveOjbs 数组的第二个成员
    moveObjs[1] = SUPER_PTR(dog, IMoveable);    

    // 将 car 指针转型为 IMoveable 接口类型指针，并赋值给 moveOjbs 数组的第三个成员
    moveObjs[2] = SUPER_PTR(car, IMoveable);    

    // 循环打印动物容器内的动物信息
    for(i=0; i<2; i++)
    {
        Animal* animal = animals[i];
        animal->eat(animal);
        animal->breathe(animal);
        animal->sayHello(animal);
    }

    // 循环打印可移动物体容器内的可移动物体移动方式的信息
    for(j=0; j<3; j++)
    {
        IMoveable* moveObj = moveObjs[j];
        moveObj->move(moveObj);
    }

    lw_oopc_delete(fish);
    lw_oopc_delete(dog);
    lw_oopc_delete(car);

    return 0;
}
```

从上边的代码中，我们惊喜地发现，在 C 语言中，借助 LW_OOPC，我们实现了将不同的动物 (Fish 和 Dog 对象) 装入 Animal 容器，然后可以用完全相同的方式调用 Animal 的方法 (比如 eat 和 breathe 方法)，而实际调用的是具体的实现类 (Fish 和 Dog) 的对应方法。这正是面向对象中的多态的概念. 同样，我们可以将 Fish 对象，Dog 对象，以及 Car 对象均视为可移动物体，均装入 IMoveable 容器，然后用完全相同的方式调用 IMoveable 接口的 move 方法。看到了吗？借助 LW_OOPC，在 C 语言下我们竟然可以轻松地实现面向对象和面向接口编程！

下面，再举一个稍微复杂的例子，它的覆盖面是足够全面的，足以一瞥面向对象编程的 3 个要素：数据抽象，继承和多态。通过这个例子，我们期望展现出 LW_OOPC 在遭遇问题本身比较复杂的情形下，是如何从容应对的，以加深读者对 LW_OOPC 的认识. (备注：该问题来自<C++ 沉思录>第八章的例子，有兴趣的读者可以对照参阅).

### 问题描述

此程序涉及的内容是用来表示算术表达式的树。例如，表达式 (-5) * (3 + 4) 对应的树为：
![math-tree](http://akagi201.qiniudn.com/math-tree.png)

一个表达式树包括代表常数，一元运算符和二元运算符的节点。这样的树结构在编译器和计算器程序中都可能用到. 我们希望能通过调用合适的函数来创建这样的树，然后打印该树的完整括号化形式。例如，我们希望打印

```
((-5)*(3+4))
(((-5)*(3+4))*((-5)*(3+4)))
```

作为输出。此外，我们不想为这些表达式的表示形式操心，更不想关心有关它们内存分配和回收的事宜。
这个程序所做的事情在很多需要处理复杂输入的大型程序中是很典型的，例如编译器，编辑器，CAD/CAM 系统等。此类程序中通常要花费很大的精力来处理类似树，图和类似的数据结构。这些程序的开发者永远需要面对诸如内存分配，灵活性和效率之类的问题。面向对象技术可以把这些问题局部化，从而确保今后发生的一系列变化不会要求整个程序中的其他各个部分随之做相应调整。

```C
#include <stdio.h>
#include "expr.h"

int main()
{
	Expr* expr1 = Expr_new();
	Expr* expr2 = Expr_new();
	Expr* expr3 = Expr_new();
	Expr* expr = Expr_new();

	expr1->initUnaryX(expr1, "-", 5);
	expr2->initBinaryX(expr2, "+", 3, 4);
	expr3->initBinary(expr3, "*", expr1, expr2);
	expr->initBinary(expr, "*", expr3, expr3);

	expr3->print(expr3);
	printf("\n");
	expr->print(expr);
	printf("\n");
 
	Expr_delete(expr);
	Expr_delete(expr3);
	Expr_delete(expr2);
	Expr_delete(expr1);

	return 0;
}
```

### 解决方案

通过考查这个树结构，会发现这里有 3 种节点。一种表示整数表达式，包含一个整数值，无子节点。另外两个分别表示一元表达式和二元表达式，包含一个操作符，分别有一个或两个子节点。我们希望打印各种节点，但是具体方式需要视要打印节点的类型而定。这就是动态绑定的用武之地了: 我们可以定义一个虚函数 (print) 来指明应当如何打印各种节点。动态绑定将会负责在运行时基于打印节点的实际类型调用正确的函数。
首先，我们抽象出"节点"的概念，抽象类的名字定为 Expr_node. 它提供了打印的抽象接口，所有的实际节点类型均从它派生：

```C
ABS_CLASS(Expr_node)
{
    void (*print)(Expr_node* t);
};
```

具体类的情形怎样？这些具体类型中最简单的一类是包含一个整数，没有子节点的节点：

```C
CLASS(Int_node)
{
    EXTENDS(Expr_node);    
	int n;           

	void (*init)(Int_node* t, int k);
};
```

其他类型又如何呢？每个类中都必须存储一个操作符 (这倒简单，本文中假定操作符最长不超过 2 个字符，所以，可以用长度为 3 的字符数组来保存)，但是如何存储子节点呢？在运行时之前，我们并不知道子节点的类型会是什么，所以我们不能按值存储子节点，必须存储指针。这样，一元和二元节点类如下所示：

```C
CLASS(Unary_node)
{
    EXTENDS(Expr_node);
	char op[3];		// 假设操作符最长不超过 2 个字符
    Expr_node* opnd;   
	
	void (*init)(Unary_node* t, const char* a, Expr_node* b);
};

CLASS(Binary_node)
{
	EXTENDS(Expr_node);
    char op[3];		// 假设操作符最长不超过 2 个字符
    Expr_node* left;
    Expr_node* right;

	void (*init)(Binary_node* t, const char* a, Expr_node* b, 
Expr_node * c);
};
```

这个设计方案可以用，不过有一个问题。用户要处理的不是值，而是指针，所以必须记住分配和释放对象。例如，我们需要这么创建表达式树：

```C
Int_node* int_node1 = Int_node_new();
Int_node* int_node2 = Int_node_new();
Int_node* int_node3 = Int_node_new();
Unary_node* unary_node = Unary_node_new();
Binary_node* binary_node1 = Binary_node_new();
Binary_node* binary_node = Binary_node_new();

int_node1->init(int_node1, 5);
int_node2->init(int_node2, 3);
int_node3->init(int_node3, 4);
unary_node->init(unary_node, "-", int_node1);
binary_node1->init(binary_node1, "+", int_node2, int_node3);
binary_node->init(binary_node, "*", unary_node, binary_node1);

lw_oopc_delete(int_node1);
……	// 删除创建的其他节点

```

也就是说，我们需要去关心每一个节点的创建和释放。我们不仅把内存管理这类烦心事推给了用户，而且对用户来说也没有什么方便的办法来处理这些事情。我们得好好想想办法了。

这里，提供一种解决内存管理问题的思路：**引用计数**，这里是针对指针，对指针的状况进行计数，对象创建的时候，引用计数为 1，凡是指针被赋值了，该指针所指对象的引用计数就自增一，每次指针要释放，都先检查对象的引用计数，让引用计数自减一，如果引用计数为 0，则释放该对象。

另外，原先的设计不够高层，用户只能直接针对节点进行操作，没有提供操作子树的概念 (这也是用户代码之所以复杂的原因之一)，我们发现，通过提供子树的概念，我们不但能够隐藏 Expr_node 继承层次，而且，对于每一个节点，我们具备了操纵左子树和右子树的能力 (原来只能操作左子节点和右子节点). 而这种功能增强完全是建立在面向对象的机制之上，我们并没有引入耦合。在非常自然和轻松的情形下，我们获得了更好的软件组件之间协作的能力，这正是面向对象的魅力所在。

这里，我们把子树的概念用类 Expr 来表示，由于子树此时成了 Expr_node 具体类的成员，同样，左右子树在 Expr_node 中同样是以指针的方式保存，所以，对 Expr 也需要进行引用计数，代码直接贴上来，细节解说参见注释：

```C
// expr.h
#ifndef EXPR_H_INCLUDED_
#define EXPR_H_INCLUDED_

#include "lw_oopc.h"

// 表达式节点
ABS_CLASS(Expr_node)
{
	int use;        // 引用计数

    void (*print)(Expr_node* t);        // 打印表达式节点
	void (*finalize)(Expr_node* t);     // 子类通过覆写 finalize 方法，实现对资源清理行为的定制
};

// 表达式 (子树的概念)，其中，init*方法族提供了构建子树的高层 API. 方便用户使用
CLASS(Expr)
{
	int use;         // 引用计数
	Expr_node* p;   // 子树的根节点

    // 构建整数表达式 (包含一个整数值，无子表达式)
	void (*initInt)(Expr* t, int);

    // 构建一元表达式 (包含一个操作符，一个子表达式)
	void (*initUnary)(Expr* t, const char*, Expr*);
    // 构建一元表达式的重载形式 (通过传入个整型值参数，构造一个子表达式为整数表达式的一元表达式)
	void (*initUnaryX)(Expr* t, const char*, int);

    // 构建二元表达式 (包含一个操作符，二个子表达式)
	void (*initBinary)(Expr* t, const char*, Expr*, Expr*);
    // 构建二元表达式的重载形式 (通过传入个整型值参数，构造两个子表达式均为整数表达式的二元表达式)
	void (*initBinaryX)(Expr* t, const char*, int, int);

	void (*print)(Expr* t);     // 打印子树
};

// 整数表达式节点
CLASS(Int_node)
{
    EXTENDS(Expr_node);     // 继承 Expr_node

	int n;                    // 整数值  

    // 初始化整数表达式节点 (传入整数值)
	void (*init)(Int_node* t, int k);   
};

// 一元表达式节点
CLASS(Unary_node)
{
    EXTENDS(Expr_node);     // 继承 Expr_node

	char op[3];		        // 假设操作符最长不超过 2 个字符
    Expr* opnd;              // 子表达式
	
    // 初始化一元表达式节点 (传入一个操作符和一个子表达式)
	void (*init)(Unary_node* t, const char* a, Expr* b);
};

// 二元表达式节点
CLASS(Binary_node)
{
	EXTENDS(Expr_node);     // 继承 Expr_node

    char op[3];		        // 假设操作符最长不超过 2 个字符
    Expr* left;             // 左子表达式
    Expr* right;            // 右子表达式

    // 初始化二元表达式节点 (传入一个操作符和两个子表达式)
	void (*init)(Binary_node* t, const char* a, Expr* b, Expr* c);
};

#endif

//expr.c
……	// 包含所需头文件

ABS_CTOR(Expr_node)
	cthis->use = 1;             // 构造函数中，将引用计数初始化为
END_ABS_CTOR
	
// Expr_node 的析构函数 (DTOR/END_DTOR 用于实现析构函数语义)
DTOR(Expr_node)
	if (--cthis->use == 0)      // 递减引用计数，如果计数为，释放自己
	{
		cthis->finalize(cthis); // 释放内存之前先清理资源 (其他需要释放的对象)
		return lw_oopc_true;	// 返回 true，表示析构成功，可以释放内存
	}
	return lw_oopc_false;		// 返回 false，表示析构失败，不能释放内存
END_DTOR

// 构建整数表达式 (包含一个整数值，无子表达式)，n 为整数值
void Expr_initInt(Expr* expr, int n)
{
	Int_node* intNode = Int_node_new(lw_oopc_file_line);
	intNode->init(intNode, n);
	
	expr->p = SUPER_PTR(intNode, Expr_node);
}

……	// 因篇幅所限，构建一元表达式、二元表达式以及对应的重载形式的函数实现代码省略

// 打印表达式 (子树)
void Expr_print(Expr* t)
{
	Expr_node* p = t->p;
	p->print(p);
}

CTOR(Expr)
FUNCTION_SETTING(initInt, Expr_initInt);
FUNCTION_SETTING(initUnary, Expr_initUnary);
FUNCTION_SETTING(initUnaryX, Expr_initUnaryX);
FUNCTION_SETTING(initBinary, Expr_initBinary);
FUNCTION_SETTING(initBinaryX, Expr_initBinaryX);
FUNCTION_SETTING(print, Expr_print);
	cthis->use = 1;             // 构造函数中，将引用计数初始化为
END_CTOR

// Expr 的析构函数 (DTOR/END_DTOR 用于实现析构函数语义)
DTOR(Expr)
	if (--cthis->use == 0)      // 递减引用计数，如果计数为，释放自己
	{
		Expr_node_delete(cthis->p);
		return lw_oopc_true;
	}
	return lw_oopc_false;
END_DTOR

// 整数表达式节点的初始化
void Int_node_init(Int_node* t, int k)
{
	t->n = k;
}

// 整数表达式节点的打印
void Int_node_print(Expr_node* t) 
{
	Int_node* cthis = SUB_PTR(t, Expr_node, Int_node);
	printf("%d", cthis->n); 
}

// 整数表达式节点的资源清理
void Int_node_finalize(Expr_node* t)
{
	// 什么都不需要做
}

CTOR(Int_node)
SUPER_CTOR(Expr_node);
FUNCTION_SETTING(init, Int_node_init);
FUNCTION_SETTING(Expr_node.print, Int_node_print);
FUNCTION_SETTING(Expr_node.finalize, Int_node_finalize);
END_CTOR

……	// 因篇幅所限，一 (二) 元表达式节点的初始化，打印，资源清理，构造等函数的实现代码省略

//main.c
#include "stdio.h"
#include "Expr.h"

int main()
{
	Expr* expr = Expr_new();
……	// 创建 expr1, expr2, expr3 的代码

	expr1->initUnaryX(expr1, "-", 5);
	expr2->initBinaryX(expr2, "+", 3, 4);
	expr3->initBinary(expr3, "*", expr1, expr2);
	expr->initBinary(expr, "*", expr3, expr3);
	
	expr3->print(expr3);
	printf("\n");
	expr->print(expr);
	printf("\n");

	Expr_delete(expr);
	……	// 删除 expr3、expr2、expr1 的代码

	return 0;
}
```

程序运行效果：

```bash
liuboyf1@ipc:~/data/custom/aklw_oopc/demo/expr$ ./a.out 
((-5)*(3+4))
(((-5)*(3+4))*((-5)*(3+4)))
```

怎么样？效果还不错吧，最重要的是，我们的 C 语言代码现在已经完全是面向对象的。

### 方案的可扩展性如何？

假设我们希望添加一种 Ternary_node 类型来表示三元操作符，如？: (也就是 if-then-else 操作符)，看看，难度有多大？
事实上，正是因为前面的设计是面向对象的，要增加一种节点类型易如反掌：

```C
// 三元表达式节点
CLASS(Ternary_node)
{
	EXTENDS(Expr_node);

	char op[3];		        // 假设操作符最长不超过 2 个字符
	Expr* left;
	Expr* middle;
	Expr* right;

    // 初始化三元表达式节点 (传入一个操作符和三个子表达式)
	void (*init)(Ternary_node* t, const char* op, Expr* left, Expr* middle, Expr* right);
};
```

在 Expr 中添加创建三元表达式的方法：

```C
// 表达式 (子树的概念)，其中，init*方法族提供了构建子树的高层 API，方便用户使用
CLASS(Expr)
{
	int use;        	// 引用计数
	Expr_node* p;   	// 子树的根节点
	……				// 既有实现
    // 构建三元表达式 (包含一个操作符，三个子表达式)
	void (*initTernary)(Expr* t, const char*, Expr*, Expr*, Expr*);
    // 构建三元表达式的重载形式 (通过传入一个整型值参数，构造三个子表达式均为整数表达式的三元表达式)
	void (*initTernaryX)(Expr* t, const char*, int, int, int);
	……				// 既有实现
};
```

请读者参照 Binary_node 的现有实现，实现出 Ternary_node，这里不再赘述。一旦实现出 Ternary_node，我们就可以这样创建表达式树并打印：

```C
……	// 创建 expr1, expr2, expr3, expr 对象 (指针)

expr1->initUnaryX(expr1, "-", 0);
expr2->initUnaryX(expr2, "-", 5);
expr3->initBinaryX(expr3, "+", 3, 4);
expr->initTernary(expr, "?:", expr1, expr2, expr3);

expr->print(expr);
printf("\n");
```

为了支持新的节点类型，对原有代码的更动很少 (仅对 Expr 类有增加方法)，而且只有新增操作 (新增类，新增方法)，但没有修改操作 (指修改原有方法)，面向对象的设计赋予了系统极大的弹性，让程序在应对变化时，更加从容。在这个例子中，LW_OOPC 帮助我们在 C 语言的世界里营造出 OO 的天地，带领我们再一次领略了面向对象的风采。

### LW_OOPC 最佳实践

说得简单一点，要想使用好 LW_OOPC 这套宏，还得首先懂面向对象，要遵循面向对象设计的那些大原则，比如开闭原则等。在 C 语言中使用面向对象，根据实际使用的情况，给出如下建议：

1. 继承层次不宜过深，建议最多三层 (接口，抽象类，具体类，参见图 1 和图 2)
    继承层次过深，在 Java/C#/C++ 中均不推崇，在 C 语言中实践面向对象的时候，尤其要遵循这一点，只有这样，代码才能简单清爽。

2. 尽量避免多重继承
    尽可能使用单线继承，但可实现多个接口 (与 Java 中的单根继承类似).

3. 尽量避免具体类继承具体类
    具体类继承具体类，不符合抽象的原则，要尽量避免。

4. 各继承层次分别维护好自己的数据
    子类尽量不要直接访问祖先类的数据，如果确实需要访问，应当通过祖先类提供的函数，以函数调用的方式间接访问。

![class-interface](http://akagi201.qiniudn.com/class-interface.png)

### LW_OOPC 的优点

1. 轻量级。

2. 广泛的适应性，能够适应各种平台，各种编译器 (能支持 C 的地方，基本上都能支持).

3. 帮助懂 OO 的 Java/C++ 程序员写出面向对象的 C 程序。

4. 使用 C，也能引入 OO 的设计思想和方法，在团队的 C/C++ 分歧严重时可能非常有用。

### LW_OOPC 的缺点

1. 无法支持重载 (C 语言不支持所致)
2. 不完全的封装 (无法区分私有，保护和公有)
    LW_OOPC 的 INTERFACE/ABS_CLASS/CLASS 三个宏展开后都是 C 语言的 struct，其成员全是公有的，宏本身并无能力提供良好地封装层次的支持，所以，只能从编程规范和编程风格上进行引导。
3. 不支持 RTTI 
    既然不支持 RTTI，那么显然也无法支持安全的向下转型 (C++ 中的 dynamic_cast 的转型功能)
4. 不支持拷贝构造以及赋值语义
5. 转换成接口的表述有点麻烦，表达形式相比 C++ 要啰嗦很多。
6. 有学习成本，需要用户学习并习惯这套宏。

前四条缺点，实质上并非是 LW_OOPC 的缺点，而是 C 相对 C++ 而言的缺点，在这里，之所以也一并列上，是希望用户不要对 LW_OOPC 抱太高的期望，毕竟它也只是一套 C 语言的宏而已，C 语言有的缺点，LW_OOPC 并不能够解决。

### 总结

尽管如此，在使用 C 语言编程的时候，在某些情形下，你可能想要通过面向对象来更好的组织代码。偶尔，你也想要用用某个设计模式，此时，这套宏能够帮得上忙，使用它，有助于写出相对易于理解和维护的面向对象的代码。因篇幅所限，本文中没有介绍 LW_OOPC 的高级特性，譬如对内存泄漏检测的支持。示例的完整代码，LW_OOPC 的最新版本以及关于这套宏的更加详细的使用指南，请读者访问<https://github.com/Akagi201/aklw_oopc>获取。最后，期望有兴趣的读者，发挥聪明才智，提出改进建议，让 LW_OOPC 变得越来越好！

### 幕后花絮

在完善 LW_OOPC 宏的过程中，我也认真研究了参考资料中列出的材料。最初 V1.0 版本有将近 25 个宏，后来，收到一些同事的反馈，认为少量宏晦涩难记，而且不易理解，经过认真考虑，删除掉 5 个宏，形成现在的 20 个宏。相比其他用 C 实现面向对象的方案，LW_OOPC 简洁优雅，每个宏的命名都经过仔细地推敲，尽可能做到简单易懂，便于记忆。

但愿 LW_OOPC 真的能够帮助到奋斗在一线的 C 程序员们。

### 参考资料

1. 高焕堂，\<UML+OOPC 嵌入式 C 语言开发精讲\>，电子工业出版社，2008 年 9 月。
2. \<Object-oriented Programming with ANSI-C\>，下载地址: <http://www.planetpdf.com/codecuts/pdfs/ooc.pdf> .
3. C 实现面向对象的方法，<http://www.eventhelix.com/RealtimeMantra/basics/object_oriented_programming_in_c.htm> .


