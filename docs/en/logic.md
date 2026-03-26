# 🧠 Program Logic

## 🎯 Purpose

The program simulates the movement of a spacecraft and checks the possibility of docking 🚀

## 🔄 Work Stages

1. Input parameters (`ax`, `ay`)
2. Initialize coordinates
3. Calculate the new state (RK4)
4. Calculate the distance
5. Check docking

## 📏 Docking Condition

Docking occurs if:

```text
R <= Rcrit
```

## 📌 Result

The program outputs:

✅ docking completed
❌ docking not completed