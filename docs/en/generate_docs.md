# 📚 Documentation Generation

The project uses **Doxygen** to automatically generate documentation from C++ code.

## 🔧 Installation

You need to install Doxygen and check the availability of the command:

```bash
doxygen --version
```

## ▶️ Generating Documentation

In the root directory of the project, run:

```bash
doxygen Doxyfile
```

## 📂 Result

After running the command, the folder will be created:

```bash
docs_output/html
```

The main documentation file:

```bash
docs_output/html/index.html
```

Open it in a browser to view.

## 🧩 What is Documented

The project documents all public interfaces, including:

structures (Params, State)
functions (fx, fy, calcR, shouldDock, rk4Step)
function parameters (@param)
return values (@return)

## ✍️ Documentation Style

The Doxygen format is used:

@brief — short description
@param — description of parameters
@return — description of the result

## 🔄 Documentation Updates

Documentation must be kept up to date when changing:

- function signatures
- algorithms
- project structure

This ensures code clarity and simplifies its maintenance.
