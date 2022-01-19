---
title: "Неравенство четырехугольника"
date: 2022-01-19T09:44:11+03:00
draft: false
---

<center> <h2>Неравенство четырехугольника</h2> </center>
<br/>
<br/>


## Формулировки решаемых задач

Неравенство четырехугольника — это условие на стоимость подотрезков при пересчете динамики, которое используется во многих оптимизациях динамики ([оптимизация Кнута — Яо](/posts/knuth_yao_optimization), [оптимизация разделяй-и-властвуй](/posts/divide_and_conquer_optimization), 1D/1D-оптимизация, [лямбда-оптимизация](/posts/lambda_optimization)). Чтобы не повторять одно и то же во всех оптимизациях, информация по неравенству четырехугольника вынесена сюда, однако обратите внимание, что нет никакого смысла читать эту статью просто так. Стоит обратить внимание на нее внимание только в том случае, если вы изучаете какую-то конкретную оптимизацию динамики, и в ходе изучения вам понадобилось неравенство четырехугольника.

В задачах на оптимизации динамики часто есть некоторая функция стоимости $w(i, j)$, от которой зависят значения динамики. Динамика пересчитывается через предыдущие значения, но дополнительно добавляется значение этой функции $w$. К примеру, в задаче разбиения массива на $k$ подотрезков оптимальным образом $w$ как раз является стоимостью подотрезка разбиения. В динамике по подотрезкам это может быть стоимость объединения двух подотрезков в один. В одномерной динамике это может быть стоимость перехода от одной точки к следующей и т.д.

В этой статье много разных доказательств, но большинство из них имеют чисто технический характер, поэтому их изучение совершенно необязательно для понимания сути происходящего.

Примеры динамик, в которых может встретиться функция стоимости:


- Задача оптимального разбиения массива на $k$ подотрезков:
    $$dp[ki][ni] = \min_{i < ni} dp[ki - 1][i] + w(i, ni)$$

- Динамика по подотрезкам:
    $$dp[l][r] = w(l, r) + \min_{l < i < r} dp[l][i] + dp[i][r]$$

- Одномерная динамика:
    $$dp[i] = \min_{j < i} dp[j] + w(j, i)$$


Везде далее мы будем рассматривать конкретные примеры задач, однако вы можете выбрать свою любимую задачу и пытаться доказывать условия для нее.

**Пример:** **Задача оптимального разбиения массива на $k$ подотрезков**

Дан массив неотрицательных чисел $a$ длины $n$. Назовем мощностью подотрезка квадрат суммы его элементов. Необходимо разбить массив $a$ на $k$ подотрезков таким образом, чтобы минимизировать сумму мощностей подотрезков разбиения. Иными словами, выделить такие $k - 1$ чисел (границы подотрезков разбиения) $i_1$, $i_2$, $\ldots$, $i_{k - 1}$, чтобы

$$(a_1 + a_2 + \ldots + a_{i_1 - 1})^2 + (a_{i_1} + a_{i_1 + 1} + \ldots + a_{i_2 - 1})^2 + \ldots + (a_{i_{k - 1}} + \ldots + a_{n})^2$$

было минимально.

В нашем случае $dp[ki][ni]$ — минимальная стоимость разбиения первых $n$ элементов на $ki$ подотрезков, а $w(i, ni)$ — мощность подотрезка $(i, ni]$, то есть $(a_{i + 1} + a_{i + 2} + \ldots + a_{ni})^2$. Далее везде мы будем подразумевать, что $w(i, ni)$ можно считать за $O(1)$. Для этого необходимо заранее посчитать префиксные суммы массива $a$ и выразить мощность подотрезка через квадрат разности префиксных сумм его концов.


**Пример:** **Динамика по подотрезкам**

Есть последовательность из $n$ кучек камней. В $i$-й из них $a_i$ камней. За одну операцию можно объединить две соседние кучки камней в одну, и если в них суммарно было $x$ камней, то эта операция стоит $x$ монет. Необходимо объединить все кучки в одну, заплатив минимальное количество монет.

Если мы обозначим стоимость объединения всех кучек с $l$-й по $r$-ю (левая граница включительно, а правая не включительно) за $dp[l][r]$, то формула пересчета динамики будет именно такой, какой нужно:

$$dp[l][r] = w(l, r) + \min_{l < i < r} dp[l][i] + dp[i][r]$$

где $w(l, r)$ — это сумма размеров кучек на полуинтервале от $l$ до $r$. Ее можно легко высчитывать при помощи префиксных сумм. Ответ, соответственно, будет храниться в $dp[0][n]$.


**Пример:** **Одномерная динамика**

Мы будем рассматривать точно такую же задачу, как и в примере задачи оптимального разбиения массива на $k$ подотрезков, однако у нас не будет ограничения на количество подотрезков. Вместо этого мы будем штрафовать за каждый отрезок, который мы возьмем. То есть функция $w$ немного поменяется: $w(i, ni) = (a_{i + 1} + a_{i + 2} + \ldots + a_{ni})^2 + c$, где $c$ — некоторая константа. Мы добавляем эту константу, потому что несложно доказать, что без нее всегда оптимально разбить массив на $n$ подотрезков длины $1$.


## Формулировка неравенства четырехугольника

Чтобы использовать в подобных задачах различные оптимизации динамики, мы хотим, чтобы выполнялись какие-то приятные свойства на $dp$ (к примеру, монотонность точки разреза или выпуклость), однако чаще всего доказывать эти свойства про динамику сложно, потому что она как-то сложно определена через себя. В этом нам как раз и помогает неравенство четырехугольника. Это некоторое условие на $w$, из которого следуют разные условия на $dp$, которые нам нужны. Однако так как $w$ чаще всего определяется как-то очень легко из условия задачи, то проверить неравенство четырехугольника для $w$ не составляет труда. Таким образом, неравенство четырехугольника для функции стоимости — простой способ доказывать корректность своего решения при помощи какой-то оптимизации динамики.

**Замечание:**
Обратите внимание, что во всех контекстах, которые мы будем рассматривать, неравенство четырехугольника будет лишь достаточным условием для использования какой-то оптимизации динамики, но не необходимым. То есть если неравенство четырехугольника выполняется, то мы точно можем применить необходимую оптимизацию динамики, однако даже если неравенство четырехугольника не выполняется, это не значит, что соответствующую оптимизацию применять нельзя.


Давайте перейдем непосредственно к формулировке неравенства четырехугольника.

**Неравенство четырехугольника:** Говорят, что функция $w$ удовлетворяет неравенству четырехугольника, если для любых индексов $a \le b \le c \le d$ выполнено неравенство:

$$w(a, c) + w(b, d) \le w(a, d) + w(b, c)$$

 Иными словами, если один отрезок лежит строго внутри другого, то поменяв их правые границы местами, суммарная стоимость отрезков не увеличится.

**Замечание:**
Это условие иногда еще называется **вогнутым** неравенством четырехугольника. Оно достаточно для задачи минимизации. Если же знак в неравенстве стоит в другую сторону, то это **выпуклое** неравенство четырехугольника, и оно, соответственно, является достаточным для задачи максимизации (представьте, что мы просто заменили функцию $w$ на $-w$, тогда мы перейдем к задаче минимизации с вогнутым неравенством четырехугольника). Если выпуклость/вогнутость явно не указана, то везде далее под <<неравенством четырехугольника>> мы подразумеваем именно вогнутое неравенство четырехугольника. Для задач максимизации все аналогично обобщается для выпуклого неравенства четырехугольника.

Также иногда эти два вида неравенств четырехугольника называют прямым и обратным.


**Пример:**
Давайте докажем неравенство четырехугольника для нашей задачи оптимального разбиения массива на $k$ подотрезков.

В нашем случае $w$ — это сумма квадратов элементов на отрезке. Давайте обозначим за $x$ сумму элементов на $[a, b)$, за $y$ сумму элементов на $[b, c)$ и за $z$ сумму элементов на $[c, d)$. Тогда неравенство четырехугольника в нашем случае можно переписать как

$$(x + y)^2 + (y + z)^2 \le (x + y + z)^2 + y^2$$

Раскрыв скобки, мы получим

$$x^2 + 2xy + y^2 + y^2 + 2yz + z^2 \le x^2 + 2xy + y^2 + 2yz + z^2 + 2xz + y^2$$

Сокращая одинаковые слагаемые с обеих сторон, мы получим

$$0 \le 2xz$$

Что верно, потому что оба числа $x$ и $z$ неотрицательны (здесь мы как раз пользуемся тем, что элементы изначального массива были неотрицательны).



**Пример:**
Теперь докажем неравенство четырехугольника для нашей динамики по подотрезкам.

Здесь $w$ — это просто сумма элементов на отрезке. На самом деле в этом случае в неравенстве достигается равенство, что несложно проверить, опять же обозначив за $x$ сумму элементов на $[a, b)$, за $y$ сумму элементов на $[b, c)$ и за $z$ сумму элементов на $[c, d)$. Тогда обе части неравенства равны $x + 2y + z$.


**Пример:**
И наконец докажем неравенство четырехугольника для нашей одномерной динамики. Но ведь в этом случае функция $w$ — это та же функция из задачи об оптимальном разбиении на $k$ подотрезков, только к ней добавили константу. Эта константа добавится дважды к обеим частям неравенства, так что оно останется верным.


На самом деле из этого можно сформулировать более общее утверждение сохранения неравенства четырехугольника:

**Предложение:** **Сохранение неравенства четырехугольника при линейных преобразованиях**

Если функция $w$ удовлетворяет неравенству четырехугольника, то функция $u(x, y) := a \cdot w(x, y) + b$ тоже удовлетворяет неравенству четырехугольника, где $a$ — неотрицательное число, а $b$ — любое.


Это утверждение несложно проверить, явно подставив определение $u$ в условие неравенства четырехугольника.

**Замечание:**
Обратите внимание, что очень важно, чтобы в предыдущем утверждении число $a$ было неотрицательным. В противном случае знак в неравенстве поменяется на противоположный.


**Предложение:** **Сумма сохраняет неравенство четырехугольника**

Если функции $w$ и $u$ удовлетворяют неравенству четырехугольника, то и $v(x, y) := w(x, y) + u(x, y)$ тоже удовлетворяет неравенству четырехугольника.


Это очевидно. Достаточно лишь сложить два неравенства.

## Ослабленное неравенство четырехугольника

Кроме того есть **ослабленное неравенство четырехугольника**, которое говорит, что при $a < c$ верно

$$w(a, c) + w(a + 1, c + 1) \le w(a, c + 1) + w(a + 1, c)$$

Это то же самое неравенство четырехугольника, но $b = a + 1$ и $d = c + 1$.

Очевидно, что ослабленное неравенство четырехугольника является частным случаем обычного неравенства четырехугольника. Однако верно следующее утверждение:

**Предложение:**
Если функция $w$ удовлетворяет ослабленному неравенству четырехугольника, то она удовлетворяет и обычному неравенству четырехугольника.


**Доказательство:**
Доказательство будет состоять из нескольких шагов, в которых мы постепенно будем доказывать неравенство четырехугольника для все более сложных четверок точек.

Сначала докажем неравенство четырехугольника, если $a = b$. В таком случае неравенство вырождается в

$$w(a, c) + w(a, d) \le w(a, d) + w(a, c)$$

Слева и справа написано одно и то же, так что на самом деле достигается даже равенство. Аналогичное верно, если $c = d$.

Далее нам удобнее будет переписать наше неравенство в немного другом виде:

$w(a, c) - w(b, c) \le w(a, d) - w(b, d)$

Теперь докажем это, если $b = a + 1$. Доказывать будем по индукции по $d - c$. Базовый случай, когда $c = d$, мы уже доказали. Теперь сделаем переход. Мы хотим доказать, что $w(a, c) - w(a + 1, c) \le w(a, d) - w(a + 1, d)$. Это верно, потому что

$$w(a, c) - w(a + 1, c) \le w(a, d - 1) - w(a + 1, d - 1) \le w(a, d) - w(a + 1, d)$$

Первое неравенство верно по предположению индукции, а второе — это ослабленное неравенство четырехугольника.

Таким образом мы доказали неравенство четырехугольника при $b = a + 1$. Теперь аналогично докажем для любого $b$. Опять перепишем неравенство в более удобном виде:

$$w(b, d) - w(b, c) \le w(a, d) - w(a, c)$$

Доказывать будем по индукции по $b - a$. База $b - a = 1$ уже доказана. Остался переход. Мы хотим доказать, что $w(b, d) - w(b, c) \le w(a, d) - w(a, c)$. Это верно, потому что

$$w(b, d) - w(b, c) \le w(a + 1, d) - w(a + 1, c) \le w(a, d) - w(a, c)$$

Первое неравенство верно по предположению индукции, а второе — это неравенство четырехугольника для $b = a + 1$, которое мы уже доказали.

Таким образом мы доказали, что из ослабленного неравенства четырехугольника следует неравенство четырехугольника для любых $a \le b \le c \le d$.


Мы поняли, что вместо того, чтобы проверять стандартное неравенство четырехугольника, можно проверять ослабленное. В некоторых задачах это может упростить доказательство. К примеру, в этой:

**Задача:**
На дороге расставлены $n$ домов. Необходимо построить $k$ магазинов так, чтобы суммарное расстояние от каждого дома до ближайшего магазина было минимально.



## Полезные следствия неравенства четырехугольника


**Обозначение:**
Обозначим за $opt(r)$ самую левую точку $l$, которая минимизирует $w(l, r)$, то есть $w(opt(r), r) \le w(i, r)$ для любого $i < r$.


**Замечание:**
Брать самую левую среди оптимальных точек $l$ необязательно. Можно взять самую правую, и все утверждения дальше все еще будут верны.


**Предложение:** **Монотонность оптимума функции, удовлетворяющей неравенству четырехугольника**

Если $w$ удовлетворяет неравенству четырехугольника, то $opt(r) \le opt(r + 1)$ для любого $r$. Иными словами, функция $opt$ не убывает.


**Доказательство:**

Предположим обратное. Пусть $opt(r) > opt(r + 1)$. Тогда $w(opt(r), r + 1) \ge w(opt(r + 1), r + 1)$ и $w(opt(r + 1), r) > w(opt(r), r)$. Во втором неравенстве знак строгий, потому что если бы достигалось равенство, то мы выбрали бы в качестве $opt(r)$ позицию $opt(r + 1)$, потому что она левее.

Теперь если просуммировать эти два неравенства, то мы получим

$$w(opt(r), r + 1) + w(opt(r + 1), r) > w(opt(r + 1), r + 1) + w(opt(r), r)$$

Что противоречит неравенству четырехугольника для $a = opt(r + 1)$, $b = opt(r)$, $c = r$, $d = r + 1$.


**Предложение:** **Неравенство четырехугольника для каждого слоя динамики оптимального разбиения массива на $k$ подотрезков**

Для фиксированного $ki$ обозначим $f_{ki}(l, r) := dp[ki][l] + w(l, r)$, где $dp$ — динамика оптимального разбиения массива на $k$ подотрезков. Тогда если $w$ удовлетворяет неравенству четырехугольника, то $f_{ki}$ тоже ему удовлетворяет.


**Доказательство:**
 Достаточно расписать необходимое неравенство:

 $$f_{ki}(a, c) + f_{ki}(b, d) = dp[ki][a] + w(a, c) + dp[ki][b] + w(b, d) \le $$

 $$ \le dp[ki][a] + w(a, d) + dp[ki][b] + w(b, c) = f_{ki}(a, d) + f_{ki}(b, c)$$


**Следствие:** **Условие монотонности точки разреза для динамики оптимального разбиения массива на $k$ подотрезков**

Если $w$ в динамике оптимального разбиения массива на $k$ подотрезков удовлетворяет неравенству четырехугольника, то $opt[ki][ni] \le opt[ki][ni + 1]$ для любых $ki$ и $ni$, где $opt[ki][ni]$ — это оптимальная точка пересчета для $dp[ki][ni]$ ($dp[ki][ni] = dp[ki - 1][opt[ki][ni]] + w(opt[ki][ni], ni)$). Иными словами, $opt[ki][ni]$ не убывает по $ni$. А значит, такую задачу можно решать при помощи [оптимизации разделяй-и-властвуй](/posts/divide_and_conquer_optimization).


**Доказательство:**
Это является прямым следствием предыдущих двух утверждений. Во-первых, если $w$ удовлетворяет неравенству четырехугольника, то ему удовлетворяет и $f_{ki}$, а $opt[ki][ni]$ — это как раз таки функция $opt$ для $f_{ki}$, так что мы знаем, что она не убывает.


**Предложение:** **Условие монотонности точки разреза по второй координате для динамики оптимального разбиения массива на $k$ подотрезков**

Если $w$ в динамике оптимального разбиения массива на $k$ подотрезков удовлетворяет неравенству четырехугольника, то $opt[ki][ni] \le opt[ki + 1][ni]$ для любых $ki$ и $ni$. А значит, это и предыдущее условие гарантируют, что такую задачу можно решать при помощи [оптимизации Кнута — Яо](/posts/knuth_yao_optimization).


**Доказательство:**
Пусть это не так. То есть $opt[ki][ni] > opt[ki + 1][ni]$ для каких-то $ki$ и $ni$. Посмотрим на оптимальные разбиения на $ki$ и $ki + 1$ подотрезков. Пусть оптимальное разбиение на $ki$ подотрезков — это $[a_{ki}, a_{ki-1})$, $[a_{ki-1}, a_{ki-2})$, $\ldots$, $[a_2, a_1)$, $[a_1, ni)$, где $a_{ki} = 0$ и $a_1 = opt[ki][ni]$. Аналогично, оптимальное разбиение на $ki + 1$ подотрезков — это $[b_{ki + 1}, b_{ki})$, $[b_{ki}, b_{ki-1})$, $\ldots$, $[b_2, b_1)$, $[b_1, ni)$, где $b_{ki + 1} = 0$ и $b_1 = opt[ki + 1][ni]$.

Мы предположили, что $a_1 = opt[ki][ni] > opt[ki + 1][ni] = b_1$. При этом $a_{ki} = 0 = b_{ki + 1} < b_{ki}$. Давайте найдем самый большой индекс $i$, такой что $a_i \ge b_i$. То есть $a_i \ge b_i$ и $a_{i + 1} < b_{i + 1}$. Мы знаем, что $i$-й с конца подотрезок одного разбиения — это $[a_{i + 1}, a_i)$, а $i$-й с конца подотрезок другого разбиения — это $[b_{i + 1}, b_i)$. При этом мы знаем, что второй отрезок лежит строго внутри первого. Мы знаем, что в таком случае мы можем поменять местами правые концы отрезков и не увеличить суммарную стоимость. То есть мы можем построить новое разбиение на $ki$ подотрезков: $[a_{ki}, a_{ki - 1})$, $[a_{ki - 1}, a_{ki - 2})$, $\ldots$, $[a_{i + 2}, a_{i + 1})$, $[a_{i + 1}, b_i)$, $[b_i, b_{i - 1})$, $\ldots$, $[b_2, b_1)$, $[b_1, ni)$, а также мы можем построить новое разбиение на $ki + 1$ подотрезков: $[b_{ki + 1}, b_{ki})$, $[b_{ki}, b_{ki-1})$, $\ldots$, $[b_{i + 2}, b_{i + 1})$, $[b_{i + 1}, a_i)$, $[a_i, a_{i - 1})$, $\ldots$, $[a_2, a_1)$, $[a_1, ni)$. Заметим, что сумма стоимостей двух таких разбиений не больше, чем сумма двух оптимальных разбиений, потому что почти все отрезки остались такими, какими были, только два вложенных отрезка мы заменили на два пересекающихся, что могло только уменьшить их стоимость. Однако так как старые разбиения были оптимальны по стоимости, то новые не могут иметь меньшую стоимость. Значит, их стоимости в точности совпадают со стоимостями старых разбиений. Однако мы выбрали $opt[ki][ni]$ так, чтобы это был самый маленький индекс, через который можно оптимально пересчитаться. Но получается, что через $b_1 = opt[ki + 1][ni]$ тоже можно пересчитаться оптимальным образом для $ki$ подотрезков. Но ведь мы предполагали, что $opt[ki + 1][ni] < opt[ki][ni]$. Получаем противоречие.


## Условие монотонности по включению

**Условие монотонности по включению:** Говорят, что функция $w$ монотонна (по включению), если для любых индексов $a \le b \le c \le d$ выполнено неравенство:

$$w(b, c) \le w(a, d)$$

То есть если один отрезок содержит другой, то у большего отрезка стоимость должна быть не меньше, чем у меньшего.

Это условие интуитивно еще более  логично, чем неравенство четырехугольника, так что почти всегда тоже выполняется.


Среди самых первых свойств неравенства четырехугольника мы доказали, что прибавление константы и умножение на константу сохраняют свойство неравенства четырехугольника. На самом деле, если выполнено еще и условие монотонности по включению, то можно обобщить данное утверждение:

**Предложение:** **Сохранение неравенства четырехугольника и монотонности по включению при выпуклых преобразованиях**

Если функция $w$ удовлетворяет неравенству четырехугольника и условию монотонности по включению, то и функция $u(x, y) := f(w(x, y))$ тоже удовлетворяет неравенству четырехугольника и условию монотонности по включению, где $f$ — неубывающая выпуклая функция.


**Доказательство:**
То, что монотонность по включению сохраняется, — очевидно, потому что функция $f$ не убывает.

Теперь докажем, что сохраняется неравенство четырехугольника. Мы знаем, что неравенство четырехугольника выполнено для $w$, то есть $w(a, c) + w(b, d) \le w(b, c) + w(a, d)$. Кроме того мы знаем, что выполнено условие монотонности по включению, поэтому $w(b, c) \le w(a, c), w(b, d) \le w(a, d)$. Для простоты обозначим $x := w(a, c)$, $y := w(b, d)$, $z := w(b, c)$, $t := w(a, d)$. Тогда мы знаем, что $x + y \le z + t$ и $z \le x, y \le t$. Мы же хотим доказать, что $f(x) + f(y) \le f(z) + f(t)$. При этом мы знаем, что функция $f$ не убывает, поэтому, $f(z) \le f(x), f(y) \le f(t)$. Осталось заметить, что

$$f(x) - f(z) \le f(t + z - y) - f(z) \le f(t) - f(y)$$

Первое неравенство верно, потому что $x \le t + z - y$, и при этом функция $f$ не убывает. Второе неравенство верно из-за свойств выпуклости. Давайте обозначим $g_{\alpha}(x) := f(x + \alpha) - f(x)$ для какой-то неотрицательной константы $\alpha$. Тогда мы знаем, что если функция $f$ выпуклая, то $g_{\alpha}$ не убывает. Если теперь переписать второе неравенство через функцию $g$, то оно становится очевидным:

$$g_{t - y}(z) \le g_{t - y}(y)$$

Это верно, потому что $z \le y$. Что и требовалось доказать.


**Пример:**
К примеру, из этого следует, что раз функция, равная сумме на отрезке, удовлетворяет неравенству четырехугольника, то и квадрат суммы элементов на отрезке тоже удовлетворяет неравенству четырехугольника.


**Предложение:** **Неравенство четырехугольника для динамики по подотрезкам**

Если $w$ из динамики по подотрезкам удовлетворяет неравенству четырехугольника и условию монотонности по включению, то $dp[l][r]$ тоже удовлетворяет неравенству четырехугольника.


**Доказательство:**
Мы хотим доказать, что $dp[a][d] + dp[b][c] \ge dp[a][c] + dp[b][d]$. Если $a = b$ или $c = d$, то легко проверить, что это неравенство обращается в равенство. Далее будем считать, что $a < b \le c < d$.

Будем доказывать это по индукции по $d - a$. База: $d - a = 0$ уже доказана, потому что в этом случае $a = b$.

Теперь совершим индукционный переход. Посмотрим на $e = opt[a][d]$, то есть точку, через которую мы пересчитали значение $dp[a][d]$. Тогда $dp[a][d] = dp[a][e] + dp[e][d] + w(a, d)$. Есть три варианта, где эта точка расположена: на $(a, b]$, на $(b, c)$ или на $[c, d)$. Мы разберем первый и второй случаи, а третий симметричен первому.

Пусть $a < e \le b$. Тогда

$$dp[a][d] + dp[b][c] = dp[a][e] + dp[e][d] + w(a, d) + dp[b][c] \ge dp[a][e] + dp[e][c] + w(a, d) + dp[b][d] \ge$$

$$\ge dp[a][e] + dp[e][c] + w(a, c) + dp[b][d] \ge dp[a][c] + dp[b][d]$$

Первое неравенство верно по предположению индукции для точек $e$, $b$, $c$ и $d$. Второе неравенство верно по монотонности $w$ по включению. Третье неравенство верно, потому что $dp[a][e] + dp[e][c] + w(a, c)$ — это какой-то способ пересчитать динамику для подотрезка $[a, c)$, а $dp[a][c]$ — это оптимальный такой пересчет. Тем самым мы доказали необходимое неравенство четырехугольника.

Теперь рассмотрим второй вариант: $b < e < c$. В этом случае мы еще рассмотрим точку $f = opt[b][c]$. Не умаляя общности можно считать, что $f \le e$. Другой случай будет симметричен. Тогда мы можем написать цепочку неравенств:

$$dp[a][d] + dp[b][c] = dp[a][e] + dp[e][d] + w(a, d) + dp[b][f] + dp[f][c] + w(b, c) \ge $$

$$ \ge dp[a][f] + dp[e][d] + w(a, d) + dp[b][e] + dp[f][c] + w(b, c) \ge$$
$$ \ge dp[a][f] + dp[e][d] + w(a, c) + dp[b][e] + dp[f][c] + w(b, d) =$$

$$= (dp[a][f] + dp[f][c] + w(a, c)) + (dp[b][e] + dp[e][d] + w(b, d)) \ge dp[a][c] + dp[b][d]$$

Первое неравенство верно по предположению индукции для точек $a$, $b$, $f$ и $e$. Чтобы получить второе неравенство, мы просто применяем неравенство четырехугольника для $w$. И наконец третье неравенство использует то, что с левой стороны написаны какие-то способы пересчитать динамику для подотрезков $[a, c)$ и $[b, d)$, а $dp[a][c]$ и $dp[b][d]$ — это оптимальные такие пересчеты. Тем самым мы доказали необходимое неравенство четырехугольника.


**Замечание:**
Обратите внимание, что монотонностью $w$ по включению мы пользовались только в первом случае, но от нее нельзя избавиться.


**Предложение:** **Условие монотонности точки разреза по обеим координатам для динамики по подотрезкам**

Если $w$ из динамики по подотрезкам удовлетворяет неравенству четырехугольника и условию монотонности по включению, то $opt[l - 1][r] \le opt[l][r] \le opt[l][r + 1]$, где $opt[l][r]$ — оптимальная точка пересчета для динамики (то есть $dp[l][r] = dp[l][opt[l][r]] + dp[opt[l][r]][r] + w(l, r)$). А значит, такую задачу можно решать при помощи [оптимизации Кнута — Яо](/posts/knuth_yao_optimization).


**Доказательство:**
По предыдущему утверждению $dp[l][r]$ удовлетворяет неравенству четырехугольника. А когда мы доказывали условия монотонности точки разреза по каждой из координат для задачи оптимального разбиения на $k$ подотрезков, мы на самом деле пользовались как раз только тем, что динамика удовлетворяет неравенству четырехугольника, так что для динамики по подотрезкам доказательства аналогичны.



## Источники



- [Хорошая статья на codeforces про значение неравенства четырехугольника в оптимизациях динамики](https://codeforces.com/blog/entry/86306)

- [Статья Фрэнсис Яо, доказывающая условия для динамики по подотрезкам](https://cse.hkust.edu.hk/mjg_lib/bibs/DPSu/DPSu.Files/p429-yao.pdf)

- [Статья, в которой представлено доказательство достаточности неравенства четырехугольника для случая оптимального разбиения на $k$ подотрезков](https://cse.hkust.edu.hk/mjg_lib/bibs/DPSu/DPSu.Files/sdarticle_204.pdf)

- [Источник информации по разным оптимизациям динамики](https://codeforces.com/blog/entry/8219)


