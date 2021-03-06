# О рогаликах

## Вводное слово

Я давно хотел написать свой рогалик, еще когда это не было мейнстримом. 
Но, как-то ни времени, ни прям большого желания не возникало. Тут вдруг выдалось
немного времени и я решил попробовать сделать не просто рогалик, а рогалик за 
7 дней. В итоге, конечно же ничего достойного не получилось, так как не было, ни
плана, ни идеи, ни понимания что и как делать. Да, и времени - полных 7 дней
на реализацию игры не было. В итоге, получилось хоть что-то. В целом, разобрался
с библиотекой NCurses, реализовал некий алгоритм расчета видимости, реализовал
некие зачатки системы действий и поиск пути для врагов.

Для себя я понял, что написать быстро немного ~~~~~кода я еще способен - это
радует. В итоге я сделал всего ~20 коммитов, из которых лишь несколько имеют в 
себе что-то существенное.

Данная статья что-то типа посмертного анализа итогов работы некоторого
количества времени, которое я смог затратить на проЭкт. В будущем, надеюсь, 
смогу найти время и сделать еще один заход на 7drl.

## 7DRL

Олдскульные рогалики для их разработчиков зачастую становятся работой на
продолжительные отрезки времени, 7DRL же разрабатываются с прицелом на
законченность. Игра должна быть готова в течении недели и точка. Зачастую, такие
игры реализуют в себе какую-то одну конкретную идею или игровую механику. На 
профильных сайтах, обычно даже проводятся конкурсы таких игр.

Первой моей проблемой было то, что я задался первоначально сделать так сказать
некий безликий hack'n'slash рогалик. Сама суть была вполне себе понятна -- 
сделать хоть что-то, так как последнее время из под моих пальцев не выходило
ничего связанного с играми и это меня напрягало.

Таким образом, первоначальная идея была такой:

1. Игрок(@) путешествует по случайно сгенерированному миру.
2. Убивает мобов(Z).
3. Копит экспу.
4. ???
5. PROFIT!!!

В итоге, когда я понял, что из-за житейских обстоятельств, я за три дня сделал
всего один коммит, который, по сути, никак не влияет на саму игру, я понял, что
уже не успею этого реализовать.

Финальная идея стала такой:

1. Игрок(@) ходит по лесу
2. Убегает от мобов(Z)
3. Чем больше он убегает, тем больше текста он сможет прочесть, в конце.

Но тогда я еще этого не знал.

## Как деды завещали

Устав от связки CMake+Netbeans, которые я использую несколько последних лет,
я представил, что у меня на руках есть лишь make и nano и принялся кодить.
В дополнение, я еще представил, что у меня нет интернета и всю справку искал 
лишь через man-ы на локальной машине. И хочу вам сказать, что не умер от этого,
хотя был близок.

Автодополнение кода, поиск-и-замена, автоматический форматтер кода -- все это
развращает разработчика (хе-хе). Ты можешь себе позволить называть классы, 
функции длинными именами из трех слов, потому, что ты не пишешь эти три слова
каждый раз, когда тебе нужно обратиться к такому же длинному методу! Здесь же,
в черном окне 80х24 ты внезапно начинаешь ощущать себя очень неуютно.

В силу условий задачи, первым из практик хорошего тона, чем я пожертвовал
был запрет на глобальные переменные, а чтобы не огрести на последовательности 
создания объектов, я сразу же отказался от конструкторов/деструкторов и всего 
вспомогательного кода, который пишется для RAII подхода.

В итоге, для разных компонент программы появились пары функций типа
xxxInit/xxxCleanup, где xxx - название какой-нибудь подсистемы.

Таким образом, scrInit - настраивает и определяет параметры консольного окна, 
парная ей scrCleanup - возвращает все как было. Уже на этом этапе получилось 
так, что любое падение приложения, портило консоль, так как, ясное дело, что
ncurses без вызова endwin не может вернуть консоль в нормальный режим.

И все остальное в таком же духе.

## Карта

Все действие в игре происходит на карте, поэтому не удивительно, что функции 
для работы с картой и тем что на ней происходит заняли больше всего кода. 
На первом этапе я не хотел много времени тратить на генерацию уровня, так как 
примерно понимал, что времени можно на это убить сколько угодно, но в результате
ничего все равно может быть не очень. Поэтому, я просто взял и случайно закидал
карту пробелами, запятыми и точками, а также октоторпами(#) которые стали играть
роль деревьев.

Все клетки проходимы, кроме деревьев. Ходить можно в восемь сторон, но 
по диагонали можно ходить только когда нет стенок, иначе говоря, не 
пролезть из угла в угол.

Сразу же от места где находится игрок, стал пускать волну во все стороны, 
приписав каждой клетке как далеко она находится от игрока. Сразу же по следу 
за ним вышли мобы, которые через всю карту чуяли где он спрятался.

## Система действий

Первоначально, система действий была сделана так, что игрок ходит, потом ходят 
по очереди все живые мобы. Данная схема приводила к тому, что игрок не мог 
вообще убежать. Злобные твари со всех сторон стекались к игроку по самым
оптимальным путям и все.

Так что, как только появились мобы и игрок, пришлось переписывать систему
действий. Я сделал унифицированную систему на основе "скорости". Игрок получил
скорость 1.0, а мобы на первое время получили скорость 0.5. Таким образом, 
на каждый ход мобов, игрок делал два хода -- это дало возможность
игроку свободно убегать.

Система очень проста - каждый игровой тик, происходит накопление очков действий.
Если очков действий больше 1.0, то юнит может сделать ход, после чего у него 
отнимается очко действия. Если очков действия на следующий ход у юнита все равно
больше чем у следующего сразу за ним, то он может сделать еще один ход, иначе
ход передается следующему за ним. И так пока не сходят все, у кого достаточно
очков действий.

Такая система позволила вырьировать скорость реакции игрока и мобов. Сделай 
скорость игрока 1.2, то каждые 5 ходов игрок будет получать дополнительный ход. 
Та же история с мобами.

Я увидел, как "весело" получается убегать от мобов, которые кучей следуют за 
тобой и решил, а почему бы не сделать игру в реальном времени? Но тут возникла 
проблема -- curses кэширует нажатия клавиш и я не нашел как прозрачно этот кэш 
сбросить. Возникала проблема, игра копила нажатия и не останавливалась, пока 
кэш не опустеет. При этом наблюдалась достаточно явная неоднородность времени, 
что не получилось вылечить, поэтому я от этой идеи отказался.

## Алгоритм расчета видимости

Для меня оказалось внезапной неожиданностью то, что расчет видимости объектов на
дискретной сетке на самом деле очень не прост. Первый подход заключался в том,
чтобы использовать алгоритм Брезенхема для рисования линий.

Идея была такой: из клетки где стоит игрок до каждой клетки пускать прямую по
алгоритму Брезенхема, как только луч столкнулся с деревом, считаем что все что
дальше будет в тени.

Потом, когда я уже вроде бы стал его реализовывать, я понял, что каждая клетка
вблизи будет проверена огромное число раз, по моим прикидкам алгоритм получался 
O(N^3), а за такое отношение мой ПК перестанет со мной говорить...

В итоге, вторая мысль, которую я и реализовал оказалась прям на поверхности.
Луч света в макромире вне сильных гравитационных полей идет по прямой. Если бы 
на карте не было препятствий в виде деревьев, мобы тоже бы шли на игрока по 
прямой. Понимаете куда я клоню? У меня к этому времени была уже рассчитанная 
карта расстояний каждой клетки карты до игрока с учетом препятствий. Так как 
идти выгодно по прямой, то цена клеток за каждым препятствием будет
соответственно на сколько-то выше, чем если бы идти напрямую. Таким образом, мне
достаточно просто сравнить расстояние от клетки по прямой с тем, сколько до нее
на самом деле идти - если это расстояние больше, то клетку не видно.

Алгоритм простой и по сути и достался бесплатно, однако влоб такое решение не 
прошло, получается так, что на сетке расстояние до клетки по эвклиду и на самом
деле отличается не в пользу того, что есть на самом деле. Пришлось 
модифицировать алгоритм, введя параметр, насколько должно быть больше расстояние
чтобы считаться объект был в тени. Такой подход сделал освещение более
натуральным, но в зависимости от установленного параметра, внезапно начинали 
проявляться вещи за деревьями, которые не должны быть видимы. И чем дальше от
игрока, тем эффект проявляется более явно.

В идеале, конечно можно рассчитать стоимость клеток без препятствий один раз на 
старте и потом сравнивать ее с реальной, но мне было лень. Так что оставил как 
есть. Параметр 0.8 давал неплохие результаты в общем случае на том случайном 
месиве символов в котором я уже видел темный лес кишаший мобами.

## Обработка ошибок

Gdb -- очень мощный дебаггер, но я до этого ни разу не работал с ним из консоли.
И знаете, это получается несколько медленнее, чем напрямую. Также, я не нашел, 
как сделать, чтобы он не глушил вывод самой программы, из-за чего я открыл для
себя это новое противоречивое ощущение -- когда вроде бы и он есть, но с другой
им очень неудобно пользоваться. Я по привычке, стараюсь по коду в явных местах
расставлять ассерты, для вещей которые не хочу делать проверочный код, но 
обычный ассерт из assert.h не дает стектрейс, из-за чего без дебаггера вообще
не понятно как программа докатилась до жизни такой. 

Поэтому, я сделал свои ассерты, которые пишут в файл стек, который можно
получить с помощью функции backtrace! Это очень нужный инструмент, для случая, 
если программа все-таки упала. Так что больше ассертов богу ассертов.

## Сюжет

В самый последний день, когда я понял, что уже ничего не успеваю, я добавил в
игру "сюжет", игра открывается неким письмом, а завершается текстом
(который очень легко пропускается, если мобы поймали игрока, а он этого еще не
понял). Текст в конце становится все больше, чем больше игрок бегает от мобов. 
Таким образом, есть даже некая цель и задачи, поэтому буду считать, что эта 
поделка, может носить гордое название игра.

## Заключение

Ну, как-бы вот и все. Код написан, собраны версии для Linux и Win32, я даже не
постеснялся выложить код на гитхаб(https://github.com/masscry/7drl). Теперь, 
используя полученный опыт и примерно поняв, что надо делать, надеюсь 
поучаствовать в 7DRL Challenge 2017.

Всем хорошего времени суток и удачных забегов!
