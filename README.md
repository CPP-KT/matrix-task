# Matrix

В этой задаче вам необходимо реализовать простой класс матрицы, хранящейся в row-major форме:

- Весь интерфейс (кроме итераторов и views) уже задан в [matrix.h](src/matrix.h);
- Размер известен в момент конструирования и далее не меняется (кроме как через `operator=`);
- Шаблон класса матрицы параметризован типом элементов `T`. Гарантируется, что в качестве него выступает тип, похожий на `int` (дёшево копируется, есть дефолтный конструктор и прочие &laquo;хорошие&raquo; свойства);
- Об исключениях и гарантиях, связанных с ними, задумываться НЕ нужно;
- Запрещено использовать какие-либо контейнеры из стандартной библиотеки (например, `std::vector`) и умные указатели.

### Требования к итераторам и views

- Все итераторы должны удовлетворять требованиям максимальной возможной для них категории;
- `RowView`, `ColView` и их константные версии должны быть корректными диапазонами (по ним должно быть можно проитерироваться при помощи "range-based for" цикла);
- Неконстантные версии views должны также поддерживать присваивающее умножение на скаляр.

### Прочие рекомендации

- Все методы крайне рекомендуется реализовывать прямо по месту объявления (иначе рискуете столкнуться с проблемами с шаблонами);
- Старайтесь по возможности пользоваться готовыми алгоритмами из стандартной библиотеки (см. [\<algorithm\>](https://en.cppreference.com/w/cpp/algorithm), [\<numeric\>](https://en.cppreference.com/w/cpp/numeric), [\<functional\>](https://en.cppreference.com/w/cpp/utility/functional)).
