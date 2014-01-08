LW_OOPC 宏配置及使用指南
========================

LW_OOPC 是一套轻量级的面向对象 C 语言编程框架。它是一套 C 语言的宏，总共 1 个.h 文件 (如果需要内存泄漏和调试打印支持，需要增加 1 个.c 文件 (lw_oopc.c，约 145 行)). 20 个宏，约 130 行代码，非常的轻量级，但却很好的支持了很多面向对象的特性，比如继承，多态。可以优美的实现面向接口编程。

注意，这里特别强调一下，使用 LW_OOPC 的前提是：在 C 语言下。如果您所在的团队已经在使用 C++，那么 LW_OOPC 对于这种情形是没有价值的。也就是说，LW_OOPC 希望能够帮助到那些懂 OO 的程序员，即便是在用 C 语言编程，依然能够编写出面向对象的程序。

言归正传，本文将对 LW_OOPC 的配置和使用方法进行讲解，并对这些宏逐个进行细致讲解。期望本文能给希望在实践中应用 LW_OOPC 的 C 程序员带来帮助。

LW_OOPC 当前版本共有两个文件：lw_oopc.h 和 lw_oopc.c. LW_OOPC 的使用非常简单，只需要将这两个文件加入工程即可。常规情况下，建议用户同时使用上述两个文件，因为借助 lw_oopc.c，我们可以监测到内存泄漏，通过打开调试开关，我们能够观察内存分配和释放的调试打印信息，这将有助于我们除错，减少调试的时间。如果你不需要监测内存泄漏 (如准备发布程序)，此时，你并不需要 lw_oopc.c，而只需要 lw_oopc.h 即可。

### LW_OOPC 配置

在 lw_oopc.h 中，有这么几行代码：

```c
// 配置宏 (两种配置选其一):
// LW_OOPC_USE_STDDEF_OFFSETOF          表示使用 C 标准定义的 offsetof
// LW_OOPC_USE_USER_DEFINED_OFFSETOF    表示使用用户自定义的 lw_oopc_offsetof 宏
#define LW_OOPC_USE_STDDEF_OFFSETOF
//#define LW_OOPC_USE_USER_DEFINED_OFFSETOF

// 是否支持内存泄露检测，缺省不支持
//#define LW_OOPC_SUPPORT_MEMORY_LEAK_DETECTOR
```

从上边的注释，我们可以看出，LW_OOPC 需要使用 offsetof 宏，如果你的开发环境能够支持 C 标准定义的 offsetof 宏，那么什么都不需要动。如果你的开发环境不能支持 C 标准定义的 offsetof 宏，那么可以选择使用用户自定义的 lw_oopc_offsetof 宏 (如果你的开发环境连用户自定义的 offsetof 宏都不支持，在这种情形下，LW_OOPC 将无法很好的支持多态特性，很遗憾，你只能与 LW_OOPC 失之交臂).

关于 LW_OOPC 对内存泄露检测以及调试打印的支持，我们将在 LW_OOPC 高级配置部分进行详细讲解。

### LW_OOPC 宏说明

1. INTERFACE

INTERFACE 用于声明接口，譬如：

```C
INTERFACE(IMoveable)
{
    void (*move)(IMoveable* t);     // Move 行为
};
```

在 LW_OOPC 中，声明接口，抽象类和具体类的方法成员比较特殊，均是函数指针类型的成员。事实上，LW_OOPC 正是借助了函数指针的特性，完成了多态功能的模拟。

2. ABS_CLASS

ABS_CLASS 用于声明抽象类，譬如：

```C
ABS_CLASS(Animal)
{
    char name[128];     // 动物的昵称 (假设小于 128 个字符)
    int age;            // 动物的年龄

    void (*setName)(Animal* t, const char* name);   // 设置动物的昵称
    void (*setAge)(Animal* t, int age);             // 设置动物的年龄
    void (*sayHello)(Animal* t);                    // 动物打招呼
    void (*eat)(Animal* t);                         // 动物都会吃（抽象方法，由子类实现）
    void (*breathe)(Animal* t);                     // 动物都会呼吸（抽象方法，由子类实现）
    void (*init)(Animal* t, const char* name, int age);		// 初始化昵称和年龄
};
```

3. CLASS

CLASS 用于声明具体类，譬如：

```C
CLASS(Fish)
{
    EXTENDS(Animal);		// 继承 Animal 抽象类
    IMPLEMENTS(IMoveable);	// 实现 IMoveable 接口

    void (*init)(Fish* t, const char* name, int age);		// 初始化昵称和年龄
};
```

在该例中，我们声明了 Fish 类，并让该类继承 Animal 抽象类，并且实现 IMoveable 接口。

4. EXTENDS 和 IMPLEMENTS

在介绍 CLASS 宏的时候，我们在代码中看到有两个宏：EXTENDS 和 IMPLEMENTS，如果你查看 lw_oopc.h 的源码，你将会发现他们是一模一样的：

```C
#define IMPLEMENTS(type)	struct type type
#define EXTENDS(type)		struct type type
```

之所以同时提供继承和实现关键字，仅仅是为了让熟悉 Java 的人更加容易理解 LW_OOPC 宏。(注意，在 LW_OOPC 中，建议将继承和实现声明写在结构体的开头，把继承和实现声明摆在显眼的位置，有助于阅读代码的人更好的理解代码).

5. ABS_CTOR 和 END_ABS_CTOR

ABS_CTOR 和 END_ABS_CTOR 用于定义抽象类的构造函数，例如：

```C
/* 设置动物的昵称 */
void Animal_setName(Animal* t, const char* name)
{
    // 这里假定 name 不会超过 128 个字符，为简化示例代码，不做保护（产品代码中不要这样写）
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
    printf("Hello！我是%s，今年%d岁了！\n", t->name, t->age);
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
```

前面，我们声明 Animal 是一个抽象类，对应的构造函数定义需要使用 ABS_CTOR 和 END_ABS_CTOR. ABS_CTOR 是 Abstract Constructor 的缩写。

6. FUNCTION_SETTING

在介绍 ABS_CTOR 和 END_ABS_CTOR 宏的时候，我们在代码中又发现一个陌生的宏：
FUNCTION_SETTING，这个宏在 LW_OOPC 中的地位非同凡响，没有它，LW_OOPC 就不可能存在。LW_OOPC 中的 CTOR 系列宏 (CTOR/END_CTOR, ABS_CTOR/END_ABS_CTOR) 除了给对象 (在 C 语言中是 struct) 分配内存，然后，最重要的一个步骤是为结构体中的函数指针成员赋值。这一过程，也可以称为函数绑定 (有点类似 C++ 中的动态联编). 函数绑定的过程由 FUNCTION_SETTING 宏来完成。
我们来看看 FUNCTION_SETTING 宏是如何实现的：

```C
#define FUNCTION_SETTING(f1, f2)	cthis->f1 = f2;
```

看到这里，想必读者应该会心一笑了。:)

7. CTOR 和 END_CTOR

CTOR 和 END_CTOR 用于定义具体类的构造函数，例如：

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
从代码上看，CTOR/END_CTOR 与 ABS_CTOR/END_ABS_CTOR 的使用方式完全相同。的确是，不过，背后，这两对宏的实现方式略有差异，建议有兴趣的读者，认真研究一下 LW_OOPC 的源码。这里，简单地说明如下：我们希望明确区分抽象类和具体类的概念，抽象类是不可以创建对象的，而具体类则可以。前面，我们声明了 Animal 是抽象类，Fish 类是具体类。那么，我们希望：

```C
Animal* animal = Animal_new();		// 不允许这样写！
Fish* fish = Fish_new();			// 允许这样写！
```

8. SUPER_CTOR

在讲解 CTOR/END_CTOR 宏的时候，又出现一个陌生的宏：SUPER_CTOR. 它的功能与 Java 中的 super 关键字非常类似。

```C
SUPER_CTOR(Animal);
```

意为：调用 Animal 类的构造函数.(建议将 SUPER_CTOR 写在"构造函数"体的开头).

9. DTOR 和 END_DTOR

DTOR 和 END_DTOR 用于定义“析构函数”，例如：

```C
// Expr_node 的析构函数 (DTOR/END_DTOR 用于实现析构函数语义)
DTOR(Expr_node)
	if (--cthis->use == 0)      // 递减引用计数，如果计数为 0，释放自己
	{
		cthis->finalize(cthis); // 释放内存之前先清理资源 (其他需要释放的对象)
		lw_oopc_free(cthis);
	}
END_DTOR
```

这里，特别说明一点，为了模拟 C++ 中的 this 指针，我们允许用户在 ABS_CTOR/END_ABS_CTOR, CTOR/END_CTOR, DTOR/END_DTOR 定义块中可以直接使用 cthis.

10. SUPER_PTR

SUPER_PTR 用于"向上转型"，将对象指针向上转型成直接父类或者直接接口：

```C
Fish* fish = Fish_new();    // 创建鱼对象

// 初始化鱼对象的昵称为：小鲤鱼，年龄为: 1 岁
    fish->init(fish, "小鲤鱼", 1);      

// 将 fish 指针转型为 Animal 类型指针，并赋值给 animals 数组的第一个成员
Animal* animal = SUPER_PTR(fish, Animal);

// 将 fish 指针转型为 IMoveable 接口类型指针，并赋值给 moveObjs 数组的第一个成员
    IMoveable* moveFish = SUPER_PTR(fish, IMoveable);
```

这里，直接父类很容易理解，直接接口，呵呵，暂且认为我是首创的吧。我们再来看一下 Fish 类的声明代码：

```C
CLASS(Fish)
{
    EXTENDS(Animal);			// 继承 Animal 抽象类
    IMPLEMENTS(IMoveable);	    // 实现 IMoveable 接口

    void (*init)(Fish* t, const char* name, int age);		// 初始化昵称和年龄
};
```

对 Fish 类来讲，IMoveable 就是它的直接接口。

11. SUPER_PTR_2 和 SUPER_PTR_3

SUPER_PTR_2 和 SUPER_PTR_3 是 SUPER_PTR 的高级版本，它们的作用与 SUPER_PTR 是完全类似的，都是向上转型。只不过，SUPER_PTR_2 是向上转两次，SUPER_PTR_3 是向上转三次。也就是说，SUPER_PTR_2 用于将自身的指针转型为爷爷辈指针，SUPER_PTR_3 用于将自身的指针转型为曾祖辈指针。看看 SUPER_PTR_2 和 SUPER_PTR_3 的代码：

```C
#define SUPER_PTR_2(cthis, father, grandfather)	\
	SUPER_PTR(SUPER_PTR(cthis, father), grandfather)

#define SUPER_PTR_3(cthis, father, grandfather, greatgrandfather)	\
	SUPER_PTR(SUPER_PTR_2(cthis, father, grandfather), greatgrandfather)
```

看到了吧，SUPER_PTR_2 其实是两次 SUPER_PTR 的叠加。SUPER_PTR_3 是三次 SUPER_PTR 的叠加。由于转型两次或者转型三次，会使得程序过于复杂，所以，建议大家合理组织类的继承关系，尽力避免使用二次转型和三次转型。

12. SUB_PTR

SUB_PTR 用于"向下转型"，将父类指针向下转型成子类：

```C
/* 鱼的吃行为*/
void Fish_eat(Animal* t)
{
Fish* fish = SUB_PTR(t, Animal, Fish);
……	// 这里可以访问 Fish 类的成员
    printf("鱼吃水草！\n");
}
```

eat 方法是 Animal 的一个方法，Fish 类覆写了该方法，注意，由于该方法的第一个参数类型是：Animal*，在实现 Fish 类的 eat 方法 Fish_eat 时，如果想要访问到 Fish 类的成员，需要将第一个参数向下转型成 Fish*，这就是 SUB_PTR 所完成的事情。

13. SUB_PTR_2 和 SUB_PTR_3

SUB_PTR_2 和 SUB_PTR_3 是 SUB_PTR 的高级版本，它们的作用与 SUB_PTR 是完全类似的，都是向下转型。只不过，SUB_PTR_2 是向下转两次，SUB_PTR_3 是向下转三次。也就是说，SUB_PTR_2 用于将自身的指针转型为孙子辈指针，SUB_PTR_3 用于将自身的指针转型为曾孙辈指针。看看 SUB_PTR_2 和 SUB_PTR_3 的代码：

```C
#define SUB_PTR_2(selfptr, self, child, grandchild)     \
	SUB_PTR(SUB_PTR(selfptr, self, child), child, grandchild)

#define SUB_PTR_3(selfptr, self, child, grandchild, greatgrandchild)    \
	SUB_PTR(SUB_PTR_2(selfptr, self, child, grandchild), grandchild, greatgrandchild)
```

看到了吧，SUB_PTR_2 其实是两次 SUB_PTR 的叠加。SUB_PTR_3 是三次 SUB_PTR 的叠加。由于转型两次或者转型三次，会使得程序过于复杂，所以，建议大家合理组织类的继承关系，尽力避免使用二次转型和三次转型。

14. INHERIT_FROM

INHERIT_FROM 用于访问直接父类的成员，例如：

```C
Dog* dog = Dog_new();       // 创建狗对象

// 初始化狗对象的昵称为：牧羊犬，年龄为: 2 岁
dog->init(dog, "牧羊犬", 2);  
INHERIT_FROM(Animal, dog, age) = 3;		// 把牧羊犬的年龄修改为 3 岁
printf("狗的年龄是：%d岁！\n", INHERIT_FROM(Animal, dog, age));	// 打印狗的年龄
```

注意，LW_OOPC 的上一个版本，我们同时提供了 INHERIT_FROM_2 和 INHERIT_FROM_3 这两个宏，INHERIT_FROM_2 用于访问爷爷辈的成员，INHERIT_FROM_3 用于访问曾祖辈的成员。我们认为应当尽量避免使用 INHERIT_FROM_2 和 INHERIT_FROM_3 宏，因为，这会导致类的继承关系中存在严重的数据耦合 (自身类可以直接访问爷爷辈，甚至曾祖父辈的成员)，这将导致程序难于理解，难于维护。因此，在当前版本中，删除了 INHERIT_FROM_2 和 INHERIT_FROM_3 宏，仅仅保留 INHERIT_FROM. 一般情况下，我们可以通过更加合理的函数封装，让当前类通过祖先类提供的方法间接地访问祖先类的成员。如果确实要在当前类中直接访问爷爷辈甚至曾祖辈的成员，我们可以先通过 SUPER_PTR_2 和 SUPER_PTR_3 将当前对象的指针转型为对应的祖先类指针，然后再通过其指针访问其成员。

写到这里，LW_OOPC 所有的宏都介绍完毕了。下面，我们介绍 LW_OOPC 对内存泄漏和调试信息打印的支持。

### LW_OOPC 高级配置

缺省情况下，LW_OOPC 不支持内存泄漏检测，如果需要支持，只要将 lw_oopc.h 文件中

```C
//#define LW_OOPC_SUPPORT_MEMORY_LEAK_DETECTOR 去掉行注释符即可
```

一旦你决定让 LW_OOPC 支持内存泄露检测，那么，你必须同时将 lw_oopc.c 加入工程。
缺省情况下，LW_OOPC 不支持调试信息打印，如果需要支持，只要将 lw_oopc.h 文件中

```C
//#define LW_OOPC_PRINT_DEBUG_INFO 去掉行注释符即可
```

### 实例 (demo/expr-advance)

将 lw_oopc.h 中 LW_OOPC_SUPPORT_MEMORY_LEAK_DETECTOR 宏打开。
main.c 的内容，一开始是这样的：

```C
#include <stdio.h>
#include "expr.h"

int main() {
  Expr* expr1 = Expr_new();
  Expr* expr2 = Expr_new();
  Expr* expr3 = Expr_new();
  Expr* expr = Expr_new();

  expr1->initUnaryX(expr1, "-", 0);
  expr2->initUnaryX(expr2, "-", 5);
  expr3->initBinaryX(expr3, "+", 3, 4);
  expr->initTernary(expr, "?:", expr1, expr2, expr3);

  expr->print(expr);
  printf("\n");

  Expr_delete(expr);
  Expr_delete(expr3);
  Expr_delete(expr2);
  Expr_delete(expr1);

  lw_oopc_report();

  return 0;
}
```

得到编译错误：

```bash
liuboyf1@ipc:~/data/custom/aklw_oopc/demo/ExprAdvance$ gcc *.c
main.c: In function ‘main’:
main.c:9:3: error: too few arguments to function ‘Expr_new’
In file included from main.c:2:0:
expr.h:16:1: note: declared here
main.c:10:3: error: too few arguments to function ‘Expr_new’
In file included from main.c:2:0:
expr.h:16:1: note: declared here
main.c:11:3: error: too few arguments to function ‘Expr_new’
In file included from main.c:2:0:
expr.h:16:1: note: declared here
main.c:12:3: error: too few arguments to function ‘Expr_new’
In file included from main.c:2:0:
expr.h:16:1: note: declared here
```

为了支持内存泄漏的检测，我们需要给每个"构造函数"传入文件名和行号参数。LW_OOPC 已经为我们准备了一个宏：lw_oopc_file_line. 这里，这个宏违反了编程规范 (本来宏应该是全部采用大写字母)，不过，这里之所以采用小写，是故意希望用户产生一种错觉，让用户以为 lw_oopc_file_line 是一个特殊的实参，该实参包含了文件和行号的信息。看看修正编译错误后的代码：

```C
#include <stdio.h>
#include "expr.h"

int main() {
  Expr* expr1 = Expr_new(lw_oopc_file_line);
  Expr* expr2 = Expr_new(lw_oopc_file_line);
  Expr* expr3 = Expr_new(lw_oopc_file_line);
  Expr* expr = Expr_new(lw_oopc_file_line);

  expr1->initUnaryX(expr1, "-", 0);
  expr2->initUnaryX(expr2, "-", 5);
  expr3->initBinaryX(expr3, "+", 3, 4);
  expr->initTernary(expr, "?:", expr1, expr2, expr3);

  expr->print(expr);
  printf("\n");

  Expr_delete(expr);
  Expr_delete(expr3);
  Expr_delete(expr2);
  Expr_delete(expr1);

  return 0;
}
```

看看
Expr* expr1 = Expr_new(lw_oopc_file_line);
相比 Expr* expr1 = Expr_new();
多了一个"实参": lw_oopc_file_line.

仅仅增加了一个参数，我们并不能检测到该程序是否有内存泄漏。我们还得在 main 函数的最后一条语句：return 0; 之前加一条语句：

``` C
lw_oopc_report();
```

运行结果：

```bash
liuboyf1@ipc:~/data/custom/aklw_oopc/demo/ExprAdvance$ ./a.out 
((-0) ? (-5) : (3+4))
lw_oopc: no memory leak.
```

看到 lw_oopc: no memory leak.这句话了吧。这表明我们的程序没有内存泄漏。另外，LW_OOPC 还会生成 memory_detector_result.txt 文件，如下图所示：

```bash
liuboyf1@ipc:~/data/custom/aklw_oopc/demo/ExprAdvance$ cat memory_detector_result.txt 
lw_oopc: no memory leak.
```

我们试着注释掉一句代码，如下所示：

```C
//Expr_delete(expr1);
```

再编译和运行一下，我们看到我们的程序有内存泄漏：

```bash
liuboyf1@ipc:~/data/custom/aklw_oopc/demo/ExprAdvance$ ./a.out 
((-0) ? (-5) : (3+4))
lw_oopc: memory leak:
memory leak in: 0x1ab6950, size: 40, file: expr.c, line: 20
memory leak in: 0x1ab67e0, size: 48, file: expr.c, line: 37
memory leak in: 0x1ab6650, size: 80, file: expr.c, line: 36
memory leak in: 0x1ab6010, size: 80, file: main.c, line: 5
liuboyf1@ipc:~/data/custom/aklw_oopc/demo/ExprAdvance$ cat memory_detector_result.txt 
lw_oopc: memory leak:
memory leak in: 0x1ab6950, size: 40, file: expr.c, line: 20
memory leak in: 0x1ab67e0, size: 48, file: expr.c, line: 37
memory leak in: 0x1ab6650, size: 80, file: expr.c, line: 36
memory leak in: 0x1ab6010, size: 80, file: main.c, line: 5
```

前面的例子，如果都是假设想要在堆上创建对象。有一种情形，我们一直没有涉及：如果我们想让代码在堆栈上创建对象，代码该如何写？
很简单，创建对象：

```C
Expr expr;
Expr_ctor(&expr);
```

销毁对象：

```C
Expr_dtor(&expr);
```

到这里，本文该结束了，希望本文已经将 LW_OOPC 的配置以及所有的宏讲明白了。:) 如果你看了这篇文章，还有疑虑，可以查看 lw_oopc.h 和 lw_oopc.c 源文件。源码面前，了无秘密.:) 如果你看了源码，还有疑问，可以与我联系。
