# homework-Algorithm-5
Институт Информационных Технологий и Кибербезопасности
Дисциплина: Алгоритмы и структуры данных
Лабораторная работа №5
Выполнил: Ибрагимов Магомед Абдуллаевич
Группа: ПИЖ-б-о-25-2

Тема: Стек, очередь, дек, приоритетная очередь

**Цель работы:**
1. Реализовать основные линейные структуры данных: стек,
очередь, дек и приоритетную очередь
2. Освоить принципы LIFO (стек) и FIFO (очередь)
3. Научиться реализовывать эти структуры на основе
динамического массива и связного списка
4. Изучить применение стека для решения задач (проверка скобок,
обратная польская запись)
5. Реализовать приоритетную очередь на основе кучи (heap)

<img width="619" height="642" alt="image" src="https://github.com/user-attachments/assets/ea45fa6e-1107-4118-a176-588bd2d84dd5" />
Рисунок 1 - Условие задания

```

#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <utility>

// 1. СТЕК
// Преобразование инфиксной записи в постфиксную

template <typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i <= topIndex; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(int initialCapacity = 10)
        : capacity(initialCapacity > 0 ? initialCapacity : 10),
          topIndex(-1) {
        data = new T[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    void push(const T& value) {
        if (topIndex >= capacity - 1) {
            resize();
        }

        data[++topIndex] = value;
    }

    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }

        return data[topIndex--];
    }

    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }

        return data[topIndex];
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    int size() const {
        return topIndex + 1;
    }
};

// Приоритет операторов
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;

    return 0;
}

// Проверка операторов
bool isOperator(char ch) {
    return ch == '+' || ch == '-' ||
           ch == '*' || ch == '/' || ch == '^';
}

// Алгоритм сортировочной станции
std::string infixToPostfix(const std::string& expression) {
    Stack<char> operators;
    std::string result;

    for (int i = 0; i < (int)expression.length(); i++) {
        char ch = expression[i];

        // Пропускаем пробелы
        if (std::isspace((unsigned char)ch)) {
            continue;
        }

        // Число (поддерживаются многозначные числа)
        if (std::isdigit((unsigned char)ch)) {
            while (i < (int)expression.length() &&
                   (std::isdigit((unsigned char)expression[i]) ||
                    expression[i] == '.')) {

                result += expression[i];
                i++;
            }

            result += ' ';
            i--;
        }

        // Открывающая скобка
        else if (ch == '(') {
            operators.push(ch);
        }

        // Закрывающая скобка
        else if (ch == ')') {
            bool foundOpening = false;

            while (!operators.isEmpty()) {
                if (operators.top() == '(') {
                    operators.pop();
                    foundOpening = true;
                    break;
                }

                result += operators.pop();
                result += ' ';
            }

            if (!foundOpening) {
                throw std::invalid_argument("Incorrect brackets");
            }
        }

        // Оператор
        else if (isOperator(ch)) {
            while (!operators.isEmpty() &&
                   operators.top() != '(' &&
                   (precedence(operators.top()) > precedence(ch) ||
                   (precedence(operators.top()) == precedence(ch) &&
                    ch != '^'))) {

                result += operators.pop();
                result += ' ';
            }

            operators.push(ch);
        }

        else {
            throw std::invalid_argument("Unknown character");
        }
    }

    // Извлекаем оставшиеся операторы
    while (!operators.isEmpty()) {
        if (operators.top() == '(') {
            throw std::invalid_argument("Incorrect brackets");
        }

        result += operators.pop();
        result += ' ';
    }

    if (!result.empty()) {
        result.pop_back();
    }

    return result;
}

// 2. ОЧЕРЕДЬ
// Объединение двух очередей чередованием

template <typename T>
class Queue {
private:
    T* data;
    int frontIndex;
    int rearIndex;
    int count;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i < count; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }

        delete[] data;

        data = newData;
        frontIndex = 0;
        rearIndex = count;
        capacity = newCapacity;
    }

public:
    Queue(int initialCapacity = 10)
        : frontIndex(0), rearIndex(0), count(0),
          capacity(initialCapacity > 0 ? initialCapacity : 10) {

        data = new T[capacity];
    }

    ~Queue() {
        delete[] data;
    }

    void enqueue(const T& value) {
        if (count >= capacity) {
            resize();
        }

        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }

        T value = data[frontIndex];

        frontIndex = (frontIndex + 1) % capacity;
        count--;

        return value;
    }

    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }

        return data[frontIndex];
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }

    void print() const {
        std::cout << "[";

        for (int i = 0; i < count; i++) {
            std::cout << data[(frontIndex + i) % capacity];

            if (i < count - 1) {
                std::cout << ", ";
            }
        }

        std::cout << "]" << std::endl;
    }
};

// Объединение двух очередей
// Элементы извлекаются из исходных очередей
// и поочередно добавляются в результирующую
void mergeQueues(Queue<int>& q1,
                 Queue<int>& q2,
                 Queue<int>& result) {

    while (!q1.isEmpty() && !q2.isEmpty()) {
        result.enqueue(q1.dequeue());
        result.enqueue(q2.dequeue());
    }

    // Добавляем оставшиеся элементы первой очереди
    while (!q1.isEmpty()) {
        result.enqueue(q1.dequeue());
    }

    // Добавляем оставшиеся элементы второй очереди
    while (!q2.isEmpty()) {
        result.enqueue(q2.dequeue());
    }
}

// 3. ДЕК
// Скользящее окно: поиск минимумов

template <typename T>
class Deque {// 
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value)
            : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    ~Deque() {
        while (!isEmpty()) {
            pop_front();
        }
    }

    // Добавление в конец
    void push_back(const T& value) {
        Node* newNode = new Node(value);

        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        count++;
    }

    // Удаление из конца
    T pop_back() {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }

        Node* temp = tail;
        T value = temp->data;

        tail = tail->prev;

        if (tail != nullptr) {
            tail->next = nullptr;
        }
        else {
            head = nullptr;
        }

        delete temp;
        count--;

        return value;
    }

    // Удаление из начала
    T pop_front() {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }

        Node* temp = head;
        T value = temp->data;

        head = head->next;

        if (head != nullptr) {
            head->prev = nullptr;
        }
        else {
            tail = nullptr;
        }

        delete temp;
        count--;

        return value;
    }

    // Первый элемент
    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }

        return head->data;
    }

    // Последний элемент
    T back() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }

        return tail->data;
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }
};

// Поиск минимумов в подмассивах длины k
void slidingWindowMin(int arr[], int n, int k) {
    if (k <= 0 || k > n) {
        throw std::invalid_argument("Incorrect k");
    }

    Deque<int> dq;

    std::cout << "Минимумы окон: [";

    for (int i = 0; i < n; i++) {

        // Удаляем индексы, которые вышли за окно
        while (!dq.isEmpty() && dq.front() <= i - k) {
            dq.pop_front();
        }

        // Удаляем элементы, которые больше
        // или равны текущему
        while (!dq.isEmpty() &&
               arr[dq.back()] >= arr[i]) {
            dq.pop_back();
        }

        // Добавляем индекс текущего элемента
        dq.push_back(i);

        // Если окно полностью сформировано
        if (i >= k - 1) {
            std::cout << arr[dq.front()];

            if (i < n - 1) {
                std::cout << ", ";
            }
        }
    }

    std::cout << "]" << std::endl;
}

// 4. ПРИОРИТЕТНАЯ ОЧЕРЕДЬ
// Реализация с фиксированной ёмкостью

template <typename T>
class PriorityQueue {
private:
    T* heap;
    int count;
    int capacity;

    // Восстановление кучи вверх
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (heap[index] > heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    // Восстановление кучи вниз
    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < count && heap[left] > heap[largest]) {
                largest = left;
            }

            if (right < count && heap[right] > heap[largest]) {
                largest = right;
            }

            if (largest == index) {
                break;
            }

            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    // Фиксированная ёмкость задаётся при создании
    PriorityQueue(int maxCapacity)
        : count(0), capacity(maxCapacity) {

        if (capacity <= 0) {
            throw std::invalid_argument("Capacity must be positive");
        }

        heap = new T[capacity];
    }

    ~PriorityQueue() {
        delete[] heap;
    }

    // Добавление элемента
    void push(const T& value) {
        if (count >= capacity) {
            throw std::overflow_error("PriorityQueue is full");
        }

        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    // Удаление элемента с наибольшим приоритетом
    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("PriorityQueue is empty");
        }

        T maxValue = heap[0];

        heap[0] = heap[count - 1];
        count--;

        if (count > 0) {
            heapifyDown(0);
        }

        return maxValue;
    }

    // Получение элемента с наибольшим приоритетом
    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("PriorityQueue is empty");
        }

        return heap[0];
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }

    int getCapacity() const {
        return capacity;
    }
};

// ГЛАВНАЯ ФУНКЦИЯ 

int main() {
    setlocale(LC_ALL, "ru");

    // Задание 1. Стек
    std::cout << "===== 1. СТЕК =====" << std::endl;

    std::string expression = "12 + 3 * (4 - 2)";

    std::cout << "Инфиксная запись: "
              << expression << std::endl;

    std::cout << "Постфиксная запись: "
              << infixToPostfix(expression) << std::endl;


    // Задание 2. Очередь
    std::cout << "\n===== 2. ОЧЕРЕДЬ =====" << std::endl;

    Queue<int> q1;
    Queue<int> q2;
    Queue<int> result;

    q1.enqueue(1);
    q1.enqueue(3);
    q1.enqueue(5);

    q2.enqueue(2);
    q2.enqueue(4);
    q2.enqueue(6);
    q2.enqueue(8);

    std::cout << "Первая очередь: ";
    q1.print();

    std::cout << "Вторая очередь: ";
    q2.print();

    mergeQueues(q1, q2, result);

    std::cout << "Объединённая очередь: ";
    result.print();


    // Задание 3. Дек
    std::cout << "\n===== 3. ДЕК =====" << std::endl;

    int arr[] = {4, 2, 7, 1, 5, 3, 6, 0};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 3;

    std::cout << "Массив: ";

    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }

    std::cout << "\nРазмер окна: " << k << std::endl;

    slidingWindowMin(arr, n, k);


    // Задание 4. Приоритетная очередь
    std::cout << "\n===== 4. ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ====="
              << std::endl;

    PriorityQueue<int> pq(5);

    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    pq.push(40);

    std::cout << "Ёмкость очереди: "
              << pq.getCapacity() << std::endl;

    std::cout << "Элементы по убыванию приоритета: ";

    while (!pq.isEmpty()) {
        std::cout << pq.pop() << " ";
    }

    std::cout << std::endl;

    return 0;
}
```

<img width="393" height="328" alt="image" src="https://github.com/user-attachments/assets/f0e2cf93-099b-4a1c-9400-0930e24eca1c" />
Рисунок 2 - Вывод

Контрольные вопросы
1. В чём заключается принцип работы стека (LIFO)?

LIFO (Last In, First Out) — последним добавлен, первым удалён. Элемент, добавленный последним, извлекается первым. Добавление и удаление происходят с одной стороны — вершины стека.

2. В чём заключается принцип работы очереди (FIFO)?

FIFO (First In, First Out) — первым добавлен, первым удалён. Элементы извлекаются в том же порядке, в котором были добавлены: добавление происходит в конец, удаление — из начала.

3. Чем дек отличается от обычной очереди?

Дек (двусторонняя очередь) позволяет добавлять и удалять элементы с обоих концов. В обычной очереди добавление происходит только в конец, а удаление — только из начала.

4. Что такое приоритетная очередь? Где она применяется?

Приоритетная очередь — структура данных, в которой каждый элемент имеет приоритет. Первым извлекается элемент с наибольшим или наименьшим приоритетом. Применяется в планировании задач, алгоритме Дейкстры, обработке событий и операционных системах.

5. Как реализовать стек на основе динамического массива?

Создать массив и переменную top, которая хранит индекс верхнего элемента. При добавлении элемент записывается в array[top], после чего top увеличивается. При удалении top уменьшается. Если массив заполнен, его размер увеличивается путём выделения нового массива и копирования элементов.

6. Как реализовать очередь на основе кольцевого буфера?

Использовать массив фиксированной или динамической ёмкости и два индекса: front — начало очереди, rear — позиция добавления. После каждой операции индексы перемещаются по кругу с помощью остатка от деления на размер массива. Это позволяет повторно использовать освободившиеся ячейки.

7. Почему приоритетную очередь обычно реализуют на куче?

Куча позволяет эффективно поддерживать порядок элементов по приоритету. Получение элемента с наивысшим приоритетом выполняется за O(1), а добавление и удаление — за O(log n).

8. Какова сложность операций push и pop в стеке?

Обе операции выполняются за O(1) в обычном случае. При расширении динамического массива операция push может занимать O(n), но её амортизированная сложность — O(1).

9. Какова сложность операций enqueue и dequeue в очереди?

В очереди на кольцевом буфере обе операции выполняются за O(1), поскольку не требуют перемещения остальных элементов.

10. Какова сложность операций в приоритетной очереди? Почему?

При реализации на бинарной куче:
- Получение максимального (или минимального) элемента — O(1).
- Добавление элемента — O(log n).
- Удаление элемента с наивысшим приоритетом — O(log n).
Это связано с тем, что после добавления или удаления может потребоваться восстановить свойства кучи, перемещая элемент по её высоте.

11. Как проверить правильность расстановки скобок с помощью стека?

При встрече открывающей скобки помещать её в стек. При встрече закрывающей скобки проверять вершину стека: она должна содержать соответствующую открывающую скобку. Если стек пуст или скобки не соответствуют друг другу, последовательность неправильная. В конце стек должен быть пустым.

12. Как реализовать очередь на двух стеках? Какова сложность?

Использовать два стека: input и output.
- При добавлении элемента помещать его в input.
- При удалении, если output пуст, переносить в него все элементы из input.
- Затем извлекать верхний элемент из output.
Амортизированная сложность enqueue и dequeue — O(1), поскольку каждый элемент переносится между стеками не более одного раза. В худшем отдельном случае dequeue может занимать O(n).

13. Как реализовать дек на основе двусвязного списка?

Каждый узел содержит данные и два указателя: на предыдущий и следующий узлы. Дек хранит указатели на начало и конец списка. Добавление и удаление элементов с обоих концов выполняются за O(1).

14. Как реализовать дек на основе кольцевого буфера?

Использовать массив и индексы начала и конца. При добавлении или удалении с любой стороны соответствующий индекс перемещается по кругу с помощью остатка от деления на размер массива. Если массив заполнен, его можно расширить.

15. Как реализовать стек с поддержкой получения минимума за O(1)?

Использовать два стека: основной и дополнительный стек минимумов.

При добавлении элемента в основной стек также добавлять его в дополнительный, если он меньше или равен текущему минимуму. При удалении элемента, если он равен вершине дополнительного стека, удалить его и оттуда.

Текущий минимум всегда находится на вершине дополнительного стека, поэтому его получение выполняется за O(1).


