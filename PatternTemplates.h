#ifndef TemplatesH
#define TemplatesH


//Шаблонный вектор

const size_t MaxSize = 100;



template<typename T>
class Iterator
{
protected:
    Iterator() {}

public:
    virtual ~Iterator() {}
    virtual void First() = 0; // Спозиционироваться на начале контейнера
    virtual void Next() = 0;  // Перейти к следующему элементу
    virtual bool IsDone() const = 0;  // Проверить, достигнут ли конец
    virtual T GetCurrent() const = 0; // Получить текущий элемент
};

//Итератор для вектора

template<typename T>
class VectorIterator : public Iterator<T>
{
private:
    T* VectorContainer;
    size_t Size;
    size_t Pos;

public:
    VectorIterator(T* container, size_t siz) : VectorContainer(container), Pos(0), Size(siz) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= Size); }
    T GetCurrent() const override { return VectorContainer[Pos]; }
};

//Итератор массива

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:
    const T* ArrayContainer;
    size_t Pos;
    size_t Size;

public:
    ArrayIterator(T* container, size_t siz) : ArrayContainer(container), Pos(0), Size(siz) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= Size); }
    T GetCurrent() const override { return ArrayContainer[Pos]; }
};


template <typename T>
//Я не смог разобраться с с заголовными файлами вектора и списка с cppreference, каким образом можно реализовать динамическое выделение памяти?
class VectorClass
{
private:

    //Пустой контейнер
    T Items[MaxSize];

    //Последний элемент вектора
    size_t End;

public:

    // Проверка того, является ли контейнер пустым
    bool IsEmpty() const { return (End == 0); }

    // Определение размера контейнера
    size_t Size() const { return End; }

    // Поместить в вектор новый элемент
    void Push_back(T newObject) { Items[End++] = newObject; }

    // Получение элемента по индексу
    T operator[](size_t index) const { return Items[index]; }

    //Нужна ли вектору функция удаления последнего элемента?
    T Pop() { return Items[--End]; }
    // Конструктор
    VectorClass() : End(0) {}

    class Iterator<T> *GetIterator()
    {
        return new VectorIterator<T>(Items, End);
    };
};

// Шаблонный массив

template <typename T>
class ArrayClass
{
private:

    T Items[MaxSize];
    size_t ArraySize;

public:

    void Add(T newObject) { Items[ArraySize++] = newObject; }

    size_t Size() const { return ArraySize; }

    T GetElement(size_t index) const { return Items[index]; }

    ArrayClass() : ArraySize(0) {}

    class Iterator<T> *GetIterator()
    {
        return new ArrayIterator<T>(Items, ArraySize);
    };
};

//Decorator
template<typename T>
class IteratorDecorator : public Iterator<T>
{
protected:
    Iterator<T> *It;

public:
    IteratorDecorator(Iterator<T> *it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};


//Adapter
template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType *Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType *container)
    : Container(container)
    {
        It = Container->begin();
    }
    virtual ~ConstIteratorAdapter() {}
    virtual void First() { It = Container->begin(); }
    virtual void Next() { It++; }
    virtual bool IsDone() const { return (It == Container->end()); }
    virtual ItemType GetCurrent() const { return *It; }
};

#endif // TemplatesH
#pragma once

