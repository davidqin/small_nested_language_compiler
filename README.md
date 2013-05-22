吉林大学软件学院大三下编译原理课程试验
---

SNL的词法分析，语法分析（递归下降）C语言实现。

使用方式
---

    make
    
    ./snl source_file

###结果
    
词法分析token results/lexical_analyst.txt

语法树会输出到stdout


文件夹说明
---


###examples

snl程序例子

###include

头文件

###lexical

词法分析程序

###syntax

语法分析程序（递归下降）

###results

编译生成的.o文件(compile/*.o)

词法分析token序列(lexical_analysis.txt)

语法分析树（syntax_analysis.txt）


待解决的问题
---

函数调用与赋值应属于表达式exp()

不支持record
