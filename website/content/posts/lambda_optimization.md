---
title: "Лямбда-оптимизация"
date: 2021-10-18T14:46:37+03:00
draft: false
---

Лямбда-оптимизация (также известная как aliens trick, дискретный метод множителей Лагранжа, WQS Binary Search, Lagrangian Relaxation и т.д.) — очень мощная и красивая идея оптимизации динамики, которая стала широко известна после IOI 2016. Позже выяснилось, что подобная задача уже встречалась ранее (к примеру, в 2012 году на зимних петрозаводских сборах, в более ранних китайских олимпиадах и даже в одной научной статье 1992 года), однако действительно популярна эта оптимизация стала именно после 2016 года. Так как это новая и сложная в деталях тема, то вокруг нее есть множество мифов и заблуждений. В этой статье я попытался собрать исчерпывающий источник всей информации, которая известна про лямбда-оптимизацию. Если вы знаете какую-то идею, которая здесь не упоминается, напишите, пожалуйста, об этом [мне](https://t.me/peltorator) в телеграме.

Кроме того, часто так случается, что лямбда-оптимизация внутри себя использует Convex Hull Trick, поэтому если вы с ним не знакомы, рекомендуется сначала [изучить его](https://neerc.ifmo.ru/wiki/index.php?title=Convex_hull_trick) перед прочтением этой статьи.

## Идея на примере задачи

Давайте рассмотрим пример задачи, которую можно решить при помощи лямбда-оптимизации.

**Задача:**
Дан массив неотрицательных чисел $a$ длины $n$. Назовем мощностью подотрезка квадрат суммы его элементов. Необходимо разбить массив $a$ на $k$ подотрезков таким образом, чтобы минимизировать сумму мощностей подотрезков разбиения. Иными словами, выделить такие $k - 1$ чисел (границы подотрезков разбиения) $i_1$, $i_2$, $\ldots$, $i_{k - 1}$, чтобы

$$(a_1 + a_2 + \ldots + a_{i_1 - 1})^2 + (a_{i_1} + a_{i_1 + 1} + \ldots + a_{i_2 - 1})^2 + \ldots + (a_{i_{k - 1}} + \ldots + a_{n})^2$$

было минимально.


Собственно, лямбда-оптимизация обычно и применяется в подобных задачах, где нужно как-то разбить массив на $k$ подотрезков оптимальным образом.

Давайте придумаем какое-нибудь решение. Построим двумерную динамику $dp[i][c]$ размера $n \times k$, которая будет равна минимальной стоимости разбиения первых $i$ элементов массива на $c$ подотрезков. Ответ на задачу, соответственно, будет храниться в $dp[n][k]$.

Чтобы пересчитать эту динамику, нужно перебрать позицию $j$, в которой будет заканчиваться предыдущий отрезок разбиения:

$$dp[i][c] = \min \limits_{j < i} dp[j][c - 1] + (a_{j + 1} + a_{j + 2} + \ldots + a_i)^2$$

Такую динамику в самом простом случае можно считать за $O(n^3 k)$ или $O(n^2 k)$, если насчитывать сумму $a_{j + 1} + \ldots + a_i$ налету или же заметить, что она равна $pref[i] - pref[j]$, где $pref$ — массив префиксных сумм массива $a$.

После чего стандартные оптимизации динамики типа разделяй и властвуй, оптимизации Кнута или Convex Hull Trick могли бы попробовать уменьшить асимптотику этого решения до $O(n k \log n)$, $O(n^2)$ или $O(nk)$.

Давайте для примера рассмотрим, как решать эту задачу при помощи Convex Hull Trick. Это решение понадобится нам далее.

Перепишем формулу через префиксные суммы:

$$dp[i][c] = \min \limits_{j < i} dp[j][c - 1] + (pref[i] - pref[j])^2$$

Раскроем скобки:

$$dp[i][c] = \min \limits_{j < i} dp[j][c - 1] + pref[i]^2 - 2 \cdot pref[i] \cdot pref[j] + pref[j]^2$$

Вынесем за знак минимума фиксированный член $pref[i]^2$:

$$dp[i][c] = pref[i]^2 + \min \limits_{j < i} dp[j][c - 1] - 2 \cdot pref[i] \cdot pref[j] + pref[j]^2$$

И перегруппируем:

$$dp[i][c] = pref[i]^2 + \min \limits_{j < i} (-2 pref[j]) \cdot pref[i] + (dp[j][c - 1] + pref[j]^2)$$

Получилось, что нужно найти минимум линейных функций вида $(-2pref[j]) \cdot x + (dp[j][c - 1] + pref[j]^2)$ в точке $x = pref[i]$. Тогда получается, что эту динамику можно считать по слоям (по возрастанию $c$) за $O(n k \log n)$ при помощи Convex Hull Trick. Можно заметить, что так как элементы массива неотрицательны, то $pref[i]$ не убывают, поэтому можно воспользоваться методом двух указателей вместо бинпоиска внутри Convex Hull Trick и уменьшить время работы до $O(n k)$.

Однако что делать, если ограничения на $n$ и $k$ очень большие? К примеру, $n, k \le 10^5$. Тут уже точно никак не обойтись этой динамикой, потому что ее размер слишком большой. В этот момент к нам на помощь как раз таки и приходит лямбда-оптимизация. Ее идея заключается в том, что мы избавимся от второй размерности в динамике. Тогда новая динамика $dp[i]$ будет просто минимальной стоимостью разбить первые $i$ элементов массива на **сколько-то** подотрезков без ограничения на их количество. Ее формула пересчета будет выглядеть следующим образом:

$$dp[i] = \min_{j < i} dp[j] + (pref[i] - pref[j])^2$$

Однако очевидно, что в этом случае оптимальное разбиение разделит наш массив на $n$ подотрезков длины $1$, потому что именно в таком случае стоимость будет минимальна.

Чтобы этого избежать, мы будем **штрафовать за каждый новый подотрезок, который мы берем**. Давайте введем константу $\lambda$ (лямбда), и тогда стоимость подотрезка будет равна не $(pref[i] - pref[j])^2$, а $(pref[i] - pref[j])^2 + \lambda$. Формула пересчета динамики поменяется от этого совсем не сильно:

$$dp[i] = \min_{j < i} dp[j] + (pref[i] - pref[j])^2 + \lambda$$

Теперь давайте подумаем, что будет происходить с ответом при разном выборе константы $\lambda$. Если лямбда равна нулю, то эта динамика ничем не отличается от динамики без штрафа. В оптимальном решении мы выберем разбиение на $n$ подотрезков. Если же мы будем постепенно увеличивать лямбду, то стоимость использования большего количества отрезков будет возрастать, и мы постепенно будем использовать все меньше и меньше подотрезков. В пределе при очень больших лямбдах (к примеру, $10^{100}$), стоимость использования еще одного подотрезка будет уже больше, чем вся стоимость разбиения, поэтому оптимальное разбиение будет использовать всего один подотрезок, в котором будут лежать все элементы массива.

После всех этих рассуждений последняя идея уже совсем очевидна: давайте запустим бинпоиск по лямбде и найдем такую лямбду, для которой в оптимальном разбиении будет ровно $k$ подотрезков, Тогда если ответ на задачу со штрафом равен $dp[n]$, то ответ на изначальную задачу — это $dp[n] - \lambda k$, потому что надо вычесть стоимость $k$ подотрезков, на которые мы разбиваем наш массив. Остается лишь заметить, что формула нашей динамики с лямбдой практически такая же, как в двумерной динамике, поэтому ее можно насчитывать за $O(n \log n)$ или даже $O(n)$ при помощи Convex Hull Trick (в отличие от динамики по слоям, здесь пересчет динамики происходит через себя, поэтому прямые нужно добавлять по ходу подсчета динамики, но их угловые коэффициенты убывают, так что это можно без проблем делать), и итоговая асимптотика решения задачи будет равна $O(n \log C)$, где $C$ — разность левой и правой границы для лямбды в бинпоиске.

Таким образом, изначальную динамику за $O(n^3 k)$ мы смогли соптимизировать до $O(n \log C)$. Невероятно!

**Замечание:**
Если вам интересно, почему мы назвали нашу константу странной буквой $\lambda$, то дело в том, что этот алгоритм с математической точки зрения можно представлять как дискретную версию [метода множителей Лагранжа](https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%BC%D0%BD%D0%BE%D0%B6%D0%B8%D1%82%D0%B5%D0%BB%D0%B5%D0%B9_%D0%9B%D0%B0%D0%B3%D1%80%D0%B0%D0%BD%D0%B6%D0%B0), в котором по историческим причинам как раз таки используется константа $\lambda$.


В принципе, нельзя сказать, что этот алгоритм какой-то особенно сложный, однако с ним часто возникают проблемы при реализации. Первый вопрос, который, возможно, уже пришел к вам в голову: а точно ли необходимая лямбда найдется? Не может ли быть такой ситуации, что для какой-то конкретной $\lambda$ оптимальное количество подотрезков разбиения — это $k + 1$, а для $\lambda + 1$ ответ уже $k - 1$, и нигде не достигается ровно $k$ подотрезков? Давайте разберемся в этом вопросе!

Определим $f(c)$ как ответ на изначальную задачу для $c$ подотрезков (то есть $dp[n][c]$ в нашей изначальной двумерной динамике). Тогда оптимальный ответ при штрафе $\lambda$ будет равен $\min_{c} f(c) + c \cdot \lambda$. Так как это минимум линейных функций, давайте изобразим это в виде прямых на плоскости (идея, схожая с Convex Hull Trick).

![test](/images/lambda_cht.svg)


Тогда алгоритм коректен, если для каждого $c$ есть такая точка $x = \lambda$, что в этой точке минимум достигается именно для $c$ подотрезков. То есть, иными словами, на картинке представлен корректный Convex Hull Trick, и никакую прямую из него не надо выкинуть. В каком же случае это будет верно? Давайте посмотрим на точки пересечения соседних прямых (для $c$ подотрезков и $c + 1$ подотрезка). Тогда необходимо, чтобы эти точки пересечения возрастали.

Чему равна точка пересечения этих прямых? Это точка, в которой $f(c) + cx = f(c + 1) + (c + 1) x$. Это можно переписать как $x = f(c) - f(c + 1)$. Мы хотим, чтобы эти точки возрастали по $c$, то есть, иными словами, $f(c - 1) - f(c) < f(c) - f(c + 1)$. Возможно, вы уже поняли, что это означает. Это свойство функции называется **вогнутостью** (или выпуклостью вниз).
Тогда если наша функция $f$ удовлетворяет этому свойству, то лямбда-оптимизация будет работать. Заодно мы поняли, что **бинпоиск можно запускать только по целым числам**. Действительно, ведь точки пересечения имеют вид $f(c) - f(c + 1)$, что является целым числом, ведь сами значения функции $f$ — это целые числа (стоимости разбиений на $c$ подотрезков).

![test](/images/lambda_intersection.svg)


Здесь есть одна маленькая тонкость. Пусть $f(c - 1) - f(c) + 1 = f(c) - f(c + 1)$, то есть точка пересечения $c$-й прямой с $c + 1$-й на единицу больше, чем точка пересечения $c - 1$-й прямой с $c$-й. Тогда может возникнуть такая ситуация, что при $\lambda = f(c - 1) - f(c)$ мы выберем ответ, в котором будет $c - 1$ отрезков, а для $\lambda = f(c) - f(c + 1)$ мы уже выберем ответ с $c + 1$ отрезками, и тогда $c$ отрезков нигде не будет достигаться.

Эта проблема решается одним очень простым приемом: при подсчете динамики давайте не просто минимизировать стоимость, но и при равных стоимостях давайте минимизировать количество используемых подотрезков. От этого подсчет динамики не станет сильно сложнее. Тогда в точке $f(c + 1) - f(c)$ оптимальное разбиение разобьет наш массив как раз ровно на $c$ подотрезков.

Аналогично, лямбда оптимизация работает, если выполняется обратное условие: $f(c - 1) - f(c) > f(c) - f(c + 1)$. Это условие называется выпуклостью (или выпуклостью вверх). Как мы поняли, лямбда-оптимизацию можно применять, если функция является выпуклой или вогнутой. Иногда эти условия так же называются условиями **строгой** выпуклости или вогнутости. Однако на практике так получается, что почти никогда функции не удовлетворяют этим условиям.
Обычно функции удовлетворяют условиям **нестрогой** выпуклости и вогнутости, которые формулируются следующим образом:

$$f(c - 1) - f(c) \ge f(c) - f(c + 1)$$

или

$$f(c - 1) - f(c) \le f(c) - f(c + 1)$$

И на самом деле, функция $f$ из нашей задачи тоже удовлетворяет именно условию нестрогой вогнутости. Это можно заметить на примере массива $[0, 0, 0]$. Для него стоимость разбиения на любое количество подотрезков равна нулю, поэтому $f(1) - f(2) = f(2) - f(3)$.

**Замечание:**
Пока что мы не доказываем, что в нашей задаче функция $f$ нестрого вогнутая, потому что это чаще всего совершенно не тривиальный факт. Мы докажем его (а также дадим инструмент для доказывания этого в других задачах) чуть позже. Однако обычно при написании решения этот факт просто берется на веру и не доказывается.


Что же делать, если наша функция нестрого выпуклая? В этом случае все еще существует оптимальное число $\lambda$, однако может возникнуть такая ситуация, что в одной точке пересекается сразу несколько прямых. К примеру, прямые для $c - 1$, $c$ и $c + 1$. Тогда в этой точке оптимальное разбиение будет использовать $c - 1$ подотрезков, а в следующей уже $c + 1$ подотрезка, и так получается, что прямая для $c$ нигде не доминирует.

![test](/images/lambda_triple_intersection.svg)


Однако это не проблема! Мы все так же можем бинпоиском найти лямбду, в которой разбиение на $k$ отрезок будет оптимальным (это наибольшая лямбда, в которой в оптимальном разбиении не более $k$ подотрезка). Однако если нам вернули, что в этой точке оптимально использовать $c$ подотрезков, и стоимость равна $Y$, то стоимость разбиения на $k$ подотрезков без штрафа будет попросту равна $Y - k \lambda$, ведь через точку $(\lambda, Y)$ проходит прямая $f(k) + xk$. Обратите внимание, что мы вычитаем из $Y$ именно $k \lambda$, а не $c \lambda$. $Y - c \lambda$ — это стоимость оптимального разбиения массива на $c$ подотрезков, а нам нужно $k$.

## Применение лямбда-оптимизации вне динамического программирования

Идею лямбда-оптимизации необязательно использовать именно для задач динамического программирования. Давайте познакомимся с примером такой задачи.

**Задача:**
Дан граф с весами на ребрах. Необходимо найти вес минимального остовного дерева, в котором степень первой вершины равна $k$.


В этой задаче можно ввести число $\lambda$ и прибавить его к весам всех ребер, инцидентных первой вершине, после чего найти в получившемся графе остов минимального веса. Если лямбда — большое отрицательное число, то всегда выгодно брать ребра из вершины $1$, и в оптимальном остове будет максимальное их количество. Если же лямбда — большое положительное число, то брать ребра из вершины $1$ очень невыгодно, и в оптимальном ответе будет только одно такое ребро.

Осталось лишь запустить бинпоиск по лямбде и найти такую лямбду, при которой в оптимальном ответе степень первой вершины будет равна $k$.

**Упражнение:**
Дано дерево с весами на ребрах. Найдите в нем паросочетание размера $k$ минимального веса.


## Восстановление ответа

Мы научились находить оптимальную стоимость разбиения массива на $k$ подотрезков при условии, что функция стоимости $f(k)$ является нестрого выпуклой. Теперь давайте поймем, как восстанавливать ответ. То есть как находить то самое оптимальное разбиение, если это спрашивается в задаче. В случае строгой выпуклости это очень просто: у нас есть конкретное $\lambda$, при котором мы разбили наш массив на $k$ подотрезков. Необходимо просто в конце запустить еще раз подсчет динамики для оптимального $\lambda$ и при подсчете поддерживать предков как в любой обычной динамике.

Проблема возникает в тот момент, когда функция $f$ является нестрого выпуклой. В этом случае нет какого-то универсального способа восстановления ответа в лямбда-оптимизации, но мы рассмотрим два, которые обычно работают.

Вероятно, при первом знакомстве с лямбда-оптимизацией не стоит читать этот раздел, и лучше вернуться сюда, когда вы прорешаете какое-то количество задач и наберетесь опыта.

### Хранение минимального и максимального количества подотрезков в оптимальных разбиениях

Мы уже и так храним не только значение динамики, но и минимальное количество подотрезков, на которые можно разбить массив, чтобы достигался оптимальный ответ. Давайте хранить не только минимальное количество подотрезков, но и максимальное! Назовем эти значения $l[i]$ и $r[i]$: минимальное и максимальное количество подотрезков, на которые можно разбить первые $i$ элементов массива с данным штрафом $\lambda$, чтобы стоимость разбиения была оптимальна. Мы уже знаем, что чем больше лямбда, тем меньше отрезков нам надо использовать, и если при одной и той же лямбде можно оптимально разбить массив и на $l[i]$ подотрезков, и на $r[i]$, то можно за ту же стоимость разбить его и на любое количество подотрезков между ними.

Теперь мы можем идти с конца и постепенно восстанавливать ответ. Мы знаем, что $l[n] \le k \le r[n]$, потому что $k$ подотрезков дают оптимальный ответ для данной лямбды. Теперь мы будем перебирать позиции по убыванию, пытаясь найти позицию $j$, через которую мы могли бы оптимально пересчитать значение динамики для $n$. Для этого должно, во-первых, выполняться, что $dp[n] = dp[j]  + (pref[n] - pref[j])^2 + \lambda$, а во-вторых, должно быть возможно разбить массив до позиции $j$ оптимальным образом ровно на $k - 1$ подотрезков (чтобы с добавлением еще одного весь массив разбился ровно на $k$ подотрезков), то есть должно быть выполнено: $l[j] \le k - 1 \le r[j]$. Когда мы нашли такое $j$, то мы знаем, что последний подотрезок разбиения — это $[j + 1, n]$, и можем продолжать процесс поиска следующего подотрезка. Так мы за $O(n)$ сможем восстановить необходимое разбиение.

### Второй бинпоиск для уточнения $k$

Пускай мы уже нашли лямбду, для которой $k$ подотрезков являются оптимальными, но минимальное количество подотрезков в оптимальном разбиении меньше $k$. Тогда запустим еще один бинпоиск уже при фиксированной лямбде, который попытается сделать разбиение ровно с $k$ подотрезками. Раньше при равной стоимости мы минимизировали количество подотрезков в разбиении, а теперь введем константу $mid$, такую что для индексов, меньших $mid$, мы будем брать большее количество подотрезков в разбиении при равной стоимости, а для индексов, не меньших $mid$, будем брать меньшее количество подотрезков в разбиении при равной стоимости. При $mid = 0$ мы всегда выбираем меньшее количество подотрезков и получим разбиение, в котором не больше $k$ подотрезков, а при $mid = n$ мы всегда предпочитаем большее количество подотрезков и получим разбиение, в котором не меньше $k$ подотрезков. Тогда мы можем запустить бинпоиск по константе $mid$ в надежде, что найдется такая константа, для которой в оптимальном разбиении будет ровно $k$ подотрезков. Разумеется, то, что такая константа найдется, не гарантируется.

Тогда мы сначала запускаем наше обычное решение без восстановления ответа, находим оптимальную лямбду, и уже для оптимальной лямбды запускаем бинпоиск по $mid$. Асимптотика этого решения — $O(n (\log C + \log n))$.

### Комбинация двух путей

Еще про один способ восстановления ответа можно прочитать в разделе <<Неравенство четырехугольника>> чуть ниже.

## Доказательство выпуклости функции $f$

Вероятно, при первом знакомстве с лямбда-оптимизацией не стоит читать этот раздел, и лучше вернуться сюда, когда вы прорешаете какое-то количество задач и наберетесь опыта.

Пока что мы принимали на веру, что функция $f(k)$ для нашей задачи является выпуклой или вогнутой. И чаще всего при решении вы и не будете это доказывать. Вы либо просто верите в это, либо, возможно, интуитивно понимаете, что стоимость перехода от решения с $k$ подотрезками к решению с $k + 1$ подотрезками убывает или возрастает ($f(k) - f(k + 1)$ убывает или возрастает). Иными словами, можно это понимать так: если есть решения для $k + 1$ и $k - 1$, то из них можно переконструировать два решения для $k$, и тогда одно из них будет иметь вес, не больший среднего арифметического $f(k + 1)$ и $f(k - 1)$.

Однако если вы все такие хотите доказать выпуклость формально, то вам могут помочь методы из этого раздела. Особенно это будет полезно для авторов задач, которые хотят доказать, что их решение верно.

### Сведение к mincost-k-flow

Пререквизиты: знание стоимостных потоков.

Стандартная задача, в которой ответ выпуклый, — это mincost-k-flow. Пусть дана сеть со стоимостями на ребрах. Определим $f(k)$ как минимальную стоимость потока величины $k$ в этой сети. Тогда утверждается, что $f(k)$ — выпуклая функция. Действительно, давайте вспомним, как ищется поток минимального веса. Чтобы перейти от потока величины $k$ к потоку величины $k + 1$, мы находим кратчайший путь в остаточной сети и пускаем по нему единицу потока. Стоимость этого увеличения как раз равна длине кратчайшего пути. Но ведь длина кратчайшего пути не убывает при возрастании $k$, поэтому $f(k + 1) - f(k)$ не убывает. Что и требовалось доказать.

То есть если ответ на нашу задачу можно выразить как стоимость минимального потока в какой-то сети, то функция $f$ будет являться выпуклой, а значит, мы можем применять в этой задаче лямбда-оптимизацию. Обратите внимание, что размер сети может быть сколь угодно большой, ведь мы не собираемся реально решать нашу задачу при помощи mincost-а. Мы лишь хотим доказать, что это можно сделать. Решать же задачу мы будем лямбда-оптимизацией.

Давайте рассмотрим пример задачи, в которой можно доказать выпуклость функции $f$ сведением к mincost-у.

**Задача:**
Дан массив $a$ длины $n$. Необходимо выбрать $k$ непересекающихся пар соседних элементов этого массива и закрыть их доминошками так, чтобы сумма всех оставшихся чисел была максимальна.


Давайте сначала заметим, что сумма всех оставшихся чисел — это сумма всех чисел минус сумма закрытых чисел, так что нам надо минимизировать сумму закрытых чисел. Во-вторых, можно заметить, что каждая доминошка закрывает ровно одну ячейку массива на четной позиции и ровно одну на нечетной. Поэтому если мы рассмотрим двудольный граф, в одной доли которого будут четные индексы, а в другой — нечетные, и между индексами $i$ и $i + 1$ для всех $i$ будет проведено ребро веса $a_i + a_{i + 1}$, то нам нужно просто в этом двудольном графе найти паросочетание размера $k$ минимального веса.

Но ведь поиск минимального паросочетания можно легко свести к поиску потока минимального веса. Необходимо лишь ориентировать ребра паросочетания слева направо и поставить на них пропускную способность $1$, а также добавить сток и исток и провести из истока ребра нулевого веса и единичной пропускной способности во все вершины левой доли, а из всех вершин правой доли провести ребра нулевого веса и единичной пропускной способности в сток. Тогда минимальный вес потока величины $k$ в этом графе будет как раз таки равен ответу для $k$ доминошек для нашей изначальной задачи, а значит, мы можем решать ее при помощи лямбда-оптимизации.

### Неравенство четырехугольника

Если наша задача заключается в разбиении массива на $k$ подотрезков, то ее квадратичную динамику можно представить в таком виде:

$$dp[i][c] = \min \limits_{j < i} dp[j][c - 1] + w(j, i)$$

Тогда верно следующее утверждение:

**Предложение:**
Если функция $w$ удовлетворяет неравенству четырехугольника, то есть для любых $a \le b \le c \le d$ выполнено

$$w(a, c) + w(b, d) \le w(a, d) + w(b, c)$$

Тогда функция $f(c) = dp[n][c]$ является нестрого выпуклой (если неравенство в обратную сторону, то вогнутой).


**Доказательство:**
Полностью доказывать это утверждение мы не будем, потому что это весьма объемный труд. Ознакомиться с ним можно во второй части [этой статьи](https://cse.hkust.edu.hk/mjg_lib/bibs/DPSu/DPSu.Files/sdarticle_204.pdf).

Если коротко, то доказательство состоит из последовательной проверки следующих фактов:

1. Если есть два оптимальных разбиения на отрезки $P$ и $Q$, и в $P$ один из отрезков — это $[a, b]$, а в $Q$ один из отрезков — это $[c, d]$, и выполняется $a \le c \le d \le b$, то эти два отрезка можно поменять на $[a, d]$ и $[c, b]$, а также поменять местами концы путей $P$ и $Q$ после точек $d$ и $b$, и полученные пути тоже будут оптимальными. Этот факт понять весьма легко. Получившиеся пути будут тоже являться некоторыми разбиениями, однако сумма их длин будет не больше, чем сумма длин изначальных путей, по неравенству четырехугольника. Тогда если изначальные были оптимальными, то на самом деле достигается равенство, и два новых пути также являются оптимальными.

2. Для любых двух оптимальных разбиений, таких что в одном из них хотя бы на $m$ отрезков больше, чем в другом, есть такая пара отрезков, как в прошлом факте, что если отрезок $[a, b]$ является $i$-м в первом разбиении, то отрезок $[c, d]$ является $i+m$-м во втором разбиении. Доказывается этот факт индукцией по обеим длинам путей.

3. И тогда совмещая два предыдущих факта, можно понять, что если есть два оптимальных пути длины $k_1$ и $k_2$, то для любого числа $k$ между ними есть оптимальный путь длины $k$.

4. Из чего в конечном итоге и следует, что $f(c)$ является выпуклой.



Получается, что если функция стоимости отрезка в задаче удовлетворяет неравенству четырехугольника, то функция ответа является выпуклой, а значит, в этой задаче можно использовать лямбда-оптимизацию. Давайте докажем, что наша изначальная задача про сумму квадратов сумм чисел в отрезках разбиения удовлетворяет этому условию, а значит, наше решение лямбда-оптимизацией было корректно.

В нашем случае $w(l, r) = (a_{l + 1} + a_{l + 2} + \ldots + a_r)^2$. Обозначим сумму на $(a, b]$ за $A$, сумму на $(b, c]$ за $B$ и сумму на $(c, d]$ за $C$. Тогда нам нужно доказать, что

$$(A + B)^2 + (B + C)^2 \le (A + B + C)^2 + B^2$$

Если раскрыть скобки и сократить равные слагаемые, получится

$$0 \le 2AC$$

что верно, потому что все числа неотрицательные. Тем самым мы доказали, что нашу задачу можно было решать лямбда-оптимизацией!

Несложно проверить, что неравенство четырехугольника выполняется и для многих других задач на лямбда-оптимизацию. К примеру, накрыть точки на прямой $k$ отрезками, минимизировав сумму квадратов их длин, или разбить массив на $k$ подотрезков, максимизировав сумму побитового <<или>> подотрезков, и так далее. Кажется, большинство задач на лямбда-оптимизацию удовлетворяют этому свойству.

**Замечание:**
Если для любых $l$ и $r$ верно, что

$$w(l, r) + w(l + 1, r + 1) \le w(l, r + 1) + w(l + 1, r)$$

то выполняется неравенство четырехугольника.


**Доказательство:**
То есть если вам тяжело доказать неравенство четырехугольника в общем виде, можно доказать его только такой частный случай.

Почему это верно? Это достаточно техническое утверждение, давайте разберем план доказательства. Просуммировав эти неравенства для $(l, r)$, $(l + 1, r)$, $(l, r + 1)$ и $(l + 1, r + 1)$ и сократив одинаковые члены, можно получить неравенство

$$w(l, r) + w(l + 2, r + 2) \le w(l, r + 2) + w(l + 2, r)$$

Продолжая эту операцию, можно получить неравенство

$$w(l, r) + w(l + k, r + k) \le w(l, r + k) + w(l + k, r)$$

Для любого $k$. После чего индукцией по $d - a$ в изначальном неравенстве четырехугольника можно применить наш факт для $k = min(d - c, b - a)$, а затем воспользоваться предположением индукции.


#### Применение 1D1D-оптимизации

И на самом деле это полезно не только с теоретической точки зрения! Наша формула для одномерной динамики при фиксированной лямбде выглядит следующим образом:

$$dp[i] = \min \limits_{j < i} dp[j] + w'(j, i)$$

где $w'(j, i) = w(j, i) + \lambda$. Если $w$ удовлетворяет неравенству четырехугольника, то и $w'$ тоже ему удовлетворяет, потому что лямбды в неравенстве сокращаются. А одномерную динамику такого вида, в которой функция пересчета удовлетворяет неравенству четырехугольника, можно считать при помощи [1D1D-оптимизации](https://codeforces.com/blog/entry/93772). Поэтому на самом деле комбинация лямбда-оптимизация + 1D1D-оптимизация решает практически все задачи на лямбда-оптимизацию.

#### Восстановление ответа при условии неравенства четырехугольника

При доказательстве выпуклости функции $f$ одним из промежуточных фактов мы доказали следующее утверждение:

**Лемма:**
Для любых двух оптимальных разбиений, таких что в одном из них хотя бы на $m$ отрезков больше, чем в другом, есть такая пара вложенных отрезков $[a, b]$ и $[c, d]$, что если отрезок $[a, b]$ является $i$-м в первом разбиении, то отрезок $[c, d]$ является $i+m$-м во втором разбиении.


И в таком случае мы поняли, что ребра $[a, b]$ и $[c, d]$ можно заменить на $[a, d]$ и $[c, b]$, а также поменять концы двух путей, и тогда если один путь имел длину $k_1$, а другой — $k_2$, то итоговые пути будут иметь длины $k_2 + m$ и $k_1 - m$. То есть если взять правильное $m$, то мы можем построить путь любой длины между $k_1$ и $k_2$.

Тогда при условии верности неравенства четырехугольника можно восстанавливать ответ следующим способом. Пусть мы уже нашли оптимальную лямбду, и для нее минимальное количество подотрезков в оптимальном разбиении — это $l$, а максимальное — это $r$. Тогда восстановим ответы для $l$ и $r$ (максимизируя/минимизируя количество подотрезков разбиения при равной стоимости), после чего используя эти два пути восстановим путь длины $k$ ($l \le k \le r$). Необходимо просто идти по отрезкам разбиения двумя указателями и проверять, не нашли ли мы такую пару вложенных отрезков, что индекс первого в первом разбиении на $k - l$ меньше, чем индекс второго во втором разбиении. И в этом случае нужно взять префикс первого пути, суффикс второго и объединить их ребром. Вот и получился оптимальный ответ для разбиения на $k$ подотрезков.

### Задача линейного программирования

Для прочтения этого раздела не обязательно быть знакомым с задачей линейного программирования, но это точно сильно облегчит понимание.

В данном разделе мы рассмотрим максимально общую задачу, к которой можно сводить изначальную, чтобы ответ был выпуклым.

На самом деле нам подходят ограниченные задачи целочисленного линейного программирования, в которых переход к вещественными числам не добавляет новых базовых решений. Давайте разбираться, что все это значит.

**Определение:**
Сформулируем задачу линейного программирования. Есть набор переменных $x_1$, $x_2$, $\ldots$, $x_n$. Имеется набор $m$ условий:

$a_{i, 1} x_{i, 1} + a_{i, 2} x_{i, 2} + \ldots + a_{i, j_i} x_{i, j_i} \le b_i$

А также линейная функция $f(x_1, x_2, \ldots, x_n) = c_1 x_1 + c_2 x_2 + \ldots + c_n x_n$. Необходимо найти значения переменных $x_i$, которые удовлетворяют всем условиям и максимизируют (или минимизируют) функцию $f$.

Если $x_i$ должны быть целыми, то эта задача называется задачей **целочисленного** линейного программирования.



Обозначим за $b$ вектор, состоящий из $b_i$. Обозначим за $g_r(b)$ тот самый максимум функции $f$ при данных ограничениях для задачи линейного программирования, а за $g(b)$ обозначим максимум функции $f$ при данных ограничениях для задачи целочисленного линейного программирования.

Без труда можно понять, что $g_r(b)$ является вогнутой функцией. Давайте возьмем $b_1$ и $b_2$, а также $0 \le t \le 1$. Пусть $x^1$ максимизирует $f$ при ограничениях $b_1$, а $x^2$ максимизирует $f$ при ограничениях $b_2$. $x = t x^1 + (1 - t) x^2$. Тогда раз $x^1$ и $x^2$ удовлетворяют всем ограничениям, то и $x$ как их линейная комбинация с суммой коэффициентов $1$ удовлетворяет им. А также $f(x) = t f(x^1) + (1 - t)f(x^2) = tg_r(b_1) + (1-t)g_r(b_2) \le g_r(tb_1 + (1-t)b_2)$, потому что $f$ — линейная функция. Что и требовалось доказать.

Однако из того, что $g_r$ является вогнутой, не следует, что $g$ обязательно является вогнутой. Ведь может так получиться, что $g(b) \le g_r(b)$. Однако иногда все таки верно, что $g(b) = g_r(b)$. И тогда мы как раз можем с уверенностью сказать, что функция $g$ тоже выпуклая. А именно, это выполняется, если любое $x$, удовлетворяющее условиям, можно выразить как афинную комбинацию (линейную комбинацию с суммой коэффициентов, равной единице) каких-то целочисленных $x^j$, удовлетворяющих условиям. В таком случае мы можем записать, что $f(x)$ равно линейной комбинации $f(x^1)$ с суммой коэффициентов $1$ (то есть некоторой средневзвешенной сумме), поэтому $f(x) \le \max_j f(x^j) \le g(b)$. Что и требовалось доказать.

Если область $\mathbb{R}^n$, где верны все условия, является ограниченной (какой-то $n$-мерный многогранник), то любая точка внутри него является какой-то афинной комбинацией вершин этого многогранника. Поэтому на самом деле нам достаточно проверить, что все вершины являются целочисленными. Иными словами, если некоторые неравенства обращаются в равенство таким образом, что все переменные можно однозначно восстановить.

К примеру, можно без труда доказать, что нашу задачу про сумму квадратов сумм элементов в подотрезках можно выразить как задачу линейного программирования, а потом проверить, что все вершины являются целочисленными. Введем $n + C_n^2$ переменных $x_1$, $x_2$, $\ldots$, $x_n$, $d_{i, j}$ ($1 \le i < j \le n$), где $x_i$ — номер отрезка, которому принадлежит $i$-й элемент, а $d_{i j}$ говорит о том, лежат ли $i$-й и $j$-й элементы в одной группе. Мы введем следующие ограничения:

$$1 \le x_1 \le x_2 \le \ldots \le x_n \le k$$

$$0 \le d_{i j} \le 1$$

$$1 - (x_j - x_i) \le d_{i j}$$

И тогда линейная фунция $f(x_i, d_{ij}) = \sum_{i = 1}^{n} a_i^2 + \sum_{i < j} 2 d_{i j} \cdot a_i \cdot a_j$ как раз таки и будет равна ответу на нашу задачу, если мы рассматриваем целочисленную версию задачи линейного программирования. На самом деле $d_{i j}$ может быть равно единице даже если $x_i$ и $x_j$ лежат в разных блоках, но от этого $f$ только увеличится, поэтому это не точка оптимума. Теперь остается лишь проверить, что если некоторые неравенства обратились в равенства так, что все переменные восстанавливаются однозначно, то решение будет целочисленным. Это останется читателю в качестве упражнения.

Также можно заметить, что на самом деле задача поиска mincost-k-flow также является частным случаем этого метода. Можно заметить, что все ограничения в потоках выражаются в виде таких неравенств, а стоимость потока — линейная функция от них. Остается лишь вспомнить, что мы знаем, что если пропускные способности всех ребер целые, то и в оптимальном ответе по всем ребрам текут целые величины потоков.


<!---TODO: альтернативный взгляд на $f(k) + kx$ через касательные к точкам на плоскости.--->

## Источники


- [Хорошая статья на английском языке](https://mamnoonsiam.github.io/posts/attack-on-aliens.html)
- [Восстановление ответа в лямбда-оптимизации](https://codeforces.com/blog/entry/92842)
- [Собрание задач и источников](https://usaco.guide/adv/lagrange)
- [О сведении к задаче линейного программирования](https://codeforces.com/blog/entry/68778?#comment-700891)
- [Связь с методом множителей Лагранжа](https://codeforces.com/blog/entry/68778?#comment-531862)
- [Альтернативный способ на понимание необходимости выпуклости фукнции $f$](https://codeforces.com/blog/entry/69625?#comment-541307)
- [Статья 1995 года, в которой доказывается выпуклость при условии неравенства четырехугольника](https://cse.hkust.edu.hk/mjg_lib/bibs/DPSu/DPSu.Files/sdarticle_204.pdf)
- [Статья 1992 года, использующая схожую идею](https://web.archive.org/web/20210511084902/https://www.osti.gov/servlets/purl/10146169)


## Задачи


- [Базовый пример задачи](https://oj.uz/problem/view/NOI19_feast)
- [Базовый пример задачи 2](https://codeforces.com/contest/1279/problem/F)
- [Решить задачу для бóльшими ограничениями лямбда-оптимизацией](https://codeforces.com/contest/739/problem/E)
[Применение лямбда-оптимизации не в задаче оптимизации динамики](https://open.kattis.com/problems/blazingnewtrails)
- [Еще одно применение лямбда-оптимизации не в задаче оптимизации динамики](http://81.4.170.42:8980/training/#/task/Tennis/statement)
- [USACO 2017 January Contest, Platinum, Problem 2. Building a Tall Barn](http://www.usaco.org/index.php?page=viewproblem2&cpid=697)
- ОСТОРОЖНО!!! СПОЙЛЕРЫ К IOI!!! [Задача с IOI2016](https://oj.uz/problem/view/IOI16_aliens)

