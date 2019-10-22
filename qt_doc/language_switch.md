## language_switch
- 在.pro文件建立.ts文件
~~~cpp
 TRANSLATIONS += english.ts chinese.ts
~~~
- 生成相应.ts文件
~~~cpp
lupdate xxx.pro
~~~

- 使用qtlinguist生成相应的.qm文件
~~~cpp
lrelease xxx.pro
~~~
-   QTranslator *qtrans倒入相应的.qm文件
~~~cpp
qtrans->load("../language_translate/chinese.qm")
~~~
>其中QTranslator翻译器需要在主函数进行建立
>~~~cpp
>a.installTranslator(w.qtrans);
>~~~