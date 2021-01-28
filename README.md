Задача:
На шахматной доске находятся в произвольной позиции N ладей (4-6) Они все одновременно начинают ходить на случайные позиции (при этом перемещаться они, естественно, могут только горизонтально либо вертикально). Между каждыми ходами каждая фигура делает паузу 200-300 миллисекунд. Если на пути фигуры оказывается другая, она ждет, пока путь освободится. Если в течение 5 секунд проход не освободился, выбирается другая позиция аналогичным случайным образом. Всё заканчивается, когда все фигуры сделают по 50 ходов.

* Кол-во фигур задаётся через параметры запуска(максимум 6) или берётся максимальное число,
* Так же в параметрах запуска через параметр `-nofield` можно отключить рисовку доски,
* Изначально фигуры стоят на главной диагонали доски,
* Пауза между ходами выбирается рандомным образом,
* Вывод в консоль производится только в главном потоке и синхронизирован с помощью очереди сообщений,
* Существует известная проблема с тем, что фигуру могут зажать другие и она никогда не сможет доходить;

Каждая ладья ходит в ждёт хода в своём потоке.  
Как только пришло время сходить, она просит менеджер сходить.  
Менеджер синхронизирует доступ к доске обычным мьютексом.  
После того, как фигура сходила 50 раз, она говорит об этом менеджеру и поток завершается.  
Менеджер передаёт в главный поток сигнал, что надо завершить поток.  
За отрисовку доски отвечает главный поток, чтобы не тормозить другие, он всё равно бездействует.