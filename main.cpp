
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
