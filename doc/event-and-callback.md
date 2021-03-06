# 基于事件与回调的编程
## 程序按顺序执行？
初学编程时，我们知道程序是按顺序执行的。后来我们学了函数与类(class)，一定程度上改变了按顺序从头到尾的写法。不过，此时的程序本质上仍然按顺序执行。那么，如果写一个“按顺序执行的程序”实现一个动作射击游戏，该怎么做呢？  

```c++
// 假设以下代码是充分面向对象的
while(true){
    if(用户按了'W'键){人物向上移动;}
    if(用户按了'A'键){人物向左移动;}
    ...
    if(用户按了'J'键){射击;}
    
    检测当前游戏场景中各物体的相互作用();
    计算当前游戏场景中各物体作用的结果();
}
```

好不容易写完一个游戏，各种代码混成一团。仅凭肉眼就很容易预见，检测物体间相互作用的代码性能很差。运行上述代码，发现游戏一卡一卡，CPU单核满载。修改代码时发现，`检测当前游戏场景中各物体的相互作用()`和 `计算当前游戏场景中各物体作用的结果()`可谓牵一发而动全身，难以维护。

上述代码可能确实把游戏场景中的各种对象封装得很漂亮，但为什么还是不够优雅？因为游戏是与用户密切交互的过程，但从最大的整体而言，上述代码`while(true)`内部的一切依然是面向过程的。于是**高度面向过程的代码劫持了程序运行的主动权，而用户的交互权几乎被剥夺。程序内部各对象之间的主动交互需求也被否决了，而只能被动接受总体过程的管理。**

结论是，在某些情况下，比如需要高度交互性的场景中，**按顺序执行的代码结构，只能被否定掉**。

## 中断信号与回调(callback)

一般而言，CPU处理信息的速度远远高于人类手动输入信息的速度。事实上CPU处理的速度比网络、硬盘、内存的输入速度都快太多了。假设我们要写程序从硬盘里读一个数。从头到尾按顺序的写法可能会写成这样：

```c++
int 从硬盘读一个数(数在硬盘中的地址){
    向硬盘发指令读一个数(数在硬盘中的地址);
    while(数据还没送来){
        什么都不做;
    }
    return 读到的数;
}
```

如果某进程中，CPU需要连续从硬盘读很多数，则一个进程就会占满CPU一个核，但CPU等硬盘发来数据的过程占据了大多数时间。这些时间本来可以让CPU用于处理别的事。

于是我们的操作系统对读硬盘做了这样的改进：

```c++
// 发起请求
void 要求从硬盘读一个数(数在硬盘中的地址, callback=收到硬盘读取完成的信号时做的操作){
    向硬盘发指令读一个数(数在硬盘中的地址);
    告诉操作系统，收到硬盘读取完成的信号后运行callback;
    // 不等待硬盘读完
    return 读硬盘任务;
}

// 收到响应后的操作
void 收到硬盘读取完成的信号时做的操作(void) interrupt 代表硬盘接口的数据类型 {
	if(硬盘表示数据准备好了) {
        告诉硬盘已收到;  //清除接受标志
        处理收到的数据;
    }
}

// 顶层调用
int 从硬盘读一个数(数在硬盘中的地址){
    读硬盘任务 = 要求从硬盘读一个数(数在硬盘中的地址);
}
```

其中处理数据的过程仅发生在第二个函数。上述代码隐含了一件事：硬盘在读取完成后，通过硬件向CPU发送特定的”中断信号“，表示硬盘传来的数据已准备好。CPU收到信号后，将主动通知操作系统查找处理此信号的函数（现代的CPU在硬件层面上确实有收到信号主动做出响应的功能）。由于`要求从硬盘读一个数`这一函数告诉操作系统，应当执行`收到硬盘读取完成的信号时做的操作`，所以操作系统将停止当前在CPU上运行的任务，转而去处理硬盘上读到的数据。这样，CPU在硬盘准备数据时可不必等待，转而去处理其他任务。当硬盘的信号到来，CPU立即中断手头任务，处理硬盘输入。

这样的代码结构使得程序的触发主动权掌握在硬盘的信号手中，提高了CPU效率。而在游戏中，如果把用户的输入看作硬盘发来的信号，可以写出这样的代码：

```c++
void 用户按下J键的操作(void) interrupt 代表键盘按键的数据类型 {
	人物开枪;
}
```

于是我们使用函数定义了一个按键的行为。维护代码时，只要增加一些函数对应不同的按键即可。

```c++
void 用户按下W键的操作(){
	人物向上移动;
}
...
```

此时我们只要写一个输入处理引擎，每隔一段时间（例如每秒60次）根据用户键盘输入的信号执行对应的callback（回调）函数。

另外，游戏场景中各对象的相互作用也可以按类似方式编写。

```c++
void 子弹与油桶碰撞后的操作(子弹对象,油桶对象){
	子弹消失;
	油桶爆炸;  // 油桶消失;周围僵尸HP减少
}
void 子弹与僵尸碰撞后的操作(子弹对象,僵尸对象){
    子弹消失;
    僵尸HP减少;
}
void 油桶爆炸后的操作(油桶对象){
    油桶消失;
    周围僵尸HP减少;
}
...
```

只要写一个固定的碰撞检测引擎，让它检查场景中的碰撞，执行不同的callback（回调）函数，就能处理整个游戏进程。我们不需要在物理引擎中注入自己的代码，而只要在外部写好callback函数。

上述过程成功实现了碰撞检测、按键检测与游戏本身逻辑的相互独立，同时让CPU不必始终处理游戏进程本身。

最后的问题是，如何向回调函数传参？从底层而言，我们可以在信号产生之后构建一个“事件”对象，记录该信号产生的原因等信息。例如子弹碰撞油桶时，产生的事件可以记录哪一颗子弹与哪一颗油桶在何时何地相撞。产生事件的过程自然由信号处理器负责，对应读硬盘的例子中`收到响应后的操作`。

## 回调安全性

在某些极端情况下，比如一颗子弹被认为同时碰到了一个油桶和一只僵尸，有可能发生什么呢？

假设计算机先执行了油桶的回调函数，油桶爆炸并炸死周围僵尸。此时被子弹碰撞的僵尸已经消失了（僵尸对象被删除，查找该僵尸对象时返回指针NULL）。然而，计算机仍然继续执行子弹碰撞僵尸的回调操作，开始读取僵尸的HP。此时将执行`NULL -> hp;`这一操作，导致程序立即出错。

在回调函数中应当重新检查输入参数的可用性。