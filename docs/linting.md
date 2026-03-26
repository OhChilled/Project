# 🧪 Статичний аналіз коду (Linting)

У цьому проєкті використовується **clang-tidy** для статичного аналізу коду.

---

## 🔍 Використані перевірки

- `readability-magic-numbers` — виявлення "магічних чисел"
- `readability-isolate-declaration` — розділення множинних оголошень змінних
- `readability-math-missing-parentheses` — перевірка пріоритету операцій у виразах

---

## ⚙️ Запуск перевірки

Для перевірки коду використовуйте команду:

```bash
clang-tidy cosmos2025.cpp library.cpp -- -I.
```
