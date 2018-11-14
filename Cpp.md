# C++


### [目录]
  * [一.ofstream,ifstream,fstream](#一.ofstream,ifstream,fstream)
    * [1.打开文件](#1.打开文件)
    * [2.关闭文件](#2.关闭文件)
    * [3.文件读写](#3.文件读写)
    * [4.其他成员函数](#4.其他成员函数)

* [二.获取字符cin,getchar,get,getline的区别](#二.C++获取字符cin,getchar,get,getline的区别)
  * [1.cin>>](#1.cin>>)
  * [2.cin.get()](#2.cin.get())
  * [3.cin.get(字符变量名)](#3.cin.get(字符变量名))
  * [4.cin.get(数组名，接收字符数目)](#4.cin.get(数组名，接收字符数目))
  * [5.cin.get()](#5.cin.get())
  * [6.cin.getline(char数组_m,int_n,string_endstr)](#6.cin.getline(char数组m,int_n,string_endstr))
  * [7.getline()](#7.getline())
  * [8.gets()](#8.gets())
  * [9.getchar()](#9.getchar())

## 一.ofstream,ifstream,fstream

```
ofstream         //文件写操作 内存写入存储设备
ifstream         //文件读操作，存储设备读区到内存中
fstream          //读写操作，对打开的文件可进行读写操作
```

#### 1.打开文件 open(文件名,打开方式[使用|符号组合使用],打开属性)

打开方式
```
ios::in	为输入(读)而打开文件
ios::out	为输出(写)而打开文件
ios::ate	初始位置：文件尾
ios::app	所有输出附加在文件末尾
ios::trunc	如果文件已存在则先删除该文件
ios::binary	二进制方式
```

打开属性
```
0	普通文件，打开操作
1	只读文件
2	隐含文件
4	系统文件
```

#### 2.关闭文件 close

#### 3.文件读写

1.可以直接通过重载">>"(in>>type,方式根据type类型)和"<<"符号来从文件流读写。

```
    ofstream out("out.txt");
    if (out.is_open()){
      out << "写入文件";
    }
```

2.getline()


#### 4.其他成员函数
```
bad()
如果在读写过程中出错，返回 true 。例如：当我们要对一个不是打开为写状态的文件进行写入时，或者我们要写入的设备没有剩余空间的时候。

fail()
除了与bad() 同样的情况下会返回 true 以外，加上格式错误时也返回true ，例如当想要读入一个整数，而获得了一个字母的时候。

eof()
如果读文件到达文件末尾，返回true。

good()
这是最通用的：如果调用以上任何一个函数返回true 的话，此函数返回 false 。

seekg( pos_type position )
seekp( pos_type position )
文件操作位置指针g为get,p为put,可参考java中seek()

```


## 二.获取字符cin,getchar,get,getline的区别

##### 1.cin>>

  会自动过滤掉不可见字符（如空格 回车 tab等），遇到空格、回车等会结束获取输入的字符串，后面的字符串会过滤掉（存放在输入流中）。如果后面还需要输入字符串，则会从前面存放的字符串开始获取。

##### 2.cin.get()

##### 3.cin.get(字符变量名)
    用来接收字符，只获取一个字符，可以接收空格，遇回车结束

##### 4.cin.get(数组名，接收字符数目)
    用来接收字符串，可以接收空格，遇回车结束。数组的最后一个字符会是‘\0’，设接收字符数目为n，如果输入的字符串大于等于n，则实际接收到的输入是字符串的前面n-1个字符，包括空格（不包括回车，遇到回车就结束了），会自动在后面增加一个‘\0’。

##### 5.cin.get()
    没有参数，主要用于舍弃输入流中不需要的字符，或者舍弃回车，即舍弃输入流中的一个字符。没有用cin.get()，则后面的s会从输入流中继续读入

##### 6.cin.getline(char数组 m,int n,string endstr)
    实际是cin.getline(接收字符串到m，接收个数n，结束字符endstr)。接收一个字符串，可以接收空格等，最后一个字符为‘\0’。结束符可以通过设置第三个参数自己设置，默认是回车。m不能为string类型。注意：实际接收到的要比n少一个，因为最后一个字符为'\0'。

##### 7.getline()
    用于string类的。使用需包含头文件#include<string>。getline(cin,string s)，接收一个字符串，可以接收空格、回车等
```
与cin.getline()的区别：
  1.cin.getline()接收输入字符串的是数组，getline（）是string类型。
  2.cin.getline()可以接收空格，但不能接收回车；getline()可以接收空格和回车
  3.cin.getline()会在数组结尾是'\0'，getline()不会
```

##### 8.gets()
    gets(m)用于string类的，需包含#include<string>。可以接收空格，遇回车结束。可用于多维数组。


##### 9.getchar()
    m=getchar(),需包含#include<string>

    getchar()函数的作用是从标准的输入stdin中读取字符。

    也就是说，getchar()函数以[字符]为单位对输入的数据进行读取。

    getchar一次读一个字符，回车'\n'也算时getchar接收的对象。

    rewind(stdin)可以清空一次缓冲区。

    在连续两次使用getchar()时
    getchar();
    getchar();
    输入'a'，回车。 第一个getchar接收了'a'，第二个getchar接收了'\n'回车，只有在每次getchar后面加入rewind(stdin)，把输入流清空一次才可以不让回车被第二次getchar接收到。
