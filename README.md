# PeakScript

## 介绍

嵌入式脚本语言

## 语句结束符

`;` 分号作为语句结束符

## 变量声明和赋值

### `var`

声明一个变量

- `var num;` 声明变量`num`,默认为`null`
- `var num = 0;` 声明变量`num`,并赋值为`0`

### `const`

声明一个常量,并赋值,此后不可修改 (常量必须在声明时立即赋值)

- `const num = 100;`

### `set`

修改一个变量的值,如果变量不存在则声明这个变量

- `set num = 0;` 如果变量不存在,声明变量,将变量赋值为`0`

### 赋值

直接使用`=`给变量赋新值

- `num = 10;`

## 数据类型

使用动态数据类型,类型包括:

### `number` 数值类型

表示数值,例如:

- `123` 正数
- `-123` 负数
- `1.23` 小数

### `string` 字符串

字符串类型,字符串声明符号有 3 种,分别为:

- `` `Hello World` `` 上点号
- `"Hello World"` 双引号
- `'Hello World'` 单引号
- `` `'Hello', "World"!` `` 不同的符号之间可以嵌套

### `array` 数组

数组类型,表示一系列数据的集合,例如:

- `[1, 2, 3]` 表示`1,2,3`的数值集合
- `[1, "a", true, null]` 一个数组中可以存在不同的数据类型
- `[1, [2, 3]]` 可以嵌套数组

可通过下标访问每个元素,下标从 0 开始,例如:

```js
var arr = [1, ["a"]];
arr[0]; // 表示元素 1
arr[2]; // 表示元素 ['a']
arr[2][0]; // 表示元素 'a'
```

### `function` 函数

函数声明,例如:

```js
// 声明一个求和函数,两个参数a和b,返回a+b的结果
function sum(a, b) {
  return a + b;
}
```

函数调用:

```js
sum(10, 20); // 调用sum函数
var s = sum(10, 20); // 使用一个变量来获取返回值
```

### `object` 对象

对象类型,对象可以包括一组变量和方法,如何定义对象:

```js
object Book {
    // 成员变量
    var name;
    // 成员方法
    function show_name() {
        print(name);
    }
}
```

使用:

```
Book.name = "Math"; // 赋值成员变量
Book.show_name(); // 调用成员方法
```

创建新对象:

```
// 创建一个新的对象
var book1 = new Book();
book1.name = "C";
book1.show_name();
```

### `bool` 布尔

逻辑类型,只有两个值:

- `true` 表示真
- `false` 表示假

### `null` 空

空表示没有数值,变量默认为空

## `enum` 枚举

```js
enum Kind {
    A, // 0
    B, // 1
    C = 5, // 5
    D, // 6
}
```

通过`Kind.`访问,例如:`Kind.A`

## 运算符

### 算数运算符

`+`,`-`,`*`,`/`,`%`,`(`,`)`

### 关系运算符

`>`,`>=`,`<`,`<=`,`==`,`!=`

### 逻辑运算符

`||`,`&&`, 提供别名`and`,`or`

### 自赋值运算符

`+=`,`-=`,`*=`,`/=`,`%=`

### 自增自减运算符

`++`,`--`,与 c++一样,符号前置时返回计算后的值,符号后置时返回计算前的值

### 非运算符

`!`,非运算符后可跟表达式或变量,返回这个表达式的非值

## 代码块

`{`,`}`,使用大括号括起来的代码部分

会在当前作用域下新建一个**作用域空间**,代码块中可以访问上面的变量和函数,代码块中的变量遵循**就近原则**  
例如:

```js
var num = 10;
{
    var num = 9;
    echo num; // 9
}
echo num; // 10
```

## 条件语句

```js
if (true) {
} else if (true) {
} else {
}
```

- 表达式可以有括号,也可以不写
- 执行的代码若只有一条语句可以不写大括号

## 循环语句

### `loop`

指定次数的循环,例如:

```js
loop(3) {
    echo "Hello"; // 将会循环3次
}
```

支持得到当前的下标:

```
loop(i in 3) {
    echo i; // 将会打印 0 1 2
}
```

### `for`

```js
for (var i = 0; i < 10; ++i) {
	echo i; // 0 ~ 9
}
```

### `for ... in`

用于遍历数组元素,例如:

```js
var arr = [1, 2, 3, 4];
for (item in arr) {
	echo item; // 遍历输出
}
```

支持获取下标:

```js
for (item, index in arr) {
    echo item; // 值
    echo index; // 下标
}
```

### `while`

```js
var num = 0;
while (num < 10) {
  // todo...
  num++;
}
```

### `do ... while`

先执行一遍,然后判断条件,例如:

```js
do {
  // todo...
} while (false);
```

### 循环控制 `break` 和 `continue`

- `break` 直接跳出当前循环
- `continue` 立即执行下一次循环

## 模块

### `import`

#### 直接导入

`import "...";`直接导入的方式会将模块/文件导入到当前代码空间中,例如:

- `import "math";` 导入内置数学模块,导入之后就可以使用其中的方法了,例如:`max()`
- `import "other.peak";` 导入其他脚本文件

#### 别名对象
`import "..." as ...;` 将模块/代码导入并赋值到对象上,例如:

- `import "math" as math;` 导入数学模块,并赋值给`math`对象,之后通过`math.max()`调用方法

#### 别名对象简写

`import ...;` 当希望导入模块和别名一致时,可以省略引号,直接到对象上,例如:

- `import math;` 这样就相当于`import "math" as math;`,之后可通过`math.xxx()`调用方法

## 注释

- `//` 或者 `#` 单行注释
- `/*`与`*/`包括 多行注释

## 异常处理

用于处理运行时的错误,`try`代码块中的代码如果发生了运行时错误,则会执行`catch`代码块,最终都会执行`finally`代码块,例如:

```js
try {
  // 执行语句
} catch {
  // 若try当中存在错误的语句,则执行
} finally {
  // 无论如何最后都会执行
}
```

其中,`catch`和`finally`可以省略

## 内置函数

- `print` 打印变量/表达式的值

## `echo` 命令

`echo`是一个特殊命令,用于打印变量/表达式的值,类似`print`的结果,是一个快捷打印的命令

## 内置模块

- `type`
- `strings`
- `arrays`
- `time`
- `console`
- `file`
- `math`
- `random`
- `algorithm`

## API 调用

通过`#include <peak.h>`引用`PeakScript`

### `peak::Script` 脚本类

#### static 方法

- `Script::LoadXXX` 系列方法用于加载脚本,并创建脚本对象
- `Script::LocateXXX` 系列方法用于重定位解释器的一些预设方法
- `Script::GetXXX` 方法用于获取解释器的一些内置对象

#### 对象成员方法

- `Execute` 方法用于执行脚本
- `FindVariable` 用于获取空间中的对象
- `AddVariable` 用于添加对象到空间中
